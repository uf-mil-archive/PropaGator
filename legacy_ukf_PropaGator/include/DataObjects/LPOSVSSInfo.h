#ifndef DATAOBJECTS_LPOSVSSINFO_H
#define DATAOBJECTS_LPOSVSSINFO_H

#include <Eigen/Dense>
#include <ros/time.h>

namespace subjugator {
    struct LPOSVSSInfo {
        ros::Time timestamp;
        Eigen::Vector3d position_NED;
        Eigen::Vector4d quaternion_NED_B;
        Eigen::Vector3d velocity_NED;
        Eigen::Vector3d angularRate_BODY;
    };
}

#endif /* DATAOBJECTS_LPOSVSSINFO_H */

