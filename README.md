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
