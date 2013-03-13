#ifndef ATTITUDEHELPERS_H
#define ATTITUDEHELPERS_H

#include <Eigen/Dense>

namespace subjugator
{
    class AttitudeHelpers
    {
    public:
        static Eigen::Vector3d LocalGravity(double lat, double depth);
        static double Markov_wStdDev(double dt, double T, double sigma);
        static Eigen::Vector4d RotationToQuaternion(const Eigen::Matrix3d& R);
        static Eigen::Matrix3d VectorSkew3(const Eigen::Vector3d& v);
        static Eigen::MatrixXd DiagMatrixFromVector(const Eigen::VectorXd& v);
    };
}

#endif /* ATTITUDEHELPERS_H */
