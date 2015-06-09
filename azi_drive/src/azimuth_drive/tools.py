from scipy.misc import derivative
from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
from geometry_msgs.msg import Quaternion, Vector3
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
        print "------ANGLE RANGE ERROR-----"
    go_ang = (2 * angle) + np.pi
    return go_ang


def exfun((x, y)):
    return (x ** 2) + (y ** 2)


def zfun((x, y)):
    return np.array([(x ** 2) + (y ** 3), 3 * y])



class line:
    def __init__(self, p1, p2):
        self.p1 = p1
        self.p2 = p2
        self.s = p2 - p1    # Shifted vector
        if self.s.any():
            self.hat = self.s / np.linalg.norm(self.s)
            x_hat = np.zeros_like(self.hat)
            x_hat[0] = 1
            self.angle = np.arccos(np.dot(x_hat, self.hat))
        else:
            rospy.logerr('0 length line in tank steer trajectory generator')
            self.s = np.array([1, 0, 0])
            self.norm = np.array([1, 0, 0])
            self.angle = 0

    # Projects a point onto the line
    # Returns a vector to the projection
    def proj_pt(self, pt):
        # Shift everything p1
        pt = pt - self.p1
        # Project in the shifted cords then shift back
        return ( np.dot(pt, self.s) / np.dot(self.s, self.s) ) * self.s + self.p1

#def linear_velocity_from_posetwist(pt):
#   return linear_velocity_from_twist(pt)

#def linear_velocity_from_twist(tws):
#   return xyz_array(tws.linear)

def position_from_posetwiststamped(pts):
    return position_from_posetwist(pts.posetwist)

def position_from_posetwist(pt):
    return position_from_pose(pt.pose)

def position_from_pose(p):
    return xyz_array(p.position)

def orientation_from_posetwiststamped(pts):
    return orientation_from_posetwist(pts.posetwist)

def orientation_from_posetwist(pt):
    return orientation_from_pose(pt.pose)

def orientation_from_pose(p):
    return quat_to_rotvec(xyzw_array(p.orientation))

def vector3_from_xyz_array(xyz):
    return Vector3(xyz[0], xyz[1], xyz[2])

def quaternion_from_xyzw_array(xyzw):
    return Quaternion(xyzw[0], xyzw[1], xyzw[2], xyzw[3])

def quaternion_from_rotvec(rot):
    return quaternion_from_xyzw_array(rotvec_to_quat(rot))

# Converts rotation vector to [x, y, z] to unit vector in the pointed to direction
#   Since we only care about orientation in the x y plane we ignore the x y components of the rotation vector
def normal_vector_from_rotvec(rot):
    theta = rot[2] #- np.pi     # Shift the angle pi degrees since pi is considered strait forward
    return np.array([np.cos(theta), np.sin(theta), 0])

def normal_vector_from_posetwist(pt):
    return normal_vector_from_rotvec(orientation_from_posetwist(pt))

if __name__ == '__main__':
    tests = [-np.pi/2, 0.0, np.pi/2]
    for test in tests:
        print clamp_angles(test)