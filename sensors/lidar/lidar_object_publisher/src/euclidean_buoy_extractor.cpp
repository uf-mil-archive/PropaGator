#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Pose.h>
// PCL specific includes
#include <pcl/ros/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/segmentation/sac_segmentation.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/segmentation/extract_clusters.h>

#include <Eigen/Core>
#include <Eigen/LU>

ros::Publisher pub;
ros::Publisher filtered_pub;
ros::Publisher large_cloud_pub;
ros::Publisher large_filtered_pub;
ros::Publisher small_cloud_pub;
ros::Publisher buoy_cloud_pub;
ros::Publisher buoy_pose_pub;

const int max_objects = 100;

const bool filter_small = false;	// Should we run an EC on smaller objects than the buoy?

const float MAX_BUOY_HEIGHT = 1.5;
const float MIN_BUOY_HEIGHT = 0;

const int MAX_PCL = 1000000;
const int MAX_BUOY_PCL = 60;//60
const int MIN_BUOY_PCL = 5;//5
const int MIN_PCL = 1;

const float LARGE_CLUSTER_TOLERANCE = 1.0;
const float BUOY_CLUSTER_TOLERANCE = 0.2;
const float SMALL_CLUSTER_TOLERANCE = 0.2;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& input) {
  ROS_ERROR("Got new pointcloud.");
 // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
  std_msgs::Header header = input->header;
  pcl::fromROSMsg(*input, *cloud);

/*//if not using voxel downsampling:

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::fromROSMsg(*input, *cloud_filtered);
*/


  ROS_DEBUG("PointCloud before filtering has: %d data points.", cloud->points.size());

  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  pcl::VoxelGrid<pcl::PointXYZ> vg;
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
  vg.setInputCloud(cloud);
  vg.setLeafSize(0.04f, 0.04f, 0.04f);//adjusts the voxel size (0.1 = .1 meter voxel) this gives us 1cm voxels
		//was all 0.01, 0.02 looks ok
  vg.filter(*cloud_filtered);
  ROS_DEBUG("PointCloud after filtering has: %d data points.", cloud_filtered->points.size());

  std_msgs::Header filtered_header = cloud_filtered->header;
  sensor_msgs::PointCloud2 filtered_out;
  pcl::toROSMsg(*cloud_filtered, filtered_out);
  filtered_out.header = filtered_header;
  filtered_pub.publish(filtered_out);

/*****************************************************************
 **  Start by removing clusters that are too large to be buoys  **
 *****************************************************************/

  // Creating the KdTree object for the search method of the extraction
  pcl::search::KdTree<pcl::PointXYZ>::Ptr large_tree (new pcl::search::KdTree<pcl::PointXYZ>);
  large_tree->setInputCloud(cloud_filtered);

  std::vector<pcl::PointIndices> large_cluster_indices;
  pcl::PointIndices::Ptr large_cluster_point_indices (new pcl::PointIndices);

  pcl::EuclideanClusterExtraction<pcl::PointXYZ> large_ec;
  large_ec.setClusterTolerance(LARGE_CLUSTER_TOLERANCE);
  large_ec.setMinClusterSize(MAX_BUOY_PCL);
  large_ec.setMaxClusterSize(MAX_PCL);
  large_ec.setSearchMethod(large_tree);
  large_ec.setInputCloud(cloud_filtered);
  large_ec.extract(large_cluster_indices);

  pcl::PointCloud<pcl::PointXYZ> large_cloud;

  int j = 0;
  for (std::vector<pcl::PointIndices>::const_iterator it = large_cluster_indices.begin(); (it != large_cluster_indices.end())/* && (j < max_objects)*/; ++it) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr large_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);

    *large_cluster_point_indices = large_cluster_indices[j];

    for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
      large_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
    }

    large_cloud_cluster->width = large_cloud_cluster->points.size();
    large_cloud_cluster->height = 1;
    large_cloud_cluster->is_dense = true;

    ROS_DEBUG("PointCloud cluster too large to be buoy: %d data points.", large_cloud_cluster->points.size());
    large_cloud = large_cloud + *large_cloud_cluster;

    // Extract the planar inliers from the input cloud
    pcl::ExtractIndices<pcl::PointXYZ> large_extract;
    large_extract.setInputCloud(cloud_filtered);
    large_extract.setIndices(large_cluster_point_indices);
    // Remove the planar inliers, extract the rest
    large_extract.setNegative(true);
    large_extract.filter(*cloud_f);
    *cloud_filtered = *cloud_f;

    j++;
  }
  sensor_msgs::PointCloud2 large_cloud_out;
  pcl::toROSMsg(large_cloud, large_cloud_out);
  large_cloud_out.header = header;
  large_cloud_pub.publish(large_cloud_out);

  sensor_msgs::PointCloud2 filt_out;
  pcl::toROSMsg(*cloud_filtered, filt_out);
  filt_out.header = header;
  large_filtered_pub.publish(filt_out);

