#! /usr/bin/env python

import roslib
import rospy
import actionlib
from uf_common.msg import PoseTwistStamped, MoveToAction, PoseTwist, Acceleration
from geometry_msgs.msg import Pose, Twist, Vector3, Point, Quaternion
from nav_msgs.msg import Odometry
from std_msgs.msg import Header

count = 0

waypoint_point = rospy.Publisher('/trajectory', PoseTwistStamped, queue_size=1)

def odom_callback(current):
    global count
    waypoint_point.publish(PoseTwistStamped(
            header = Header(
                stamp=rospy.Time.now(),
                frame_id="/base_link",
                ),
            posetwist = PoseTwist(
                pose = Pose(
                    position = Point(x = current.pose.pose.position.x,
                                     y = current.pose.pose.position.y,
                                     z = current.pose.pose.position.z),

                    orientation = Quaternion(x = current.pose.pose.orientation.x,
                                             y = current.pose.pose.orientation.y,
                                             z = current.pose.pose.orientation.z, 
                                             w = current.pose.pose.orientation.w)
                ),
                twist = Twist(
                    linear = Vector3(x = current.twist.twist.linear.x,
                                     y = current.twist.twist.linear.y,
                                     z = current.twist.twist.linear.z),
                
                    angular = Vector3(x = current.twist.twist.angular.x,
                                      y = current.twist.twist.angular.y,
                                      z = current.twist.twist.angular.z)
                ),
                acceleration = Acceleration(
                    linear = Vector3(x=0,y=0,z=0),
                    angular = Vector3(x=0,y=0,z=0)
                )

                )
            )
                
        )
    count = count + 1


rospy.init_node('azi_waypoint_reset')
rospy.Subscriber('/odom', Odometry, odom_callback)
while count < 2:
    None
rospy.loginfo("New Position Accepted")

            