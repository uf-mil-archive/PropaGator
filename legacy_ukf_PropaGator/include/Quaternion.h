#ifndef QUATERNION_H
#define QUATERNION_H

#include <Eigen/Dense>

// I ported this class over for a few reasons. The first of which is it follows the conventions
// for quaternion representation in Dr. Crane's book, and the methods that the INS, etc. were derived.
// Secondly, I have already debugged this code, and a major shift to eigens transformation based
// math looks too time consuming.
namespace subjugator
{
    class MILQuaternionOps
    {
    public:
        static Eigen::Matrix3d Quat2Rot(const Eigen::Vector4d& q);
        static Eigen::Vector4d QuatNormalize(const Eigen::Vector4d &q);
        static Eigen::Vector4d QuatMultiply(const Eigen::Vector4d& q, const Eigen::Vector4d& p);
        static Eigen::Vector4d QuatConjugate(const Eigen::Vector4d& q);
        static Eigen::Vector4d QuatInverse(const Eigen::Vector4d& q);
        static Eigen::Vector3d QuatRotate(const Eigen::Vector4d& q, const Eigen::Vector3d vec);
        static Eigen::Vector4d RotVec2Quat(const Eigen::Vector3d& sigma);
    };
}

#endif /* QUATERNION_H */
