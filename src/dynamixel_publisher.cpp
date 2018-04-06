// ROS Default Header File
#include "ros/ros.h"
// MsgTutorial Message File Header
// The header file is automatically created when building the package.
#include "my_dynamixel_controller/MsgDynamixel.h"
#include "dynamixel_msgs/JointState.h"

void msgCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{
  ROS_INFO("Motor Ids = %i", msg->motor_ids[0]);
  ROS_INFO("Motor Temp = %i", msg->motor_temps[0]); 
  ROS_INFO("Goal Position = %f", msg->goal_pos);	 //Shows the 'stamp.sec' message
  ROS_INFO("Current Position = %f", msg->current_pos); //Shows the 'stamp.nsec' message
  ROS_INFO("Error = %f", msg->error); //Shows the 'data' message
  ROS_INFO("Velocity = %f", msg->velocity);
  ROS_INFO("Load = %f", msg->load);
  ROS_INFO("Moving = %i", msg->is_moving);
}



int main(int argc, char **argv) //Node Main Function
{
  ros::init(argc, argv, "dynamixel_publisher");	// Initializes Node Name
  ros::init(argc, argv, "dynamixel_subscriber"); //Initializes Node Name

  ros::NodeHandle nh;	// Node handle declaration for communication with ROS system
  // Declare publisher, create publisher 'ros_tutorial_pub' using the 'MsgTutorial'
  // message file from the 'ros_tutorials_topic' package. The topic name is
  // 'ros_tutorial_msg' and the size of the publisher queue is set to 100.

  ros::Publisher dynamixel_publisher = nh.advertise<my_dynamixel_controller::MsgDynamixel>("tilt_controller/command", 100); 
  ros::Subscriber dynamixel_subscriber = nh.subscribe("tilt_controller/state", 100, msgCallback);

  // Set the loop period. '10' refers to 10 Hz and the main loop repeats at 0.1 second intervals
  ros::Rate loop_rate(0.2);
  //ros_tutorials_topic::MsgTutorial msg;	// Declares message 'msg' in 'MsgTutorial' message
  my_dynamixel_controller::MsgDynamixel msg;						// file format
						// Variable to be used in message
  //ros::spinOnce();
  
  
  float count = 0;
  int Estado = 1;
  while (ros::ok()){
  // msg.stamp = ros::Time::now(); // Save current time in the stamp of 'msg'
   msg.data = count;		// Save the the 'count' value in the data of 'msg'
   ROS_INFO("send msg = %f", msg.data);  // Print the 'stamp.sec' message
  // ROS_INFO("send msg = %d", msg.stamp.nsec); // Print the 'stamp.nsec' message
  // ROS_INFO("send msg = %d", msg.data);       // Print the 'data' message	
  
   dynamixel_publisher.publish(msg); // Publishes 'msg' message		
   loop_rate.sleep();		  // Goes to sleep according to the loop rate defined above.
   
   //++count; 			  // Increase count variable by one
   //if(count >= 16) count = 0;
   
   switch(Estado){
   case 1:
    if(count < 6.28) count += 3.14/5;
    else Estado = 2;
   break;

   case 2:
    if(count > 0) count-= 3.14/5;
    else Estado = 1;
   break; 

   default:
    Estado = 1;
   break;
  }
   ros::spinOnce();
 }
   

   return 0;
}







