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
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/normal_3d.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>

#include <Eigen/Core>
#include <Eigen/LU>

ros::Publisher filtered_pub;
ros::Publisher large_cloud_pub;
ros::Publisher large_filtered_pub;
ros::Publisher small_cloud_pub;
ros::Publisher buoy_cloud_pub;
ros::Publisher buoy_marker_pub;

typedef pcl::PointXYZ PointT;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& input) {
    ROS_DEBUG("Got new pointcloud.");
    // Convert the sensor_msgs/PointCloud2 data to pcl/PointCloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>), cloud_f (new pcl::PointCloud<pcl::PointXYZ>);
    std_msgs::Header header = input->header;
    pcl::fromROSMsg(*input, *cloud);
    //all the objects needed
    pcl::PCDReader reader;
    pcl::PassThrough<PointT> pass;
    pcl::NormalEstimation<PointT, pcl::Normal> ne;
    pcl::SACSegmentationFromNormals<PointT, pcl::Normal> seg; 
    pcl::PCDWriter writer;
    pcl::ExtractIndices<PointT> extract;
    pcl::ExtractIndices<pcl::Normal> extract_normals;
    pcl::search::KdTree<PointT>::Ptr tree (new pcl::search::KdTree<PointT> ());
    //data sets
    pcl::PointCloud<PointT>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);
    pcl::PointCloud<PointT>::Ptr cloud_filtered2 (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals2 (new pcl::PointCloud<pcl::Normal>);
    pcl::ModelCoefficients::Ptr coefficients_plane (new pcl::ModelCoefficients), coefficients_sphere (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers_plane (new pcl::PointIndices), inliers_sphere (new pcl::PointIndices);
    ROS_DEBUG("PointCloud before filtering has: %i data points.", (int)cloud->points.size());
    
    //filter our NaNs
    pass.setInputCloud (cloud);
    pass.setFilterFieldName ("z");
    pass.setFilterLimits (0, 2.0);    
    pass.filter (*cloud_filtered);
    ROS_DEBUG("PointCloud after filtering has: %i data points." , (int)cloud_filtered->points.size());
    
    *cloud_filtered = *cloud;//remove the pass through filter basically
    //estimate normal points
    ne.setSearchMethod (tree);
    ne.setInputCloud (cloud_filtered);
    //ne.setKSearch(10);
    ne.setRadiusSearch(0.025);
    ne.compute (*cloud_normals);

    // Create the segmentation object for the planar model and set all the parameters
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl::SACMODEL_NORMAL_PLANE);
    seg.setNormalDistanceWeight (0.05);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setMaxIterations (100);
    seg.setDistanceThreshold (5.0);
    seg.setInputCloud (cloud_filtered);
    seg.setInputNormals (cloud_normals);
    // Obtain the plane inliers and coefficients
    seg.segment (*inliers_plane, *coefficients_plane);
    std::cerr << "Plane coefficients: " << *coefficients_plane << std::endl;

    // Extract the planar inliers from the input cloud
    extract.setInputCloud (cloud_filtered);
    extract.setIndices (inliers_plane);
    extract.setNegative (false);
    
    // Remove the planar inliers, extract the rest
    extract.setNegative (true);
    extract.filter (*cloud_filtered2);
    extract_normals.setNegative (true);
    extract_normals.setInputCloud (cloud_normals);
    extract_normals.setIndices (inliers_plane);
    extract_normals.filter (*cloud_normals2);
    
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl::SACMODEL_SPHERE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setNormalDistanceWeight (0.1);
    seg.setMaxIterations (100);
    seg.setDistanceThreshold (5.0);
    seg.setRadiusLimits (0, 1.0);
    seg.setInputCloud (cloud_filtered2);
    seg.setInputNormals (cloud_normals2);
    
    extract.setInputCloud (cloud_filtered2);
    extract.setIndices (inliers_sphere);
    extract.setNegative (false);
    pcl::PointCloud<PointT>::Ptr sphere_cloud (new pcl::PointCloud<PointT> ());
    extract.filter(*sphere_cloud);
    
    pcl::PointCloud<pcl::PointXYZ> sphere_cloud_in = *sphere_cloud;
    sensor_msgs::PointCloud2 sphere_cloud_out;
    pcl::toROSMsg(sphere_cloud_in, sphere_cloud_out);
    sphere_cloud_out.header = header;
    buoy_cloud_pub.publish(sphere_cloud_out);

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
  buoy_cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/sphere_cloud", 1);

  buoy_marker_pub = nh.advertise<visualization_msgs::MarkerArray>("/buoy_markers", 1);

  // Spin
  ros::spin();
}
