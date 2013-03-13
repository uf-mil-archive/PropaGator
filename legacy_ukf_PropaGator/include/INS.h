#ifndef INS_H
#define INS_H

#include <Eigen/Dense>
#include <ros/time.h>

#include "Kalman.h"


namespace subjugator {
    struct INSData {
        INSData(Eigen::Vector3d p, Eigen::Vector3d v, Eigen::Vector4d q, Eigen::Vector3d w_body, ros::Time time) :
            Position_NED(p),
            Velocity_NED(v),
            Orientation_NED_B(q),
            AngularRate_BODY(w_body),
            time(time) { }
        
        Eigen::Vector3d Position_NED;
        Eigen::Vector3d Velocity_NED;
        Eigen::Vector4d Orientation_NED_B;
        Eigen::Vector3d AngularRate_BODY;
        ros::Time time;
    };

    class INS {
    public:
        INS(double lat, Eigen::Vector3d w_dif_prev, Eigen::Vector3d a_body_prev, Eigen::Vector3d p_prev,
            Eigen::Vector3d v_prev, Eigen::Vector3d g, Eigen::Vector4d q_prev, Eigen::Vector3d w_bias, Eigen::Vector3d a_bias,
            ros::Time time);

        void Update(ros::Time currentTime, Eigen::Vector3d w_body, Eigen::Vector3d a_body);
        void Reset(const KalmanData& kData);
        INSData GetData() {
            return INSData(p_prev, v_prev, q_prev, w_dif_prev, prevTime);
        }

    private:
        static const double r_earth = 6378137;    // Radius of the earth (m)
        static const double w_ie_e = 7.292115e-5;  // Angular rate of the earth (rad/s
        static const double MAX_ACC_MAG = 20.0;    // m/s^2
        static const double MAX_ANG_RATE = 12; // rad/s

        double lat;
        Eigen::Vector3d g;
        
        Eigen::Vector3d w_dif_prev;
        Eigen::Vector3d a_body_prev;
        Eigen::Vector3d p_prev;
        Eigen::Vector3d v_prev;
        Eigen::Vector4d q_prev;

        Eigen::Vector3d w_bias;
        Eigen::Vector3d a_bias;

        ros::Time prevTime;
    };
}

#endif /* INS_H */
