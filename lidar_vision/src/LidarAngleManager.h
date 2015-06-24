#ifndef LIDAR_ANGLE_MANAGER_H_
#define LIDAR_ANGLE_MANAGER_H_

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include "dynamixel_servo/DynamixelFullConfig.h"
#include "dynamixel_servo/DynamixelJointConfig.h"
#include "dynamixel_servo/DynamixelStatus.h"
#include "dynamixel_servo/DynamixelControlTableRequest.h"
#include "dynamixel_servo/DynamixelControlTablePost.h"
#include "dynamixel_servo/DynamixelStatus.h"
#include "lidar_vision/lidar_servo_mode.h"
#include <cmath>

class LidarAngleManager
{
	/*
	 * Enumerators
	 */
private:
	enum Mode
	{
		PAN,
		STATIC
	};

	/*
	 *  Private vars
	 */
private:
	//Publishers
	//ros::Publisher dynamixel_config_full_pub_;
	ros::Publisher dynamixel_config_position_pub_;
	ros::Publisher dynamixel_control_table_pub_;
	//ros::Publisher joint_pub_;

	//Subscribers
	ros::Subscriber dynamixel_control_table_sub_;
	ros::Subscriber dynamixel_status_sub_;
	
	//Services
	ros::ServiceServer lidar_mode_srv_;

	//min and max angle
	float abs_max_angle_, abs_min_angle_;
	
	//Servo id
	int servo_id_;
	
	//Current angle
	//Keeps a cache of the last published servo angle
	//float current_angle_; Legacy

	// Track mode
	Mode mode_;

	//Sin wave(pan mode) parameters
	int sin_sample_count_;
	float offset_;
	float out_frequency_;
	float in_frequency_;
	float amplitude_;

	//Static mode parameters
	float nominal_angle_;

	//TF
	tf::TransformBroadcaster angle_br_;

	/*
	 *  Private functions
	 */
private:
	//Conversion between dynamixel setting and radians
	float DynamixelToRads(int dynamixelSetting);

	//Get the abs max and min angles
	void GetLimits(const dynamixel_servo::DynamixelControlTablePost config);
	
	//Get the current servo data
	void GetServoData(const dynamixel_servo::DynamixelStatus status);

	//Respond to lidar_servo_mode request
	bool SetServoMode(lidar_vision::lidar_servo_mode::Request& req, lidar_vision::lidar_servo_mode::Response& res);

	//Reset the sine wave
	void SetSineWave(float max, float min, float out_frequency);

	/*
	 * 	Public functions
	 */
public:
	//Constructor
	LidarAngleManager();
	
	//Destructor
	//~LidarAngleManager();
	
	//Run the node
	void Run();

	//Setup ROS
	void Setup();
};

/*
 *  Constructor
 */
LidarAngleManager::LidarAngleManager():
		abs_max_angle_(3.4), abs_min_angle_(2.7),
		sin_sample_count_(0), in_frequency_(50), out_frequency_(1),
		mode_(PAN), nominal_angle_(M_PI)
{
	SetSineWave(abs_max_angle_, abs_min_angle_, out_frequency_);

	// in_frequency_ = 50 Hz		TODO: un-hardcode this value

	// fs = output frequency
	//out_frequency_ = 1;				//TODO: un-hardcode this value
}

/*
 *  Sets up the node
 */
void LidarAngleManager::Setup()
{
	/*
	 * 			Initilize ROS
	 */
	//Initilze the node ("lidar_control_node")
	//Initilze the node handle
	ros::NodeHandle n;

	//Initilze publishers
	//dynamixel_config_full_pub_ = n.advertise<dynamixel_servo::DynamixelFullConfig>("/dynamixel/dynamixel_full_config", 10);
	dynamixel_config_position_pub_ = n.advertise<dynamixel_servo::DynamixelJointConfig>("/dynamixel/dynamixel_joint_config", 10);
	dynamixel_control_table_pub_ = n.advertise<dynamixel_servo::DynamixelControlTableRequest>("/dynamixel/dynamixel_control_table_request", 10);

	//Initilze services
	lidar_mode_srv_ = n.advertiseService("lidar_servo_mode", &LidarAngleManager::SetServoMode, this);

	//joint_pub_ = n.advertise<sensor_msgs::JointState>("joint_states",1);
	
	/*
	 * 		Initilize the parameters
	 */
	//Holds the servo to use to control the lidar
	//<param name="$(arg servo_namespace)/lidar/bow/id" value="1" type="int"/>
	n.param<int>("servo/lidar/bow/id", servo_id_, 1);
	
	//Holds the maximum angle (rad) to set the lidar to
	//n.param<float>("lidar/max_angle", abs_max_angle_, 3.14159);
	//Holds the minimum angle (rad) to set the lidar to
	//n.param<float>("lidar/min_angle", abs_min_angle_, 2.7);
	//Holds the angular speed (rad/s)
	//n.param<float>

	
	//Wait for subscriber on control table request
	//Timeout of 5 seconds
	ros::Time start = ros::Time::now();
	ros::Duration timeout(5);
	while(dynamixel_control_table_pub_.getNumSubscribers() == 0 && (ros::Time::now() - start) < timeout);
	if(dynamixel_control_table_pub_.getNumSubscribers() !=0)
	{
		//Send out request for control table
		dynamixel_servo::DynamixelControlTableRequest request;
		request.id = servo_id_;
		dynamixel_control_table_pub_.publish(request);
		//When the data comes in it'll reset the min and max
	}
	else
	{
		ROS_WARN("Failed to talk to dynamixel server. No subscribers to /dynamixel/dynamixel_control_table_request");
	}
	
	//Initilze the subscribers
	dynamixel_control_table_sub_ = n.subscribe("/dynamixel/dynamixel_control_table_post", 10, &LidarAngleManager::GetLimits, this);
	dynamixel_status_sub_ = n.subscribe("/dynamixel/dynamixel_status_post", 10, &LidarAngleManager::GetServoData, this);

	//Configure lidar servo settings
	//TODO
	
}

