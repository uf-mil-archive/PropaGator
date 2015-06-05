#!/usr/bin/env python

#reference paper = Nonlinear Control of an Autonomous Underwater Vehicle: A RISE-Based Approach

import roslib
roslib.load_manifest('controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Vector3Stamped, Point, Wrench, PoseStamped
from std_msgs.msg import Header,Bool, Float64
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry
import numpy,math,tf,threading
from tf import transformations
from uf_common.orientation_helpers import xyz_array, xyzw_array
from uf_common.msg import PoseTwistStamped
from controller.srv import Enable, EnableResponse
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster

class Controller(object):
    def __init__(self):

        self.d_x = 30
        self.d_y = 20
        self.d_z = 1
        self.p_x = .5
        self.p_y = .5
        self.p_z = 3

        self.killed = False
        self.enable = True
        self.odom_active = False

        self.K_p = numpy.ones((6,6))
        self.K_d = numpy.ones((6,6))
        
        self.desired_state_set = False
        self.desired_state = numpy.ones(6)
        self.desired_state_dot = numpy.ones(6)
        self.state = numpy.ones(6)
        self.previous_error = numpy.ones(6)
        self.state_dot = numpy.ones(6)
        self.state_dot_body = numpy.ones(6)

        self.lock = threading.Lock()

        rospy.Subscriber("pd_d_gain", Point, self.d_gain_callback)
        rospy.Subscriber("pd_p_gain", Point, self.p_gain_callback)
        rospy.Subscriber('/trajectory', PoseTwistStamped, self.desired_state_callback)
        rospy.Subscriber('/odom', Odometry, self.odom_callback)
        self.controller_wrench = rospy.Publisher('wrench', WrenchStamped, queue_size = 1)
        self.waypoint_progress = rospy.Publisher('waypoint_progress', Bool, queue_size = 1)
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)

        self.z_error = 0
        self.x_error = 0
        self.y_error = 0

    def set_kill(self):
        self.killed = True
        rospy.logwarn('PD_Controller KILLED: %s' % self.kill_listener.get_kills())

    def clear_kill(self):
        self.killed = False
        rospy.logwarn('PD_Controller ACTIVE: %s' % self.kill_listener.get_kills())

    def d_gain_callback(self, msg):
        self.d_x = msg.x
        self.d_y = msg.y
        self.d_z = msg.z

        self.K_d = numpy.array([
        [self.d_x,0,0,0,0,0],
        [0,self.d_y,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,self.d_z]])

    def p_gain_callback(self, msg):
        self.p_x = msg.x
        self.p_y = msg.y
        self.p_z = msg.z

        self.K_p = numpy.array([
        [self.p_x,0,0,0,0,0],
        [0,self.p_y,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,self.p_z]])

    def desired_state_callback(self,desired_posetwist):
        self.lock.acquire()
        self.desired_state_set = True
        self.desired_state = numpy.concatenate([xyz_array(desired_posetwist.posetwist.pose.position), transformations.euler_from_quaternion(xyzw_array(desired_posetwist.posetwist.pose.orientation))])
        self.desired_state_dot = self._jacobian(self.desired_state).dot(numpy.concatenate([xyz_array(desired_posetwist.posetwist.twist.linear), xyz_array(desired_posetwist.posetwist.twist.angular)]))
        self.lock.release()

    def odom_callback(self, current_posetwist):
        self.lock.acquire()
        self.odom_active = True
        self.state = numpy.concatenate([xyz_array(current_posetwist.pose.pose.position), transformations.euler_from_quaternion(xyzw_array(current_posetwist.pose.pose.orientation))])
        self.state_dot = self._jacobian(self.state).dot(numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)]))
        self.state_dot_body = numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)])
        if (not self.desired_state_set):
            self.desired_state = self.state
            self.desired_state_set = True
        self.lock.release()

    def _jacobian(self, x):
        # maps body linear+angular velocities -> global linear velocity/euler rates
        sphi, cphi = math.sin(x[3]), math.cos(x[3])
        stheta, ctheta, ttheta = math.sin(x[4]), math.cos(x[4]), math.tan(x[4])
        spsi, cpsi = math.sin(x[5]), math.cos(x[5])
        rate
        J = numpy.zeros((6, 6))
        J[0:3, 0:3] = [
            [ ctheta * cpsi, -cphi * spsi + sphi * stheta * cpsi,  sphi * spsi + cphi * stheta * cpsi],
            [ ctheta * spsi,  cphi * cpsi + sphi * stheta * spsi, -sphi * cpsi + cphi * stheta * spsi],
            [-stheta       ,                sphi * ctheta       ,                cphi * ctheta       ],]
        
        J[3:6, 3:6] = [
            [1, sphi * ttheta,  cphi * ttheta],
            [0, cphi         , -sphi         ],
            [0, sphi / ctheta,  cphi / ctheta],]
        return J

    def _jacobian_inv(self, x):
        # maps global linear velocity/euler rates -> body linear+angular velocities
        sphi, cphi = math.sin(x[3]), math.cos(x[3])
        stheta, ctheta = math.sin(x[4]), math.cos(x[4])
        spsi, cpsi = math.sin(x[5]), math.cos(x[5])
        
        J_inv = numpy.zeros((6, 6))
        J_inv[0:3, 0:3] = [
            [       ctheta * cpsi              ,        ctheta * spsi              ,        -stheta],
            [sphi * stheta * cpsi - cphi * spsi, sphi * stheta * spsi + cphi * cpsi,  sphi * ctheta],
            [cphi * stheta * cpsi + sphi * spsi, cphi * stheta * spsi - sphi * cpsi,  cphi * ctheta],]
        J_inv[3:6, 3:6] = [
            [1,     0,       -stheta],
            [0,  cphi, sphi * ctheta],
            [0, -sphi, cphi * ctheta],]
        return J_inv

    def main_loop(self, event):
        
        
        self.lock.acquire()
        #print 'desired state', desired_state
        #print 'current_state', state
        def smallest_coterminal_angle(x):
            return (x + math.pi) % (2*math.pi) - math.pi

        self.x_error = abs(abs(self.desired_state[1]) - abs(self.state[1]))
        self.y_error = abs(abs(self.desired_state[0]) - abs(self.state[0]))
        self.z_error = abs((self.desired_state[5] % 2*math.pi) - ((self.state[5] + 3.14) % 2*math.pi))

        self.to_terminal()

        # sub pd-controller sans rise
        e = numpy.concatenate([self.desired_state[0:3] - self.state[0:3], map(smallest_coterminal_angle, self.desired_state[3:6] - self.state[3:6])]) # e_1 in paper
        vbd = self._jacobian_inv(self.state).dot(self.K_p.dot(e) + self.desired_state_dot)
        e2 = vbd - self.state_dot_body
        output = self.K_d.dot(e2)

        
        self.lock.release()
        if (not(self.odom_active)):
            output = [0,0,0,0,0,0]
        if (self.enable & self.killed==False):
            self.controller_wrench.publish(WrenchStamped(
                header = Header(
                    stamp=rospy.Time.now(),
                    frame_id="/base_link",
                    ),
                wrench=Wrench(
                    force = Vector3(x= output[0],y= output[1],z= 0),
                    torque = Vector3(x=0,y= 0,z= output[5]),
                    ))
                    
                    )

            if((self.x_error < 1) & (self.y_error < 1) & (self.z_error < 1)):
                    self.waypoint_progress.publish(True)

        if (self.killed == True):
            rospy.logwarn('PD_Controller KILLED: %s' % self.kill_listener.get_kills())
            self.controller_wrench.publish(WrenchStamped(
                    header = Header(
                        stamp=rospy.Time.now(),
                        frame_id="/base_link",
                        ),
                    wrench=Wrench(
                        force = Vector3(x= 0,y= 0,z= 0),
                        torque = Vector3(x=0,y= 0,z= 0),
                        ))
                        
                        )

    def timeout_callback(self, event):
        self.odom_active = False

    def to_terminal(self):
        print "X: ", self.x_error
        print "Y: ", self.y_error
        print "Z: ", self.z_error

if __name__ == '__main__':
    rospy.init_node('controller')
    controller = Controller()
    #rospy.on_shutdown(controller.shutdown)
    rospy.Timer(rospy.Duration(1.0/35.0), controller.main_loop)
    rospy.Timer(rospy.Duration(1), controller.timeout_callback)
    rospy.spin()