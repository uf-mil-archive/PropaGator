#!/usr/bin/env python
PKG='test_azi_drive'
import sys
import unittest

from azimuth_drive import Azi_Drive
from azimuth_drive import Tools
from time import time
import numpy as np


class Test_Azi_Drive(unittest.TestCase):
    '''Unit tests for the Azi Drive package
    TODO:
        - Add convexity tests for the objective function
            -- Requires restructuring of the way objective(*) is defined and called
            -- Is the best way to test that an objective function will work
    '''

    def setUp(self):
        self.maxDiff = None

    def test_thrust_matrix(self):
        net = Azi_Drive.thrust_matrix((0.0, 0.0)) * np.matrix([10, 10]).T
        self.assertTrue(np.allclose(net.T, [-20, 0, 0]))

    def test_net_force(self):
        net = Azi_Drive.net_force((0.0, 0.0), (10.0, 10.0))
        assert np.allclose(net.T, [-20, 0, 0]), "{}".format(net)

    def test_singularity(self):
        cost = Azi_Drive.singularity_avoidance((0.0, 0.0))

    def test_allocation(self):
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
            net = Azi_Drive.net_force(alpha_nought, u_nought)
            print 'Asking thrusts:\n', u_nought
            print 'Asking angles:\n', alpha_nought
            print 'Net:\n', net
            difference = net - tau, 
            success = np.linalg.norm(net - tau) < 0.1
            print 'Difference\n', difference
            return success


        tests = {
            (0, 0, 0): True,
            (20, 70, -3): True,
            (100, 0, 0): True,
            (0, 0, 5): True,
            (180, 0, 0): True,
            (200, 0, 0): True,
            (100, 20, -3): True,
            (120, 15, 12): True,
        }
        results = {}
        for test in tests.keys():
            print 'Testing', test
            result = run_test(*test)
            results[test] = result
        self.assertEqual(tests, results)
        # self.assertEquals(1, 1, "1!=1")

    def test_jacobian_test_1(self):
        def test_1((x, y)):
            return (x ** 2) + (y ** 2)

        jacobian = Tools.jacobian(test_1, np.array([1.0, 1.0]))
        self.assertTrue(np.allclose(jacobian, [2.0, 2.0]))

    def test_jacobian_test_2(self):
        def test_2((x, y)):
            return np.array([(x ** 2) + (y ** 3), 3 * y])

        exp_result = np.matrix([
            [2, 3],
            [0, 3],
        ])

        jacobian = Tools.jacobian(test_2, np.array([1.0, 1.0]))
        self.assertTrue(np.allclose(jacobian, exp_result))

    def test_jacobian_singularity_avoidance(self):
        jacobian = Tools.jacobian(Azi_Drive.singularity_avoidance, np.array([1.0, 1.0]), dx=0.1)
        self.assertEqual(jacobian.shape, (1, 2))

    def test_jacobian_thrust_matrix(self):
        u = np.matrix([10.0, 10.0])
        B = Azi_Drive.thrust_matrix

        def testable_thrust_matrix((alpha_1, alpha_2)):
            u = np.matrix([10.0, 10.0])
            alpha = (alpha_1, alpha_2)
            Btimes_u = B(alpha) * u.T
            return Btimes_u.getA1()

        jacobian = Tools.jacobian(testable_thrust_matrix, np.array([1.0, 1.0]), dx=0.1)




if __name__ == '__main__':
    import rosunit
    rosunit.unitrun(PKG, 'test_azi_drive', Test_Azi_Drive)
