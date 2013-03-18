How to setup LIDAR for new computers:

 - Add the udev file (10-propagator-navid-motordriver.rules) located in this directory to /lib/udev/rules.d/

 - Under Network Connections, edit wired connection (LADAR). Go to IPv4 Settings tab, in drop down menu select "Manual", then
	click add under Addresses, then enter "169.254.205.2" for the Address, "255.255.255.0" for the Netmask
	and leave Gateway blank.

 - To manually talk to I/O board, use provided script 'lidar_motordriver'. You can add it to you ~/bin/ folder.

 - Startup LADAR with launch file (roslaunch laser_tf_py lidar.launch) or manually:

	rosrun LMS1xx LMS100
	rosrun laser_tf_py laser_tf_broadcaster.py
	rosrun pointcloud_publisher pointcloud_publisher
		etc.

	Bagfiles are located in laser_tf_py/bagfiles
