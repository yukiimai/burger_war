#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Twist.h"

#define _USE_MATH_DEFINES
#include <cmath>

static bool EnemyfollowActivateFlag = false;

void enemyfollow_activate_Callback(const std_msgs::Bool::ConstPtr& msg)
{
  EnemyfollowActivateFlag = msg->data;
  //ROS_INFO("I heard: [%d]", msg->data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "enemy_search_node");
  ros::NodeHandle n;

  ros::Publisher _pub = n.advertise<geometry_msgs::Twist>("search_vel", 10);
  ros::Subscriber _sub = n.subscribe("enemyfollow_activate", 10, enemyfollow_activate_Callback);
  geometry_msgs::Twist _msg;
  _msg.linear.x = 0;
  _msg.linear.y = 0;
  _msg.linear.z = 0;
  _msg.angular.x = 0;
  _msg.angular.y = 0;
  _msg.angular.z = (M_PI/180) * 10;

  ros::Rate loop_rate(20);

  while (ros::ok())
  {
    if(EnemyfollowActivateFlag)
    {
      _pub.publish(_msg);
    }

    ros::spinOnce();
    loop_rate.sleep();
  }


  return 0;
}
