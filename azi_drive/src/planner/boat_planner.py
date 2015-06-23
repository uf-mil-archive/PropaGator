import numpy as np
from scipy import optimize, misc
from azimuth_drive import Tools
from optimization import Optimize
from visualize import visualize_spline
from time import time


class Boat_Planner(object):
    '''Boat Planner

    Bibliography:
        [1] Fossen, Thor; Handbook of Marine Craft Hydrodynamics and Motion Control:
                Ch 7, Models for Ships, Offshore Structures and Underwater Vehicles.
            > Used for realizing that a reasonably accurate water-resistance model was unrealistic

    '''

    # Do some fitting to determine the actual cross-coupling
    B = np.eye(3)

    @classmethod
    def rotation_mat(self, theta):
        '''rotation_mat(theta) -> rotation matrix
        Generates a 2x2 rotation matrix for an angle, theta, in radians
        Bibliography:
            [1] https://en.wikipedia.org/wiki/Rotation_matrix
        '''
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


        TODO:
            - Do everything in the world frame
            - Rotate the control-input, and apply everything in the world frame
            - Apply resistance as a function of the sideslip angle
        '''
        rotation = self.rotation_mat((np.pi / 2) - theta)
        xdot_world, ydot_world = np.dot(rotation, [xdot, ydot])
        u_x_world, u_y_world = np.dot(rotation, [u_x, u_y])

        # Water resistance terms
        # -np.sign(xdot) * 0.1 * xdot ** 2
        # ydotdot = -np.sign(ydot) * 0.2 * ydot ** 2
        ydotdot = 0.0
        Xdot = np.array(
            [xdot_world, ydot_world, thetadot, 0.0, ydotdot, 0.0]
        ) + np.hstack([
            [0.0, 0.0, 0.0], 
            np.dot(self.B, [u_x_world, u_y_world, u_theta])
        ])
        return Xdot


if __name__ == '__main__':
    print 'Boat Planner Demo; This is not intended to be run as main'
    bp = Boat_Planner()

    x0 = np.array([
        0.0, 0.0, 0.0,  # Positional
        10.0, 10.0, 0.0,  # dots
    ])

    xf = np.array([
        100.0, 100.0, np.pi/2,
        0.0, 0.0, 0.0,
    ])

    time_end = 15
    dt = 0.1
    time_range = np.arange(0, time_end, dt)

    tic = time()
    control_tape, spline, success = Optimize.direct_transcription(
        bp.boat_dynamics, 
        x0, xf, 
        dt=dt, time_end=time_end, 
        control_dimension=3, 
        control_bounds=[
            (-70, 100),
            (-30, 30),
            (-10, 10)
        ]
    )
    toc = time() - tic
    print success, 'took {} seconds'.format(toc)

    # print control_tape

    # control_tape = np.zeros((len(time_range), 3)) + -0.03 * np.hstack([np.zeros((len(time_range), 2)), np.ones((len(time_range), 1))])
    # spline = Optimize.forward_sim(bp.boat_dynamics, x0, control_tape, dt=dt, time_end=time_end)
    visualize_spline(spline[:, :3], 'Pathviz', animate=True, iteration_speed=1, lasts=0, has_theta=True, end_point=xf[:2], vel_markers=True)



