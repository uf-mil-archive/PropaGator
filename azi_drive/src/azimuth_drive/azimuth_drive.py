from __future__ import division
from scipy import optimize, misc
# We need: minimize, and misc.derivative
import numpy as np
import time
from tools import Tools

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

        Author: Jacob Panikulam

        '''
        # Early Checks
        # Convert to numpy arrays
        alpha_0 = np.array(alpha_0)
        u_0 = np.array(u_0)

        # Desired
        tau = np.array([fx_des, fy_des, m_des])
        if np.allclose([0.0, 0.0, 0.0], tau, atol=5.0):
            return np.array([0.0, 0.0]), -u_0, True

        # Linearizations
        d_singularity = Tools.jacobian(self.singularity_avoidance, pt=alpha_0, order=3, dx=0.01)
        dBu_dalpha = self.thrust_jacobian(alpha_0, u_0)
        d_power = self.power_cost_scale

        B = self.thrust_matrix(alpha_0)

        def get_s((delta_angle, delta_u)):
            '''Equality constraint
            'S' is the force/torque error

                s + B(alpha_0) * delta_u + jac(B*u)*delta_alpha 
                    = -tau - B(alpha_0) * u_0
            -->   
            
                s = (-B(alpha_0) * delta_u) - jac(B*u)*delta_alpha - tau - B*alpha_0 *u_0
            '''
            s = -np.dot(B, delta_u) - np.dot(dBu_dalpha, delta_angle) + tau - np.dot(B, u_0)
            return s

        def objective((delta_angle_1, delta_angle_2, delta_u_1, delta_u_2)):
            '''Objective function for minimization'''

            delta_u = np.array([delta_u_1, delta_u_2])

            delta_angle = np.array([delta_angle_1, delta_angle_2])
            s = get_s((delta_angle, delta_u))

            # Sub-costs
            # P = np.diag([1.0, 1.0])
            # power = Tools.quadratic(delta_angle + alpha_0, P)
            power = 0

            G = thrust_error_weights = np.diag([20.0, 20.0, 40.0])
            thrust_error = (s * G * s.T).A1

            Q = angle_change_weight = np.diag([4.0, 4.0])
            angle_change = np.dot(
                np.dot(delta_angle, angle_change_weight),
                np.transpose(delta_angle)
            )

            A = angle_weight = np.diag([2.0, 2.0])
            angle_cost = Tools.quadratic((delta_angle + alpha_0), A)

            singularity = np.dot(d_singularity, delta_angle).A1

            # Note, the error will increase when adding power and singularity costs
            # cost = power + thrust_error + angle_change + singularity + angle_cost
            cost = thrust_error
            return cost

        # Formatted as such because I thought I would do something 
        #  funky by varying the bounds
        u_max = self.u_max
        u_min = self.u_min
        alpha_max = self.alpha_max
        alpha_min = self.alpha_min
        delta_alpha_min = self.delta_alpha_min
        delta_alpha_max = self.delta_alpha_max
        thruster_min = self.thruster_min

        # Plot the cost function (visually observe convexity)
        if test_plot:
            def objective_r(*args):
                return objective(args)

            x_range = np.linspace(-np.pi, np.pi, 100)
            y_range = np.linspace(-np.pi, np.pi, 100)

            X, Y = np.meshgrid(x_range, y_range)
            f = np.vectorize(objective_r)
            Z = f(X, Y, np.zeros(len(X)), np.zeros(len(Y)))

            fig = plt.figure(figsize=(14, 6))
            ax = fig.add_subplot(1, 2, 1, projection='3d')
            p = ax.plot_surface(X, Y, Z, rstride=4, cstride=4, linewidth=0)
            plt.show()

        minimization = optimize.minimize(
            fun=objective,
            x0=(0.1, 0.1, 0.0, 0.0),
            method='SLSQP',
            constraints=[
                # Inequality requires nonnegativity, i.e. fun(o) >= 0
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_u_1 + u_0[0]) + u_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_u_1 + u_0[0] - u_min},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_1 + alpha_0[0]) + alpha_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_1 + alpha_0[0] - alpha_min},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_1) + delta_alpha_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_1 - delta_alpha_min},

                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_u_2 + u_0[1]) + u_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_u_2 + u_0[1] - u_min},

                # Account for minimum thruster output
                # {'type': 'ineq', 'fun': 
                    # lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): abs(delta_u_1 + u_0[0]) - thruster_min},
                # {'type': 'ineq', 'fun': 
                    # lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): abs(delta_u_2 + u_0[1]) - thruster_min},

                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_2 + alpha_0[1]) + alpha_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_2 + alpha_0[1] - alpha_min},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): -(delta_angle_2) + delta_alpha_max},
                {'type': 'ineq', 'fun': 
                    lambda (delta_angle_1, delta_angle_2, delta_u_1, delta_u_2): delta_angle_2 - delta_alpha_min},
            ]
        )
        delta_alpha_1, delta_alpha_2, delta_u_1, delta_u_2 = minimization.x
        delta_alpha, delta_u = np.array([delta_alpha_1, delta_alpha_2]), np.array([delta_u_1, delta_u_2])

        if not minimization.success:
            success = False
            if minimization.message == 'Inequality constraints incompatible':
                pass  # WE FUCT UP!
        else:
            success = True
        return delta_alpha, delta_u, success


if __name__ == '__main__':
    print "Don't execute this as main."
    tic = time.time()
    u_0 = np.array([0.0, 0.0])
    alpha_0 = np.array([0.0, 0.0])

    for k in range(10):


        delta_alpha, delta_u, s =  Azi_Drive.map_thruster(
            100, 10, 5, 
            alpha_0=alpha_0, 
            u_0=u_0,
        )
        toc = time.time() - tic
        print 'took {} seconds'.format(toc)
        u_0 += delta_u
        alpha_0 += delta_alpha
        print u_0
        print Azi_Drive.net_force(alpha_0, u_0)