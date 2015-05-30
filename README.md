PropaGator
==========

This is the repository for the University of Florida's Propagator Autonomous Robot.  You need Ubuntu 14.04, and ROS Indigo. For some installation instructions, see github.com/uf-mil/getting_started
## Install 
This repository contains ROS packages specific to the
PropaGator robot.
This repository depends on uf-mil's software-common and
hardware-common repositories.

This repository also depends on
[socat](http://www.dest-unreach.org/socat/), installable
with:

    sudo apt-get install socat

and autoreconf, installable with:

    sudo apt-get install autoconf


This repository also depends on the lms1xx package,
installable with:

    roscd && cd ~/catkin_ws/src && git clone https://github.com/RCPRG-ros-pkg/RCPRG_laser_drivers && cd RCPRG_laser_drivers && git submodule init && git submodule update

Source the PropaGator bashrc file into your .bashrc
  
    sudo echo "source ~/catkin_ws/src/uf-mil/PropaGator/bashrc" >> ~/.bashrc


## Getting Started
All of this is written for Ubuntu 14.04 (Trusty Tahr) and ROS Indigo (ros.org/install) - get the desktop-full version.
First, follow [this tutorial](https://help.github.com/articles/generating-ssh-keys/) for setting up git. Then follow the first three ROS tutorials, and make your catkin workspace.
A good linux tutorial is [here](http://info.ee.surrey.ac.uk/Teaching/Unix/)

If you would like to see the ROS commands these terminal commands correspond to look in the PropaGator bashrc file

### Everything below this point assumes you have sourced the bashrc file above

### Simulators

To start the PropaGator simulator run 'sim' from your terminal
If you get an error it is typically a problem that happens everytime you update Ubuntu. Fix with:

    rm -fr /tmp/pyode-build && mkdir -p /tmp/pyode-build && cd /tmp/pyode-build && sudo apt-get build-dep -y python-pyode && sudo apt-get remove -y python-pyode && apt-get source --compile python-pyode && sudo dpkg -i python-pyode_*.deb

### Run PropaGator

To start servos and actuators that must be started for movement run 'run' from your terminal

To start Azi Drive thruster mapper, visualizer, and azi drive waypoint handler run 'azi' from your terminal

To start the position controller that uses Azi Drive run 'controller' from your terminal (this is the part of the control system that controls and send a position and orientation to azi drive for the boat as it moves from one point to another)

To start the odometry and gps run 'nav' from you terminal


### Kill Handling
* Any node that updates the actuators directly or indirectly should follow the following conventions for safety reasons. For a Cpp example see the [stm32f3discovery_imu_driver.cpp](https://github.com/uf-mil/hardware-common/blob/master/stm32f3discovery_imu_driver/src/stm32f3discovery_imu_driver.cpp), for a python example see [thruster_control.py](https://github.com/uf-mil/PropaGator/blob/master/motor_control/scripts/thruster_control.py), for kill handling documentation see this [readme](https://github.com/uf-mil/software-common/blob/master/kill_handling/readme.md):
  * On startup create a kill listner and kill broadcaster
  * Directly after initlizing the kill broadcaster, call its clear function (in case the node previously called a kill)
vvv  * The kill listener's killed callback should imideatly stop normal operation and output the nodes zero
    * Do not echo the kill by broadcasting your own kill, only kill when your node needs to call a kill such as on shutdown
  * The kill listener's unkilled callback should resume normal operation
  * On shutdown broadcast a kill to the kill master (if the chain of motor command is broken the entire system should be killed until it can be resolved)
    * Make sure that you are killing on shutdown appropriatly the lowest level control nodes must take special care to zero there actuators since they directly interface with the actuatorsuytre

## TODO
* It would be incredibly usefull for all controllers to have a quick switch to RC mode a standard method for this is in the works - Fixed!!

