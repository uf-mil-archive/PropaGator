#!/usr/bin/python
from __future__ import division
from dynamixel_servo.msg import DynamixelFullConfig 
import rospy
import time

if __name__ == "__main__":
    #INITIALLY WHEN THE BOAT STARTS THE CABLE SHOULD BE FEED OVER THE TOP OF THE SERVO



    rospy.init_node('reset_dynamixel')
    pub = rospy.Publisher('dynamixel/dynamixel_full_config', DynamixelFullConfig, queue_size=8)

    def dynam_config(_id,_pos,_speed,_torque,_acc,_mode,_vel):
        time.sleep(.3)
        pub.publish(
                DynamixelFullConfig(
                    id=_id,
                    goal_position=_pos,
                    moving_speed=_speed,
                    torque_limit=_torque,
                    goal_acceleration=_acc,
                    control_mode=_mode,
                    goal_velocity=_vel  # This is for wheel mode, we don't use it
                )
            )
        


    # Reset all dynamixels on the boat to the format needed to start
    # ONLY A PATCH FOR RIGHT NOW -- RESETS THE SERVOS AFTER SHUTTING DOWN run.launch WITHOUT RESTING POWER

    dynam_config(2,0,0,1023,38,0,0.0)
    dynam_config(2,0,0,1023,38,0,0.0)
    dynam_config(2,0,0,1023,38,1,0.0)
    dynam_config(2,0,0,1023,38,1,0.0)
    dynam_config(3,0,0,1023,38,0,0.0)
    dynam_config(3,0,0,1023,38,0,0.0)
    dynam_config(3,0,0,1023,38,1,0.0)
    dynam_config(3,0,0,1023,38,1,0.0)
    #dynam_config(4,0,1.4,143,20,1,0.0)
    #dynam_config(4,0,1.4,143,20,0,1.4)



       
