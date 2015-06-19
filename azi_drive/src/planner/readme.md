Azi Planner
===========

This planner is not inherently associated with the Propagator project. 

This is an optimizational trajectory planner, in the sense that it treats trajectory generation as an optimization problem. Due to the nonlinear nature of the boat's dynamics, there is no guarantee that the solution produced is actually the physical optimal. However, it is likely a decent approximation. I am not aware of a reasonably fast way to get the physical optimal in modern computer science as of today.

# How this works
`Direct transcription`, is used to generate a trajectory that is `feasible` for the boat. This is a list of *states*, describing the trajectory of the vehicle, and the time that we should be reaching them. `TV-LQR` is used to *achieve* this trajectory in real-life. It creates the controller, and this is used to reject disturbances. Things like nonlinear water-resistance, wind, and model errors are rejected as disturbances.

# Things to try

## Things that will actually work
- Direct collocation
- Direct transcription
- Pseudospectral methods
- LQR for trajectory stabilization

# TODO

## Components
- Simplified water-resistance model
- TV-LQR
- Model uncertainty
- CVXGEN implementation

## Tools
- None right now!

## Math
- Add control interpolation to avoid big spikes at the end of a trajectory
- Figure out how to not require constant torques (Maybe this is necessary?)

## Fixes
- Get the main-loop in visualize.py to use the new visualize_spline function


# Glossary

`Direct Transcription` - A method of generating a trajectory given a cost to associate with a trajectory. More depth in the implementation docstring.

`feasible` - Allowed within the constraints, i.e. can be completed in the provided time, does not require thrust greater than the boat is capable, etc

`LQR` - [Linear Quadratic Regulator](https://en.wikipedia.org/wiki/Linear-quadratic_regulator), a way of finding a controller that is optimal with some caveats. More depth in implementation docstring

`TV-LQR` - Time-Variant LQR, a method of tracking a trajectory

`State` - A state, typically denoted by capital "X" in the implementation, is a vector of...[states](https://i.imgur.com/xwXs2.png). This means, for example `[positon_x, position_y, velocity_x, velocity_y]`

`u` - u is shorthand for "control input vector", a list of control inputs, ex: commands to thrusters 1 and 2.

`Dynamics` - Here, I very frequently use "dynamics" as a short-hand for referring to "A difference equation that relates state X<sub>t</sub> to state X<sub>t + 1</sub>. This is typically of the form:
######           X<sub>t + 1</sub> = dt * f(X<sub>t</sub>, u<sub>t</sub>) + X<sub>t</sub>. 
Sorry Jason.

