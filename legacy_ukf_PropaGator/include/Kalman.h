#ifndef KALMAN_H
#define KALMAN_H

#include <Eigen/Dense>
#include <ros/time.h>

namespace subjugator
{
    class KalmanData
    {
    public:
        double DepthError;
        Eigen::Vector3d VelocityError;
        Eigen::Vector4d ErrorQuaternion;
        Eigen::Vector3d Acceleration_bias;
        Eigen::Vector3d Gyro_bias;
        Eigen::Vector3d PositionErrorEst;    // This also has depth error superimposed!
        ros::Time time;

        KalmanData();
        KalmanData(double depthError, Eigen::Vector3d velError, Eigen::Vector4d errorQuat, Eigen::Vector3d a_bias, Eigen::Vector3d w_bias, Eigen::Vector3d pestErr, ros::Time time) :
            DepthError(depthError), VelocityError(velError), ErrorQuaternion(errorQuat), Acceleration_bias(a_bias),
            Gyro_bias(w_bias), PositionErrorEst(pestErr), time(time)
        {
        }
    };

    class KalmanFilter
    {
    public:
        typedef Eigen::Matrix<double, 13, 1> Vector13d;    // x_hat
        typedef Eigen::Matrix<double, 1, 26> RowVector26d;    // helper vectors
        typedef Eigen::Matrix<double, 1, 27> RowVector27d;
        typedef Eigen::Matrix<double, 13, 13> Matrix13d;
        typedef Eigen::Matrix<double, 7, 7> Matrix7d;
        typedef Eigen::Matrix<double, 7, 1> Vector7d;
        typedef Eigen::Matrix<double, 13, 7> Matrix13x7d;
        typedef Eigen::Matrix<double, 13, 12> Matrix13x12d;
        typedef Eigen::Matrix<double, 13, 26> Matrix13x26d;
        typedef Eigen::Matrix<double, 13, 27> Matrix13x27d;
        typedef Eigen::Matrix<double, 12, 13> Matrix12x13d;
        typedef Eigen::Matrix<double, 12, 12> Matrix12d;
        typedef Eigen::Matrix<double, 4, 27> Matrix4x27d;
        typedef Eigen::Matrix<double, 3,27> Matrix3x27d;
        typedef Eigen::Matrix<double, 7, 27> Matrix7x27d;
        typedef Eigen::Matrix<double, 7, 26> Matrix7x26d;
    public:
        KalmanFilter(int L, Eigen::Vector4d q_hat, Matrix13d P_hat,
                     double alpha, double beta, double kappa, double bias_var_f, double bias_var_w,
                     Eigen::Vector3d white_noise_sigma_f, Eigen::Vector3d white_noise_sigma_w, double T_f,
                     double T_w, double depth_sigma, Eigen::Vector3d dvl_sigma, Eigen::Vector3d att_sigma,
                     ros::Time startTime);
        void Update(const Vector7d& z, const Eigen::Vector3d& f_IMU,
                     const Eigen::Vector3d& v_INS, const Eigen::Vector4d& q_INS, ros::Time currentTime);
        void Reset();
        KalmanData GetData()
        {
            Eigen::Vector3d x_hat_57 = x_hat.block<3,1>(4,0);
            Eigen::Vector4d q_hat_tilde_inverse(sqrt(1-(x_hat_57.transpose()*x_hat_57)(0,0)),
                    -1.0*x_hat_57(0), -1.0*x_hat_57(1), -1.0*x_hat_57(2));
            
            return KalmanData(x_hat(0),
                x_hat.block<3,1>(1,0),
                q_hat_tilde_inverse,
                x_hat.block<3,1>(7,0),
                x_hat.block<3,1>(10,0),
                P_est_error, prevTime);
        }
    private:
        static const double g0 = 9.80665;

        int L;

        Vector13d x_hat;

        // Internally the units of the noise for the gyro and IMU are converted to match the units of the Kalman filter
        // provided they are specified as documented in the matlab sensorCharacteristics file.
        Eigen::Vector4d q_hat;

        Matrix13d P_hat;
        Matrix7d R;
        Eigen::Vector3d P_est_error;

        double alpha;
        double beta;
        double kappa;
        double lambda;

        double W_s[2];
        double W_c[2];

        Eigen::Matrix3d white_bias_sigma_f;
        double bias_var_f;
        Eigen::Matrix3d T_f_inv;

        Eigen::Matrix3d white_bias_sigma_w;
        double bias_var_w;
        Eigen::Matrix3d T_w_inv;

        double T_f;
        double T_w;
        ros::Time prevTime;
    };
}

#endif /* KALMAN_H */
