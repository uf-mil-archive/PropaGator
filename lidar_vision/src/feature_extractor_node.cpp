#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

class FeatureExtractor
{
private:	//Vars
	ros::Subscriber pc_sub_;
	ros::Publisher bouy_pub_, shore_pub_;

private:	//Functions
	void extract(const sensor_msgs::PointCloud2::ConstPtr& pc)
	{

	}

public:		//Functions
	//Constructor
	FeatureExtractor()
	{
		ros::NodeHandle private_nh("~");

		std::string topic;

		//Get some ros params
		//topic = private_nh.resolveName("euclidean_cluster_tolerance");		//Get the topic
		//private_nh.param<double>(topic.c_str(), cluster_tolerance_, 0.5);
		//ROS_INFO("Topic %s value %f", topic.c_str(), cluster_tolerance_);

		ros::NodeHandle public_nh;
		//Set up subscribers and publishers
		topic = public_nh.resolveName("segmented_pc");		//Get the topic
		pc_sub_ = public_nh.subscribe<sensor_msgs::PointCloud2>(topic.c_str(), 100, &FeatureExtractor::extract, this);
	}
};

/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "feature_extractor_node");
	ros::NodeHandle nh;
	ros::Rate update_rate(100);

	//Set up the extractor
	FeatureExtractor extractor;

	while(ros::ok())
	{
		update_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
