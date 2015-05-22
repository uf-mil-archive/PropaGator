#!/usr/bin/env python
import roslib
roslib.load_manifest('camera_docking')
import sys
import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError



class image_converter:

  def __init__(self):
    self.image_pub = rospy.Publisher("image_topic_2",Image, queue_size = 1)

    cv2.namedWindow("Image window", 1)
    self.bridge = CvBridge()
#    self.image_sub = rospy.Subscriber("image_raw/theora",Image,self.callback)
    self.image_sub = rospy.Subscriber("/camera/image_raw",Image,self.callback)
  def callback(self,data):
    try:
      cv_image = self.bridge.imgmsg_to_cv2(data, "rgb8")
    except CvBridgeError, e:
      print e

    (rows,cols,channels) = cv_image.shape

    cv2.circle(cv_image, (cols/2,rows/2), 100, (255,0,0), 10) 

    #cv2.imshow("Image window", cv_image)
    #cv2.waitKey(1)
    
    try:
      self.image_pub.publish(self.bridge.cv2_to_imgmsg(cv_image, "rgb8"))
    except CvBridgeError, e:
      print e

def main(args):
  i = 0
  ic = image_converter()
  rospy.init_node('image_converter')

  try:
    rospy.spin()
  except KeyboardInterrupt:
    print "Shutting down"
  cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
