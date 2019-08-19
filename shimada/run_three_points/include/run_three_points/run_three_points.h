#include "ros/ros.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Quaternion.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>

class runThreePoints
{
private:
  void callbackJudge(const std_msgs::Bool::ConstPtr& msg);
  void callbackOdom(const nav_msgs::Odometry::ConstPtr& msg);
  void runStart();
  void nextSeqence();
  void calcDistOdom();
  void calcAngleOdom();
  //void runStraight(double distance);
  //void turnLeft(double angle);
  //void turnRight(double angle);

private:
  double STRAIGHT_DIST = 1.0;
  double SIDE_DIST = 0.4;
  double SIDE_ANGLE = 0.262;  //1.57/6(15deg)
  
  enum NUM_SEQ{
    NUM_SEQ_1,
    NUM_SEQ_2,
    NUM_SEQ_3,
    NUM_SEQ_4,
    NUM_SEQ_5,
    NUM_SEQ_6,
    NUM_SEQ_7,
    NUM_SEQ_8,
    NUM_SEQ_9,
    NUM_SEQ_10,
    NUM_SEQ_END
  };

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
  
  double odom_dist = 0.0;
  double now_roll = 0.0;
  double now_pitch = 0.0;
  double now_yaw = 0.0;
  double start_roll = 0.0;
  double start_pitch = 0.0;
  double start_yaw = 0.0;
  double diff_rad = 0.0;  
  
  NUM_SEQ num_seq;
};

