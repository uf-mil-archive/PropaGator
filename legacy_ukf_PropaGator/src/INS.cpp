#include <iostream>

#include "AttitudeHelpers.h"
#include "Quaternion.h"

#include "INS.h"

using namespace subjugator;
using namespace std;
using namespace Eigen;

/**
 * The constructor for the INS
 *
 * @param[in]    lat                The local latitude(rad)
 * @param[in]    w_dif_prev        The previous unbiased gyro reading minus earth rotation if used(rad/s)
 * @param[in]    a_body_prev        The previous unbiased accelerometer reading(m/s^2)
 * @param[in]    p_prev            The initial NED position(m)
 * @param[in]    v_prev            The initial NED velocity(m)
 * @param[in]    g                The local gravity vector expressed in NED(m/s^2)
 * @param[in]    q_prev            The initial quaternion
 * @param[in]    w_bias            rad/s
 * @param[in]    a_bias            m/s^2
 * @param[in]    time
 *
 */
INS::INS(double lat, Vector3d w_dif_prev, Vector3d a_body_prev, Vector3d p_prev,
    Vector3d v_prev, Vector3d g, Vector4d q_prev, Vector3d w_bias, Vector3d a_bias,
    ros::Time startTime):
    lat(lat), g(g),
    w_dif_prev(w_dif_prev), a_body_prev(a_body_prev), p_prev(p_prev),
    v_prev(v_prev), q_prev(q_prev), w_bias(w_bias), a_bias(a_bias),
    prevTime(startTime) { }

void INS::Update(ros::Time currentTime, Vector3d w_body, Vector3d a_body)
{
    // Update dt
    double dt = (currentTime - prevTime).toSec();
    prevTime = currentTime;
    assert(0 <= dt && dt <= 0.150);

    if(a_body.norm() > MAX_ACC_MAG)
    {
        return;
    }
    if(w_body.norm() > MAX_ANG_RATE)
        return;

    Eigen::Vector3d w_en_n;
    w_en_n(0) = v_prev(1) / (r_earth - p_prev(2));
    w_en_n(1) = -v_prev(0) / (r_earth - p_prev(2));
    w_en_n(2) = -v_prev(1) * tan(lat) / (r_earth - p_prev(2));
    
    Vector3d w_ie_n(w_ie_e*std::cos(lat), 0, -1.0*w_ie_e*std::sin(lat));

    // Rotate w_ie_n from NED to body - commented out since our IMU can't measure earth's rotation. This just adds noise
    Vector3d w_ie_b(0.0,0.0,0.0);// = MILQuaternionOps::QuatRotate(MILQuaternionOps::QuatInverse(q_prev), w_ie_n);
    Vector3d w_dif = (w_body - w_bias);// - w_ie_b;
    Vector3d sigma = dt / 2.0 *(w_dif + w_dif_prev);


    Eigen::Vector4d q = MILQuaternionOps::QuatNormalize(MILQuaternionOps::QuatMultiply(q_prev, MILQuaternionOps::RotVec2Quat(sigma)));

    // Integrate body forces
    Vector3d a_dif = (a_body - a_bias);
    Vector3d v_int = dt / 2.0 * (a_dif + a_body_prev);

    Vector3d u_n = MILQuaternionOps::QuatRotate(q_prev, (v_int + ((.5*sigma).cross(v_int))));

    // Sum all the components to get velocity (NED)
    Eigen::Vector3d v = v_prev + u_n + dt * g -    (2.0 * dt * AttitudeHelpers::VectorSkew3(w_ie_n) - dt * AttitudeHelpers::VectorSkew3(w_en_n)) * v_prev;

    // Integrate velocity to get position
    Eigen::Vector3d p = p_prev + dt / 2.0 * (v + v_prev);

    // Save previous values
    w_dif_prev = w_dif;
    a_body_prev = a_dif;
    p_prev = p;
    v_prev = v;
    q_prev = q;
}

void INS::Reset(const KalmanData& kData)
{
    p_prev -= kData.PositionErrorEst;

    v_prev -= kData.VelocityError;

    q_prev = MILQuaternionOps::QuatMultiply(q_prev, kData.ErrorQuaternion);
    //a_bias = kData.Acceleration_bias;
    //w_bias = kData.Gyro_bias;
}

