from scipy.misc import derivative
import numpy as np

class Tools(object):
    @staticmethod
    def partial_derivative(func, var=0, point=[], order=3, dx=1e-6):
        '''Compute a numerical estimate of the  partial derivative of a function 'func'
             at point 'point'
        point - A numpy array setting the point to evaluate around
        var - Take the partial derivative in terms of this argument
        order - Number of points to use in finite-differencing
        dx - Step-size to use in finite-differencing

        '''
        args = np.copy(point[:])
        def wraps(x):
            args[var] = x
            return func(np.array(args))
        return derivative(wraps, point[var], dx=dx)

    @staticmethod
    def jacobian(func, pt, order=3, dx=1e-6):
        '''Compute an numerical extimate of the jacobian at a point defined by a numpy array, 'pt' 
                for a function 'func',

            order - Number of points to use for computing partial_derivatives
            dx - Step-size for finite-differencing

        Special reminder for Jake:
        
        Jacobian F = 
        [dF_1/dx_1, ... , dF_1/dx_n]
        [dF_2/dx_1, ... , dF_2/dx_n]
         ...        ...         ...
        [dF_m/dx_1  ... , dF_m/dx_n]

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

def clamp_angles(input_angle):
    angle = np.clip(input_angle, -np.pi/2, np.pi/2)
    if input_angle != angle:
        print "------CHANGE COMPATIBILITY-----"
    go_ang = (2 * angle) + np.pi
    return go_ang
    if go_ang > 0.0:
        return go_ang % (2 * np.pi)
    else:
        return ((2 * np.pi) - (go_ang % (2 * np.pi)))



def exfun((x, y)):
    return (x ** 2) + (y ** 2)

def zfun((x, y)):
    return np.array([(x ** 2) + (y ** 3), 3 * y])


if __name__ == '__main__':
    tests = [-np.pi/2, 0.0, np.pi/2]
    for test in tests:
        print clamp_angles(test)