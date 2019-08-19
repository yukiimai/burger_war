#include "run_three_points/run_three_points.h"

runThreePoints::runThreePoints()
{
  ros::NodeHandle nh;
  
  sub_judge = nh.subscribe("scoreget_activate", 5, &runThreePoints::callbackJudge, this);
  sub_odom = nh.subscribe("odom", 5, &runThreePoints::callbackOdom, this);
  pub_run = nh.advertise<geometry_msgs::Twist>("score_vel", 5);
  
  num_seq = NUM_SEQ_1;
}

void runThreePoints::callbackJudge(const std_msgs::Bool::ConstPtr& msg)
{
  static bool pre_msg = false;

  if (!pre_msg && msg->data) //走行開始時の初期化
  {
    start_position_odom = now_odom;//現在位置を走行開始地点とする
    num_seq = NUM_SEQ_1;
  }else if(!msg->data)//終了
  {
    num_seq = NUM_SEQ_END;
  }
    
  pre_msg = msg->data;
}

void runThreePoints::callbackOdom(const nav_msgs::Odometry::ConstPtr& msg)
{
  now_odom = *msg;
  quat_msg.x = now_odom.pose.pose.orientation.x;
  quat_msg.y = now_odom.pose.pose.orientation.y;
  quat_msg.z = now_odom.pose.pose.orientation.z;
  quat_msg.w = now_odom.pose.pose.orientation.w;
}

void runThreePoints::calcDistOdom()
{
  static float diff_x;
  static float diff_y;
  
  diff_x = now_odom.pose.pose.position.x - start_position_odom.pose.pose.position.x;
  diff_y = now_odom.pose.pose.position.y - start_position_odom.pose.pose.position.y;
  odom_dist = sqrt(pow(diff_x, 2) + pow(diff_y, 2));
}

void runThreePoints::calcAngleOdom()
{
  tf::Quaternion quat(quat_msg.x, quat_msg.y, quat_msg.z, quat_msg.w);
  tf::Matrix3x3(quat).getRPY(now_roll, now_pitch, now_yaw);//クォータニオン→オイラー角
  
  tf::Quaternion start_quat(start_position_odom.pose.pose.orientation.x,
                            start_position_odom.pose.pose.orientation.y,
                            start_position_odom.pose.pose.orientation.z,
                            start_position_odom.pose.pose.orientation.w);
  tf::Matrix3x3(start_quat).getRPY(start_roll, start_pitch, start_yaw);//クォータニオン→オイラー角

  diff_rad = start_yaw - now_yaw;
}

void runThreePoints::nextSeqence()
{
  twist_run.linear.x = 0.0;
  twist_run.angular.z = 0.0;
  start_position_odom = now_odom;
  //num_seq++;
}

void runThreePoints::runStart()//走行シナリオ
{
  switch(num_seq)
  {
    case NUM_SEQ_1://前進
      calcDistOdom();
      if(odom_dist >= STRAIGHT_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_2;
      }else 
      {
        twist_run.linear.x = 1.0;
        twist_run.angular.z = 0.0;
      }  
      break;
    case NUM_SEQ_2://後退
      calcDistOdom();
      if(odom_dist >= STRAIGHT_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_3;
      }else 
      {  
        twist_run.linear.x = -1.0;
        twist_run.angular.z = 0.0;
      }
      break;
    case NUM_SEQ_3://左回転
      calcAngleOdom();
      if(diff_rad >= SIDE_ANGLE)
      {
        nextSeqence();
        num_seq = NUM_SEQ_4;
      }else 
      {  
        twist_run.linear.x = 0.0;
        twist_run.angular.z = 0.1;
      }
      break;
    case NUM_SEQ_4://前進
      calcDistOdom();
      if(odom_dist >= SIDE_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_5;
      }else 
      {  
        twist_run.linear.x = 1.0;
        twist_run.angular.z = 0.0;
      }
      break;
    case NUM_SEQ_5://後退
      calcDistOdom();
      if(odom_dist >= SIDE_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_6;
      }else 
      {  
        twist_run.linear.x = -1.0;
        twist_run.angular.z = 0.0;
      }
      break;
    case NUM_SEQ_6://右回転
      calcAngleOdom();
      if(diff_rad <= SIDE_ANGLE)
      {
        nextSeqence();
        num_seq = NUM_SEQ_7;
      }else 
      {  
        twist_run.linear.x = 0.0;
        twist_run.angular.z = -0.1;
      }
      break;
    case NUM_SEQ_7://右回転
      calcAngleOdom();
      if(diff_rad <= SIDE_ANGLE)
      {
        nextSeqence();
        num_seq = NUM_SEQ_8;
      }else 
      {  
        twist_run.linear.x = 0.0;
        twist_run.angular.z = -0.1;
      }
      break;
    case NUM_SEQ_8://前進
      calcDistOdom();
      if(odom_dist >= SIDE_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_9;
      }else 
      {  
        twist_run.linear.x = 1.0;
        twist_run.angular.z = 0.0;
      }
      break;
    case NUM_SEQ_9://後退
      calcDistOdom();
      if(odom_dist >= SIDE_DIST)
      {
        nextSeqence();
        num_seq = NUM_SEQ_10;
      }else 
      {  
        twist_run.linear.x = -1.0;
        twist_run.angular.z = 0.0;
      }
      break;
    case NUM_SEQ_10://左回転
      calcAngleOdom();
      if(diff_rad >= SIDE_ANGLE)
      {
        nextSeqence();
        num_seq = NUM_SEQ_END;
      }else 
      {  
        twist_run.linear.x = 0.0;
        twist_run.angular.z = 0.1;
      }
      break;                  
    case NUM_SEQ_END:
      twist_run.linear.x = 0.0;
      twist_run.angular.z = 0.0;
      break; 
    default:
      break;
  }  
}

void runThreePoints::spin()
{
  ros::Rate rate(20);
  while(ros::ok())
  {
    //上位から指令が来たら走り始める
    runStart();

    pub_run.publish(twist_run);    

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
