from __future__ import division
from cvxopt import matrix, solvers
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
from time import time
from fossen import Azi_Drive


if __name__ == '__main__':
    B = Azi_Drive.thrust_matrix(np.matrix([np.pi, np.pi]).T)

    test_f = np.matrix([25, 25]).T
    print B * test_f

    u_nought = np.matrix([25.0, 25.0]).T
    alpha_nought = np.matrix([0.0, 0.0]).T

    print 'Initial Forces:', u_nought.T, 'Initial Angles:', alpha_nought.T
    fx, fy, moment = -10, -12, 3
    print 'Target force {} N, Target Moment {} N*m'.format((fx, fy), moment)

    for k in range(10):
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
        print 'dForces:\n', u_nought, '\ndAngles:\n', alpha_nought
        print 'Net:\n', Azi_Drive.thrust_matrix(alpha_nought) * u_nought