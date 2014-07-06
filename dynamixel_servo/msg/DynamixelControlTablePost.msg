#This file contains the entire control table of a MX-64T servo that will be returned when a DynamixelControlTableRequest
#		message is sent to the Dynamixel server.
#
#It should be noted that the values returned in this message are as they would apper in the Dynamixel's registers, thus
#		they are not in the standard units specified by ROS REP 103. http://www.ros.org/reps/rep-0103.html
#       Please refer to the Dynamixxel MX-64T's manual for mor information
#
#The Baud rate is an EPROM address value on each servo and must match value on the ROS parameter server.
#		Note: all servos on the bus need to have the same baud rate.

# Control table parameters of a MX-64T Dynamixel servo with Firmware Version 36
#----------- dynamixel memory map locations -----------------
# EEPROM addresses will survive a power cycling of the servo and hence have been designated with "EPROM"
uint16 model_num
uint8 firmware_ver
uint8 id
uint8 baud
uint8 return_delay_time
uint16 cw_angle_limit
uint16 ccw_angle_limit
# note 1 byte is skipped here according the the manual
uint8 high_temp_limit
uint8 low_voltage_limit
uint8 high_voltage_limit
uint16 max_torque
uint8 status_return_level
uint8 alarm_led
uint8 alarm_shutdown
# note 1 byte is skipped here per FIRMWARE version 36
uint16 multi_turn_offset
uint8 resolution_divider
# note 1 byte is skipped here per FIRMWARE version 36
#----------------------------------------------------
uint8 torque_enable
uint8 led
uint8 pid_d_gain
uint8 pid_i_gain
uint8 pid_p_gain
# note 2 byte are skipped here per the manual
uint16 goal_position
uint16 moving_speed
uint16 torque_limit
uint16 present_position
uint16 present_speed
uint16 present_load
uint8 present_voltage
uint8 present_temp
uint8 instruction_registered
# note 1 byte is skipped here per the manual
uint8 is_moving
uint8 eeprom_lock
uint16 punch
# note 19 byte are skipped here per the manual
uint16 consuming_current
uint8 torque_control_mode
uint16 goal_torque
uint8 goal_acceleration