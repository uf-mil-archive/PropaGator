# Parrotfish Drive
'''
ISSUES:
The algorithm fails to converge in cases where the commanded force is very far from current force

TODO:
- Add water drag terms
- Use CVXOPT instead of scipy minimize (Should be orders of magnitude faster)
-- Or try implementation in C++

Bibliography:
[1] Constrained Nonlinear Control Allocation With Singularity Avoidance Using Sequential Quadratic Programming,
    Tor A. Johansen, Thor Fossen, Svein Beirge

Conventions: 

X+ : Backward
Y+ : Right
Z+ : Down

^ is boat
__________ Y+
|\
|^\
|^ \
|^  \
|    Z+
X+
'''
from __future__ import division
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
from time import time

class Azi_Drive(object):

    u_max = 100
    u_min = -100
    alpha_max = 3 * np.pi
    alpha_min = -3 * np.pi
    delta_alpha_max = 0.1
    delta_alpha_min = -delta_alpha_max

    manueverability = 1
    offsets = [
        np.array([0.15,  -0.3]),
        np.array([0.15,  0.3]),
    ]

    power_cost_scale = 1

    @classmethod
    def set_max_delta_alpha(self, delta_alpha):
        self.delta_alpha_max = np.fabs(delta_alpha)
        self.delta_alpha_min = - self.delta_alpha_max

    @classmethod
    def thrust_matrix(self, alpha):
        '''Produce a thruster effort -> vehicle force conversion matrix for a given angle set
        (Alpha is a vector)
        '''
        matrix = []
        for i in range(2):
            c = -np.cos(alpha[i, 0])
            s = -np.sin(alpha[i, 0])

            l_x, l_y = self.offsets[i]
            col = np.array([
                c, 
                s, 
                np.dot([-l_y, l_x], np.array([c, s]))
            ])
            matrix.append(col)
        # If we convert this directly it will be rows, must use transpose
        return np.matrix(matrix).T

    @classmethod
    def singularity_avoidance(self, alpha):
        '''Cost for approaching singularity
        epsilon is to avoid issues with division by 0,
        q is a manueverability constant

        Higher q -> higher manueverability, higher power consumption
        Lower q -> Less manueverability, less power consumption
        '''
        epsilon = 0.1
        q = self.manueverability
        B_alpha = self.thrust_matrix(alpha)
        return q / (epsilon + np.linalg.det(B_alpha * B_alpha.T))

    @classmethod
    def power_consumption(self, u):
        '''Power consumption is assumed to be perfectly linear in u
        (That is to say, effort = k*u where k is some constant)
        '''
        return self.power_cost_scale * u


    @classmethod
    def map_thruster(self, fx_des, fy_des, m_des, alpha_0, u_0):
        '''Optimize for a single thruster using the Fossen method
        To make this work, alpha_0 and u_0 must be varied

        u_0 -> Root force
        alpha_0 -> Root angle (initial angle)


        We compute based on the minimization of the following nonlinear cost function:

        power(u) + s.T * G * s + (delta_theta).T * Q * (delta_theta) + (q / det(B(theta) * B(theta).T))

        It turns out that this problem is approximately linear in the range of allowable delta_theta
            so that if we linearize the problem, we can find an acceptable accurate minimum

        It is due to this linearization that we are computing over the FOTA and using u_0 + delta_u instead of u directly.


        Glossary:
            FOTA: First Order Taylor Approximation
            power(u): Cost for the power consumption of a thruster exerting an effort, u
            Tau: Desired forces
            s: Control error (What the boat is doing minus Tau)
            alpha: vector of angles
            alpha_0: vector of current thruster orientations
            d_alpha: vector of angle changes

            B, or thrust_matrix: Matrix that maps a control input, "u" at a particular "alpha" to a net force exerted by the boat
            singularity: when B becomes singular (noninvertible)
        '''

        tau = np.matrix([fx_des, fy_des, m_des]).T # Desired

        d_singularity = misc.derivative(
            func=self.singularity_avoidance, 
            x0=alpha_0,
            order=3, # Number of points
        )

        d_power = self.power_cost_scale

        dB_dalpha = misc.derivative(
            func=self.thrust_matrix,
            x0=alpha_0,
            order=3,
        )
        B = self.thrust_matrix(alpha_0)

        def get_s((delta_angle, delta_u)):
            '''Equality constraint
            'S' is the force/torque error
            '''
            B_1 = dB_dalpha[:, 0] * u_0[0] * delta_angle[0]
            B_2 = dB_dalpha[:, 1] * u_0[1] * delta_angle[1]

            s = -((B * delta_u) + (B * u_0) + B_1 + B_2 - tau)
            # print 'Attempting s for :\n', delta_angle, '\n', delta_u
            # print 'Value for s:\n', s
            # s = -(B * delta_u) + (B * u_0) + (dB_dalpha * u_0) * delta_angle - tau
            return s

        def objective((delta_angle_1, delta_angle_2, delta_u_1, delta_u_2)):
            '''Objective function for minimization'''

            delta_u = np.matrix([delta_u_1, delta_u_2]).T
            delta_angle = np.matrix([delta_angle_1, delta_angle_2]).T
            s = get_s((delta_angle, delta_u))

            # Sub-costs
            power = np.sum(d_power * delta_u)

            G = thrust_error_weights = np.diag([1000, 1000, 1000])
            thrust_error = s.T * G * s

            Q = angle_change_weight = np.diag([10, 10])
            angle_change = delta_angle.T * angle_change_weight * delta_angle
            # angle_change = (angle_change_weight * delta_angle) ** 2

            singularity = d_singularity * delta_angle

            cost = power + thrust_error + angle_change + singularity
            return cost[0, 0]

        u_max = self.u_max
        u_min = self.u_min
        alpha_max = self.alpha_max
        alpha_min = self.alpha_min
        delta_alpha_min = self.delta_alpha_min
        delta_alpha_max = self.delta_alpha_max

        minimization = optimize.minimize(
            fun=objective,
            x0=(0, 0, 0, 0),
            method='SLSQP',
            constraints=[
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_u_1 + u_0[0, 0] - u_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_u_1 + u_0[0, 0] - u_min},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_1 + alpha_0[0, 0] - alpha_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_1 + alpha_0[0, 0] - alpha_min},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_1 - delta_alpha_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_1 - delta_alpha_min},

                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_u_2 + u_0[1, 0] - u_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_u_2 + u_0[1, 0] - u_min},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_2 + alpha_0[1, 0] - alpha_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_2 + alpha_0[1, 0] - alpha_min},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_2 - delta_alpha_max)},
                {'type': 'ineq', 'fun': lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_2 - delta_alpha_min},
            ]
        )
        return minimization

