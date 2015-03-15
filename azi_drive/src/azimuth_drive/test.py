from __future__ import division
from cvxopt import matrix, solvers
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
from time import time
from azimuth_drive import Azi_Drive

def run_test(fx, fy, moment):
    u_nought = np.matrix([0.0, 0.0]).T
    alpha_nought = np.matrix([np.pi, np.pi]).T

    tau = np.matrix([fx, fy, moment]).T
    for k in range(20):
        tic = time()
        p = Azi_Drive.map_thruster(fx_des=fx, fy_des=fy, m_des=moment, alpha_0=alpha_nought, u_0=u_nought)
        # print p
        # assert p.success, "Failed to achieve {}".format((fx, fy, moment))
        toc = time() - tic

        delta_angle_1, delta_angle_2, delta_u_1, delta_u_2 = p.x

        d_theta = np.matrix([delta_angle_1, delta_angle_2]).T
        d_force = np.matrix([delta_u_1, delta_u_2]).T
        alpha_nought += d_theta
        u_nought += d_force
        # print 'Angle Change:\n', d_theta, 'Force Change:\n', d_force
        # print 'dForces:\n', u_nought, '\ndAngles:\n', alpha_nought
    net = Azi_Drive.thrust_matrix(alpha_nought) * u_nought
    print 'Asking thrusts:\n', u_nought
    print 'Asking angles:\n', alpha_nought
    print 'Net:\n', net
    print 'Difference\n', net - tau, np.linalg.norm(net - tau) < 0.1


if __name__ == '__main__':

    tests = [
        (0, 0, 0),
        (20, 70, -3),
        (100, 0, 0), 
        (0, 0, 5),
        (180, 0, 0),
        (200, 0, 0),
        (100, 20, -3),
        (120, 15, 12),
    ]
    for test in tests:
        # print 'Testing', test
        # run_test(*test)
        pass

    u_nought = np.matrix([0.0, 0.0]).T
    alpha_nought = np.matrix([3.61, 1.26]).T

    print 'Initial Forces:', u_nought.T, 'Initial Angles:', alpha_nought.T
    fx, fy, moment = 20, 70, 3
    print 'Target force {} N, Target Moment {} N*m'.format((fx, fy), moment)


    for k in range(20):
        tic = time()
        p = Azi_Drive.map_thruster(fx_des=fx, fy_des=fy, m_des=moment, alpha_0=alpha_nought, u_0=u_nought)
        print p
        toc = time() - tic
        print 'Iteration {} took {} seconds'.format(k, toc)
        delta_angle_1, delta_angle_2, delta_u_1, delta_u_2 = p.x

        d_theta = np.matrix([delta_angle_1, delta_angle_2]).T
        d_force = np.matrix([delta_u_1, delta_u_2]).T
        alpha_nought += d_theta
        u_nought += d_force
        print 'Angle Change:\n', d_theta, 'Force Change:\n', d_force
        print 'Forces:\n', u_nought, '\nAngles:\n', alpha_nought
        print 'Net:\n', Azi_Drive.thrust_matrix(alpha_nought) * u_nought

    print 'feasibility'
    print Azi_Drive.least_squares_test([0, 0,], [fx, fy, moment])
