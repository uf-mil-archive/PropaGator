#ifndef LIDAR_PROSSESOR_H_
#define LIDAR_PROSSESOR_H_

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "tf/transform_listener.h"
#include <algorithm>		//for remove_if
#include <cmath>
#include "geometry_msgs/PoseStamped.h";

struct f_2dpt 
{
	float x, y;
};

using namespace std;

class LidarProssesor
{
	/*
	 * Private vars
	 */
	//Laser scan subscriber
	ros::Subscriber scan_sub;
	
	//Output publisher
	ros::Publisher out_pub;

	//Transform
	tf::TransformListener lidar_tf;

	//State
	bool prossesing_, first_;

	//Current pts
	vector<float> ranges_;

	//Angles
	float min_angle_, max_angle_, angle_inc_;
	//Ranges
	//static float min_range_, max_range_;
	float min_range_, max_range_;

	//Configuration params		
	//	max_point_range_weight
	//		This value controls how much weight is given to points at the minimum range
	//		The scalling is minimum range to maximum range linearly coresponds to max_point_range weight to 0
	double max_point_range_weight_;
	//	angulat_point_weight
	//		This value controls how quickly the weighting reduces from the points nominal angle
	//		The nominal weight is reduced by angular_decay_ per angle_inc_
	double angular_decay_;
	// Potential field
	//		1081 is the size of the range message from laser
	//int potential_field[1081];
	
	//maping params
	float range_to_weight_slope_;
	float range_to_weight_y_intercept_ ;

	/*
	 * Private functions
	 */
private:
	void GetNewScan(const sensor_msgs::LaserScan& scan);

	void FindBestVector();
	
	//static bool isOutOfRange(const float& range);
	bool isOutOfRange(const float& range);
	
	f_2dpt GeneratePoint(float magnitude, float angle);
	
	vector<f_2dpt> GeneratePoints();
	
	float RangeToWeight(float range);
	
	/*
	 * 	Public functions
	 */
public:
	void Run();
	
	void Setup();
	
	LidarProssesor();

};

//Static var initilization
//float LidarProssesor::min_range_ = 0;
//float LidarProssesor::max_range_ = 0;

LidarProssesor::LidarProssesor():
		prossesing_(false)
	{}

void LidarProssesor::GetNewScan(const sensor_msgs::LaserScan& scan)
{
	if(!prossesing_)
	{
		if(first_ == true)	//If its the first msg get the params from the laser
		{
			first_ = false;
			min_angle_ = scan.angle_min;
			max_angle_ = scan.angle_max;
			angle_inc_ = scan.angle_increment;
			min_range_ = scan.range_min;
			max_range_ = scan.range_max;
			range_to_weight_slope_ = -max_point_range_weight_ / (max_range_ - min_range_);
			range_to_weight_y_intercept_ = -range_to_weight_slope_ * max_range_;
			ROS_INFO("min angle: %f\tmax angle %f\tangle inc. %f\tmin_range %f\tmax range %f", min_angle_, 
					max_angle_, angle_inc_, min_range_, max_range_);
		}
		//ROS_INFO("Scan vector");
		ranges_ = scan.ranges;
		//ROS_INFO("After scan vector");
		prossesing_ = true;
	}
}

void LidarProssesor::Run()
{
	ros::Rate loop_freq(10);
	
	while(ros::ok())
	{
		if(prossesing_)
		{
			FindBestVector();
			//ROS_INFO("After Find best vector");
			prossesing_ = false;
		}
		
		loop_freq.sleep();
		ros::spinOnce();
	}
}

void LidarProssesor::Setup()
{
	/*
	 * Initilize ros
	 */
	ros::NodeHandle n;

	//Initilize subscribers
	scan_sub = n.subscribe("/lidar/scan", 1, &LidarProssesor::GetNewScan, this);		//Queue size selected so that we will drop waiting messages

	//initilize publishers
	out_pub = n.advertise<geometry_msgs::PoseStamped>("/lidar/minimum_pts", 10);
	
	//Get params
	n.param<double>("/lidar/processing/max_point_range_weight", max_point_range_weight_, 100.0);
	n.param<double>("/lidar/processing/angular_point_weight", angular_decay_, 0.05);
}

