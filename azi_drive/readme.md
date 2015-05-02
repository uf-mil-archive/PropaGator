Azi\_Drive
==========

Azimuth Drive Controller

Takes a Wrench, and attempts to achieve that wrench using the available actuators.

This is not trivial, because the boat is overactuated, and there is thus no simple way to choose the pair of thruster forces and thruster angles that 'optimally' achieves some desired boat-force and boat-torque.

# Notes

Thruster 2 is on the right, thruster 3 is on the left

# Usage

    rosrun boatsim sim
    rosrun azi_drive azi_drive_node.py
    rosrun azi_drive control_manager.py  # Run this in a seperate window, this will stop the thrusters if you manually interrupt Azi_Drive (CTRL+C)

If you get weird errors, try a catkin_make or removing erroneous .pyc files

For the old controller, 

    roslaunch boat_launch controller.launch

# Testing

To run  unittests, do

    python azi_drive/tests/test_azi_drive.py


If the unittests are not passing, this node _will not_ work.


# What's in here?

* A module for control allocation for azimuth thrusters
* A node for taking a wrench on /wrench and producing thrust and angle requests that satisfy it
* A tool for numerical estimation of jacobians for functions that give and take numpy arrays
* A tool for numerical estimation of partial derivatives
* A node (control_manager.py) for handling sudden shutdown of the azi_drive nodes

# Shutdown procedure
For now, you must command a zero wrench directly. The boat control architecture was not designed around services (We should do a redesign), which are required for using rospy's on_shutdown callback. In on_shutdown, _messages) are not guaranteed to be sent, only service calls and parameters.

# TODO

- Implement a 2D simulation in pygame that is less...janky
- Use CVXOPT or try C++ implementation 
- Implement path planner that takes advantage of this tool
- Add water drag terms

# Issues

- Does not correctly produce pure strafing motion (Error in B matrix?)
-- Fixed, using [this line from zdrive2](https://github.com/uf-mil/PropaGator/blob/master/z_drive2/scripts/z_drive2#L130)
-- wtf?


With questions and concerns, please contact Jacob Panikulam via email or facebook (If he is still alive).