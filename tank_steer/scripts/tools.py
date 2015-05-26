from uf_common.orientation_helpers import xyz_array, xyzw_array, quat_to_rotvec
from uf_common.orientation_helpers import rotvec_to_quat
from geometry_msgs.msg import Quaternion, Vector3
import numpy as np

#def linear_velocity_from_posetwist(pt):
#	return linear_velocity_from_twist(pt)

#def linear_velocity_from_twist(tws):
#	return xyz_array(tws.linear)

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
#	Since we only care about orientation in the x y plane we ignore the x y components of the rotation vector
def normal_vector_from_rotvec(rot):
	theta = rot[2] - np.pi		# Shift the angle pi degrees since pi is considered strait forward
	return np.array([np.cos(theta), np.sin(theta), 0])

def normal_vector_from_posetwist(pt):
	return normal_vector_from_rotvec(orientation_from_posetwist(pt))