f_2dpt LidarProssesor::GeneratePoint(float magnitude, float angle)
{
	f_2dpt pt;
	pt.x = magnitude * cos(angle);
	pt.y = magnitude * sin(angle);
	return pt;
}

bool LidarProssesor::isOutOfRange(const float& range)
{
	return (range <= min_range_ || range >= max_range_ - 5);
}

/*
 *  Ignores Z values
 */
vector<f_2dpt> LidarProssesor::GeneratePoints()
{
	float current_angle = min_angle_;
		vector <f_2dpt> pts;
		//Remove outliers
		//ranges_.erase(remove_if(ranges_.begin(), ranges_.end(), LidarProssesor::isOutOfRange));		Won't work because I need to know the angles
		for(int iii = 0; iii < ranges_.size(); ++iii)
		{
			if(!isOutOfRange(ranges_[iii]))			//If in range make a point
			{
				//Generate points
				pts.push_back(GeneratePoint(ranges_[iii], current_angle));
			}
			else
			{
				//ummm... do nothing???
			}
			
			//Update angle
			current_angle += angle_inc_;
		}
}

float LidarProssesor::RangeToWeight(float range)
{
	//Linear maping of range to weight
	return range_to_weight_slope_ * range + range_to_weight_y_intercept_;
}

void LidarProssesor::FindBestVector()
{
	//Generate a psedo-potential feild in the 1 dimensional space (angular radians)
	float current_angle = min_angle_;
	float* potential_field = new float[ranges_.size()]/*{0}/*Not sure if this works*/;				//This should initilize array to 0
	
	//Give prefrence to the forward direction
	int center = ranges_.size() / 2;
	for(int iii = 0; iii < ranges_.size(); ++iii)
	{
		potential_field[iii] = abs(center - iii) * 0.1 * max_point_range_weight_;
	}
	
	//Remove outliers
	//ranges_.erase(remove_if(ranges_.begin(), ranges_.end(), LidarProssesor::isOutOfRange));		Won't work because I need to know the angles
	for(int iii = 0; iii < ranges_.size(); ++iii)
	{
		if(!isOutOfRange(ranges_[iii]))			//If in range make add to the vector fields
		{
			float nominal_weight = RangeToWeight(ranges_[iii]);
			
			//ROS_INFO("Nominal weight is %f", nominal_weight);
			
			//Generate weights
			for(int jjj = 0; jjj < ranges_.size(); ++jjj)
			{
				float new_weight = nominal_weight - (abs(jjj - iii) *  angular_decay_);
				if(new_weight <= 0)
				{
					//ROS_INFO("Add nothing!");
					continue;
				}
				//ROS_INFO("New_weight %f", new_weight);
				potential_field[jjj] += new_weight;
			}
		}
		/*else
		{
			//ummm... do nothing???
		}*/
		
		//Update angle
		current_angle += angle_inc_;
	}
	
	//Find the minimums
	//Defualt to forward
	int min = potential_field[ranges_.size() / 2];
	float angle_of_min = 0.0;
	for (int iii = 1; iii < ranges_.size(); ++iii)
	{
		if(min > potential_field[iii])
		{
			//ROS_INFO("Old min: %i, New Min: %i", min, potential_field[iii]);
			min = ranges_[iii];
			angle_of_min = min_angle_ + iii * angle_inc_;
		}
	}
	//ROS_INFO("Min angle is: %f, Max angle is: %f", min_angle_ * 180 / 3.14159, max_angle_ * 180 / 3.14159);
	ROS_INFO("Minimal angle is: %f degrees", angle_of_min * 180 / 3.14159);
	
	//Publish the result
	geometry_msgs::PoseStamped result;
	result.pose.position.x = 0;
	result.pose.position.y = 0;
	result.pose.position.z = 0;
	result.pose.orientation.x = 0;
	result.pose.orientation.y = 0;
	result.pose.orientation.z = 1;
	result.pose.orientation.w = cos(angle_of_min + 3.14159 / 2);
	result.header.frame_id = "lidar";
	result.header.stamp = ros::Time::now();
		
	out_pub.publish(result);
	
	delete[] potential_field;
	potential_field = NULL;
}

#endif
