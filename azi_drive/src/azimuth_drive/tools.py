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
    def jacobian(func, pt, order=3, dx=1e-6, scalar_func=False):
        jac = [None]*len(pt)
        for index, dim in enumerate(pt):
            jac[index] = Tools.partial_derivative(func, index, pt, order, dx)
        
        jacobian = np.matrix(jac)
        # Check if this is a scalar valued function
        if jacobian.shape[0] == 1:
            return jacobian
        else:
            return jacobian.T

def exfun((x, y)):
    print x,y 
    return (x ** 2) + (y ** 2)

def zfun((x, y)):
    return np.array([(x ** 2) + (y ** 3), 3 * y])

def thrust_matrix((alpha_1, alpha_2)):
    offsets = [
        np.array([0.15,  -0.3]),
        np.array([0.15,  0.3]),
    ]
    alpha = (alpha_1, alpha_2)
    matrix = []
    for i in range(2):
        c = -np.cos(alpha[i])
        s = -np.sin(alpha[i])

        l_x, l_y = offsets[i]
        col = np.array([
            c, 
            s, 
            np.dot([-l_y, l_x], np.array([c, s]))
        ])
        matrix.append(col)
    # If we convert this directly it will be rows, must use transpose
    return np.matrix(matrix).T

def testable_thrust_matrix((alpha_1, alpha_2)):
    u = np.matrix([10.0, 10.0])
    alpha = (alpha_1, alpha_2)
    Btimes_u = B(alpha) * u.T
    return Btimes_u.A1

    
if __name__ == '__main__':
    print Tools.jacobian(zfun, np.array([1.0, 1.0]))
    u = np.matrix([10.0, 10.0])
    B = thrust_matrix
    print Tools.jacobian(testable_thrust_matrix, np.array([0.0, 0.0]))
    print Tools.jacobian(exfun, np.array([0.0, 0.0]), scalar_func=True)
