#!/usr/bin/env python

### 
###	This node takes cmds from the joy node and publishes pwms
###	The joystick Left Stick & Right Stick outputs in the range [-1,1],
### This node maps them to pwm values and publishes the same to the 
### std_msgs :: pwm1 and pwm2. The servos are held in one position.
###												

### To Do:: Define Modes :: RC // Autonomous // Etc
### To Do:: Define all joystick buttons and axes 
### Authored by Khimya Khetarpal on March,20,2015

### Please update revisons to it with your name and date and valuable comments. 
###	Github handles this just look at the file history or use git blame (Kevin French)


## May 25, 2015 #Khimya Khetarpal
### Using the logit function for finer control on the remote.
### For reference: non linear function used is: y = [((0.0001*logit(x/2+0.5))+0.0015) for x in arange(-1, 1.1, 0.1)]



import rospy
from numpy import arange
import math
import matplotlib.pyplot as plt

from dynamixel_servo.msg import DynamixelFullConfig
from std_msgs.msg import Float64
from sensor_msgs.msg import Joy
from std_msgs.msg import Header
from std_msgs.msg import Float64
from std_msgs.msg import Bool

from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster

killed = False
zero_pwm = 1.5e-3
rc_active = True
back_btn_pressed = False

AXIS = {
	'LEFT_STICK_X': 0,
	'LEFT_STICK_Y': 1,
	'RIGHT_STICK_X': 2,
	'RIGHT_STICK_Y': 3,
	
}

BTNS = {
	'A':0, #Green
	'B':1, #Red
	'X':2, #Blue
	'Y':3, #Yellow
	'L_BUMPER':4, #findout
	'R_BUMPER':5, #findout
	'BACK':6, #Back Button
	'START':7, #Start Button
	'KILL':8, #Kill Switch #Center Big Switch
}

pwm_port_pub = rospy.Publisher('stm32f3discovery_imu_driver/pwm1', Float64, queue_size = 10)
pwm_starboard_pub = rospy.Publisher('stm32f3discovery_imu_driver/pwm2', Float64, queue_size = 10)
rc_state_pub = rospy.Publisher('rc/status', Bool, queue_size = 10)

def logit(p):
	l = math.log(p + 0.00001) - math.log(1.00001 - p)
	#l = math.log(p) - math.log(1 - p)
	return l

def clip(val):
	if (val < 0.001):
		val = 0.001
	elif (val > 0.002):
		val = 0.002
	else:
		val = val
	return val

def killed_cb():
	global killed
	killed = True
	zero_pwms()
	rospy.logwarn('%s killed because %s' % (rospy.get_name(), kill_listener.get_kills()) )

def unkilled_cb():
	global killed
	killed = False
	rospy.loginfo('%s unkilled' % rospy.get_name())

def on_shutdown():
	# Kill system on shutdown
	kill_broadcaster.send(True)

def zero_pwms():
	pwm1 = Float64(zero_pwm)
	pwm_port_pub.publish(pwm1)
	pwm2 = Float64(zero_pwm)
	pwm_starboard_pub.publish(pwm2)

def pub_rc_state(event):
	rc_state_pub.publish(rc_active)

def start():
	global kill_listener
	global kill_broadcaster

	#Init node
	rospy.init_node('remote_control')

	kill_broadcaster = KillBroadcaster(id=rospy.get_name(), description='Remote control kill')
	kill_listener = KillListener(killed_cb, unkilled_cb)
	try:
		kill_broadcaster.clear()
	except rospy.service.ServiceException, e:
		rospy.logwarn(str(e))
	
	#Init subscribers and publishers
	rospy.Subscriber('joy', Joy, xbox_cb, queue_size = 10)

	#Timer to output if the RC node is active
	rospy.Timer(rospy.Duration(0.5), pub_rc_state)

	# Add a shutdown hook
	rospy.on_shutdown(on_shutdown)

	#Spin
	rospy.spin()

def xbox_cb(joy_msg):
	global killed
	global back_btn_pressed
	global rc_active

	# Toggle RC state
	if joy_msg.buttons[BTNS['BACK']]:
		if not back_btn_pressed:
			back_btn_pressed = True
			rc_active = not rc_active
	else:
		back_btn_pressed = False


	
	# If kill_broadcaster.send(killed) is put outside these if statments then
	#	rc will echo any other kill with its own kill msg which prevents the RC node from ever coming out of a kill
	#	without pressing the start button
	if (joy_msg.buttons[BTNS['KILL']] == 1):	#Press KILL Switch
		# Kill the boat
		killed = True
		kill_broadcaster.send(True)

	if (joy_msg.buttons[BTNS['START']] == 1):	#Press START Switch
		# Do not update killed here as there may be other kills that affect the weather the boat should or shouldn't be killed
		kill_broadcaster.send(False)

		
	if killed:
		zero_pwms()
	else:
		#pwm1 = Float64(0.0005*(joy_msg.axes[AXIS['LEFT_STICK_Y']]) + zero_pwm)  #LEFT_STICK
		pwm1 = Float64((0.0001*logit((joy_msg.axes[AXIS['LEFT_STICK_Y']])/2+0.5))+0.0015)
		pwm1 = clip(pwm1)
		pwm_port_pub.publish(pwm1)
		#pwm2 = Float64(0.0005*(joy_msg.axes[AXIS['RIGHT_STICK_Y']]) + zero_pwm)  #RIGHT_STICK
		pwm2 = Float64((0.0001*logit((joy_msg.axes[AXIS['RIGHT_STICK_Y']])/2+0.5))+0.0015)
		pwm2 = clip(pwm2)
		pwm_starboard_pub.publish(pwm2) 

if __name__ == '__main__':
	try:
		start()
	except rospy.ROSInterruptException: pass 
