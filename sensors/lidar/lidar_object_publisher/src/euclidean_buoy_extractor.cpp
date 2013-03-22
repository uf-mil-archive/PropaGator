#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <visualization_msgs/MarkerArray.h>
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

ros::Publisher filtered_pub;
ros::Publisher large_cloud_pub;
ros::Publisher large_filtered_pub;
ros::Publisher small_cloud_pub;
ros::Publisher buoy_cloud_pub;
ros::Publisher buoy_marker_pub;

const bool filter_small = false;	// Should we run an EC on smaller objects than the buoy?
const bool filter_large = true;

const int MIN_PTS = 10;			// When should we toss out a pcl when too small

const float MAX_BUOY_HEIGHT = 0.5;//1.5 for old bag
const float MIN_BUOY_HEIGHT = -0.5;//0 old bag

const int MAX_PCL = 1000000;
const int MAX_BUOY_PCL = 60;
const int MIN_BUOY_PCL = 5;
const int MIN_PCL = 1;

const float LARGE_CLUSTER_TOLERANCE = 1.0;
const float BUOY_CLUSTER_TOLERANCE = 0.2;
const float SMALL_CLUSTER_TOLERANCE = 0.2;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& input) {
  ROS_DEBUG("Got new pointcloud.");
 // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
  std_msgs::Header header = input->header;
  pcl::fromROSMsg(*input, *cloud);

  ROS_DEBUG("PointCloud before filtering has: %f data points.", cloud->points.size());

  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  pcl::VoxelGrid<pcl::PointXYZ> vg;
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
  vg.setInputCloud(cloud);
  vg.setLeafSize(0.04f, 0.04f, 0.04f);//adjusts the voxel size (0.1 = .1 meter voxel) this gives us 4cm voxels
  vg.filter(*cloud_filtered);
  ROS_DEBUG("PointCloud after filtering has: %f data points.", cloud_filtered->points.size());

  std_msgs::Header filtered_header = cloud_filtered->header;
  sensor_msgs::PointCloud2 filtered_out;
  pcl::toROSMsg(*cloud_filtered, filtered_out);
  filtered_out.header = filtered_header;
  filtered_pub.publish(filtered_out);

  int j = 0;

  /*****************************************************************
   **  Start by removing clusters that are too large to be buoys  **
   *****************************************************************/

  if (filter_large && cloud_filtered->points.size() > MIN_PTS) {
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

    j = 0;
    for (std::vector<pcl::PointIndices>::const_iterator it = large_cluster_indices.begin(); (it != large_cluster_indices.end()); ++it) {
      pcl::PointCloud<pcl::PointXYZ>::Ptr large_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);

      *large_cluster_point_indices = large_cluster_indices[j];

      for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
        large_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
      }

      large_cloud_cluster->width = large_cloud_cluster->points.size();
      large_cloud_cluster->height = 1;
      large_cloud_cluster->is_dense = true;

      ROS_DEBUG("PointCloud cluster too large to be buoy: %f data points.", large_cloud_cluster->points.size());
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
  }

  /*******************************************************************
   **  Next remove clusters too small to be buoys (maybe optional)  **
   *******************************************************************/

  if (filter_small && cloud_filtered->points.size() > MIN_PTS) {
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
    for (std::vector<pcl::PointIndices>::const_iterator it = small_cluster_indices.begin(); (it != small_cluster_indices.end()); ++it) {
      pcl::PointCloud<pcl::PointXYZ>::Ptr small_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
      *small_cluster_point_indices = small_cluster_indices[j];

      for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
        small_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
      }
      small_cloud_cluster->width = small_cloud_cluster->points.size();
      small_cloud_cluster->height = 1;
      small_cloud_cluster->is_dense = true;

      ROS_DEBUG("PointCloud cluster too small to be buoy: %f data points.", small_cloud_cluster->points.size());

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

  if (cloud_filtered->points.size() > MIN_PTS) {
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

    visualization_msgs::MarkerArray buoy_marker_array;
    visualization_msgs::Marker buoy_marker;

    pcl::PointCloud<pcl::PointXYZ> buoy_cloud;

    j = 0;
    for (std::vector<pcl::PointIndices>::const_iterator it = buoy_cluster_indices.begin(); (it != buoy_cluster_indices.end()); ++it) {
      pcl::PointCloud<pcl::PointXYZ>::Ptr buoy_cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
      *buoy_cluster_point_indices = buoy_cluster_indices[j];

      for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++) {
        buoy_cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
      }
      buoy_cloud_cluster->width = buoy_cloud_cluster->points.size();
      buoy_cloud_cluster->height = 1;
      buoy_cloud_cluster->is_dense = true;

      ROS_DEBUG("PointCloud representing the buoy Cluster: %f data points.", buoy_cloud_cluster->points.size());
      buoy_cloud = buoy_cloud + *buoy_cloud_cluster;

      sensor_msgs::PointCloud2 cloud_out;
      pcl::toROSMsg(*buoy_cloud_cluster, cloud_out);
      cloud_out.header = header;
      buoy_cloud_pub.publish(cloud_out);
      Eigen::Vector4f centroid;
      pcl::compute3DCentroid(*buoy_cloud_cluster, centroid);

      //convert to marker array
      if (centroid[2] < MAX_BUOY_HEIGHT && centroid[2] > MIN_BUOY_HEIGHT) {
        buoy_marker.header.frame_id = "/world";
        buoy_marker.header.stamp = ros::Time::now();
        //buoy_marker.ns = "basic_shapes";
        buoy_marker.id = j;
        buoy_marker.type = visualization_msgs::Marker::SPHERE;
        buoy_marker.action = visualization_msgs::Marker::ADD;
        buoy_marker.pose.position.x = centroid[0];
        buoy_marker.pose.position.y = centroid[1];
        buoy_marker.pose.position.z = centroid[2];
        buoy_marker.pose.orientation.x = 0.0;
        buoy_marker.pose.orientation.y = 0.0;
        buoy_marker.pose.orientation.z = 0.0;
        buoy_marker.pose.orientation.w = 1.0;
        buoy_marker.scale.x = 0.3;
        buoy_marker.scale.y = 0.3;
        buoy_marker.scale.z = 0.4;
        buoy_marker.color.r = 1.0f;
        buoy_marker.color.g = 0.0f;
        buoy_marker.color.b = 0.0f;
        buoy_marker.color.a = 1.0;
        buoy_marker.lifetime = ros::Duration();
        buoy_marker_array.markers.push_back(buoy_marker);
      }

      j++;
    }
  //  buoy_marker_array.header = header;
    buoy_marker_pub.publish(buoy_marker_array);

    sensor_msgs::PointCloud2 buoy_cloud_out;
    pcl::toROSMsg(buoy_cloud, buoy_cloud_out);
    buoy_cloud_out.header = header;
    buoy_cloud_pub.publish(buoy_cloud_out);
  }
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init(argc, argv, "euclidean_buoy_extractor");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe("/cloud_3d", 1, cloud_callback);

  // Create a ROS publisher for the output point cloud
  filtered_pub = nh.advertise<sensor_msgs::PointCloud2>("/filtered_cloud", 1);
  large_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/large_cloud", 1);
  large_filtered_pub = nh.advertise<sensor_msgs::PointCloud2>("/large_filtered_cloud", 1);
  small_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/small_cloud", 1);
  buoy_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/buoy_cloud", 1);

  buoy_marker_pub = nh.advertise<visualization_msgs::MarkerArray>("/buoy_markers", 1);

  // Spin
  ros::spin();
}
