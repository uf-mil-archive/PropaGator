from scipy.misc import derivative
import numpy as np

class Tools(object):
    @staticmethod
    def partial_derivative(func, var=0, point=[], order=3, dx=1e-6):
        args = point[:]
        def wraps(x):
            args[var] = x
            return func(*args)
        return derivative(wraps, point[var], dx=dx)

    @staticmethod
    def jacobian(func, pt, order=3, dx=1e-6):
        jac = [None]*len(pt)
        for index, dim in enumerate(pt):
            jac[index] = Tools.partial_derivative(func, index, pt, order, dx)

        return np.matrix(jac).T

def exfun(x, y):
    return (x ** 2) + (y ** 2)

def zfun(x, y):
    return np.array([(x ** 2) + (y ** 3), 3 * y])

def testfun(alph1, alph2):
    print alph1, alph2
    almat = np.matrix([alph1, alph2]).T
    return np.array((B(almat) * u).T[0, :])[0]


if __name__ == '__main__':
    from azimuth_drive import Azi_Drive
    # print Tools.partial_derivative(exfun, 1, [2, 1])
    # print Tools.jacobian(zfun, [1, 1])

    u = np.matrix([20, 20]).T
    B = Azi_Drive.thrust_matrix
    print Tools.jacobian(testfun, [1, 1])
