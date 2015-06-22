/****************************************************************************************************
 * 									Laser_to_pc_node												*
 ****************************************************************************************************
 * The laser to point cloud node converts laser scans taken from the Lidar lms1xx node and converts *
 * them into standard point clouds from the point cloud library. The output of this node is 		*
 * lidar/raw_pc  																					*
 ****************************************************************************************************/

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <laser_geometry/laser_geometry.h>
#include <tf/transform_listener.h>
#include <ros/time.h>
#include <ros/duration.h>
#include <cmath>

/****************************************
 * 				LaserToPC2				*
 ****************************************
 * This class provides the functionality*
 * for transforming laser_scans to PCs	*
 * Additionally this class was made so 	*
 * that the transform would remain in 	*
 * Scope. This is necessary otherwise 	*
 * the Transform would constantly fail	*
 ****************************************/
//TODO: Make a singleton
class LaserToPC2
{
private:
	tf::TransformListener transform_;			//Used to obtain transform from lidar frame_
	ros::Subscriber laser_sub_;					//Subscriber for Raw Laser Scan
	laser_geometry::LaserProjection projector_;	//Projection class to project from lidar frame to frame_
	std::string frame_;							//Frame to transform to
	ros::Publisher pc_pub_;						//point cloud publisher

private:
	// Check to make sure the point is not on the boat
	//	Since this check is done in the frame of reference of the lidar,
	//	the lidar scans are 2d, and the lidar's field of view is limited to 270 deg
	//	only the Y direction of angles with absolute value greater than 90 deg matter
	bool isSelf(float mag, float angle)
	{
		angle = fabs(angle);
		if(angle > (M_PI / 2.0))
		{
			// Check Y values
			float y = sin(angle) * mag;
			if (y < 0.65)	// Boat is about 1 m wide
			{
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	void ConvertLaser(const sensor_msgs::LaserScan::ConstPtr& scan_in)
	{
		sensor_msgs::LaserScan temp = *scan_in;
		sensor_msgs::LaserScan* scan = &temp;

		// Remove points that are to close and or the boat itself
		//	The LaserProjection class only removes points that are to far away
		//	It does not remove points that are to close
		//	To workaround this we iterate through the raw laser scan and change all values
		//	that are minimum value to above maximum value
		float angle = scan->angle_min;
		for(std::vector<float>::iterator it = scan->ranges.begin(); it != scan->ranges.end(); ++it)
		{
			// 1 cm epsilon
			if (*it <= scan->range_min + 0.01 || isSelf(*it, angle))
			{
				*it = scan->range_max + 0.01;
			}

			// Increment
			angle += scan->angle_increment;
		}

		//Convert to point cloud
		sensor_msgs::PointCloud2 cloud;					//Define a new pc msg
		bool transform_ready = false;
		try
		{
			//Wait until available
			if(transform_.waitForTransform(
						scan->header.frame_id,							//Transform from lidar
						frame_.c_str(),	 								//to frame_
																		//Most recent time (start_time + number_of_points * time_increment)
						scan->header.stamp + ros::Duration().fromSec(scan->ranges.size() * scan->time_increment),
						ros::Duration(2)))								//Timeout after 2 seconds
			{
				transform_ready = true;
			}
			else
			{
				ROS_WARN("Transform from %s to %s timed out", scan->header.frame_id.c_str(), frame_.c_str());
			}

		}
		catch (tf::TransformException& e)
		{
			ROS_ERROR("Transform error from %s to %s: %s", scan->header.frame_id.c_str(), frame_.c_str(), e.what());
		}

		//If we can transform then do it!
		if(transform_ready)
		{
			//Project laser scan into a point cloud in frame_
			try		// It is possible that some of the transforms are to far into the past
			{
				projector_.transformLaserScanToPointCloud(frame_.c_str(), *scan, cloud, transform_);
			}
			catch(tf::TransformException& e)
			{
				ROS_ERROR("Transform error from %s to %s: %s", scan->header.frame_id.c_str(), frame_.c_str(), e.what());
			}

			//Publish the cloud
			pc_pub_.publish(cloud);
		}
	}

public:
	//TODO: Replace base_link with map for slam
	LaserToPC2():
		frame_("/enu")
	{
		ros::NodeHandle nh;
		ros::param::param<std::string>("~pc_tf_frame", frame_, "/enu");
		std::string topic = nh.resolveName("scan");		//Get the topic
		pc_pub_ = nh.advertise<sensor_msgs::PointCloud2>("raw_pc", 100);
		laser_sub_ = nh.subscribe<sensor_msgs::LaserScan>(topic.c_str(), 10, &LaserToPC2::ConvertLaser, this);
	}

};

/********************************
 * 			Main				*
 ********************************/

int main(int argc, char** argv)
{
	//Initilize ros
	ros::init(argc, argv, "laser_to_pc_node");
	ros::NodeHandle nh;
	ros::Rate update_rate(100);					//Update every 100Hz

	LaserToPC2 converter;

	while(ros::ok())
	{
		update_rate.sleep();
		ros::spinOnce();
	}

	return 0;
}
