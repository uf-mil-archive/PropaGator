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
import math

from dynamixel_servo.msg import DynamixelFullConfig
from controller.msg import ControlThrustConfig
from controller.srv import register_controller, request_controller
from sensor_msgs.msg import Joy
from std_msgs.msg import Header
from std_msgs.msg import Bool
from std_msgs.msg import String

from kill_handling.listener import KillListener
from kill_handling.broadcaster import KillBroadcaster

from dynamixel_servo.msg import DynamixelFullConfig

killed = False
ZERO_PWM = 1.5e-3
current_controller = None
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

pwm_pub = rospy.Publisher('/controller/thruster_config', ControlThrustConfig, queue_size = 10)
servo_pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size = 10)
request_controller_proxy = rospy.ServiceProxy('/controller/request_controller', request_controller)
register_controller_proxy = rospy.ServiceProxy('/controller/register_controller', register_controller)

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
	cmd = ControlThrustConfig()
	cmd.controller = 'xbox_rc'
	cmd.pulse_width = ZERO_PWM

	cmd.id = ControlThrustConfig.PORT
	pwm_pub.publish(cmd)

	cmd.id = ControlThrustConfig.STARBOARD
	pwm_pub.publish(cmd)


def start():
	global kill_listener
	global kill_broadcaster

	#Init node
	rospy.init_node('remote_control')

	# Register xbox_rc as a controller
	rospy.wait_for_service('/controller/register_controller', 30.0)
	register_controller_proxy('xbox_rc')

	# Request to switch to xbox_rc
	request_controller_proxy('xbox_rc')

	kill_broadcaster = KillBroadcaster(id=rospy.get_name(), description='Remote control kill')
	kill_listener = KillListener(killed_cb, unkilled_cb)
	try:
		kill_broadcaster.clear()
	except rospy.service.ServiceException, e:
		rospy.logwarn(str(e))
	
	#Init subscribers and publishers
	rospy.Subscriber('joy', Joy, xbox_cb, queue_size = 1)
	rospy.Subscriber('/controller/current_controller', String, current_controller_cb, queue_size = 1)

	# Add a shutdown hook
	rospy.on_shutdown(on_shutdown)

	#Spin
	rospy.spin()

def current_controller_cb(msg):
	global current_controller
	current_controller = msg.data


def xbox_cb(joy_msg):
	global killed
	global back_btn_pressed
	global current_controller

	# Toggle RC state
	if joy_msg.buttons[BTNS['BACK']]:
		if not back_btn_pressed:
			back_btn_pressed = True
			if current_controller != 'xbox_rc':
				request_controller_proxy('xbox_rc')
			else:
				request_controller_proxy('azi_drive')
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
		cmd = ControlThrustConfig()
		cmd.controller = 'xbox_rc'

		cmd.id = ControlThrustConfig.PORT
		cmd.pulse_width = 0.0005*(joy_msg.axes[AXIS['LEFT_STICK_Y']]) + ZERO_PWM  #LEFT_STICK
		#				 (0.0001*logit((joy_msg.axes[AXIS['LEFT_STICK_Y']])/2+0.5))+0.0015
		pwm_pub.publish(cmd)

		cmd.id = ControlThrustConfig.STARBOARD
		cmd.pulse_width = 0.0005*(joy_msg.axes[AXIS['RIGHT_STICK_Y']]) + ZERO_PWM  #RIGHT_STICK
		#				 (0.0001*logit((joy_msg.axes[AXIS['RIGHT_STICK_Y']])/2+0.5))+0.0015
		pwm_pub.publish(cmd)

			# Zero servos
		for x in range(2,4):
			servo_pub.publish(DynamixelFullConfig(
				id=						x,
				goal_position= 			math.pi,
				moving_speed=			12, # near maximum, not actually achievable ...
				torque_limit=			1023,
				goal_acceleration=		38,
				control_mode=			DynamixelFullConfig.JOINT,
				goal_velocity=			10,
			))

if __name__ == '__main__':
	try:
		start()
	except rospy.ROSInterruptException: pass 
