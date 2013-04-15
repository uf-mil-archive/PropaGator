#!/usr/bin/env python

import roslib
roslib.load_manifest('camera_tf')
import rospy
import tf

rospy.init_node('camera_tf')

pos = [.9144,.1524,.3048]

def tf_callback(event):

        camera_tf_publisher=tf.TransformBroadcaster()
        camera_tf_publisher.sendTransform((pos[0],pos[1],pos[2]),
                             tf.transformations.quaternion_from_euler(0, 0, 0),
                             rospy.Time.now(),
                             "camera",
                             "/base_link")


rospy.Timer(rospy.Duration(.5),tf_callback)

rospy.spin()
