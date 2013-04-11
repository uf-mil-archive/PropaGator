#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <visualization_msgs/MarkerArray.h>
// pcl16 specific includes
#include <pcl16/ros/conversions.h>
#include <pcl16/point_cloud.h>
#include <pcl16/point_types.h>
#include <pcl16/filters/voxel_grid.h>
#include <pcl16/sample_consensus/model_types.h>
#include <pcl16/sample_consensus/method_types.h>
#include <pcl16/segmentation/sac_segmentation.h>

#include <pcl16/ModelCoefficients.h>
#include <pcl16/io/pcd_io.h>
#include <pcl16/ModelCoefficients.h>
#include <pcl16/io/pcd_io.h>
#include <pcl16/point_types.h>
#include <pcl16/filters/extract_indices.h>
#include <pcl16/filters/passthrough.h>
#include <pcl16/features/normal_3d.h>
#include <pcl16/sample_consensus/method_types.h>
#include <pcl16/sample_consensus/model_types.h>
#include <pcl16/segmentation/sac_segmentation.h>

#include <pcl16/kdtree/kdtree_flann.h>
#include <pcl16/surface/mls.h>

#include <Eigen/Core>
#include <Eigen/LU>

ros::Publisher filtered_pub;
ros::Publisher large_cloud_pub;
ros::Publisher large_filtered_pub;
ros::Publisher small_cloud_pub;
ros::Publisher buoy_cloud_pub;
ros::Publisher buoy_marker_pub;

typedef pcl16::PointXYZ PointT;

void cloud_callback(const sensor_msgs::PointCloud2ConstPtr& input) {
    ROS_DEBUG("Got new pointcloud.");
    // Convert the sensor_msgs/PointCloud2 data to pcl16/PointCloud
    pcl16::PointCloud<pcl16::PointXYZ>::Ptr cloud (new pcl16::PointCloud<pcl16::PointXYZ>), cloud_f (new pcl16::PointCloud<pcl16::PointXYZ>);
    std_msgs::Header header = input->header;
    pcl16::fromROSMsg(*input, *cloud);
    //all the objects needed
    pcl16::PCDReader reader;
    pcl16::PassThrough<PointT> pass;
    pcl16::NormalEstimation<PointT, pcl16::Normal> ne;
    pcl16::SACSegmentationFromNormals<PointT, pcl16::Normal> seg; 
    pcl16::PCDWriter writer;
    pcl16::ExtractIndices<PointT> extract;
    pcl16::ExtractIndices<pcl16::Normal> extract_normals;
    pcl16::search::KdTree<PointT>::Ptr tree (new pcl16::search::KdTree<PointT> ());

    //data sets
   // pcl16::PointCloud<PointT>::Ptr cloud (new pcl16::PointCloud<PointT>);
    pcl16::PointCloud<PointT>::Ptr cloud_filtered (new pcl16::PointCloud<PointT>);
    pcl16::PointCloud<pcl16::Normal>::Ptr cloud_normals (new pcl16::PointCloud<pcl16::Normal>);
    pcl16::PointCloud<PointT>::Ptr cloud_filtered2 (new pcl16::PointCloud<PointT>);
    pcl16::PointCloud<pcl16::Normal>::Ptr cloud_normals2 (new pcl16::PointCloud<pcl16::Normal>);
    pcl16::ModelCoefficients::Ptr coefficients_plane (new pcl16::ModelCoefficients), coefficients_sphere (new pcl16::ModelCoefficients);
    pcl16::PointIndices::Ptr inliers_plane (new pcl16::PointIndices), inliers_sphere (new pcl16::PointIndices);
    ROS_DEBUG("PointCloud before filtering has: %i data points.", (int)cloud->points.size());
    
    //filter our NaNs
    pass.setInputCloud (cloud);
    pass.setFilterFieldName ("z");
    pass.setFilterLimits (0, 1.5);
    pass.filter (*cloud_filtered);
    ROS_DEBUG("PointCloud after filtering has: %i data points." , (int)cloud_filtered->points.size());

    *cloud_filtered = *cloud;//remove the pass through filter
    //estimate normal points
    ne.setSearchMethod (tree);
    ne.setInputCloud (cloud_filtered);
    ne.setKSearch(20);
    //ne.setRadiusSearch(0.025);
    ne.compute (*cloud_normals);
    
    // Create the segmentation object for the planar model and set all the parameters
    seg.setOptimizeCoefficients (true);
    seg.setModelType (pcl16::SACMODEL_NORMAL_PLANE);
    seg.setNormalDistanceWeight (0.01);
    seg.setMethodType (pcl16::SAC_RANSAC);
    seg.setMaxIterations (100);
    seg.setDistanceThreshold (10);
    seg.setInputCloud (cloud_filtered);
    seg.setInputNormals(cloud_normals);

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
    
    *cloud_filtered2 = *cloud;//just test the segmenter, remove filters
    *cloud_normals2 = *cloud_normals;//just test the segmenter, remove filters
    seg.setOptimizeCoefficients (true);
    seg.setModelType(pcl16::SACMODEL_NORMAL_SPHERE);
    seg.setMethodType (pcl16::SAC_MSAC);
    seg.setNormalDistanceWeight (0.01);
    seg.setMaxIterations(100000);
    seg.setDistanceThreshold (0.01);
    seg.setRadiusLimits (0, 10.0);
    seg.setInputCloud (cloud_filtered2);
    seg.setInputNormals (cloud_normals2);
    seg.segment(*inliers_sphere, *coefficients_sphere);
    
    extract.setInputCloud (cloud_filtered2);
    extract.setIndices (inliers_sphere);
    extract.setNegative (false);
    pcl16::PointCloud<PointT>::Ptr sphere_cloud (new pcl16::PointCloud<PointT> ());
    extract.filter(*sphere_cloud);
    
    pcl16::PointCloud<pcl16::PointXYZ> sphere_cloud_in = *sphere_cloud;
    sensor_msgs::PointCloud2 sphere_cloud_out;
    pcl16::toROSMsg(sphere_cloud_in, sphere_cloud_out);
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
