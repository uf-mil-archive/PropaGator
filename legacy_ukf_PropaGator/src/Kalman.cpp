#include <boost/math/constants/constants.hpp>

#include "AttitudeHelpers.h"
#include "Quaternion.h"

#include "Kalman.h"

using namespace subjugator;
using namespace std;
using namespace Eigen;

KalmanFilter::KalmanFilter(int L, Vector4d q_hat, Matrix13d P_hat,
             double alpha, double beta, double kappa, double bias_var_f, double bias_var_w,
             Vector3d white_noise_sigma_f, Vector3d white_noise_sigma_w, double T_f,
             double T_w, double depth_sigma, Vector3d dvl_sigma, Vector3d att_sigma,
             ros::Time startTime) :
             L(L), q_hat(q_hat), P_hat(P_hat), alpha(alpha),
             beta(beta), kappa(kappa), bias_var_f(bias_var_f*g0),
             bias_var_w(bias_var_w*boost::math::constants::pi<double>()/(180.0*3600.0)),
             T_f(T_f), T_w(T_w), prevTime(startTime)
{
    lambda = alpha*alpha*(L+kappa) - L;

    x_hat = Vector13d::Zero();

    // Initialize Weighting Parameters
    W_c[0] = (lambda / (L + lambda)) + (1 - alpha*alpha + beta);
    W_c[1] = 1.0 / (2.0 * (L + lambda));

    W_s[0] = lambda / (L + lambda);
    W_s[1] = 1.0 / (2.0 * (L + lambda));

    white_bias_sigma_f = g0 * AttitudeHelpers::DiagMatrixFromVector(white_noise_sigma_f);
    white_bias_sigma_w = boost::math::constants::pi<double>() / (180.0 * 60.0) * AttitudeHelpers::DiagMatrixFromVector(white_noise_sigma_w);

    T_f_inv = 1.0 / T_f * Matrix3d::Identity();
    T_w_inv = 1.0 / T_w * Matrix3d::Identity();

    // Build the R Matrix
    Matrix3d W_dvl = AttitudeHelpers::DiagMatrixFromVector(dvl_sigma.cwiseProduct(dvl_sigma));
    Matrix3d W_att = (boost::math::constants::pi<double>()/180.0)*(boost::math::constants::pi<double>()/180.0)*AttitudeHelpers::DiagMatrixFromVector(att_sigma.cwiseProduct(att_sigma));

    R = Matrix7d::Zero();
    R(0,0) = depth_sigma*depth_sigma;
    R.block<3,3>(1,1) = W_dvl;
    R.block<3,3>(4,4) = W_att;

    P_est_error = Vector3d::Zero();
}

