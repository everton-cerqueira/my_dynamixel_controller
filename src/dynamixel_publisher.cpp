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
#define ERROR_POS 0.05
#define ERROR_NEG -0.05

#define TX 0
#define RX 1


void motor_command(motor_cam_tutorial::image_cmd service_request, ros::Publisher dynamixel_publisher);
bool motor_init(float qtd_pos);

struct Motor{
 float motor_state[4], count, pos;
 int Estado;
 bool moving;
 my_dynamixel_controller::MsgDynamixel msg;
}MX28;

bool motor_init(float qtd_pos)
{
  if(qtd_pos > 50 || qtd_pos <= 0){
   ROS_ERROR("Qtd_pos value should be between 0 and 6.14");
   return false;
  }
  MX28.Estado = TX; 
  MX28.count = 0;
  MX28.pos = 6.14/qtd_pos;
  for(int i = 0; i < 4; i++) MX28.motor_state[i] = 0xff;
  MX28.moving = false;
  MX28.msg.data = 0;   
  return true;
}

void msgCallback(const dynamixel_msgs::JointState::ConstPtr& msg)
{   
 MX28.motor_state[GOAL_POS] = msg->goal_pos;
 MX28.motor_state[CURRENT_POS] = msg->current_pos;
 MX28.motor_state[ERROR] = msg->error;
 MX28.motor_state[LOAD] = msg->load;
 MX28.moving = msg->is_moving;   
 
 /*ROS_INFO("Goal Position = %f", msg->goal_pos);	 
   ROS_INFO("Current Position = %f", msg->current_pos); 
   ROS_INFO("Error = %f", msg->error); 
   ROS_INFO("Moving = %i", msg->is_moving);*/
}  


int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "dynamixel_publisher");	
  ros::init(argc, argv, "dynamixel_subscriber");
  ros::NodeHandle nh;	

  float qtd_pos;
  nh.param("Qtd_Pos", qtd_pos, qtd_pos);
   
  if(!motor_init(qtd_pos)) return 0;

  ros::Publisher dynamixel_publisher = nh.advertise<my_dynamixel_controller::MsgDynamixel>("tilt_controller/command", 100); 
  ros::Subscriber dynamixel_subscriber = nh.subscribe("tilt_controller/state", 100, msgCallback);
  
  motor_cam_tutorial::image_cmd service_request;
  service_request.request.cmd = true;
  service_request.request.angle = 0.0;
  service_request.request.path = "/home/everton/motor_cam/";
  
  ros::Rate loop_rate(5); // Set the loop period (Hz)
  	
  //ros::spinOnce();
  
   while (ros::ok()){	
    loop_rate.sleep();		  
    ros::spinOnce();
    motor_command(service_request, dynamixel_publisher);
   }
   
   return 0;
}


void motor_command(motor_cam_tutorial::image_cmd service_request, ros::Publisher dynamixel_publisher)
{
  ros::NodeHandle nh;	
  ros::ServiceClient ros_tutorials_service_client = nh.serviceClient<motor_cam_tutorial::image_cmd>("image_cmd");
  
  switch(MX28.Estado)
  {
   case TX:
    MX28.msg.data = MX28.count; 
    dynamixel_publisher.publish(MX28.msg);
    ROS_INFO("Send Position = %f", MX28.msg.data);		 
    MX28.Estado = RX;
   break;		
		
   case RX:
    if((MX28.count - MX28.motor_state[CURRENT_POS] <= ERROR_POS) && (MX28.count - MX28.motor_state[CURRENT_POS] >= ERROR_NEG))
    {
     ROS_INFO("Current Position = %f", MX28.motor_state[CURRENT_POS]); 
     service_request.request.angle = MX28.count;
     ros_tutorials_service_client.call(service_request);
     if(service_request.response.result == 1)
     {
      MX28.count += MX28.pos;
      if(MX28.count >= 6.14) MX28.count = 0;
      MX28.Estado = TX;  	 
      }else MX28.Estado = RX;	 
     }else MX28.Estado = TX; 	 
   break;		
  }		
} 



