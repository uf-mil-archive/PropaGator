import numpy as np
from scipy import optimize, misc
from azimuth_drive import Tools
from optimization import Optimize
from visualize import visualize_spline


class Boat_Planner(object):

    # Do some fitting to determine the actual cross-coupling
    B = np.eye(3)

    @classmethod
    def rotation_mat(self, theta):
        c, s = np.cos(theta), np.sin(theta)
        return np.array([
            [c, -s],
            [s,  c],
        ])

    @classmethod
    def boat_dynamics(self, (x, y, theta, xdot, ydot, thetadot), (u_x, u_y, u_theta)):
        '''boat_dynamics((x, y, theta, xdot, ydot, thetadot), u):

        xdot, ydot, u_x, u_y are all in the boat frame, x-forward, y-left

        returns the derivative of the state in the world frame
        '''
        # No resistance modelled
        rotation = self.rotation_mat((np.pi / 2) - theta)
        xdot_world, ydot_world = np.dot(rotation, [xdot, ydot])
        u_x_world, u_y_world = np.dot(rotation, [u_x, u_y])

        Xdot = np.array([xdot_world, ydot_world], thetadot, 0.0, 0.0, 0.0]) + np.hstack([[0.0, 0.0, 0.0], np.dot(self.B, [u_x_world, u_y_world, u_theta])])
        return Xdot


if __name__ == '__main__':
    bp = Boat_Planner()

    x0 = np.array([
        0.0, 0.0, 0.0,  # Positional
        1.0, 0.0, 0.5,  # dots
    ])

    time_end = 100
    dt = 0.1
    time_range = np.arange(0, time_end, dt)
    control_tape = np.zeros((len(time_range), 3)) + -0.03 * np.hstack([np.zeros((len(time_range), 2)), np.ones((len(time_range), 1))])
    spline = Optimize.forward_sim(bp.boat_dynamics, x0, control_tape, dt=dt, time_end=time_end)
    visualize_spline(spline[:, :3], 'Pathviz', animate=True, iteration_speed=1, lasts=0, has_theta=True)



