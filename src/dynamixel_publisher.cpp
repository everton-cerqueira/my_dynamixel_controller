// ROS Default Header File
#include "ros/ros.h"
// MsgTutorial Message File Header
// The header file is automatically created when building the package.

#include "my_dynamixel_controller/MsgDynamixel.h"
#include "dynamixel_msgs/JointState.h"
#include "motor_cam_tutorial/image_cmd.h"

#define GOAL_POS 0
#define CURRENT_POS 1
#define ERROR 2
#define LOAD 3

#define START 0
#define HALF_PI 1
#define PI 2
#define PI_HALF 3
#define TWO_PI 4
#define ERROR_POS 0.01

void motor_command(motor_cam_tutorial::image_cmd service_request);
void motor_init(float pos1, float pos2, float pos3, float pos4, float pos5);

struct Motor{
 float motor_state[4], count, motor_position[5];
 int Estado;
 bool moving;
}MX28;

void motor_init(float pos1, float pos2, float pos3, float pos4, float pos5)
{
 MX28.Estado = 1; 
 MX28.count = 0;
 MX28.motor_position[0] = pos1;
 MX28.motor_position[1] = pos2;
 MX28.motor_position[2] = pos3;
 MX28.motor_position[3] = pos4;
 MX28.motor_position[4] = pos5; 
 
 return;
}

void msgCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{   
   MX28.motor_state[GOAL_POS] = msg->goal_pos;
   MX28.motor_state[CURRENT_POS] = msg->current_pos;
   MX28.motor_state[ERROR] = msg->error;
   MX28.motor_state[LOAD] = msg->load;
   MX28.moving = msg->is_moving;   


   ROS_INFO("Goal Position = %f", msg->goal_pos);	 
   ROS_INFO("Current Position = %f", msg->current_pos); 
   ROS_INFO("Error = %f", msg->error); 
   ROS_INFO("Moving = %i", msg->is_moving);
}  


int main(int argc, char **argv)
{
  //my_dynamixel_controller::MsgDynamixel msg;
  
  motor_init(0,1.57,3.14,4.71,6.28);

  ros::init(argc, argv, "dynamixel_publisher");	
  ros::init(argc, argv, "dynamixel_subscriber");
    
  ros::NodeHandle nh;	

  ros::Publisher dynamixel_publisher = nh.advertise<my_dynamixel_controller::MsgDynamixel>("tilt_controller/command", 100); 
  ros::Subscriber dynamixel_subscriber = nh.subscribe("tilt_controller/state", 100, msgCallback);
  
  motor_cam_tutorial::image_cmd service_request;
  service_request.request.cmd = true;
  service_request.request.angle = 0.0;
  service_request.request.path = "./";

  
  
  ros::Rate loop_rate(0.2); // Set the loop period (Hz)
  	
  //ros::spinOnce();
  
   while (ros::ok()){	
    loop_rate.sleep();		  
    ros::spinOnce();
    motor_command(service_request);
   }
   
   return 0;
}



void motor_command(motor_cam_tutorial::image_cmd service_request)
{
  my_dynamixel_controller::MsgDynamixel msg;
  
  ros::NodeHandle nh;	
  
  ros::Publisher dynamixel_publisher = nh.advertise<my_dynamixel_controller::MsgDynamixel>("tilt_controller/command", 100); 
  ros::Subscriber dynamixel_subscriber = nh.subscribe("tilt_controller/state", 100, msgCallback);
  
  ros::ServiceClient ros_tutorials_service_client = nh.serviceClient<motor_cam_tutorial::image_cmd>("image_cmd");
  
 
 switch(MX28.Estado)
 {
   case 1:
    msg.data = MX28.motor_position[START]; 
    dynamixel_publisher.publish(msg);
    ROS_INFO("send msg = %f", msg.data); 
    MX28.Estado = 2;
   break;
  
    case 2:
     if(MX28.motor_position[START] - MX28.motor_state[CURRENT_POS] <= ERROR_POS)
     {
      service_request.request.angle = MX28.motor_state[CURRENT_POS];  
      if(ros_tutorials_service_client.call(service_request) == 1)MX28.Estado = 3;
      else 
      {
	   std::cout<<"Service return = "<<service_request.response.result<<std::endl;
	   MX28.Estado = 2;
      }
     }else MX28.Estado = 1;
    break;


   case 3:
    msg.data = MX28.motor_position[HALF_PI]; 
    dynamixel_publisher.publish(msg);
    ROS_INFO("send msg = %f", msg.data);
    MX28.Estado= 4;
   break;

   case 4: 
    if(MX28.motor_position[HALF_PI] - MX28.motor_state[CURRENT_POS] <= ERROR_POS) 
      {
       //service_request.angle = MX28.motor_state[CURRENT_POS];  
      //ros_tutorials_service_client.call(service_request);
      MX28.Estado = 5;
     }else MX28.Estado = 3;
   break;

  case 5:
    msg.data = MX28.motor_position[PI]; 
    dynamixel_publisher.publish(msg);
    ROS_INFO("send msg = %f", msg.data);
    MX28.Estado = 6;
  break;

  case 6:   
    if(MX28.motor_position[PI] - MX28.motor_state[CURRENT_POS] <= ERROR_POS) 
     {
      //service_request.angle = MX28.motor_state[CURRENT_POS];  
      //ros_tutorials_service_client.call(service_request);
      MX28.Estado = 7;
     }else MX28.Estado = 5;
  break;
 
  case 7:
    msg.data = MX28.motor_position[PI_HALF]; 
    dynamixel_publisher.publish(msg);
    ROS_INFO("send msg = %f", msg.data);
    MX28.Estado = 8;
  break;

  case 8:    
   if(MX28.motor_position[PI_HALF] - MX28.motor_state[CURRENT_POS] <= ERROR_POS) 
     {
      //service_request.angle = MX28.motor_state[CURRENT_POS];  
      //ros_tutorials_service_client.call(service_request);
      MX28.Estado = 9;
     }else MX28.Estado = 7;
  break;

  case 9:
    msg.data = MX28.motor_position[TWO_PI]; 
    dynamixel_publisher.publish(msg);
    ROS_INFO("send msg = %f", msg.data);
    MX28.Estado = 10;
  break;

  case 10:
    if(MX28.motor_position[TWO_PI] - MX28.motor_state[CURRENT_POS] <= ERROR_POS) 
     {
      //service_request.angle = MX28.motor_state[CURRENT_POS];  
      //ros_tutorials_service_client.call(service_request);
      MX28.Estado = 1;
     }else MX28.Estado = 9;
  break;
  }	

} 



