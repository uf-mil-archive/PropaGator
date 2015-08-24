#!/usr/bin/env python
import rospy
import roslib
roslib
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
from controller.srv import Enable, EnableResponse
from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster



class PID_controller:

    def __init__(self, Derivator=0, Integrator=0, Integrator_max=500, Integrator_min=-500):

        

        '''

        Structure gain matrix for flexible use
        This allows the gain function to access all gains by simple indexing scheme
        Place x and y gains in the first row of the 6x3
        Place x gains in the bottom row 

        Gain array layout:

            [  p_x.   i_x.   d_x.]
            [  p_y.   i_y.   d_y.]
            [  0.      0.      0.]
            [  0.      0.      0.]
            [  0.      0.      0.]
            [  p_z.   i_z.   d_z.]

        '''

        self.K = numpy.zeros((6,3))
        self.K[0:6, 0:6] = [
            [50,0,10], # pid_x
            [100,0,10], # pid_y
            [0,0,0],
            [0,0,0],
            [0,0,0],
            [100,0,10], # pid_z
        ]

        # Set PID soptions
        self.Derivator= Derivator
        self.Integrator= Integrator
        self.Integrator_max= Integrator_max
        self.Integrator_min= Integrator_min

        # Kill functions
        self.odom_active = False
        self.killed = False

        # Create arrays to be used
        self.desired_state = numpy.ones(6)
        self.desired_velocity = numpy.ones(6)
        self.current_state = numpy.zeros(6)
        self.current_velocity = numpy.zeros(6)
        self.current_error = numpy.ones(6)


        self.lock = threading.Lock()

        # Used to reset the desired state on startup
        self.desired_state_set = False

        # ROS components
        self.controller_wrench = rospy.Publisher('wrench', WrenchStamped, queue_size = 1)
        self.waypoint_progress = rospy.Publisher('waypoint_progress', Bool, queue_size = 1)
        self.kill_listener = KillListener(self.set_kill, self.clear_kill)
        rospy.Subscriber('/trajectory', PoseTwistStamped, self.trajectory_callback)
        rospy.Subscriber('/odom', Odometry, self.odom_callback)
        rospy.Subscriber("pid_d_gain", Point, self.d_gain_callback)
        rospy.Subscriber("pid_p_gain", Point, self.p_gain_callback)
        rospy.Subscriber("pid_i_gain", Point, self.i_gain_callback)

    def p_gain_callback(self, msg):
        x = msg.x
        y = msg.y
        z = msg.z

        self.K[0:2, 0] = [x,y]
        self.K[5, 0] = z

    def i_gain_callback(self, msg):
        x = msg.x
        y = msg.y
        z = msg.z

        self.K[0:2, 1] = [x,y]
        self.K[5, 1] = z

    def d_gain_callback(self, msg):
        x = msg.x
        y = msg.y
        z = msg.z

        self.K[0:2, 2] = [x,y]
        self.K[5, 2] = z

    def set_kill(self):
        self.killed = True
        rospy.logwarn('PD_Controller KILLED: %s' % self.kill_listener.get_kills())

    def clear_kill(self):
        self.killed = False
        rospy.logwarn('PD_Controller ACTIVE: %s' % self.kill_listener.get_kills())

    def trajectory_callback(self, desired_trajectory):
        self.lock.acquire()
        self.desired_state_set = True
        # Get dessired pose and orientation 
        desired_pose = xyz_array(desired_trajectory.posetwist.pose.position)
        desired_orientation = transformations.euler_from_quaternion(xyzw_array(desired_trajectory.posetwist.pose.orientation))
        # Get desired linear and angular velocities
        desired_lin_vel = xyz_array(desired_trajectory.posetwist.twist.linear)
        desired_ang_vel = xyz_array(desired_trajectory.posetwist.twist.angular)
        # Add desired position to desired state matrixs
        self.desired_state = numpy.concatenate([desired_pose, desired_orientation])
        # Add desired velocities to velocity matrix
        self.desired_velocity = numpy.concatenate([desired_lin_vel, desired_ang_vel])
        self.lock.release()

    def odom_callback(self, current_pos):
        self.lock.acquire()
        self.odom_active = True
        # Grab current position and orientation and 0 linear Z and angluar X and Y
        # Get current position 
        current_position = xyz_array(current_pos.pose.pose.position)
        current_orientation = numpy.array(transformations.euler_from_quaternion(xyzw_array(current_pos.pose.pose.orientation)))
        # Zero unneccesary elements
        current_position[2] = 0
        current_orientation[0:2] = 0
        # Add current position to state matrix
        self.current_state = numpy.concatenate([current_position, current_orientation])
        # Get current velocities
        current_lin_vel = xyz_array(current_pos.twist.twist.linear)
        current_ang_vel = xyz_array(current_pos.twist.twist.angular)
        # Add current velocities to velocity matrix
        self.current_velocity = numpy.concatenate([current_lin_vel, current_ang_vel])
        # If the desired state has not yet been set, set desired and current as the same
        # Resets the controller to current position on bootup
        if (not self.desired_state_set):
            self.desired_state = self.current_state
            self.desired_state_set = True

        self.lock.release()

    def PID(self, variable):
        # Index in state number we want to access
        state_number = 0
        if variable == 'x': state_number = 0
        if variable == 'y': state_number = 1
        if variable == 'z': state_number = 5

        #self.current_error = self.desired_state[state_number] - self.current_state[state_number]
        #rospy.logwarn(variable + ": " + str(self.current_error[state_number]))
        p = self.K[state_number, 0] * self.current_error[state_number]
        i = (self.Integrator + self.current_error[state_number]) * self.K[state_number, 1]
        d = self.K[state_number, 2] * (self.current_error[state_number] - self.Derivator)
        self.Derivator = self.current_error[state_number]

        if self.Integrator > self.Integrator_max:
            self.Integrator = self.Integrator_max
        elif self.Integrator < self.Integrator_min:
            self.Integrator = self.Integrator_min

        PID = p + i + d 
        return PID

    def timeout_callback(self, event):
        self.odom_active = False

    def jacobian_inv(self, x):
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

        def smallest_coterminal_angle(x):
            return (x + math.pi) % (2*math.pi) - math.pi

        self.lock.acquire()

        # Get linear and angular error between desired and current pose - /enu
        linear_error = self.desired_state[0:3] - self.current_state[0:3]
        angular_error = map(smallest_coterminal_angle, self.desired_state[3:6] - self.current_state[3:6])

        # Combine errors into one array
        error_enu = numpy.concatenate([linear_error, angular_error])
        rospy.logwarn(error_enu)
        # Translate /enu errors into /base_link errors
        error_base = self.jacobian_inv(self.current_state).dot(error_enu)
        # Take away velocity from error to avoid overshoot
        final_error = error_base - self.current_velocity
        # Place errors to be sent into main error array
        self.current_error = [final_error[0], final_error[1], 0, 0, 0, final_error[5]]

        self.lock.release()

        # Send error values through PID controller
        x = self.PID('x')
        y = self.PID('y')
        z = self.PID('z')

        # Combine into final wrenches
        wrench = [x,y,0,0,0,z]

        # If odometry has not been aquired, set wrench to 0
        if (not(self.odom_active)):
            wrench = [0,0,0,0,0,0]

        # If ready to go...
        if (self.killed == False):
            self.controller_wrench.publish(WrenchStamped(
                header = Header(
                    stamp=rospy.Time.now(),
                    frame_id="/base_link",
                    ),
                wrench=Wrench(
                    force = Vector3(x= wrench[0],y= wrench[1],z= 0),
                    torque = Vector3(x=0,y= 0,z= wrench[5]),
                    ))
                    
                    )

        # If not ready to go...
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

if __name__ == "__main__":
    rospy.init_node('controller')

    controller = PID_controller(0, 500, -500)
    #rospy.on_shutdown(controller.shutdown)
    rospy.Timer(rospy.Duration(1.0/20.0), controller.main_loop)
    rospy.Timer(rospy.Duration(1), controller.timeout_callback)
    rospy.spin()


