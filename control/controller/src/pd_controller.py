#!/usr/bin/env python

#reference paper = Nonlinear Control of an Autonomous Underwater Vehicle: A RISE-Based Approach
# Reference Frames:
#       odom.pose                   /enu
#       odom.Twist                  /base_link
#       desired_posetwist.pose      /enu
#       desired_posetwist.twist     desired_posetwist.pose   i.e. /enu + desired angle
#       Wrench                      /base_link

import roslib
roslib.load_manifest('controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Vector3Stamped, Point, Wrench, PoseStamped
from std_msgs.msg import Header,Bool, Float64
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry
import numpy,math,tf,threading
from tf import transformations
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.msg import PoseTwistStamped
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster
from collections import deque

class Controller(object):
    def __init__(self):

        # Old gains
        self.d_x = 175
        self.d_y = 90
        self.d_z = 60
        self.p_x = 30
        self.p_y = 30
        self.p_z = 40

        #self.d_x = 175
        #self.d_y = 90
        #self.d_z = 90
        #self.p_x = 40
        #self.p_y = 40
        #self.p_z = 100

        self.killed = False
        self.enable = True
        self.odom_active = False

        self.K_d = numpy.array([
        [self.d_x,0,0,0,0,0],
        [0,self.d_y,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,self.d_z]])

        self.K_p = numpy.array([
        [self.p_x,0,0,0,0,0],
        [0,self.p_y,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,0],
        [0,0,0,0,0,self.p_z]])

        # Averaging parameters
        self.last_average = numpy.zeros(6)
        self.num_to_avg = 75 # At 50 hz this is 50 samples is one second of data
        self.outputs = deque([numpy.zeros(6) for i in range(0, self.num_to_avg)])
        
        self.desired_state_set = False
        self.desired_state = numpy.ones(6)
        self.desired_state_dot = numpy.ones(6)
        self.state = numpy.ones(6)
        self.previous_error = numpy.ones(6)
        self.state_dot = numpy.ones(6)
        self.state_dot_body = numpy.ones(6)

        self.lock = threading.Lock()

        # Get tf listener
        self.tf_listener = tf.TransformListener()

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

        self.dz_error = 0
        self.dx_error = 0
        self.dy_error = 0

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
        self.desired_state_dot = numpy.concatenate([xyz_array(desired_posetwist.posetwist.twist.linear), xyz_array(desired_posetwist.posetwist.twist.angular)])
        self.lock.release()

    def odom_callback(self, current_posetwist):
        self.lock.acquire()
        self.odom_active = True

        # Grab position; 0 Z
        pose = xyz_array(current_posetwist.pose.pose.position)
        pose[2] = 0

        # Grab orientation; 0 pitch and roll
        orientation = numpy.array(transformations.euler_from_quaternion(xyzw_array(current_posetwist.pose.pose.orientation)))
        orientation[0:2] = 0

        self.state = numpy.concatenate([xyz_array(current_posetwist.pose.pose.position), transformations.euler_from_quaternion(xyzw_array(current_posetwist.pose.pose.orientation))])
        self.state_dot = numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)])
        if (not self.desired_state_set):
            self.desired_state = self.state
            self.desired_state_set = True
        self.lock.release()

    def main_loop(self, event):
        
        self.lock.acquire()
        #print 'desired state', desired_state
        #print 'current_state', state
        def smallest_coterminal_angle(x):
            return (x + math.pi) % (2*math.pi) - math.pi

        

        # sub pd-controller sans rise
        rot = None
        # Get tf from /enu to /base_link
        #   Since we are dealing with differences and not absolute positions not translation is required
        try:
            (_, rot) = self.tf_listener.lookupTransform('/base_link', '/enu', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException) as e:
            rospy.logwarn('Tf exception: ' + str(e))

        if rot is not None:
            # Convert to euler angle ( we only care about rotation about z)
            theta = quat_to_rotvec(numpy.array(rot))[2] 

            # Difference in /enu frame
            to_desired_state = self.desired_state[0:2] - self.state[0:2]

            # Convert to base_link
            s = numpy.sin(theta)
            c = numpy.cos(theta)
            rot_matrix_enu_base = numpy.array([[c, -s],
                                               [s, c]])
            to_desired_state = rot_matrix_enu_base.dot(to_desired_state)
            to_desired_state = numpy.insert(to_desired_state, 2, 0) # Append a zero for z


            #                                    Note: Angular differences are the same in base_link as enu so no tf required
            e = numpy.concatenate([to_desired_state, map(smallest_coterminal_angle, self.desired_state[3:6] - self.state[3:6])]) # e_1 in paper

            # Convert desired state dot into base_link
            #   angular velocities do not rquire transformation as they are differences (rendering them frameless)
            #   To convert velocites from enu + desired_orientation to base link:
            #       transform to enu: rotate the velocity vector by desired_orientation
            #       transform to base link: rotate the velocity vector by the angle between enu and base_link
            vels = self.desired_state_dot[0:2]
            theta2 = self.desired_state[5]
            s = numpy.sin(theta2)
            c = numpy.cos(theta2)
            rot_matrix_desired_enu = numpy.array([[c, -s],
                                                  [s, c]])
            vels = rot_matrix_desired_enu.dot(vels)
            vels = rot_matrix_enu_base.dot(vels)

            vels = numpy.insert(vels, 2, 0)

            desired_state_dot = numpy.concatenate([vels, self.desired_state_dot[3:6]])

            #print 'Desired_state tf: ', desired_state_dot
            e_dot = desired_state_dot - self.state_dot
            output = self.K_p.dot(e) + self.K_d.dot(e_dot)

            # Apply simple moving average filter
            new = output / self.num_to_avg
            old = (self.outputs[0] / self.num_to_avg) 
            self.last_average = self.last_average - old + new
            self.outputs.popleft()
            self.outputs.append(output)

            # Resuse output var
            #print 'Outputs: ', self.outputs
            output = self.last_average

            #print 'Last Average: ', output
            self.lock.release()

            self.x_error = e[0]
            self.y_error = e[1]
            self.z_error = e[5]

            self.dx_error = e_dot[0]
            self.dy_error = e_dot[1]
            self.dz_error = e_dot[5]

            #self.to_terminal()            
            
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

        else:
            self.lock.release()

    def timeout_callback(self, event):
        self.odom_active = False

    def to_terminal(self):
        print "X: ", self.x_error
        print "Y: ", self.y_error
        print "Z: ", self.z_error

        print "dX: ", self.dx_error
        print "dY: ", self.dy_error
        print "dZ: ", self.dz_error

if __name__ == '__main__':
    rospy.init_node('controller')
    controller = Controller()
    #rospy.on_shutdown(controller.shutdown)
    rospy.Timer(rospy.Duration(1.0/50.0), controller.main_loop)
    rospy.Timer(rospy.Duration(1), controller.timeout_callback)
    rospy.spin()
