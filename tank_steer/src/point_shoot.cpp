#include <ros/ros.h>
#include <ros/rate.h>
#include <actionlib/server/simple_action_server.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/WrenchStamped.h>

#include "uf_common/MoveToAction.h"

/****************************************************************************
 * 							Point and shoot for tank steer					*
 * 	This node implements the moveit simple action server					*
 * 		It takes a desired pose from the moveit action and generates the 	*
 * 		required torque and force to achieve the desired goal				*
 * 	As the name suggest the robot will move by first pointing towards its 	*
 * 		endpoint then moving towards it linearly once reaching its goal xyz	*
 * 		it will stop and rotate to the desired orientation					*
 * 	When there is no goal to achieve the node will station hold				*
 * 	Since the robot can only control linear motion in one direction at a 	*
 * 		time, the robot will station hold by mantaining only the 			*
 * 		controllable direction and orientation until the robot drifts out 	*
 * 		of the allowable orthogonal direction, at this point the robot will	*
 * 		send itself a new goal to return to the previously specified 		*
 * 		waypoint															*
 * 	When moving in the linear direction a K * sqrt(error) controller will 	*
 * 		be used, as it will move the robot towards the goal at max speed 	*
 * 		and then ease off as it approaches (more details are disscused in 	*
 * 		implementation)														*
 ****************************************************************************/

// Class for the controller
// TODO: move to separate header and cpp

class PointShoot
{
	// Public vars
public:

	// Private vars
private:

	// Public functions
public:
	void run_();

	PointShoot();

	// Private functions
private:
	void update_();

	void newGoal_(const uf_common::MoveToAction::ConstPtr &goal);

	void getCurrentPoseTwist_(const nav_msgs::Odometry::ConstPtr &msg);

	geometry_msgs::WrenchStamped calculateForce();

	geometry_msgs::WrenchStamped calculateTorque();

	void updateErrors();

};

/*
 * 			Constructor
 * 	Grabs all params, initializes vars, subs, pubs
 */
PointShoot::PointShoot()
{

}

/*
 * 			Run
 * 	This function handles the main ros loop
 * 		such as sleep rate and ros::ok()
 */
void PointShoot::run_()
{
	while(ros::ok())
	{
		update_();
		ros::spinOnce();
	}
}

/*
 * 			Update
 * 	This function updates the controller
 */
void PointShoot::update_()
{

}

/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{
	//Initialize ROS
	ros::init(argc, argv, "point_shoot");

	// Initilize controler
	PointShoot ps;

	// Run the node
	//ps.run_();

	return 0;
}