/*******************************************************************
 **  Next remove clusters too small to be buoys (maybe optional)  **
 *******************************************************************/

  if (filter_small) {
    // Creating the KdTree object for the search method of the extraction
    pcl::search::KdTree<pcl::PointXYZ>::Ptr small_tree (new pcl::search::KdTree<pcl::PointXYZ>);
    small_tree->setInputCloud(cloud_filtered);

    std::vector<pcl::PointIndices> small_cluster_indices;
    pcl::PointIndices::Ptr small_cluster_point_indices (new pcl::PointIndices);

    pcl::EuclideanClusterExtraction<pcl::PointXYZ> small_ec;
    small_ec.setClusterTolerance(SMALL_CLUSTER_TOLERANCE);
    small_ec.setMinClusterSize(MIN_PCL);
    small_ec.setMaxClusterSize(MIN_BUOY_PCL);
    small_ec.setSearchMethod(small_tree);
    small_ec.setInputCloud(cloud_filtered);
    small_ec.extract(small_cluster_indices);

    pcl::PointCloud<pcl::PointXYZ> small_cloud;

    j = 0;
    for (std::vector<pcl::PointIndices>::const_iterator it = small_cluster_indices.begin(); (it != small_cluster_indices.end()) && (j < max_objects); ++it) {
      pcl::PointCloud<pcl::PointXYZ>::Ptr small_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
      *small_cluster_point_indices = small_cluster_indices[j];

      for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
        small_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
      }
      small_cloud_cluster->width = small_cloud_cluster->points.size();
      small_cloud_cluster->height = 1;
      small_cloud_cluster->is_dense = true;

      ROS_DEBUG("PointCloud cluster too small to be buoy: %d data points.", small_cloud_cluster->points.size());

      small_cloud = small_cloud + *small_cloud_cluster;

      // Extract the planar inliers from the input cloud
      pcl::ExtractIndices<pcl::PointXYZ> small_extract;
      small_extract.setInputCloud(cloud_filtered);
      small_extract.setIndices(small_cluster_point_indices);
      // Remove the planar inliers, extract the rest
      small_extract.setNegative(true);
      small_extract.filter(*cloud_f);
      *cloud_filtered = *cloud_f;

      j++;
    }
    sensor_msgs::PointCloud2 small_cloud_out;
    pcl::toROSMsg(small_cloud, small_cloud_out);
    small_cloud_out.header = header;
    small_cloud_pub.publish(small_cloud_out);
  }

/***************************************************
 **  Now extract remaining clusters of buoy size  **
 ***************************************************/

  // Creating the KdTree object for the search method of the extraction
  pcl::search::KdTree<pcl::PointXYZ>::Ptr buoy_tree (new pcl::search::KdTree<pcl::PointXYZ>);
  buoy_tree->setInputCloud(cloud_filtered);

  std::vector<pcl::PointIndices> buoy_cluster_indices;
  pcl::PointIndices::Ptr buoy_cluster_point_indices (new pcl::PointIndices);

  pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
  ec.setClusterTolerance(BUOY_CLUSTER_TOLERANCE);
  ec.setMinClusterSize(MIN_BUOY_PCL);
  ec.setMaxClusterSize(MAX_BUOY_PCL);
  ec.setSearchMethod(buoy_tree);
  ec.setInputCloud(cloud_filtered);
  ec.extract(buoy_cluster_indices);

  geometry_msgs::PoseArray buoy_pose_msg;
  geometry_msgs::Pose buoy_pose;

  pcl::PointCloud<pcl::PointXYZ> buoy_cloud;

  j = 0;
  for (std::vector<pcl::PointIndices>::const_iterator it = buoy_cluster_indices.begin(); (it != buoy_cluster_indices.end())/* && (j < max_objects)*/; ++it) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr buoy_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
    *buoy_cluster_point_indices = buoy_cluster_indices[j];

    for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
      buoy_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
    }
    buoy_cloud_cluster->width = buoy_cloud_cluster->points.size();
    buoy_cloud_cluster->height = 1;
    buoy_cloud_cluster->is_dense = true;

    ROS_DEBUG("PointCloud representing the buoy Cluster: %d data points.", buoy_cloud_cluster->points.size());
    buoy_cloud = buoy_cloud + *buoy_cloud_cluster;

    sensor_msgs::PointCloud2 cloud_out;
    pcl::toROSMsg(*buoy_cloud_cluster, cloud_out);
    cloud_out.header = header;
    buoy_cloud_pub.publish(cloud_out);
    Eigen::Vector4f centroid;
    pcl::compute3DCentroid(*buoy_cloud_cluster, centroid);

    //convert to pose array
    if (centroid[2] < MAX_BUOY_HEIGHT && centroid[2] > MIN_BUOY_HEIGHT) {
      buoy_pose.position.x = centroid[0];
      buoy_pose.position.y = centroid[1];
      buoy_pose.position.z = centroid[2];
      buoy_pose_msg.poses.push_back(buoy_pose);
    }
/*//not really needed right now...
    // Extract the planar inliers from the input cloud
    pcl::ExtractIndices<pcl::PointXYZ> buoy_extract;
    buoy_extract.setInputCloud(cloud_filtered);
    buoy_extract.setIndices(buoy_cluster_point_indices);
    // Remove the planar inliers, extract the rest
    buoy_extract.setNegative(true);
    buoy_extract.filter(*cloud_f);
    *cloud_filtered = *cloud_f;
*/
    j++;
  }
  buoy_pose_msg.header = header;
  buoy_pose_pub.publish(buoy_pose_msg);

  sensor_msgs::PointCloud2 buoy_cloud_out;
  pcl::toROSMsg(buoy_cloud, buoy_cloud_out);
  buoy_cloud_out.header = header;
  buoy_cloud_pub.publish(buoy_cloud_out);

}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init(argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe("/cloud_3d", 1, cloud_callback);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2>("/lidar_object", 1);
  filtered_pub = nh.advertise<sensor_msgs::PointCloud2>("/filtered_cloud", 1);
  large_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/large_cloud", 1);
  large_filtered_pub = nh.advertise<sensor_msgs::PointCloud2>("/large_filtered_cloud", 1);
  small_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/small_cloud", 1);
  buoy_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/buoy_cloud", 1);

  buoy_pose_pub = nh.advertise<geometry_msgs::PoseArray>("/buoy_pose", 1);

  // Spin
  ros::spin();
}
