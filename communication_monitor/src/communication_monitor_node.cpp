#include <ros/ros.h>
#include <std_msgs/String.h>
#include <map>
#include <cmath>
#include <kill_handling/broadcaster.h>
/********************************************************************************
 * The purpose of this node is to notify the system of loss of communications	*
 * It is designed to have one node per machine on the network					*
 * The node works by publishing its identification out to all other 			*
 * Communication nodes, for each new id received the node adds this to the list	*
 * of id's to keep track of, if it doesn't hear from a node within some time 	*
 * Frame it alerts the system that a communication failure has occurred			*
 * an extension of this would be to attempt to correct loss of communication	*
 * but for now it will just notify the system									*
 ********************************************************************************/

/***TODO
 * See if we can use ros diagnostics to make this system more reusable
 * 		Do so
 * Add error checking to input parameters
 * Add the ability to tell the systems that an id is being shutdown on purpose i.e. two
 * 		users are on the ros system each with there own CommMonitor and one leaves
 */

class CommMonitor
{
private:		//Typedefs
	typedef std::map<std::string, int> 		comm_map;
	typedef comm_map::iterator 				comm_itter;
	typedef std::pair<std::string, int> 	comm_pair;

private:		//Vars
	//If you haven't heard from <id_> in over <update_freq_>^-1 * <max_num_of_drops_> throw an error
	std::string id_;
	double update_freq_;
	int max_num_of_drops_;
	ros::Rate update_rate_;
	std::map<std::string, int> registered_communicators;
	std_msgs::String name_msg_;
	ros::Publisher pub_;
	ros::Subscriber sub_;
	kill_handling::KillBroadcaster kill_broadcaster_;

private:		//Functions
	//Subscriber callback, register new ids and reset number of drops
	void UpdateMap(const std_msgs::String::ConstPtr& id)
	{
		// Don't listen to your own msgs
		if(id->data == id_.c_str())
		{
			return;
		}
		std::pair<comm_itter, bool> it;
		comm_pair input(id->data, 0);
		it = registered_communicators.insert(input);

		//Check if element already exist
		if(!it.second)
		{
			//Element was not inserted because it already exist
			//So set the number of drops to 0
			it.first->second = 0;
		}
		else
		{
			//Element was inserted
			ROS_INFO("Registering communicator %s with communicator %s",
									id->data.c_str(),
									id_.c_str());
		}

		/*Start of alternative method
		comm_itter it = registered_communicators.find(id->data);
		if(it == registered_communicators.end())
		{
			ROS_INFO("Registering communicator %s with communicator %s",
						id->data.c_str(),
						id_);
			registered_communicators.in
		}
		*/

	}

public:			//Functions
	//Constructor
	CommMonitor(std::string name) :
		update_rate_(1.0),
		kill_broadcaster_(name, "Lost communication link")
	{
		ros::NodeHandle private_nh("~");
		ros::NodeHandle nh;

		//Get some private parameters
		id_ = name;
		//Initialize the only message we'll ever use
		name_msg_.data = id_.c_str();

		//Get some non-private parameters
		private_nh.param<double>("update_freq", update_freq_, 10.0);			//Default to 10 Hz
		update_rate_ = ros::Rate(update_freq_);									//Set the update rate

		private_nh.param<int>("max_num_of_drops", max_num_of_drops_, 9);		//Default to 9 times

		//Initialize publisher and subscribers
		pub_ = nh.advertise<std_msgs::String>("comm_check", 1);
		sub_ = nh.subscribe<std_msgs::String>("comm_check", 1, &CommMonitor::UpdateMap, this);
	}

	//Run (this is the main logic)
	void Run()
	{
		//Publish the id
		pub_.publish(name_msg_);

		//Update
		bool lost_communication = false;
		for(comm_itter it = registered_communicators.begin(); it != registered_communicators.end(); ++it)
		{
			it->second++;
			if(it->second > max_num_of_drops_)
			{
				lost_communication = true;
				ROS_ERROR("Lost communications with %s, from %s for %f seconds",
						it->first.c_str(),
						id_.c_str(),
						it->second * pow(update_freq_, -1.0));
			}
		}

		// Kill if communication is lost
		if (lost_communication)
		{
			kill_broadcaster_.send(true);
		}
		else
		{
			kill_broadcaster_.send(false);
		}

		update_rate_.sleep();
		ros::spinOnce();
	}
};


/************************
 * 			Main		*
 ************************/
int main(int argc, char** argv)
{

	//Initialize ROS
	ros::init(argc, argv, "Unnamed_communication_monitor_", ros::init_options::AnonymousName);
	ros::NodeHandle nh;

	CommMonitor monitor(ros::this_node::getName());

	while(ros::ok())
	{
		monitor.Run();
	}
	return 0;
}
