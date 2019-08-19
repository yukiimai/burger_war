#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Twist.h"

#define _USE_MATH_DEFINES
#include <cmath>

// 敵サーチ時の角速度[度/s]
static const double ANGULAR_BELOCITY = 10.0;
// 敵サーチフラグトピック名
static const std::string TOPIC_ENEMYFOLLOW_ACTIVATE = "enemyfollow_activate";
// 敵サーチ時の移動速度トピック名
static const std::string TOPIC_SEARCH_VEL = "search_vel";

static bool EnemyfollowActivateFlag = false;

// 敵サーチフラグトピック取得時
void enemyfollow_activate_Callback(const std_msgs::Bool::ConstPtr& msg)
{
  EnemyfollowActivateFlag = msg->data;
  //ROS_INFO("I heard: [%d]", msg->data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "enemy_search_node");
  ros::NodeHandle n;

  ros::Publisher _pub = n.advertise<geometry_msgs::Twist>(TOPIC_SEARCH_VEL, 10);
  ros::Subscriber _sub = n.subscribe(TOPIC_ENEMYFOLLOW_ACTIVATE, 10, enemyfollow_activate_Callback);
  geometry_msgs::Twist _msg;
  _msg.linear.x = 0;
  _msg.linear.y = 0;
  _msg.linear.z = 0;
  _msg.angular.x = 0;
  _msg.angular.y = 0;
  _msg.angular.z = (M_PI/180) * ANGULAR_BELOCITY;

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
