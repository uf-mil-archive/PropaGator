#include <iostream>

#include <boost/math/constants/constants.hpp>

#include "AttitudeHelpers.h"
#include "Quaternion.h"
#include "Triad.h"

#include "NavigationComputer.h"

using namespace subjugator;
using namespace std;
using namespace Eigen;

NavigationComputer::NavigationComputer(const Config &conf):
    conf(conf),
    white_noise_sigma_f(0.0005,0.0005,0.0005), white_noise_sigma_w(0.05,0.05,0.05)
{
    referenceGravityVector = AttitudeHelpers::LocalGravity(conf.latitudeDeg*boost::math::constants::pi<double>()/180.0, 0);
    reset();
}

void NavigationComputer::reset() {
    depthRefAvailable = false;
    attRefAvailable = false;
    velRefAvailable = false;
    
    attCount = 0;
    
    initialized = false;
}

void NavigationComputer::TryInit(ros::Time currentTime, Vector3d w_body, Vector3d a_body)
{
    assert(!initialized);

    if(!depthRefAvailable || !attRefAvailable) // || !velRefAvailable)
        return;

    //INS initialization
    ins = std::auto_ptr<INS>(
            new INS(
                    conf.latitudeDeg*boost::math::constants::pi<double>()/180.0,
                    w_body,
                    a_body,    // a_body prev MUST be taken from a valid IMU packet!
                    Vector3d(0, 0, depthRef), // initialPosition
                    Vector3d(0, 0, 0), // initialVelocity
                    referenceGravityVector,    // Gravity vector from file or equation
                    attRef,    //
                    Vector3d::Zero(),    // Initial gyro bias, rad/s
                    Vector3d::Zero(),    // Initial accelerometer bias, m/s^2
                    currentTime
            ));


    // Kalman Initialization
    // Now that we have an initial attitude estimate, initialize the error terms for the kalman
    z = Vector7d::Zero();

    Eigen::Matrix<double, 13, 13> covariance;
    covariance = .01*Matrix<double, 13, 13>::Identity();
    covariance(0,0) *= .01;
    covariance.block<3,3>(2,2) = 10*covariance.block<3,3>(2,2);

    kFilter = std::auto_ptr<KalmanFilter>(
            new KalmanFilter(
                    13,
                    attRef,
                    covariance,
                    alpha, beta, kappa, bias_var_f, bias_var_w,
                    white_noise_sigma_f, white_noise_sigma_w, T_f,
                    T_w, depth_sigma, conf.vel_sigma, conf.att_sigma,
                    currentTime
            ));

    nextKalmanTime = ros::Time(0);
    kalmanCount = 0;

    initialized = true;
}

void NavigationComputer::updateKalmanTo(ros::Time time)
{
    assert(initialized);
    
    if(time >= nextKalmanTime) {
        nextKalmanTime = time + ros::Duration(1./50); // 50 Hz
    } else {
        return;
    }

    INSData insdata = ins->GetData();

    // Constant error kalman errors
    if(attRefAvailable)
    {
        attRefAvailable = false;
        Vector4d tempQuat = MILQuaternionOps::QuatMultiply(MILQuaternionOps::QuatInverse(attRef), insdata.Orientation_NED_B);
        if (tempQuat(0) < 0)
            tempQuat *= -1;
        z.block<3,1>(4,0) = tempQuat.block<3,1>(1,0);
    }
    if(depthRefAvailable)
    {
        depthRefAvailable = false;
        z(0) = insdata.Position_NED(2) - depthRef;
    }
    if(velRefAvailable)
    {
        //velRefAvailable = false;
        z.block<3,1>(1,0) = insdata.Velocity_NED - velRef;
    }

    kFilter->Update(z, -1*Acceleration_BODY_RAW, insdata.Velocity_NED, insdata.Orientation_NED_B, time);

    if(++kalmanCount >= 100)    // 2s reset time
    {
        kalmanCount = 0;
        resetErrors();
    }

}

void NavigationComputer::resetErrors()
{
    ins->Reset(kFilter->GetData());
    kFilter->Reset();
    z = Vector7d::Zero();
}

LPOSVSSInfo NavigationComputer::GetNavInfo()
{
    assert(initialized);

    // Subtract errors to build best current estimate
    KalmanData kdata = kFilter->GetData();
    INSData insdata = ins->GetData();

    LPOSVSSInfo info;
    info.timestamp = insdata.time;
    info.quaternion_NED_B = MILQuaternionOps::QuatMultiply(insdata.Orientation_NED_B, kdata.ErrorQuaternion);
    info.angularRate_BODY = insdata.AngularRate_BODY - kdata.Gyro_bias;
    info.position_NED = insdata.Position_NED - (kdata.PositionErrorEst + (insdata.time - kdata.time).toSec() * kdata.VelocityError);
    info.velocity_NED = insdata.Velocity_NED - kdata.VelocityError;
    
    return info;
}

void NavigationComputer::UpdateIMU(const IMUInfo& imu)
{
    Acceleration_BODY_RAW = imu.acceleration; // used in updateKalmanTo
    
    if(initialized && (imu.timestamp < ins->GetData().time || imu.timestamp > ins->GetData().time + ros::Duration(0.1))) // reinitialize if time goes backwards or forwards too far
        reset();
    
    // The INS has the rotation info already, so just push the packet through
    if(initialized) {
        ins->Update(imu.timestamp, imu.ang_rate, imu.acceleration);
    }
    
    if(attCount == 0) {
        // Reset the sums
        magSum = Vector3d::Zero();
        accSum = Vector3d::Zero();
    }

    // We just do a very basic average over the last 10 samples (reduces to 20Hz)
    // the magnetometer and accelerometer
    magSum += imu.mag_field;
    accSum += imu.acceleration;

    attCount = (attCount + 1) % 10;
    if(attCount == 0) {
        // Now we play some games to get a gravitational estimate. We can't feed in the
        // gravity best estimate, because then you get circular dependencies between
        // the filter and the reference sensor, and the filter drifts badly. So we make the assumption
        // that accelerations are short lived. To ensure this, we check to make sure the gravitational
        // average is close to the magnitude of normal gravity. If it isn't we ignore it, and reference
        // attitude updates come in slower.

        Vector3d bodyg = -accSum / 10;    // The INS data gives -ve gravity. This is so we get the proper direction of gravity
        Vector3d bodym = magSum / 10;

        if(bodyg.norm() <= referenceGravityVector.norm() * 1.04) {
            // Bad acceleration data would just hurt the filter, eliminate it

            attRef = triad(referenceGravityVector, conf.referenceNorthVector, bodyg, bodym);
            attRefAvailable = true;

            if(!initialized)
                TryInit(imu.timestamp, imu.ang_rate, imu.acceleration);
        }
    }
    
    if(initialized)
        updateKalmanTo(imu.timestamp);
}

void NavigationComputer::UpdateDepth(double depth)
{
    // The depth inside the packet is given in NED
    
    if(std::abs(depth) > MAX_DEPTH)
        return;

    depthRef = depth;
    depthRefAvailable = true;
}

void NavigationComputer::UpdateVel(Vector3d vel, bool is_world_frame)
{
    if(!initialized)
        return;
    
    if(is_world_frame) {
        velRef = vel;
    } else {
        // Rotate dvl data from SUB to NED
        velRef = MILQuaternionOps::QuatRotate(GetNavInfo().quaternion_NED_B, vel);
    }
    velRefAvailable = true;
}
