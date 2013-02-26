#!/usr/bin/env python

#reference paper = Nonlinear Control of an Autonomous Underwater Vehicle: A RISE-Based Approach

import roslib
roslib.load_manifest('controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Vector3Stamped, Point, Wrench, PoseStamped
from std_msgs.msg import Header
from sensor_msgs.msg import Imu
import numpy,math,tf


rospy.init_node('controller')
controller_wrench = rospy.Publisher('wrench', WrenchStamped)

#----------------------------------------------------------------------------------

def _jacobian(x):
    # maps body linear+angular velocities -> global linear velocity/euler rates
    sphi, cphi = math.sin(x[3]), math.cos(x[3])
    stheta, ctheta, ttheta = math.sin(x[4]), math.cos(x[4]), math.tan(x[4])
    spsi, cpsi = math.sin(x[5]), math.cos(x[5])
    
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

def _jacobian_inv(x):
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
    
    
#---------------collect state information as soon as it is posted--------------------

current_pos = numpy.zeros((3,1))
current_orient = numpy.zeros((3,1))
state = numpy.zeros((6,1))
current_ang_vel = numpy.zeros((3,1))
current_lin_vel = numpy.zeros((3,1))
desired_state = numpy.zeros((6,1))
desired_state_dot = numpy.zeros((6,1))
previous_desired_state = numpy.zeros((6,1))

def pose_callback(msg):
	global current_pos,current_orient,state
	current_pos = [msg.position.x,msg.position.y,msg.position.z]
	current_orient = tf.transformations.euler_from_quaternion([msg.orientation.x,msg.orientation.y,msg.orientation.z,msg.orientation.w])
	state = numpy.append(current_pos,current_orient)

def ang_vel_callback(msg):
	global current_ang_vel
	current_ang_vel = [msg.angular_velocity.x,msg.angular_velocity.y,msg.angular_velocity.z]

def lin_vel_callback(msg):
	global curren_lin_vel
	current_lin_vel = msg

def desired_state_callback(msg):
	global previous_desired_state,desired_state
	
	previous_desired_state = desired_state
	desired_pos = [msg.position.x,msg.position.y,msg.position.z]
	desired_orient = tf.transformations.euler_from_quaternion([msg.orientation.x,msg.orientation.y,msg.orientation.z,msg.orientation.w])
	desired_state = numpy.append(desired_pos,desired_orient)


rospy.Subscriber('/current_pose', PoseStamped, pose_callback)
rospy.Subscriber('/imu', Imu, ang_vel_callback)
rospy.Subscriber('/linear_vel', Vector3Stamped, lin_vel_callback)
rospy.Subscriber('/desired_pose', PoseStamped, desired_state_callback)

#set controller gains
rospy.set_param('p_gain', {'x':1.0,'y':1.0,'yaw':1.0})
rospy.set_param('d_gain', {'x':1.0,'y':1.0,'yaw':1.0})

#----------------------------------------------------------------------------------

dt = .05 #sec
K = numpy.array([
	[rospy.get_param('p_gain/x'),0,0,0,0,0],
	[0,rospy.get_param('p_gain/y'),0,0,0,0],
	[0,0,1,0,0,0],
	[0,0,0,1,0,0],
	[0,0,0,0,1,0],
	[0,0,0,0,0,rospy.get_param('p_gain/yaw')]])

Ks = numpy.array([
	[rospy.get_param('d_gain/x'),0,0,0,0,0],
	[0,rospy.get_param('d_gain/y'),0,0,0,0],
	[0,0,1,0,0,0],
	[0,0,0,1,0,0],
	[0,0,0,0,1,0],
	[0,0,0,0,0,rospy.get_param('d_gain/yaw')]])
	
def update_callback(event):
	global desired_state,previous_desired_state,current_lin_vel,current_ang_vel,state

	state_dot = numpy.append(current_lin_vel,current_ang_vel)
	desired_state_dot = numpy.subtract(desired_state,previous_desired_state)/dt
		
	e = desired_state - state
	vbd = _jacobian_inv(state)*(K*e + desired_state_dot)
	e2 = vbd - state_dot
	
	output = Ks*e2

	controller_wrench.publish(WrenchStamped(
						header = Header(
							stamp=rospy.Time.now(),
							frame_id="/base_link",
							),
						wrench=Wrench(
							force = Vector3(x= output[0,0],y= output[1,1],z= 0),
							torque = Vector3(x=0,y= 0,z= output[5,5]),
							))
							)	

rospy.Timer(rospy.Duration(dt), update_callback)
rospy.spin()



		
