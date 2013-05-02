#!/usr/bin/env python

#reference paper = Nonlinear Control of an Autonomous Underwater Vehicle: A RISE-Based Approach

import roslib
roslib.load_manifest('controller')
import rospy
from geometry_msgs.msg import WrenchStamped, Vector3, Vector3Stamped, Point, Wrench, PoseStamped
from std_msgs.msg import Header
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry
import numpy,math,tf
from tf import transformations
from uf_common.orientation_helpers import xyz_array, xyzw_array
from uf_common.msg import PoseTwistStamped


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
    
#---------------collect desired state information as soon as it is posted--------------------
global desired_state_set
desired_state_set = False
desired_state = numpy.zeros(6)
desired_state_dot = numpy.zeros(6)
state = numpy.zeros(6)
state_dot = numpy.zeros(6)
state_dot_body = numpy.zeros(6)

def desired_state_callback(desired_posetwist):
    global desired_state,desired_state_dot,desired_state_set
    desired_state_set = True	
    desired_state = numpy.concatenate([xyz_array(desired_posetwist.posetwist.pose.position), transformations.euler_from_quaternion(xyzw_array(desired_posetwist.posetwist.pose.orientation))])
    desired_state_dot = _jacobian(desired_state).dot(numpy.concatenate([xyz_array(desired_posetwist.posetwist.twist.linear), xyz_array(desired_posetwist.posetwist.twist.angular)]))

rospy.Subscriber('/trajectory', PoseTwistStamped, desired_state_callback)

#----------------------------------------------------------------------------------

#set controller gains
rospy.set_param('p_gain', {'x':2.6,'y':2.6,'yaw':5.0})
rospy.set_param('d_gain', {'x':0.3,'y':0.3,'yaw':1.0})
#.25,400
#----------------------------------------------------------------------------------

K = numpy.array([
	[rospy.get_param('p_gain/x'),0,0,0,0,0],
	[0,rospy.get_param('p_gain/y'),0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,rospy.get_param('p_gain/yaw')]])

Ks = numpy.array([
	[rospy.get_param('d_gain/x'),0,0,0,0,0],
	[0,rospy.get_param('d_gain/y'),0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,0],
	[0,0,0,0,0,rospy.get_param('d_gain/yaw')]])
def odom_callback(current_posetwist):
        global desired_state,desired_state_dot,state,stat_dot,state_dot_body,desired_state_set
	
        state = numpy.concatenate([xyz_array(current_posetwist.pose.pose.position), transformations.euler_from_quaternion(xyzw_array(current_posetwist.pose.pose.orientation))])
	state_dot = _jacobian(state).dot(numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)]))
	state_dot_body = numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)])
	if (not desired_state_set):
           desired_state = state
           desired_state_set = True
	
def update_callback(event):
	
	global desired_state,desired_state_dot,state,stat_dot,state_dot_body

	#state = numpy.concatenate([xyz_array(current_posetwist.pose.pose.position), transformations.euler_from_quaternion(xyzw_array(current_posetwist.pose.pose.orientation))])
	#state_dot = _jacobian(state).dot(numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)]))
	#state_dot_body = numpy.concatenate([xyz_array(current_posetwist.twist.twist.linear), xyz_array(current_posetwist.twist.twist.angular)])
		
	print 'desired state', desired_state
        print 'current_state', state		
	def smallest_coterminal_angle(x):
		return (x + math.pi) % (2*math.pi) - math.pi
	e = numpy.concatenate([desired_state[0:3] - state[0:3], map(smallest_coterminal_angle, desired_state[3:6] - state[3:6])]) # e_1 in paper
 #       print "yaw_error", e[5]
	vbd = _jacobian_inv(state).dot(K.dot(e) + desired_state_dot)
	e2 = vbd - state_dot_body
	
	output = Ks.dot(e2)
	#print "vbd",vbd
        #print "state_dot_body",state_dot_body
#	print 'output_z_torque',output[5]
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
rospy.Timer(rospy.Duration(.1),update_callback)
rospy.Subscriber('/odom', Odometry, odom_callback)
rospy.spin()



		
