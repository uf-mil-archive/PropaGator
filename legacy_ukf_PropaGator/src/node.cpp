#include <tf/transform_listener.h>
#include <tf/message_filter.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

#include <uf_common/param_helpers.h>
#include <uf_common/msg_helpers.h>
#include <uf_common/Float64Stamped.h>

#include "NavigationComputer.h"
#include "Quaternion.h"

using namespace std;
using namespace sensor_msgs;
using namespace message_filters;
using namespace geometry_msgs;
using namespace nav_msgs;
using namespace uf_common;

static Eigen::Vector3d NED_from_ENU(Eigen::Vector3d x) {
    return Eigen::Vector3d(x[1], x[0], -x[2]);
}

struct Node {
    ros::NodeHandle nh;
    ros::NodeHandle private_nh;
    tf::TransformListener tf_listener;
    
    string fixed_frame;
    
    message_filters::Subscriber<Imu> imu_sub;
    message_filters::Subscriber<Vector3Stamped> mag_sub;
    TimeSynchronizer<Imu, Vector3Stamped> sync;
    message_filters::Subscriber<Float64Stamped> depth_sub;
    message_filters::Subscriber<Vector3Stamped> worldvel_sub;
    tf::MessageFilter<geometry_msgs::Vector3Stamped> worldvel_filter;
    message_filters::Subscriber<Vector3Stamped> bodyvel_sub;
    tf::MessageFilter<geometry_msgs::Vector3Stamped> bodyvel_filter;
    ros::Timer timer;
    
    ros::Publisher odometry_pub;
    ros::Publisher pose_pub;
    ros::Publisher attref_pub;
    
    boost::scoped_ptr<subjugator::NavigationComputer> navComputer;
    string imu_frame;
    
    Node() :
        private_nh("~"),
        imu_sub(nh, "imu/data_raw", 1),
        mag_sub(nh, "imu/mag", 1),
        sync(imu_sub, mag_sub, 10),
        depth_sub(nh, "depth", 1),
        worldvel_sub(nh, "worldvel", 1),
        worldvel_filter(worldvel_sub, tf_listener, "", 10),
        bodyvel_sub(nh, "bodyvel", 1),
        bodyvel_filter(bodyvel_sub, tf_listener, "", 10) {
        
        ROS_ASSERT(private_nh.getParam("fixed_frame", fixed_frame));
        worldvel_filter.setTargetFrame(fixed_frame);
        
        subjugator::NavigationComputer::Config navconf;
        navconf.referenceNorthVector = NED_from_ENU(get_Vector3(private_nh, "localMagneticVector"));
        ROS_ASSERT(private_nh.getParam("latitudeDeg", navconf.latitudeDeg));
        navconf.vel_sigma = get_Vector3(private_nh, "vel_sigma");
        navconf.att_sigma = get_Vector3(private_nh, "att_sigma");
        
        navComputer.reset(new subjugator::NavigationComputer(navconf));
        
        odometry_pub = nh.advertise<Odometry>("odom", 1);
        pose_pub = private_nh.advertise<PoseStamped>("pose", 1);
        attref_pub = private_nh.advertise<PoseStamped>("attref", 1);
        
        sync.registerCallback(boost::bind(&Node::imu_callback, this, _1, _2));
        depth_sub.registerCallback(boost::bind(&Node::depth_callback, this, _1));
        bodyvel_filter.registerCallback(boost::bind(&Node::vel_callback, this, _1, false));
        worldvel_filter.registerCallback(boost::bind(&Node::vel_callback, this, _1, true));
    }
    
    void imu_callback(const ImuConstPtr& imu, const Vector3StampedConstPtr& mag) {
        assert(imu->header.stamp == mag->header.stamp); // always true due to TimeSynchronizer
        
        assert(imu->header.frame_id == mag->header.frame_id);
        
        if(imu->header.frame_id != imu_frame)
            navComputer->reset();
        imu_frame = imu->header.frame_id;
        bodyvel_filter.setTargetFrame(imu_frame);
        
        subjugator::IMUInfo imuinfo;
        imuinfo.timestamp = imu->header.stamp;
        imuinfo.acceleration = xyz2vec(imu->linear_acceleration);
        imuinfo.ang_rate = xyz2vec(imu->angular_velocity);
        imuinfo.mag_field = xyz2vec(mag->vector);
        
        navComputer->UpdateIMU(imuinfo);
        
        publish();
    }
    
    void depth_callback(const Float64StampedConstPtr& depth) {
        navComputer->UpdateDepth(depth->data);
    }
    
    void vel_callback(const Vector3StampedConstPtr& vel, bool is_world_frame) {
        geometry_msgs::Vector3Stamped out;
        tf_listener.transformVector(is_world_frame ? fixed_frame : imu_frame, *vel, out);
        Eigen::Vector3d v = xyz2vec(out.vector);
        navComputer->UpdateVel(is_world_frame ? NED_from_ENU(v) : v, is_world_frame);
    }
    
    void publish() {
        if(!navComputer->getInitialized())
            return;
        
        subjugator::LPOSVSSInfo info = navComputer->GetNavInfo();
        
        // Emit the LPOSInfo every iteration
        Odometry msg;
        msg.header.stamp = info.timestamp;
        msg.header.frame_id = fixed_frame;
        msg.child_frame_id = imu_frame;
        
        Eigen::Vector4d ENU_from_NED = subjugator::MILQuaternionOps::QuatNormalize(Eigen::Vector4d(0, 1, 1, 0));
        
        Eigen::Vector3d position_ENU = subjugator::MILQuaternionOps::QuatRotate(ENU_from_NED, info.position_NED);
        msg.pose.pose.position = vec2xyz<Point>(position_ENU);
        Eigen::Vector4d orientation_ENU = subjugator::MILQuaternionOps::QuatMultiply(ENU_from_NED, info.quaternion_NED_B);
        msg.pose.pose.orientation = vec2wxyz<Quaternion>(orientation_ENU);
        
        Eigen::Vector3d velocity_BODY = subjugator::MILQuaternionOps::QuatRotate(
            subjugator::MILQuaternionOps::QuatConjugate(info.quaternion_NED_B),
            info.velocity_NED);
        msg.twist.twist.linear = vec2xyz<Vector3>(velocity_BODY);
        msg.twist.twist.angular = vec2xyz<Vector3>(info.angularRate_BODY);
        
        odometry_pub.publish(msg);
        
        PoseStamped msg3;
        msg3.header.stamp = info.timestamp;
        msg3.header.frame_id = fixed_frame;
        msg3.pose.position = vec2xyz<Point>(position_ENU);
        msg3.pose.orientation = vec2wxyz<Quaternion>(orientation_ENU);
        pose_pub.publish(msg3);
        
        PoseStamped msg2;
        msg2.header.stamp = info.timestamp;
        msg2.header.frame_id = fixed_frame;
        msg2.pose.position = vec2xyz<Point>(position_ENU);
        Eigen::Vector4d attref_ENU = subjugator::MILQuaternionOps::QuatMultiply(ENU_from_NED, navComputer->getAttRef());
        msg2.pose.orientation = vec2wxyz<Quaternion>(attref_ENU);
        attref_pub.publish(msg2);
    }
};


int main(int argc, char** argv) {
    ros::init(argc, argv, "legacy_ukf");
    
    Node n;
    
    ros::spin();
    
    return 0;
}
