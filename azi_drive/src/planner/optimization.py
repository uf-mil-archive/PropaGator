import numpy as np
import cvxopt
from scipy import optimize, misc
from visualize import visualize_spline
from time import time


class Optimize(object):
    @classmethod
    def direct_transcription(self, dynamics, x0, xf, dt=0.1, time_end=10, control_dimension=2, control_fraction=10):
        '''direct_transcription()
        Compute a feasible optimal trajectory via direct transcription using sequential least-squares programming (slsqp),
            not interpolating state beyond dynamics, and interpolating control using a zero-order hold.

        Arguments:
            dynamics - x[t + 1] = dynamics(x, u) + x
            x0 - initial state
            xf - targeted end-state
            dt - time step size
            time_end - time in seconds to plan a trajectory for (horizon length)
            control_dimension - length of the control-input vector, u
            control_fraction - fraction of states that governs how many different control inputs will be varied
                                in short, the vehicle will change its control input at 1/control_fraction of the states

        Concept:
            Dynamics: x[t + 1] = dynamics(x, u) + x
                where x is the state at time t

        Bibliography:
            [1] Olivares, Alberto; Direct Transcription Methods for Optimal Control [Online]
                Available: http://www.tsc.urjc.es/~staffetti/mcco/slides_mcco/talk_olivares.pdf
            [2] Tedrake, Russ; Underactuated Robotics, Lec 9-10 [Online]
                Available: https://courses.edx.org/courses/MITx/6.832x/3T2014/courseware/week5/week5_overview/

        '''

        time_range = np.arange(0, time_end, dt)

        initial_guesses = [0.0 for i in range(len(time_range) // control_fraction)]  * control_dimension
        constraints = []
        u_max = 100
        u_min = -100
        for k in range(len(initial_guesses)):
            constraints.append(
                {
                    'type': 'ineq',
                    'fun': lambda u: u_max - u[k]
                }
            )
            constraints.append(
                {
                    'type': 'ineq',
                    'fun': lambda u: u[k] - u_min
                }
            )

        def objective(u_vector):
            '''Evaluate the objective given a control tape by forward-simulating the vehicle dynamics
                Issues: Slow!
                Formulate the final position instruction as a constraint and not a cost 
                    (We should _always_ satisfy boundary conditions +/- slack)
            '''
            control_tape = self.undiscretize(u_vector, len(time_range), dims=control_dimension)
            spline = self.forward_sim(
                dynamics_func=dynamics, 
                x0=x0, 
                u_vec = control_tape,
                dt=dt, 
                time_end=time_end,
                # target=xf, tol=3
            )
            # visualize_spline(spline[:, :2], 'Path Visualization', animate=False, iteration_speed=5, lasts=0.05)
            # return 5 * np.sum((spline - xf) ** 2)
            return (5 * np.linalg.norm(spline[-1] - xf)) + (40 * len(spline))

        opt = optimize.minimize(
            fun=objective,
            x0=initial_guesses,
            method='SLSQP',
            constraints=constraints,
            tol=1e-1
        )

        u_vector = opt.x
        control_tape = self.undiscretize(u_vector, len(time_range), dims=control_dimension)
        spline = self.forward_sim(dynamics, x0, control_tape, dt=dt, time_end=time_end,
            # target=xf, tol=3
        )
        # visualize_spline(spline[:, :2], 'Pathviz', animate=True, iteration_speed=5, lasts=0, end_point=xf)
        return control_tape, spline, opt.success

    @classmethod
    def undiscretize(self, u_vector, desired_length, dims=2):
        '''undiscretize(u_vector, desired_length)
        Unpack the shortened list of control inputs using 0-order hold'''
        control_list = np.array(u_vector).reshape((len(u_vector) // dims, dims))
        target_length = desired_length // len(control_list)
        undiscretized = np.repeat(control_list, target_length, axis=0)
        return undiscretized

    @classmethod
    def forward_sim(self, dynamics_func, x0, u_vec, dt=0.1, time_end=10, target=None, tol=1e-3):
        '''forward_sim(dynamics_func, x0, u_vec, dt=0.1, time_end=10)
        Forward simulate the dynamics of a system
        '''
        state_length = len(x0)
        x = np.copy(x0);
        _range = np.arange(0, time_end, dt)
        trajectory = np.zeros((len(_range), state_length))

        assert len(_range) == len(u_vec), ("The number of control inputs is not equal "
            "to the number of time steps: {} != {}").format(
                len(_range), len(u_vec)
            )

        if target is not None:
            # Don't rely on this.
            for n, t in enumerate(_range):
                x = dt * dynamics_func(x, u_vec[n]) + x
                trajectory[n] = x
                if np.linalg.norm(x - target) < tol:
                    print 'Returning shortened trajectory', n
                    return trajectory[:n + 1]

        # These are separate so we don't evaluate a waste "if" each iteration
        else:
            for n, t in enumerate(_range):
                x = dt * dynamics_func(x, u_vec[n]) + x
                trajectory[n] = x
        return trajectory


if __name__ == '__main__':
    print 'Optimization Demo; This is not intended to be run as main'

    B = np.diag([1, 1])
    def dyn_func((x, y, xdot, ydot), u):
        xdotdot = -np.sign(xdot) * 0.1 * xdot ** 2
        ydotdot = -np.sign(ydot) * 0.1 * ydot ** 2
        return np.array([xdot, ydot, xdotdot, ydotdot]) + np.hstack([0.0, 0.0, np.dot(B, u)])

    x0 = np.array([0.0, 10.0, 50.0, 30.0])
    xf = np.array([-10.0, -20.0, 0.0, -0.0])

    dt = 0.1
    time_end = 10

    tic = time()
    control_tape, spline, success = Optimize.direct_transcription(dyn_func, x0, xf, dt=dt, time_end=time_end)
    print control_tape
    toc = time() - tic
    print 'took {} seconds'.format(toc)

    if success:
        # spline = Optimize.forward_sim(dyn_func, x0, control_tape, dt=dt, time_end=time_end)
        visualize_spline(spline[:, :2], 'Pathviz', animate=True, iteration_speed=1, lasts=0, end_point=xf)
    else:
        print("Failed to find feasible solution; If you formatted your dynamics properly, then "
            "this means you _cannot_ reach xf within {} seconds".format(time_end))