#include "ros/ros.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>

class runThreePoints
{
private:
  void callbackJudge();
  void callbackOdom(const nav_msgs::Odometry::ConstPtr& msg);
  void runStart();
  void runStraight();
  void runLeft();
  void runRight();

private:
  double STRAIGHT_DIST = 1.0;
  double LEFT_DIST = 0.3;
  double RIGHT_DIST = 0.3;
  double LEFT_ANGLE = 0.262;   //1.57/6(15deg)
  double RIGHT_ANGLE = 0.262;  //1.57/6(15deg)
  bool flag_run_straight = false;
  bool flag_run_left = false;
  bool flag_run_right = false;
  
public:
  runThreePoints();
  void spin();
  
public:
  nav_msgs::Odometry start_position_odom;
  nav_msgs::Odometry now_odom;
  geometry_msgs::Twist twist_run;
  geometry_msgs::Quaternion quat_msg;//入力値
  ros::Subscriber sub_camera;
  ros::Subscriber sub_judge;
  ros::Subscriber sub_odom;
  ros::Publisher pub_run;  
};
