How to setup LIDAR for new computers:

 - Add the udev file (10-propagator-io-board.rules) located in this directory to /lib/udev/rules.d/

 - Under Network Connections, edit wired connection (LADAR). Go to IPv4 Settings tab, in drop down menu select "Shared to other computers"

 - To manually talk to I/O board, use provided script 'lidar_ioboard'

 - Startup LADAR with launch file (roslaunch laser_tf_py lidar.launch) or manually:

rosrun LMS1xx LMS100
rosrun laser_tf_py laser_tf_broadcaster.py
rosrun pointcloud_publisher pointcloud_publisher
