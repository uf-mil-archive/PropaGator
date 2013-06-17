#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "pcl/point_cloud.h"
#include "std_msgs/Bool.h"
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include "pointcloud_publisher/SavePCLData.h"
#include <iostream>
#include <fstream>
#include <sstream>


/*creates a service called save_cloudxyz. to use it, run this node and
  then rosservice call /save_cloudxyz*/

using namespace std;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

PointCloud pclmsg;// (new PointCloud);
bool new_data = false;
bool publish_ok = true;
int fileIterator = 0;
int cloudcount = 0;
int max_size = 1;

void pclCallback(const sensor_msgs::PointCloud2::ConstPtr& msg){

	//convert the point cloud2 msg to a pointcloud msg
	pcl::fromROSMsg(*msg, pclmsg);
        if (cloudcount < 10){
                cloudcount++;
                if (pclmsg.points.size() > max_size){
                        max_size = pclmsg.points.size();
                         ROS_WARN("current max is %i",max_size); 
                 }
        }
        else if (cloudcount == 10){
                publish_ok = false;
                cloudcount++;
                 ROS_WARN("final max is %i",max_size); 
        }
        //new_data = true;
        if ((!publish_ok) && (pclmsg.points.size() == max_size)){
                ofstream pcldatafile;
	        char filename [50];
	        /*save file so its format matches what the cmu calibration
	        toolbox is expecting*/
	        //sprintf(filename, "..//cloudxyz_data//laser_board%d.xyz", fileIterator);
                //sprintf(filename, "~//laser_board%d.xyz", fileIterator);
	        fileIterator++;
	        pcldatafile.open("laser_board0.xyz");
	        size_t i;
	        for (i = 0; i < pclmsg.points.size(); ++i){
                        if (!(pclmsg.points[i].z < -1) && !(pclmsg.points[i].x < 0) && !(pclmsg.points[i].y > 2)){
		                pcldatafile << " " << pclmsg.points[i].x << " " << pclmsg.points[i].y << " " << pclmsg.points[i].z << "\n";
                        }	        
                }
                pcldatafile.close();
                ROS_WARN("%i",pclmsg.points.size()); 
                publish_ok = true;
        } 
 
}

bool save(pointcloud_publisher::SavePCLData::Request &req,
			pointcloud_publisher::SavePCLData::Response &res){
	if(publish_ok){
		ofstream pcldatafile;
		char filename [50];
		/*save file so its format matches what the cmu calibration
		toolbox is expecting*/
		sprintf(filename, "..//cloudxyz_data//laser_board%d.xyz", fileIterator);
		fileIterator++;
		pcldatafile.open(filename);
		size_t i;
		for (i = 0; i < pclmsg.points.size(); ++i){
			pcldatafile << " " << pclmsg.points[i].x << " " << pclmsg.points[i].y << " " << pclmsg.points[i].z << "\n";
		}
		pcldatafile.close();
		return true;
	}
	return false;
}


int main(int argc, char** argv) {
      
	ros::init(argc, argv, "convert_PCL2_to_PCL");
	ros::NodeHandle node_;
       
	ros::Subscriber pcl2_sub_ = node_.subscribe<sensor_msgs::PointCloud2> ("/cloud_3d", 100, pclCallback);
	ros::Publisher pcl_pub_ = node_.advertise<sensor_msgs::PointCloud> ("/pcl_pointcloud", 100);
	//ros::ServiceServer service = node_.advertiseService("/save_cloudxyz", &save);
       
        
	while(ros::ok()){
                ros::spinOnce();    
        }
	return 0;
}