void KalmanFilter::Update(const Vector7d& z, const Vector3d& f_IMU,
             const Vector3d& v_INS, const Vector4d& q_INS, ros::Time currentTime)
{
    // Update dt
    double dt = (currentTime - prevTime).toSec();
    prevTime = currentTime;
    assert(0 <= dt && dt <= 0.150);

    // Gyro Error
    double sigma_Q_w = AttitudeHelpers::Markov_wStdDev(dt, T_w, bias_var_w);
    double sigma_Q_w_squared = sigma_Q_w*sigma_Q_w;
    Matrix3d Q_w = Matrix3d::Zero();
    Q_w(0,0) = Q_w(1,1) = Q_w(2,2) = sigma_Q_w_squared;

    // Accelerometer error
    double sigma_Q_f = AttitudeHelpers::Markov_wStdDev(dt, T_f, bias_var_f);
    double sigma_Q_f_squared = sigma_Q_f*sigma_Q_f;
    Matrix3d Q_f = Matrix3d::Zero();
    Q_f(0,0) = Q_f(1,1) = Q_f(2,2) = sigma_Q_f_squared;

    Matrix3d R_hat = MILQuaternionOps::Quat2Rot(q_hat);

    Matrix3d tempQSkew = AttitudeHelpers::VectorSkew3(x_hat.block<3,1>(4,0));
    tempQSkew *= tempQSkew;

    Matrix3d Q_eps = (0.25 * (Matrix3d::Identity() - tempQSkew))
            * white_bias_sigma_w * white_bias_sigma_w;

    Matrix12d Q = Matrix12d::Zero();
    Q.block<3,3>(0,0) = R_hat * white_bias_sigma_f * R_hat.transpose();
    Q.block<3,3>(3,3) = Q_eps;
    Q.block<3,3>(6,6) = Q_f;
    Q.block<3,3>(9,9) = Q_w;

    // Replicate x_hat into a matrix form
    Matrix13x27d x_hat_mat = x_hat * RowVector27d::Ones();

    int limit = 2 * L + 1;

    // Calculate the square root of the covariance matrix
    Matrix13d root_cov =((L + lambda)*P_hat).llt().matrixU();
    root_cov.transposeInPlace();

    Matrix13x26d root_cov_expanded;
    root_cov_expanded.block<13,13>(0, 0) = root_cov;
    root_cov_expanded.block<13,13>(0,13) = -1.0 * root_cov;

    Matrix13x27d chi = x_hat_mat;    // need a deep copy here
    chi.block<13,26>(0,1) += root_cov_expanded;

    Matrix13x27d chi_dot;
    Matrix13x27d chi_pred;
    Matrix4x27d q_hat_chi;

    for(int i = 0; i < limit; i++)
    {
        Vector3d chi_57 = chi.block<3,1>(4,i); // orientation
        Vector3d chi_810 = chi.block<3,1>(7,i); // a bias
        Vector3d chi_1113 = chi.block<3,1>(10,i); // w bias

        Vector4d q_tilde_inverse(sqrt(1.0 - (chi_57.transpose()*chi_57)(0,0)),
                -1.0*chi_57(0), -1.0*chi_57(1), -1.0*chi_57(2));
        q_hat_chi.col(i) = MILQuaternionOps::QuatMultiply(q_INS, q_tilde_inverse);

        // Skew matrices for the chi_dot equation
        Matrix3d S_beta = AttitudeHelpers::VectorSkew3(chi_1113);
        Matrix3d S_eps_tilde = AttitudeHelpers::VectorSkew3(chi_57);

        // Velocity error vector before rotation
        Vector3d delv_dot_nonrot = chi_810 + 2*(S_eps_tilde + S_eps_tilde*S_eps_tilde)*f_IMU;

        // Calculate the time derivative of chi - f(x)_dot
        chi_dot(0,i) = chi(3,i);
        chi_dot.block<3,1>(1,i) = MILQuaternionOps::QuatRotate(q_hat_chi.col(i), delv_dot_nonrot);
        chi_dot.block<3,1>(4,i) = (1.0 / 2.0 * chi_1113 + 1.0 / 2.0 * S_beta * chi_57);
        chi_dot.block<3,1>(7,i) = (-1.0 * T_f_inv * chi_810);
        chi_dot.block<3,1>(10,i) = (-1.0 * T_w_inv * chi_1113);

        chi_pred.col(i) = chi.col(i) + dt * chi_dot.col(i);
    }

    // Get the sum for x_pred W_2
    Vector13d sum_W2 = Vector13d::Zero();
    for(int i = 1; i < limit; i++)
    {
        sum_W2 += chi_pred.col(i);
    }

    Vector13d chi_pred_a1 = chi_pred.col(0);
    Vector13d x_pred = (W_s[0]*chi_pred_a1) + (W_s[1] * sum_W2);
    Matrix13x26d x_pred_mat = x_pred * RowVector26d::Ones();

    Vector13d P_int1 = chi_pred_a1 - x_pred;
    Matrix13x26d P_int2 = chi_pred.block<13,26>(0,1) - x_pred_mat;
    Matrix13d P_int = W_c[0] * P_int1 * P_int1.transpose()
            + W_c[1] * P_int2 * P_int2.transpose();

    Matrix13x12d gamma = Matrix13x12d::Zero();
    gamma.block<12,12>(1,0).setIdentity();
    Matrix13d P_pred = P_int + gamma*Q*gamma.transpose();

    // Predict the measurement
    Matrix3x27d gamma_2 = Matrix3x27d::Zero();
    for(int i = 0; i < limit; i++)
    {
        Matrix3d R_chi = MILQuaternionOps::Quat2Rot(q_hat_chi.col(i));
        Matrix3d gamma_skew = AttitudeHelpers::VectorSkew3(chi_pred.block<3,1>(4,i));
        gamma_2.col(i) = chi_pred.block<3,1>(1,i) - (R_chi*(2.0*(gamma_skew + gamma_skew*gamma_skew))*v_INS);
    }

    Matrix7x27d gamma_mat = Matrix7x27d::Zero();
    gamma_mat.row(0) = chi_pred.row(0);
    gamma_mat.block<3,27>(1,0) = gamma_2;
    gamma_mat.block<3,27>(4,0) = chi_pred.block<3,27>(4,0);

    // Get the sum for z_pred
    Vector7d sum_Z2 = Vector7d::Zero();
    for(int j = 1; j< limit;j++)
    {
        sum_Z2 += gamma_mat.col(j);
    }

    Vector7d gamma_mat_a1 = gamma_mat.col(0);
    Vector7d z_pred = W_s[0] * gamma_mat_a1 + W_s[1] * sum_Z2;
    Matrix7x26d z_pred_mat = z_pred * RowVector26d::Ones();

    Vector7d X_zz1 = gamma_mat_a1 - z_pred;
    Matrix7x26d X_zz2 = gamma_mat.block<7,26>(0,1) - z_pred_mat;
    Matrix7d X_zz = W_c[0] * X_zz1 * X_zz1.transpose()
            + W_c[1] * X_zz2 * X_zz2.transpose() + R;

    Matrix13x7d X_xz = W_c[0] * P_int1 * X_zz1.transpose()
            + W_c[1] * P_int2 * X_zz2.transpose();

    Matrix13x7d K = X_xz * X_zz.inverse();

    Vector7d innovation = z - z_pred;

    x_hat = x_pred + K*innovation;

    P_hat = P_pred - K * X_zz * K.transpose();

    // This was reversed in the sub, but matlab says this way?
    q_hat = MILQuaternionOps::QuatMultiply(q_INS, GetData().ErrorQuaternion);

    // Lastly, approximate the position (x,y) error by integrating the velocity errors
    P_est_error.block<2,1>(0,0) += dt * x_hat.block<2,1>(1,0);
    P_est_error(2) = x_hat(0);
}

void KalmanFilter::Reset()
{
    x_hat.block<7,1>(0,0) = Vector7d::Zero();
    P_est_error = Vector3d::Zero();
}
