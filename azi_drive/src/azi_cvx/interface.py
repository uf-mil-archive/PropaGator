import azi_cvxbind
import numpy as np
from azimuth_drive import Azi_Drive
from time import time
'''
Problem:
    - The second thruster does not get real inputs

Possible causes:
    - cvxgen has an error with the use of 's'
    - cvxgen has an error with using slack variables in a cost function

    - I am not properly extracting the results (This seems likely)

Problem: 
    - Everything is ridiculous except in the x direction

Possible causes:
    -> try adding a max du (Maybe the problem relates to allowing changes beyond the linearity of the jacobian?)

'''

if __name__ == '__main__':
    # Azi_Drive.positions = [
        # (-0.0, 0.0),
        # (-1., 0.),
    # ]

    u_0 = np.array([0.1, 0.1]).astype(np.double)
    a_0 = np.array([0.1, 0.1]).astype(np.double)

    # Ba = Azi_Drive.thrust_matrix(a_0)
    # jBa_u = Azi_Drive.thrust_jacobian(a_0, u_0)

    a_min = -3 * np.pi / 2
    a_max = 3 * np.pi / 2
    u_min = -100.0
    u_max = 100.
    da_max = 0.1

    tau = np.array([200.0, 200.0, 0.0]).astype(np.double)
    Q = np.diag([10., 10., 30.]).astype(np.double)
    Ohm = np.diag([0., 0.]).astype(np.double)

    tic = time()
    for k in range(200):
        Ba = Azi_Drive.thrust_matrix(a_0).astype(np.double)
        jBa_u = Azi_Drive.thrust_jacobian(a_0, u_0).astype(np.double)

        soln = cvxbind.ksolve(
            u_0.flatten(),
            a_0.flatten(),
            Ohm.flatten(),
            Q.flatten(),
            Ba.transpose().flatten(),
            jBa_u.transpose().A1,
            u_max,
            u_min,
            a_min,
            a_max,
            da_max,
            tau.flatten(),
        )
        # du_1, du_2, da_1, da_2, s1, s2, s3 = soln
        du_1, du_2, da_1, da_2 = soln


        du = np.array([du_1, du_2]).astype(np.double)
        da = np.array([da_1, da_2]).astype(np.double)
        # s = np.array([s1, s2, s3])
        # print 's', s
        # print 'du:', du, '| da:', da
        a_0 += da
        u_0 += du
    print 'u:', u_0, '| alpha:', a_0
    print 'Net Force', Azi_Drive.net_force(a_0, u_0)
    print
    print 'Took {} seconds'.format(time() - tic)


# boost::python::list cvxsolve(
#   boost::python::numeric::array u_0,
#   boost::python::numeric::array a_0,
#   boost::python::numeric::array Ohm,
#   boost::python::numeric::array Q,
#   boost::python::numeric::array Ba,
#   boost::python::numeric::array jBa_u,
#   double u_max, 
#   double u_min, 
#   double a_min, 
#   double a_max, 
#   double da_max,
#   boost::python::numeric::array tau 
# )

