Azi Planner
===========

This planner is not inherently associated with the Propagator project. It is a trajectory optimization planner.


# Things to try
- Parameterize a spline in point-position and velocities, then generate a cost that describes the length of the trajectory, then try to optimize
-- > Local minima are _fine_, *a solution* is what we care about. Any non-ridiculous solution that is within bounds

## Things that will actually work
- Direct collocation
- Pseudospectral methods
- LQR for trajectory stabilization


# Experiments

- The minimum time path should be a straight line given no other constraints. Will this work? (It does!)

# TODO

## Tools
- Add a "visualize path" tool
- Full spline from points


## Math

- Derive A(x) for A(x)*x + B(alpha)*u

- Determine an appropriate spline for policy search (Looks like cubic hermite is not really the best solution for this?
Varun suggested configuration-space search, but this is impractical for a boat-like dynamic vehicle)