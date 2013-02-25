#!/usr/bin/env python

import roslib
roslib.load_manifest('Controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Point, Wrench
from auv_msgs.msg import LinearVelocity
from std_msgs.msg import Header
import numpy


rospy.init_node('controller')
controller_wrench = rospy.Publisher('wrench', WrenchStamped)


def _jacobian(x):
    # maps body linear+angular velocities -> global linear velocity/euler rates
    sphi, cphi = math.sin(x[3]), math.cos(x[3])
    stheta, ctheta, ttheta = math.sin(x[4]), math.cos(x[4]), math.tan(x[4])
    spsi, cpsi = math.sin(x[5]), math.cos(x[5])
    
    J = numpy.zeros((6, 6))
    J[0:3, 0:3] = [
        [ ctheta * cpsi, -cphi * spsi + sphi * stheta * cpsi,  sphi * spsi + cphi * stheta * cpsi],
        [ ctheta * spsi,  cphi * cpsi + sphi * stheta * spsi, -sphi * cpsi + cphi * stheta * spsi],
        [-stheta       ,                sphi * ctheta       ,                cphi * ctheta       ],
     ]
  
    J[3:6, 3:6] = [
        [1, sphi * ttheta,  cphi * ttheta],
        [0, cphi         , -sphi         ],
        [0, sphi / ctheta,  cphi / ctheta],
    ]
    return J

def _jacobian_inv(x):
    # maps global linear velocity/euler rates -> body linear+angular velocities
    sphi, cphi = math.sin(x[3]), math.cos(x[3])
    stheta, ctheta = math.sin(x[4]), math.cos(x[4])
    spsi, cpsi = math.sin(x[5]), math.cos(x[5])
    
    J_inv = numpy.zeros((6, 6))
    J_inv[0:3, 0:3] = [
        [       ctheta * cpsi              ,        ctheta * spsi              ,        -stheta],
        [sphi * stheta * cpsi - cphi * spsi, sphi * stheta * spsi + cphi * cpsi,  sphi * ctheta],
        [cphi * stheta * cpsi + sphi * spsi, cphi * stheta * spsi - sphi * cpsi,  cphi * ctheta],
    ]
    J_inv[3:6, 3:6] = [
        [1,     0,       -stheta],
        [0,  cphi, sphi * ctheta],
        [0, -sphi, cphi * ctheta],
    ]
    return J_inv
    
current_pos = [0,0,0]
current_orient = [0,0,0]
#--collect state information as soon as it is posted--------------------
def pose_callback(msg):
	current_pos = msg.position
	current_orient = tf.transformations.euler_from_transformation(msg.orientation)

#rospy.Subscriber('pose', Pose, pose_callback)

current_ang_vel = [0,0,0]
def ang_vel_callback(msg):
	current_ang_vel = msg.angular_velocity

#rospy.Subscriber('imu', Imu, ang_vel_callback)

current_lin_vel = [0,0,0]
def lin_vel_callback(msg):
	current_lin_vel = msg

#rospy.Subscriber('linear_vel', LinearVelocity, lin_vel_callback)

desired_state = [0,0,0,0,0,0]
desired_state_dot = [0,0,0,0,0,0]
previous_desired_state = []

def desired_state_callback(msg):
	previous_desired_state = desired_state
	desired_state = msg

#rospy.Subscriber('custom', custom, desired_state_callback)

#-----------------------------------------------------------------------


dt = .05 #sec
def update_callback(event):

	state = numpy.append(numpy.matrix(current_pos),numpy.matrix(current_orient))
	state_dot = numpy.matrix([current_lin_vel,current_ang_vel])

	desired_state_dot = (numpy.matrix(desired_state) - numpy.matrix(previous_desired_state))/dt
	e = desired_state - state
	vbd = _jacobian_inv(state)*(K*e + desired_state_dot)
	e2 = vbd- state_dot
	
	output = Ks*e2
	
	controller_wrench.publish(WrenchStamped(
						header = Header(
							stamp=rospy.Time.now(),
							frame_id="/base_link",
							),
						wrench=Wrench(
							force = Vector3(x= output[0],y= output[1],z= 0),
							torque = Vector3(x=0,y= 0,z= output[5]),
							))
							)	


rospy.Timer(rospy.Duration(dt), update_callback)


rospy.spin()



		
