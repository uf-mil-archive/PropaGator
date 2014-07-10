#ifndef LIDAR_PROSSESOR_H_
#define LIDAR_PROSSESOR_H_

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <vector>
#include "tf/transform_listener.h"
//#include <algorithm>		//for remove_if
#include <cmath>

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


	/*
	 * Private functions
	 */
private:
	void GetNewScan(const sensor_msgs::LaserScan& scan);

	void FindBestVector(vector<f_2dpt> pts);
	
	//static bool isOutOfRange(const float& range);
	bool isOutOfRange(const float& range);
	
	f_2dpt GeneratePoint(float magnitude, float angle);
	
	vector<f_2dpt> GeneratePoints();
	
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
		}
		ranges_ = scan.ranges;
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
			FindBestVector(GeneratePoints());
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
	return (range <= LidarProssesor::min_range_ || range >= LidarProssesor::max_range_);
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

void LidarProssesor::FindBestVector(vector<f_2dpt> pts)
{
	
}

#endif
