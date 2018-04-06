// ROS Default Header File
#include "ros/ros.h"
// MsgTutorial Message File Header
// The header file is automatically created when building the package.
#include "my_dynamixel_controller/MsgDynamixel.h"
#include "dynamixel_msgs/JointState.h"
// Message callback function. This is a function is called when a topic
// message named 'ros_tutorial_msg' is received. As an input message,
// the 'MsgTutorial' message of the 'ros_tutorials_topic' package is received.

void msgCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
  ROS_INFO("Goal Position = %f", msg->goal_pos);	 //Shows the 'stamp.sec' message
  ROS_INFO("Current Position = %f", msg->current_pos); //Shows the 'stamp.nsec' message
  //ROS_INFO("recieve msg = %d", msg->data); //Shows the 'data' message
}

int main(int argc, char **argv) //Node Main Function
{

 ros::init(argc, argv, "dynamixel_subscriber"); //Initializes Node Name
 ros::NodeHandle nh; //Node handle declaration for communication with ROS system
 
 // Declares subscriber. Create subscriber 'ros_tutorial_sub' using the 'MsgTutorial'
 // message file from the 'ros_tutorials_topic' package. The topic name is
 // 'ros_tutorial_msg' and the size of the publisher queue is set to 100.

 ros::Subscriber dynamixel_subscriber = nh.subscribe("tilt_controller/state", 100, msgCallback);

 // A function for calling a callback function, waiting for a message to be
 // received, and executing a callback function when it is received

 ros::spin();
 
 return 0;

} 
