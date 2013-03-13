#ifndef NAVIGATIONCOMPUTER_H
#define NAVIGATIONCOMPUTER_H

#include <Eigen/Dense>

#include "EigenUtils.h"

#include "DataObjects/IMUInfo.h"
#include "DataObjects/LPOSVSSInfo.h"
#include "INS.h"
#include "Kalman.h"

namespace subjugator {
    class NavigationComputer {
    public:
        struct Config {
            Vector3d referenceNorthVector;
            double latitudeDeg;

            Vector3d vel_sigma; // m/s
            Vector3d att_sigma; // degrees
        };
        
        NavigationComputer(const Config &config);
        
        void UpdateIMU(const IMUInfo& imu);
        void UpdateDepth(double depth);
        void UpdateVel(Vector3d vel, bool is_world_frame);
        
        bool getInitialized() { return initialized; }
        LPOSVSSInfo GetNavInfo();
        void reset();
        
        Vector4d getAttRef() {
            return attRef;
        }

    private:
        Config conf;

        static const double alpha = 0.4082;
        static const double beta = 2.0;
        static const double kappa = 0;
        static const double bias_var_f = 0.000004; // g's
        static const double bias_var_w = 26.2; // 3600ths of a degree???
        static const double T_f = 5; // seconds; TODO
        static const double T_w = 100; // seconds; TODO
        static const double depth_sigma = 0.02; // m

        static const double MAX_DEPTH = 15; // m

        Vector3d referenceGravityVector;
        Vector3d white_noise_sigma_f; // g's
        Vector3d white_noise_sigma_w; // 60ths of a degree??

        bool depthRefAvailable;
        bool attRefAvailable;
        bool velRefAvailable;
        double depthRef;
        Vector4d attRef;
        Vector3d velRef;
        Vector7d z;

        int attCount;
        Vector3d magSum;
        Vector3d accSum;
        Vector3d Acceleration_BODY_RAW;

        std::auto_ptr<KalmanFilter> kFilter;
        std::auto_ptr<INS> ins;

        bool initialized;

        ros::Time nextKalmanTime;
        int kalmanCount;

        void TryInit(ros::Time currentTime, Vector3d w_body, Vector3d a_body);
        void updateKalmanTo(ros::Time time);
        void resetErrors();
    };
}

#endif /* NAVIGATIONCOMPUTER_H */
