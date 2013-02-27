#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
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


ros::Publisher pub;
ros::Publisher filtered_pub;
ros::Publisher filtered2_pub;

const int max_objects = 1;

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


  // Create the segmentation object for the planar model and set all the parameters
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_sphere (new pcl::PointCloud<pcl::PointXYZ> ());
  pcl::PCDWriter writer;
  seg.setOptimizeCoefficients(true); //true
  seg.setModelType(pcl::SACMODEL_SPHERE);
  seg.setMethodType(pcl::SAC_RANSAC);
  seg.setMaxIterations(10000);//10000
  seg.setDistanceThreshold(0.02);
//  seg.setRadiusLimits(0.01, 0.12);// we want our sphere to have RADIUS between x and y METERS
  seg.setRadiusLimits(0.01, 0.1);

  int i=0, nr_points = (int)cloud_filtered->points.size();
  while (cloud_filtered->points.size() > 0.3 * nr_points)// 0.3
  {
    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud(cloud_filtered);
    seg.segment(*inliers, *coefficients);
    if (inliers->indices.size() == 0)
    {
      ROS_WARN("Could not estimate a spherical model for the given dataset.");
      break;
    }

    // Extract the planar inliers from the input cloud
    pcl::ExtractIndices<pcl::PointXYZ> extract;
    extract.setInputCloud(cloud_filtered);
    extract.setIndices(inliers);
    extract.setNegative(false);

    // Write the planar inliers to disk
    extract.filter(*cloud_sphere);
    ROS_WARN("PointCloud representing the spherical component: %d data points.", cloud_sphere->points.size());


    std_msgs::Header filtered2_header = cloud_sphere->header;
    sensor_msgs::PointCloud2 filtered2_out;
    pcl::toROSMsg(*cloud_sphere, filtered2_out);
    filtered2_out.header = filtered2_header;
    filtered2_pub.publish(filtered2_out);



    // Remove the planar inliers, extract the rest
    extract.setNegative(false);// true
    extract.filter(*cloud_f);
    *cloud_filtered = *cloud_f;
  }

  // Creating the KdTree object for the search method of the extraction
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
  tree->setInputCloud(cloud_filtered);

  std::vector<pcl::PointIndices> cluster_indices;
  pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
  ec.setClusterTolerance(0.25); //0.02 = 2cm	//0.25 found buoy...
  ec.setMinClusterSize(20);//100
  ec.setMaxClusterSize(60);//25000
  ec.setSearchMethod(tree);
  ec.setInputCloud(cloud_filtered);
  ec.extract(cluster_indices);

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
    pub.publish(cloud_out);
    ROS_ERROR("PUBLISHED!!");

    j++;
  }

/*
  int j = 0;
  for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
  {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>);
    for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++)
      cloud_cluster->points.push_back (cloud_filtered->points[*pit]);
    cloud_cluster->width = cloud_cluster->points.size ();
    cloud_cluster->height = 1;
    cloud_cluster->is_dense = true;

    ROS_DEBUG("PointCloud representing the Cluster: %d data points.", cloud_cluster->points.size());

    sensor_msgs::PointCloud2 cloud_out;
    pcl::toROSMsg(*cloud_cluster, cloud_out);
    cloud_out.header = header;
    pub.publish(cloud_out);
    ROS_ERROR("PUBLISHED!!");

    j++;
  }
*/
}

int main(int argc, char** argv) {
  // Initialize ROS
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // Create a ROS subscriber for the input point cloud
  ros::Subscriber sub = nh.subscribe ("/cloud", 1, cloud_callback);

  // Create a ROS publisher for the output point cloud
  pub = nh.advertise<sensor_msgs::PointCloud2> ("/lidar_object", 1);
  filtered_pub = nh.advertise<sensor_msgs::PointCloud2> ("/filtered_cloud1", 1);
  filtered2_pub = nh.advertise<sensor_msgs::PointCloud2> ("/filtered_cloud2", 1);

  // Spin
  ros::spin ();
}
