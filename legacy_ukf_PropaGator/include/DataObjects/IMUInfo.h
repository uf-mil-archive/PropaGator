#ifndef DATAOBJECTS_IMUINFO_H
#define DATAOBJECTS_IMUINFO_H

#include <Eigen/Dense>
#include <ros/time.h>

namespace subjugator {
    struct IMUInfo {
        ros::Time timestamp;
        Eigen::Vector3d acceleration;
        Eigen::Vector3d ang_rate;
        Eigen::Vector3d mag_field;
    };
}

#endif /* DATAOBJECTS_IMUINFO_H */

