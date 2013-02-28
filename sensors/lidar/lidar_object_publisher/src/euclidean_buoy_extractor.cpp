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
ros::Publisher buoy_cloud_pub;
ros::Publisher buoy_pose_pub;

const int max_objects = 10;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& input) {
  ROS_ERROR("NEW POINTCLOUD");
 // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
  std_msgs::Header header = input->header;
  pcl::fromROSMsg(*input, *cloud);

  ROS_DEBUG("PointCloud before filtering has: %d data points.", cloud->points.size());

  // Create the filtering object: downsample the dataset using a leaf size of 1cm
  pcl::VoxelGrid<pcl::PointXYZ> vg;
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
  vg.setInputCloud(cloud);
  vg.setLeafSize(0.01f, 0.01f, 0.01f);//adjusts the voxel size (0.1 = .1 meter voxel) this gives us 1cm voxels
  vg.filter(*cloud_filtered);
  ROS_DEBUG("PointCloud after filtering has: %d data points.", cloud_filtered->points.size());


  std_msgs::Header filtered_header = cloud_filtered->header;
  sensor_msgs::PointCloud2 filtered_out;
  pcl::toROSMsg(*cloud_filtered, filtered_out);
  filtered_out.header = filtered_header;
  filtered_pub.publish(filtered_out);

  // Creating the KdTree object for the search method of the extraction
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
  tree->setInputCloud(cloud_filtered);

  std::vector<pcl::PointIndices> cluster_indices;
  pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
  ec.setClusterTolerance(0.2); //0.02 = 2cm	//0.25 found buoy...
  ec.setMinClusterSize(5);//100
  ec.setMaxClusterSize(60);//25000
  ec.setSearchMethod(tree);
  ec.setInputCloud(cloud_filtered);
  ec.extract(cluster_indices);

  geometry_msgs::PoseArray buoy_pose_msg;
  geometry_msgs::Pose buoy_pose;

  int j = 0;
  for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); (it != cluster_indices.end()) && (j < max_objects); ++it)
  {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
    for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++)
      cloud_cluster->points.push_back(cloud_filtered->points[*pit]);
    cloud_cluster->width = cloud_cluster->points.size();
    cloud_cluster->height = 1;
    cloud_cluster->is_dense = true;

    ROS_DEBUG("PointCloud representing the Cluster: %d data points.", cloud_cluster->points.size());

    sensor_msgs::PointCloud2 cloud_out;
    pcl::toROSMsg(*cloud_cluster, cloud_out);
    cloud_out.header = header;
    buoy_cloud_pub.publish(cloud_out);
    Eigen::Vector4f centroid;
    pcl::compute3DCentroid(*cloud_cluster, centroid);

    //convert to pose array
    buoy_pose.position.x = centroid[0];
    buoy_pose.position.y = centroid[1];
    buoy_pose.position.z = centroid[2];
    buoy_pose_msg.poses.push_back(buoy_pose);

    j++;
  }
  buoy_pose_msg.header = header;
  buoy_pose_pub.publish(buoy_pose_msg);
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/cloud_3d", 1, cloud_callback);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("/lidar_object", 1);
  filtered_pub = nh.advertise<sensor_msgs::PointCloud2> ("/filtered_cloud1", 1);
  buoy_cloud_pub = nh.advertise<sensor_msgs::PointCloud2> ("/buoy_cloud", 1);
  buoy_pose_pub = nh.advertise<geometry_msgs::PoseArray>("/buoy_pose", 0);

  // Spin
  ros::spin ();
}
