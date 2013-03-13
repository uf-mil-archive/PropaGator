#ifndef TRIAD_H
#define TRIAD_H

#include <Eigen/Dense>


namespace subjugator
{
    Eigen::Vector4d triad(const Eigen::Vector3d& v1_NED, const Eigen::Vector3d& v2_NED, const Eigen::Vector3d& v1_BOD, const Eigen::Vector3d& v2_BOD);
}

#endif /* TRIAD_H */
