from scipy.misc import derivative
import numpy as np

class Tools(object):
    @staticmethod
    def partial_derivative(func, var=0, point=[], order=3, dx=1e-6):
        args = np.copy(point[:])
        def wraps(x):
            args[var] = x
            return func(np.array(args))
        return derivative(wraps, point[var], dx=dx)

    @staticmethod
    def jacobian(func, pt, order=3, dx=1e-6):
        '''Compute an numerical extimate of the jacobian at pt=pt for a function 'func',

        Reminder for Jacob:
        J = 
        [dF_1/dx1, ... , dF_1/dx_n]
        [dF_2/dx1, ... , dF_1/dx_n]

        '''
        jac = [None] * len(pt)

        for index, dim in enumerate(pt):
            jac[index] = Tools.partial_derivative(func, index, pt, order, dx)

        jacobian = np.matrix(jac)
        # Check if this is a scalar valued function
        if jacobian.shape[0] == 1:
            return jacobian
        else:
            return jacobian.T


def exfun((x, y)):
    return (x ** 2) + (y ** 2)

def zfun((x, y)):
    return np.array([(x ** 2) + (y ** 3), 3 * y])


if __name__ == '__main__':
    print Tools.jacobian(zfun, np.array([1.0, 1.0]))
    print Tools.jacobian(exfun, np.array([0.0, 0.0]))
