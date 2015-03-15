Azi\_Drive
==========

Azimuth Drive Controller

Takes a Wrench, and attempts to achieve that wrench using the available actuators

# Notes

Thruster 2 is on the right, thruster 3 is on the left

# Usage

rosrun boatsim sim
roslaunch boat_launch controller.launch

# TODO
- Use CVXOPT or try C++ implementation 
- Implement path planner that takes advantage of this tool
- Add water drag terms
- Implement actual ROS node

# Issues
- Does not always converge

With questions and concerns, please contact Jacob Panikulam.
