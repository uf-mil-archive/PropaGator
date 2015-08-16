Controller
==========

# Controller
Controller contains software to arbitrate control of the robot and provide control algorithims such as proportional derivitive controllers

# Notes
* We should add an integral term to the PD controller
* Architecture redesign must accomidate a smarter control arbiter
	* Control arbiter should be able to check the state of all nodes required by a controller


### Frames

Forward is x-positive, left is y-positive, up is z-positive. According to ros convention.

# Usage

rosrun controller control_arbiter.py
rosrun controller pd_controller.py

To use the control arbiter and allow msgs to go through you must register the controller using the below mentioned register_controller service. To set the active controller you must use the below mentioned set_controller service.

# Testing

???


# What's in here?

* A control arbiter which intercepts thruster and servo cmds and only passes the active controllers cmds through. The control arbiter also has tie ins to the kill system and float system allowing it to make sure the boat behaves as expected
	* A service for registering new controllers
	* A service for requsting a particular controller
	* A publisher stating the current controller
* A proportional derivative controller used to produce wrenches pased on the boats desired trajectory and its current position

# Shutdown procedure

All nodes are designed for safe shutdown

# TODO

* Write the control arbiter node
* Add Integral term to proportional derivative controller

# Issues


