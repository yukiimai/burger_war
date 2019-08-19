#include "run_three_points/run_three_points.h"

runThreePoints::runThreePoints()
{
  ros::NodeHandle nh;
  
  sub_odom = nh.subscribe("odom", 5, &runThreePoints::callbackOdom, this);

  pub_run = nh.advertise<geometry_msgs::Twist>("cmd_vel", 5);
}

void runThreePoints::callbackJudge()
{
  if (sub_judge)
  {
    start_position_odom = now_odom;//
    flag_run_straight = true;
    flag_run_left = true;
    flag_run_right = true;
  }else 
  {
    flag_run_straight = false;
    flag_run_left = false;
    flag_run_right = false;
  }
}

void runThreePoints::callbackOdom(const nav_msgs::Odometry::ConstPtr& msg)
{
  now_odom = *msg;
  quat_msg.x = now_odom.pose.pose.orientation.x;
  quat_msg.y = now_odom.pose.pose.orientation.y;
  quat_msg.z = now_odom.pose.pose.orientation.z;
  quat_msg.w = now_odom.pose.pose.orientation.w;
}

void runThreePoints::runStart()
{
  twist_run.linear.x = 1.0;
  twist_run.angular.z = 0.0;
  
}

void runThreePoints::runStraight()
{
  static float diff_x;
  static float diff_y;
  static float odom_dist;
  bool flag_straight_first = false;
  bool flag_straight_second = false;  
  
  while (flag_run_straight)
  {
    diff_x = start_position_odom.pose.pose.position.x - now_odom.pose.pose.position.x;
    diff_y = start_position_odom.pose.pose.position.y - now_odom.pose.pose.position.y;
    odom_dist = sqrt(pow(diff_x, 2) + pow(diff_y, 2));
    
    if (odom_dist < STRAIGHT_DIST && !flag_straight_first)
    {
      twist_run.linear.x = 1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist > STRAIGHT_DIST && !flag_straight_first)
    {
      flag_straight_first = true;
    }

    if (odom_dist < STRAIGHT_DIST && !flag_straight_second && flag_straight_first)
    {
      twist_run.linear.x = -1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist <= 0.0 && !flag_straight_second && flag_straight_first)
    {
      flag_straight_second = true;
    }
    
    if (flag_straight_first && flag_straight_second)
    {
      flag_run_straight = false;
      flag_straight_first = false;
      flag_straight_second = false;
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.0;
    }
    pub_run.publish(twist_run);
  }
}

void runThreePoints::runLeft()
{
  static float diff_x;
  static float diff_y;
  static float diff_omega;
  static float odom_dist;
  bool flag_left_first = false;
  bool flag_left_second = false;
  bool flag_left_third = false;
  bool flag_left_fourth = false;
  
  double now_r, now_p, now_y;//出力値
  double start_r, start_p, start_y;//出力値

  tf::Quaternion quat(quat_msg.x, quat_msg.y, quat_msg.z, quat_msg.w);
  tf::Matrix3x3(quat).getRPY(now_r, now_p, now_y);//クォータニオン→オイラー角
  
  tf::Quaternion start_quat(start_position_odom.pose.pose.orientation.x,
                            start_position_odom.pose.pose.orientation.y,
                            start_position_odom.pose.pose.orientation.z,
                            start_position_odom.pose.pose.orientation.w);
  tf::Matrix3x3(start_quat).getRPY(start_r, start_p, start_y);//クォータニオン→オイラー角
  
  while (flag_run_left)
  {
    diff_x = start_position_odom.pose.pose.position.x - now_odom.pose.pose.position.x;
    diff_y = start_position_odom.pose.pose.position.y - now_odom.pose.pose.position.y;
    odom_dist = sqrt(pow(diff_x, 2) + pow(diff_y, 2));
    diff_omega = start_y - now_y;
    
    if (now_y < diff_omega && !flag_left_first)
    {
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.1;
    } else if (now_y >= diff_omega && !flag_left_first)
    {
      flag_left_first = true;
    }

    if (odom_dist < LEFT_DIST && !flag_left_second && flag_left_first)
    {
      twist_run.linear.x = 1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist >= LEFT_DIST && !flag_left_second && flag_left_first)
    {
      flag_left_second = true;
    }

    if (odom_dist < LEFT_DIST && !flag_left_third && flag_left_second)
    {
      twist_run.linear.x = -1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist <= 0.0 && !flag_left_third && flag_left_second)
    {
      flag_left_third = true;
    }    

    if (now_y < diff_omega && !flag_left_fourth && flag_left_third)
    {
      twist_run.linear.x = 0.0;
      twist_run.angular.z = -0.1;
    } else if (now_y <= 0.0 && !flag_left_fourth && flag_left_third)
    {
      flag_left_fourth = true;
    }
    
    if (flag_left_first && flag_left_second && flag_left_third && flag_left_fourth)
    {
      flag_run_left = false;
      flag_left_first = false;
      flag_left_second = false;
      flag_left_third = false;
      flag_left_fourth = false;
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.0;
    }
    pub_run.publish(twist_run);
  }
}

