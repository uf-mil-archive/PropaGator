# Parrotfish Drive
'''

FIXXXXXXXXXXXXXXXXXXXXX:
- I was trying to implement our own jacobian estimation, but ran into issues

Now, test.py and azi_drive.py in this folder are the old versions of the file
Try running each of the functions and see if we get the same result from both old and new versions
Also, if we can't, port the new unit tests to the old version



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
from tools import Tools
import numpy as np
from time import time
# from tools import Tools
from mpl_toolkits.mplot3d.axes3d import Axes3D
import matplotlib.pyplot as plt

class Azi_Drive(object):

    u_max = 100
    u_min = -100
    alpha_max = np.pi
    alpha_min = -np.pi
    delta_alpha_max = 0.1
    delta_alpha_min = -delta_alpha_max

    manueverability = 1
    offsets = [
        np.array([0.15,  -0.3]),
        np.array([0.15,  0.3]),
    ]

    power_cost_scale = 10

    @classmethod
    def set_max_delta_alpha(self, delta_alpha):
        self.delta_alpha_max = np.fabs(delta_alpha)
        self.delta_alpha_min = - self.delta_alpha_max

    @classmethod
    def thrust_matrix(self, (alpha_1, alpha_2)):
        '''Produce a thruster effort -> vehicle force conversion matrix for a given angle set
        (Alpha is a vector)
        '''
        alpha = (alpha_1, alpha_2)
        matrix = []
        for i in range(2):
            c = -np.cos(alpha[i])
            s = -np.sin(alpha[i])


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
    def net_force(self, alpha, u):
        if isinstance(alpha, np.matrix):
            alpha = alpha.A1
        if not isinstance(u, np.matrix):
            u = np.matrix(u).T
        return self.thrust_matrix(alpha) * u

    @classmethod
    def least_squares_test(self, alpha, tau):
        '''Compute the least-squares control allocation solution for an angle
        (This one is intended for testing)
        '''
        alpha = np.matrix(alpha).T
        tau = np.matrix(tau).T
        return self.least_squares(alpha, tau)

    @classmethod
    def least_squares(self, alpha, tau):
        '''Compute the least-squares control allocation solution for an angle'''
        return np.linalg.lstsq(self.thrust_matrix(alpha), tau)

    @classmethod
    def singularity_avoidance(self, (alpha_1, alpha_2)):
        '''Cost for approaching singularity
        epsilon is to avoid issues with division by 0,
        q is a manueverability constant

        Higher q -> higher manueverability, higher power consumption
        Lower q -> Less manueverability, less power consumption
        '''
        epsilon = 0.1
        q = self.manueverability
        B_alpha = self.thrust_matrix((alpha_1, alpha_2))
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
            s: Control error (What the boat is doing minus Tau), you want this to be zero
            alpha: vector of angles
            alpha_0: vector of current thruster orientations
            d_alpha: vector of angle changes

            B, or thrust_matrix: Matrix that maps a control input, "u" at a particular "alpha" to a net force exerted by the boat
            singularity: when B becomes singular (noninvertible)
        '''

        tau = np.matrix([fx_des, fy_des, m_des]).T # Desired
        alpha_1, alpha_2 = alpha_0.A1

        # d_singularity = misc.derivative(
        #     func=self.singularity_avoidance, 
        #     x0=alpha_0,
        #     order=5, # Number of points
        #     dx=0.1,
        # )
        d_singularity = Tools.jacobian(self.singularity_avoidance, pt=np.array([alpha_1, alpha_2]), order=3, dx=0.01)

        d_power = self.power_cost_scale

        # dB_dalpha = misc.derivative(
        #     func=self.thrust_matrix,
        #     x0=(0, 0),
        #     order=5,
        #     dx=0.1,
        # )
    
        def linearized_net_force(alpha):
            return self.net_force(alpha, u_0).A1

        dB_dalpha = Tools.jacobian(linearized_net_force, pt=np.array([alpha_1, alpha_2]), order=3, dx=0.01)

        B = self.thrust_matrix((alpha_1, alpha_2))

        def get_s((delta_angle, delta_u)):
            '''Equality constraint
            'S' is the force/torque error
            '''
            # B_1 = dB_dalpha[:, 0] * u_0[0] * delta_angle[0]
            # B_2 = dB_dalpha[:, 1] * u_0[1] * delta_angle[1]



            # s = -((B * delta_u) + (B * u_0) + B_1 + B_2 - tau)
            print 'Attempting s for :\n', delta_angle, '\n', delta_u
            print 'Linearized dleta', (dB_dalpha * delta_angle)
            s = -((B * delta_u) + (B * u_0) + (dB_dalpha * delta_angle) - tau)
            
            # print 'Value for s:\n', s
            return s

        def objective((delta_angle_1, delta_angle_2, delta_u_1, delta_u_2)):
            '''Objective function for minimization'''

            delta_u = np.matrix([delta_u_1, delta_u_2]).T
            delta_angle = np.matrix([delta_angle_1, delta_angle_2]).T
            s = get_s((delta_angle, delta_u))

            # Sub-costs
            power = np.sum(np.power(d_power * (u_0 + delta_u), 2))

            G = thrust_error_weights = np.diag([20, 20, 20])
            thrust_error = s.T * G * s

            Q = angle_change_weight = np.diag([1000, 1000])
            angle_change = delta_angle.T * angle_change_weight * delta_angle
            # angle_change = (angle_change_weight * delta_angle) ** 2

            singularity = (d_singularity * delta_angle).A1

            cost = power + thrust_error + angle_change + singularity
            # cost = thrust_error

            return cost[0, 0]

        def objective_r(*args):
            return objective(args)


        u_max = self.u_max
        u_min = self.u_min
        alpha_max = self.alpha_max
        alpha_min = self.alpha_min
        delta_alpha_min = self.delta_alpha_min
        delta_alpha_max = self.delta_alpha_max

        '''Plot the cost function (visually observe convexity)'''
        # x_range = np.linspace(-np.pi, np.pi, 100)
        # y_range = np.linspace(-np.pi, np.pi, 100)

        # X, Y = np.meshgrid(x_range, y_range)
        # f = np.vectorize(objective_r)
        # Z = f(X, Y, np.zeros(len(X)), np.zeros(len(Y)))

        # fig = plt.figure(figsize=(14, 6))
        # ax = fig.add_subplot(1, 2, 1, projection='3d')
        # p = ax.plot_surface(X, Y, Z, rstride=4, cstride=4, linewidth=0)
        # plt.show()

        def minimize(method):
            minimization = optimize.minimize(
                fun=objective,
                x0=(0.0, 0.0, 0.0, 0.0),
                method=method,
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
        p = minimize('SLSQP')
        if p.success == False:
            print p
            print '-------------------Switching to L-BFGS-B'
            p = minimize('L-BFGS-B')
        return p
