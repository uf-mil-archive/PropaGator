from __future__ import division
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
from time import time
from azimuth_drive import Azi_Drive



if __name__ == '__main__':


    u_nought = np.matrix([0.0, 0.0]).T
    alpha_nought = np.matrix([0.1, 0.1]).T

    print 'Initial Forces:', u_nought.T, 'Initial Angles:', alpha_nought.T
    fx, fy, moment = 20.0, 0, 0.0
    print 'Target force {} N, Target Moment {} N*m'.format((fx, fy), moment)

    tau = np.matrix([fx, fy, moment]).T
    for k in range(1):
        tic = time()
        p = Azi_Drive.map_thruster(fx_des=fx, fy_des=fy, m_des=moment, alpha_0=alpha_nought, u_0=u_nought)
        print p
        # assert p.success, "Failed to achieve {}".format((fx, fy, moment))
        toc = time() - tic

        delta_angle_1, delta_angle_2, delta_u_1, delta_u_2 = p.x

        d_theta = np.matrix([delta_angle_1, delta_angle_2]).T
        d_force = np.matrix([delta_u_1, delta_u_2]).T
        alpha_nought += d_theta
        u_nought += d_force
        # print 'Angle Change:\n', d_theta, 'Force Change:\n', d_force
        # print 'dForces:\n', u_nought, '\ndAngles:\n', alpha_nought
        net = Azi_Drive.net_force(alpha_nought, u_nought)
        print 'Asking thrusts:\n', u_nought
        print 'Asking angles:\n', alpha_nought
        print 'Net:\n', net
        difference = net - tau, 
        success = np.linalg.norm(net - tau) < 0.1
        print 'Difference\n', difference