void runThreePoints::runRight()
{
  static float diff_x;
  static float diff_y;
  static float diff_omega;
  static float odom_dist;
  bool flag_right_first = false;
  bool flag_right_second = false;
  bool flag_right_third = false;
  bool flag_right_fourth = false;
  
  double now_r, now_p, now_y;//出力値
  double start_r, start_p, start_y;//出力値

  tf::Quaternion quat(quat_msg.x, quat_msg.y, quat_msg.z, quat_msg.w);
  tf::Matrix3x3(quat).getRPY(now_r, now_p, now_y);//クォータニオン→オイラー角
  
  tf::Quaternion start_quat(start_position_odom.pose.pose.orientation.x,
                            start_position_odom.pose.pose.orientation.y,
                            start_position_odom.pose.pose.orientation.z,
                            start_position_odom.pose.pose.orientation.w);
  tf::Matrix3x3(start_quat).getRPY(start_r, start_p, start_y);//クォータニオン→オイラー角
  
  while (flag_run_right)
  {
    diff_x = start_position_odom.pose.pose.position.x - now_odom.pose.pose.position.x;
    diff_y = start_position_odom.pose.pose.position.y - now_odom.pose.pose.position.y;
    odom_dist = sqrt(pow(diff_x, 2) + pow(diff_y, 2));
    diff_omega = start_y - now_y;
    
    if (diff_omega < RIGHT_ANGLE && !flag_right_first)
    {
      twist_run.linear.x = 0.0;
      twist_run.angular.z = -0.1;
    } else if (diff_omega >= RIGHT_ANGLE && !flag_right_first)
    {
      flag_right_first = true;
    }

    if (odom_dist < RIGHT_DIST && !flag_right_second && flag_right_first)
    {
      twist_run.linear.x = 1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist >= RIGHT_DIST && !flag_right_second && flag_right_first)
    {
      flag_right_second = true;
    }

    if (odom_dist < RIGHT_DIST && !flag_right_third && flag_right_second)
    {
      twist_run.linear.x = -1.0;
      twist_run.angular.z = 0.0;
    } else if (odom_dist <= 0.0 && !flag_right_third && flag_right_second)
    {
      flag_right_third = true;
    }    

    if (diff_omega > RIGHT_ANGLE && !flag_right_fourth && flag_right_third)
    {
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.1;
    } else if (diff_omega <= 0.0 && !flag_right_fourth && flag_right_third)
    {
      flag_right_fourth = true;
    }
    
    if (flag_right_first && flag_right_second && flag_right_third && flag_right_fourth)
    {
      flag_run_right = false;
      flag_right_first = false;
      flag_right_second = false;
      flag_right_third = false;
      flag_right_fourth = false;
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.0;
    }
    pub_run.publish(twist_run);
  }
}

void runThreePoints::spin()
{
  ros::Rate rate(20);
  while(ros::ok())
  {
    //上位から指令が来たら走り始める
    //runStart();
    ros::spinOnce();
    rate.sleep();
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "run_three_points");
  
  runThreePoints runthreepoints;
  
  runthreepoints.spin();
}
