import numpy as np
import cvxopt
from scipy import optimize, misc
from visualize import visualize_spline
from time import time


class Optimize(object):
    @classmethod
    def direct_transcription(self):
        '''direct_transcription()
        Compute a feasible optimal trajectory via direct transcription using sequential least-squares programming (slsqp)

        Concept:
            x[n + 1] = Ax + Bu

        Bibliography:
            [1] Olivares, Alberto; Direct Transcription Methods for Optimal Control [Online]
                Available: http://www.tsc.urjc.es/~staffetti/mcco/slides_mcco/talk_olivares.pdf
            [2] Tedrake, Russ; Underactuated Robotics, Lec 9-10 [Online]
                Available: https://courses.edx.org/courses/MITx/6.832x/3T2014/courseware/week5/week5_overview/

        '''

        dynamics = np.diag([1, 1])
        x0 = np.array([0.0, 0.0])
        B = np.diag([1, 1])

        # initial_guesses = [0.0 for i in ]
        constraints = {}
        for k in range(10):
            # Constraint 1 
            constraints.update(
                {
                    'type': 'ineq',
                    'fun': lambda u: 'a'
                }
            )

        def objective(u_vector):

            return sum(sub_objective(u_vector))

        optimize.minimize(
            fun=objective,
            x0=initial_guesses,
            method='SLSQP',
            constraints=time_constraints
        )

        return


    @classmethod
    def forward_sim(self, dynamics_func, x0, u_vec, dt=0.1, time_end=10):
        x = np.copy(x0);
        _range = np.arange(0, time_end, dt)
        trajectory = np.zeros((len(_range), 2))

        for n, t in enumerate(_range):
            x = dt * dynamics_func(x, [0, 0]) + x
            trajectory[n] = x
        return trajectory


if __name__ == '__main__':
    # Optimize.direct_collocation()
    print "Running optimization demo"
    # from spline.spline import Hermite, Waypoint
    start_pt = np.array([10.0, -70.0])

    # A = np.diag([0.2, 0.5])
    A = np.array([
        [-1.0, 0.4],
        [0.32, -9],
    ])
    B = np.diag([1., 1.])

    dyn_func = lambda x, u: np.dot(A, x) + np.dot(B, u)
    tic = time()
    dspline = Optimize.forward_sim(dyn_func, start_pt, NotImplemented, dt=0.05, time_end=100)
    toc = time() - tic
    print 'Finished generating spline of length', len(dspline), 'took', toc, 'seconds'
    # dspline = Hermite.spline(Waypoint(start_pt, start_m), Waypoint(end_pt, end_m))
    tic = time()
    visualize_spline(dspline, 'Path Visualization', animate=True, iteration_speed=5)
    toc = time() - tic
    print 'Visualization took', toc, 'seconds'
