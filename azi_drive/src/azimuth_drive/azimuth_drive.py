from __future__ import division
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
import time
from tools import Tools
from azi_drive import azi_cvxbind

'''
Ideas for optimization:
    - Compute all of the linearizations in advance

TODO:
    - Split the mapper into a minimzation function (where we can test the minmization object)
        and a mapper, which does the actual mapping
        --> easier unit testing

    - If we fail to find a solution, try again with an angle slightly outside of jacobian singularity

'''

class Azi_Drive(object):    

    center_of_mass = np.array([0.0, 0.0])  # This is NOT actually true!
    positions = [
        # l_x, l_y, offsets from [2]
        # (-0.15, -0.3),
        # (-0.15, 0.3),

        # Taken from simulation
        (-0.7239, -0.3048),
        (-0.7239, 0.3048),
    ]

    # Max/min force from each thruster
    u_max = 100
    u_min = -70
    # min(abs(u)), about 10 Newtons
    thruster_min = 10

    # Max/min angle of each thruster
    alpha_max = np.pi / 2
    alpha_min = -np.pi / 2

    # Max/min change in thruster angle (Rotation speed of servo, really)
    delta_alpha_max = np.pi / 10
    delta_alpha_min = -delta_alpha_max

    # Derivative of cost for greater trust (Not doing any nonlinear modelling for power cost)
    power_cost_scale = 20.0

    @classmethod
    def set_delta_alpha_max(self, bound):
        self.delta_alpha_max = bound
        self.delta_alpha_min = -bound

    @classmethod
    def set_alpha_bound(self, (_min, _max)):
        self.alpha_max = _max
        self.alpha_min = _min

    @classmethod
    def set_thrust_bound(self, (_min, _max)):
        self.u_max = _max
        self.u_min = _min

    @classmethod
    def thrust_matrix(self, alpha):
        '''Produce a thruster effort -> vehicle force conversion matrix for a given angle
        B(alpha) * [T1, T2].T = [Fx, Fy, Torque].T

        - alpha is a np array of angles
        - positions is an array of positions

        '''
        assert len(alpha) == len(self.positions), ("ERROR: Number of thruster positions and set " +
            "angles differs {} vs {}").format(len(alpha), len(self.positions))

        thruster_matrix = []
        for thruster, position in enumerate(self.positions):
            
            l_x, l_y = np.subtract(position, self.center_of_mass)
            angle = alpha[thruster]
            c = np.cos(angle)
            s = np.sin(angle)
            thruster_column = np.transpose(
                np.array([[
                    c, # f in x
                    -s, # f in y
                    np.cross((l_x, l_y), (c, -s)) # Moment about Z
                ]])
            )
            thruster_matrix.append(thruster_column)

        return np.hstack(thruster_matrix)

    @classmethod
    def singularity_avoidance(self, alpha):
        '''Cost for approaching singularity
            In practice, this cost appears to be generally very small
        '''
        epsilon = 0.1
        q = 1
        B_alpha = self.thrust_matrix(alpha)
        return q / (epsilon + np.linalg.det(np.dot(B_alpha, B_alpha.T)))

    @classmethod
    def power_consumption(self, u):
        '''Power consumption is assumed to be perfectly linear in u
        (That is to say, effort = k*u where k is some constant)
        '''
        return u * self.power_cost_scale

    @classmethod 
    def thrust_jacobian(self, alpha_0, u_0):
        '''Evaluate the jacobian of 
            J: f(v) = B(alpha) * u 
        at u_0, alpha_0, 
        '''
        def thrust_function(alpha):
            return np.dot(self.thrust_matrix(alpha), u_0)

        return Tools.jacobian(thrust_function, alpha_0)

    @classmethod
    def net_force(self, alpha, u):
        '''Determine the net force given an angle list and thruster force list'''
        B = self.thrust_matrix(alpha)
        return np.dot(B, u)

    @classmethod
    def map_thruster(self, fx_des, fy_des, m_des, alpha_0, u_0, test_plot=False):
        '''Compute the optimal thruster configuration for Propagator using the Fossen method
        To make this work, alpha_0 and u_0 must be varied

        u_0 -> Root force
        alpha_0 -> Root angle (initial angle)

        We compute based on the minimization of the following nonlinear cost function:

        power(u) + s.T * G * s + (delta_theta).T * Q * (delta_theta) + (q / det(B(theta) * B(theta).T)) [1]

        It turns out that this problem is approximately convex in the range of allowable delta_theta
            so that if we linearize the problem, we can find an acceptable accurate minimum

        It is due to this linearization that we are computing over the FOTA and using u_0 + delta_u 
            instead of u directly.

        Arguments:
            fx_des, fy_des - X and Y components of desired force
            m_des - Desired torque about the Z axis
            alpha_0 - Current or initial thruster orientations (angle from the x-axis, positive left)
            u_0 - Current or initial thruster efforts
            test_plot - This should always be false in practice. Used to visually test for objective convexity

        Glossary of Terms:
            FOTA - First Order Taylor Approximation
            power(u) - Cost for the power consumption of a thruster exerting an effort, u
            Tau - Desired action (Fx, Fy, Torque about Z)
            Underactuated - B (The control-input-matrix, or thruster_matrix) is noninvertible
            B, or thrust_matrix - Matrix that maps a control input, "u" at a particular "alpha" 
                to a net force and torque experienced by the boat
            Singularity - when B becomes singular (noninvertible), we try to avoid 
                thruster configurations where the boat becomes underactuated

        Glossary of Variables:
            s - Control error (What the boat is doing minus Tau), you want this to be zero
            alpha - vector of angles
            alpha_0 - vector of current thruster orientations
            d_alpha - vector of angle changes
            u - vector of thruster forces
            u_0 - vector of current thruster forces
            d_u - vector of force changes
            q - boat manueverability constant
            epsilon - Used to avoid numerical issues when dividing by numbers near zero


        Bibliography:
            [1] Tor A. Johansen, Thor I. Fossen, and Svein P. Berge, 
                "Constrained Nonlinear Control Allocation With Singularity 
                    Avoidance Using Sequential Quadratic Programming",
                See: http://www.fossen.biz/home/papers/tcst04.pdf

            [2] Darsan Patel, Daniel Frank, and Dr. Carl Crane, 
                "Controlling an Overactuated Vehicle with Application to an Autonomous
                    Surface Vehicle Utilizing Azimuth Thrusters",
                See: http://bit.ly/1ayOdlg

            [3] Mattingley, Jacob "CVXGEN"
                See: http://cvxgen.com/docs/license.html

        Author: Jacob Panikulam

        '''
        # Early Checks
        # Convert to numpy arrays
        alpha_0 = np.array(alpha_0).astype(np.double)
        u_0 = np.array(u_0).astype(np.double)

        # Desired
        tau = np.array([fx_des, fy_des, m_des]).astype(np.double)

        # Linearizations
        # Singularity unused
        # d_singularity = Tools.jacobian(self.singularity_avoidance, pt=alpha_0, order=3, dx=0.01)
        dBu_dalpha = self.thrust_jacobian(alpha_0, u_0).astype(np.double)
        d_power = self.power_cost_scale
        B = self.thrust_matrix(alpha_0).astype(np.double)

        Q = np.diag([10., 10., 30.]).astype(np.double)
        Ohm = np.diag([5., 5.]).astype(np.double)

        soln = azi_cvxbind.ksolve(
            u_0.flatten(),
            alpha_0.flatten(),
            Ohm.flatten(),
            Q.flatten(),
            B.transpose().flatten(),  # Transpose because cvxgen is column-major indexed
            dBu_dalpha.transpose().A1,
            self.u_max,
            self.u_min,
            self.alpha_min,
            self.alpha_max,
            self.delta_alpha_max,
            tau.flatten(),
        )

        du_1, du_2, da_1, da_2 = soln
        delta_u = np.array([du_1, du_2]).astype(np.double)
        delta_alpha = np.array([da_1, da_2]).astype(np.double)

        success = True  # Minimization always succeeds, QP's, baby!
        return delta_alpha, delta_u, success


if __name__ == '__main__':
    print "Don't execute this as main."
    tic = time.time()
    u_0 = np.array([0.0, 0.0])
    alpha_0 = np.array([0.0, 0.0])

    for k in range(500):
        delta_alpha, delta_u, s =  Azi_Drive.map_thruster(
            50, 30, 10, 
            alpha_0=alpha_0, 
            u_0=u_0,
        )
        toc = time.time() - tic
        # print 'took {} seconds'.format(toc)
        u_0 += delta_u
        alpha_0 += delta_alpha
        # print u_0
    print Azi_Drive.net_force(alpha_0, u_0)
    print 'took {} seconds'.format(toc)
