Azi\_Drive
==========

Azimuth Drive Controller

Takes a Wrench, and attempts to achieve that wrench using the available actuators.

This is not trivial, because the boat is overactuated, and there is thus no simple way to choose the pair of thruster forces and thruster angles that 'optimally' achieves some desired boat-force and boat-torque.

# Notes

Thruster 2 is on the right, thruster 3 is on the left

### Frames

Forward is x-positive, right is y-positive, down is z-positive. According to nautical convention.


# Usage

    rosrun boat_sim sim
    rosrun azi_drive azi_drive_node.py
    rosrun azi_drive control_manager.py  # Run this in a seperate window, this will stop the thrusters if you manually interrupt Azi_Drive (CTRL+C)

To visualize, run
    rosrun azi_drive visualize_azi_drive.py

If you get weird errors, try a catkin_make or removing erroneous .pyc files

For the old controller, 

    roslaunch boat_launch controller.launch

or
    roslaunch boat_launch navigation.launch
    rosrun controller pd_controller.py  # This actually works, but commands insane forces

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
* A pygame node for visualizing the force vectors generated

# Shutdown procedure

For now, you must command a zero wrench directly. The boat control architecture was not designed around services (We should do a redesign), which are required for using rospy's on_shutdown callback. In on_shutdown, _messages) are not guaranteed to be sent, only service calls and parameters.

# TODO

- Implement a 2D simulation in pygame that is less...janky
- Use CVXOPT or try C++ implementation 
- Implement path planner that takes advantage of this tool
- Add water drag terms
- A velocity controller that allows a Twist set with an arbitrary reference frame
- More reasonable force-clipping (Not attempting unchievable force, but also being smarter about it)

# Issues

- Does not correctly produce pure strafing motion in _real_ world, but does in simulation. This is because the simulation and the real boat have differing centers of mass.


- Sometimes goes out of angle bounds (on unachievable commands), don't allow this.

- There may be a problem relating to the inequality constraints in map_thrusters. Haven't been able to nail it down.

With questions and concerns, please contact Jacob Panikulam via email or facebook (If he is still alive).