/*
 * 	Runs the node
 */
void LidarAngleManager::Run()
{
	/*
	 *  Other
	 */
	ros::Rate sleep_time(in_frequency_);		// Sampeling rate = 50 Hz

	/*
	 * 			Main loop
	 */
	while(ros::ok())
	{
		float out_angle = M_PI;

		if(mode_ == PAN)
		{
			// Move the Lidar in a sin wave
			// offset = (max + min) / 2
			// Amplitued = (max - min) / 2
			// Sampeling rate = 100 Hz		TODO: un-hardcode this value
			// fs = output frequency
			// sin wave = offset + amplitude * sin((2*pi*fs) * sin_sample_count_ / (sampeling rate)

			++sin_sample_count_;

			//wrap sin_sample_count_ between 0 and sampeling rate
			if(sin_sample_count_ > in_frequency_ * 2)
			{
				sin_sample_count_ = 0;
			}

			out_angle = offset_ + amplitude_ * sin(2 * M_PI * out_frequency_ * sin_sample_count_ / in_frequency_);
		}
		else
		{
			// Static mode
			out_angle = nominal_angle_;
		}
		

		dynamixel_servo::DynamixelJointConfig msg;
		msg.id = servo_id_;
		msg.goal_position = out_angle;
		dynamixel_config_position_pub_.publish(msg);

		//ROS_INFO("Out Angle: %f", out_angle);

		//Wait for next run
		sleep_time.sleep();
		ros::spinOnce();
	}
}

/*
 *  Conversion dynamixel setting * 360 / 4095 = degrees, degrees * pi / 180 = radians
 */
inline float LidarAngleManager::DynamixelToRads(int dynamixelSetting)
{
	return (dynamixelSetting * 360 * 3.14159) / (4095 * 180);
}

/*
 * Get the limits from the servo server
 */
void LidarAngleManager::GetLimits(const dynamixel_servo::DynamixelControlTablePost config)
{
	//Check to make sure we got the correct servo
	if(config.id == servo_id_)
	{
		// As of 06/23/15 the lidar servo angle limits are not correctly set
		//		in the dynimixel_servo_server. To prevent damage to the servo
		//		the proper functionality of this callback is disabled and software
		//		limits are used instead
		abs_max_angle_ = abs_max_angle_;//DynamixelToRads(config.ccw_angle_limit);
		abs_min_angle_ = abs_min_angle_;//DynamixelToRads(config.cw_angle_limit);

		SetSineWave(abs_max_angle_, abs_min_angle_, out_frequency_);

		// Stop the subscriber
		dynamixel_control_table_sub_.shutdown();
	}
	else	//Otherwise request servo id again
	{
		//Send out request for control table
		dynamixel_servo::DynamixelControlTableRequest request;
		request.id = servo_id_;
		dynamixel_control_table_pub_.publish(request);
	}
}

/*
 * Get the servo data
 */
void LidarAngleManager::GetServoData(const dynamixel_servo::DynamixelStatus status)
{
	if(status.id == servo_id_)
	{
		float current_angle = status.present_position;
		tf::Transform trans;
		trans.setOrigin(tf::Vector3(0,0,0));
		tf::Quaternion q;
		q.setRPY(0, M_PI - current_angle, 0);
		trans.setRotation(q);
		angle_br_.sendTransform(tf::StampedTransform(trans, ros::Time::now(), "lidar_base", "lidar"));
	}
}

/*
 * Respond to the lidar_servo_mode service
 */
bool LidarAngleManager::SetServoMode(lidar_vision::lidar_servo_mode::Request& req, lidar_vision::lidar_servo_mode::Response& res)
{

	if (req.mode == lidar_vision::lidar_servo_mode::Request::PAN)
	{
		// Pan mode
		float max = req.max_angle;
		float min = req.min_angle;
		// Check ranges
		if (min >= max)
		{
			return false;
		}

		SetSineWave(max, min, req.freq);

		mode_ = PAN;

	}
	else
	{
		// Static mode
		float angle = req.nominal_angle;
		if (angle > abs_max_angle_)
		{
			angle = abs_max_angle_;
		}
		else if (angle < abs_min_angle_)
		{
			angle = abs_min_angle_;
		}

		nominal_angle_ = req.nominal_angle;
		mode_ = STATIC;

	}
	res.valid_angles = true;
	return true;
}


void LidarAngleManager::SetSineWave(float max, float min, float out_frequency)
{
	// Clip to abs_max and abs_min
	max = max > abs_max_angle_ ? abs_max_angle_ : max;
	min = min < abs_min_angle_ ? abs_min_angle_ : min;

	//TODO: add limits to frequency
	out_frequency_ = out_frequency;

	// offset = (max + min) / 2
	offset_ = (max + min) / 2;

	// Amplitude = (max - min) / 2
	amplitude_ = (max - min) / 2;
}

#endif
