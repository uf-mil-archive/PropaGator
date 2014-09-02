/****************************************************************************************************
 * 								euclidean_pc_segmentation_node									*
 ****************************************************************************************************
 * This node provides segmentation of a point cloud													*
 * The method used is segmentation by closeness, the euclidean distance is calculated and all points*
 * Within a radius will be grouped																	*
 ****************************************************************************************************/

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/search/kdtree.h>
#include <pcl/PointIndices.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

/***TODO:
 * 			Design a method to dynamically chain different filters together i.e. raw_pc --> noise_filter --> region_grow_segment --> object_detection
 * 					Possibly use an array of function pointers and execute in order
 * 			Create a map tf frame for slam
 * 			Move laser_scan onto the map instead of base_link
 * 			Remove all points below the surface of the water
 * 			Flatten all other points to 2d by projecting them onto the surface of the water
 * 			Deal with cases where no clusters are found
 ****/

class EuclideanClusterExtractor
{
private:	//Typedefs
	typedef pcl::PointXYZ point;

private:	//Vars
	ros::Subscriber pc_sub_;
	ros::Publisher pc_pub_;
	double cluster_tolerance_;			//Max distance in meters between points
	int min_cluster_size_;				//Minimum number of points in a cluster
	int max_cluster_size_;				//Maximum number of points in a cluster

private:	//Functions
	/********************************************************
	 *        			segment_cloud						*
	 ********************************************************
	 * This function takes a point cloud and segments it	*
	 * Then publishes it on segmented_pc					*
	 ********************************************************/
	void segmentCloud(const sensor_msgs::PointCloud2::ConstPtr& pc)
	{
		//Convert Point cloud msg to point cloud
		pcl::PointCloud<point>::Ptr raw_pc(new pcl::PointCloud<point>);
		pcl::fromROSMsg(*pc, *raw_pc);

		//Create KdTree for search method (see pcl documentation for more info on KdTrees)
		//And give it input
		pcl::search::KdTree<point>::Ptr tree(new pcl::search::KdTree<point>);
		tree->setInputCloud(raw_pc);

		//Create a vector of vectors which represent the indices of each cluster
		std::vector<pcl::PointIndices> cluster_indices;

		//Set up the Euclidean Extractor
		pcl::EuclideanClusterExtraction<point> extractor;
		extractor.setClusterTolerance(cluster_tolerance_);		//Points must be within cluster_tolerance meters of eachother
		extractor.setMinClusterSize(min_cluster_size_);			//Min number of points in cluster
		extractor.setMaxClusterSize(max_cluster_size_);			//Max number of points in cluster
		extractor.setSearchMethod(tree);						//search method
		extractor.setInputCloud(raw_pc);						//Input cloud

		//Perform extraction
		extractor.extract(cluster_indices);
		//pcl::extractEuclideanClusters(*raw_pc, cluster_indices, )
		ROS_INFO("Number of clusters is %i", (int)cluster_indices.size());

		//Publish the segmented cloud
		sensor_msgs::PointCloud2::Ptr out_pc(new sensor_msgs::PointCloud2);														//Create the ros msg

		for(std::vector<pcl::PointIndices>::const_iterator cluster_it = cluster_indices.begin();	//Iterate through clusters
				cluster_it != cluster_indices.end(); ++cluster_it)
		{
			pcl::PointCloud<point>::Ptr cluster_pc(new pcl::PointCloud<point>);						//Make a new point cloud

			for(std::vector<int>::const_iterator index_it = cluster_it->indices.begin();			//Iterate through indices
					index_it != cluster_it->indices.end(); index_it++)
			{
				cluster_pc->push_back(raw_pc->points[*index_it]);									//Add points to new cloud
			}

			cluster_pc->width = cluster_pc->points.size();											//Number of points
			cluster_pc->height = 1;																	//Organized 1D data
			cluster_pc->is_dense = true;															//No values of inf or NaN

			pcl::toROSMsg(*cluster_pc, *out_pc);													//Fill the msg
			out_pc->header.frame_id = pc->header.frame_id;											//Set frame id
			out_pc->header.stamp = pc->header.stamp;												//Set time stamp
			pc_pub_.publish(out_pc);																//Publish the msgs
		}

	}

public:		//Functions
	EuclideanClusterExtractor()
	{
		ros::NodeHandle private_nh("~");

		//Get some ros params
		std::string topic = private_nh.resolveName("euclidean_cluster_tolerance");		//Get the topic
		private_nh.param<double>(topic.c_str(), cluster_tolerance_, 0.5);
		ROS_INFO("Topic %s value %f", topic.c_str(), cluster_tolerance_);
		topic = private_nh.resolveName("euclidean_min_cluster_size");
		private_nh.param<int>(topic.c_str(), min_cluster_size_, 5);
		ROS_INFO("Topic %s value %i", topic.c_str(), min_cluster_size_);
		topic = private_nh.resolveName("euclidean_max_cluster_size");
		private_nh.param<int>(topic.c_str(), max_cluster_size_,  2147483647/*2^31 - 1*/);
		ROS_INFO("Topic %s value %i", topic.c_str(), max_cluster_size_);

		ros::NodeHandle public_nh;
		//Set up subscribers and publishers
		topic = public_nh.resolveName("raw_pc");		//Get the topic
		pc_sub_ = public_nh.subscribe<sensor_msgs::PointCloud2>(topic.c_str(), 100, &EuclideanClusterExtractor::segmentCloud, this);
		pc_pub_ = public_nh.advertise<sensor_msgs::PointCloud2>("segmented_pc", 100);
	}
};

/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "euclidean_pc_segmentation_node");
	ros::NodeHandle nh;
	ros::Rate update_rate(100);

	//Set up the extractor
	EuclideanClusterExtractor seg;

	while(ros::ok())
	{
		update_rate.sleep();
		ros::spinOnce();
	}
	return 0;
}
