Tank Steer Controller

This node implements a simple tank steer controller by locking both thrusters in strait back and controling only the force of each thruster.
Since we have two control inputs and three degrees of freedom (x, y, theta) the system is underactuated. Using the boat as a frame of reference where x is strait forward (towards the bow) the controller can control x and theta. There is no control of y which makes it impossible to station hold. Instead of station holding a low level path planner maintains a postion through the following logic. If the boat leaves some radius then the boat will give itself a new goal to achieve and activate the controller again to return to the position.

## Nodes
* tank_steer_PD.py
  * The tank steer primative driver converts a wrench into pwm settings for the boat to achieve
* low_level_path_planner.py
  * The low level path planner generates a strait line path from the current point to the desired point irregardless of obsicals. It then generates a wrench to keep the boat on the path

## TODO
replace point_shoot with low_level_path_planner.py
Create pool test
Add kill handling to both nodes
Add quick RC switch
Finish this readme
Add a launch file

## Usage

## Issues