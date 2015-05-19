PropaGator
==========
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

    roscd && cd ../src && git clone https://github.com/RCPRG-ros-pkg/RCPRG_laser_drivers && cd RCPRG_laser_drivers && git submodule init && git submodule update

## Notes
### Actuators
* Any node that updates the actuators directly or indirectly should follow the following conventions for safety reasons. For a Cpp example see the [stm32f3discovery_imu_driver.cpp](https://github.com/uf-mil/hardware-common/blob/master/stm32f3discovery_imu_driver/src/stm32f3discovery_imu_driver.cpp), for a python example see [thruster_control.py](https://github.com/uf-mil/PropaGator/blob/master/motor_control/scripts/thruster_control.py), for kill handling documentation see this [readme](https://github.com/uf-mil/software-common/blob/master/kill_handling/readme.md):
  * On startup create a kill listner and kill broadcaster
  * Directly after initlizing the kill broadcaster, call its clear function (in case the node previously called a kill)
  * The kill listener's killed callback should imideatly stop normal operation and output the nodes zero
  * The kill listener's unkilled callback should resume normal operation
  * On shutdown broadcast a kill to the kill master (if the chain of motor command is broken the entire system should be killed until it can be resolved)
    * Make sure that you are killing on shutdown appropriatly the lowest level control nodes must take special care to zero there actuators since they directly interface with the actuators

## TODO
* It would be incredibly usefull for all controllers to have a quick switch to RC mode a standard method for this is in the works

