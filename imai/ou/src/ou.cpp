/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */

#include "ou.h"

Ou::Ou()
{
	ros::NodeHandle nh;
	ros::Subscriber sub_enemy_pos = nh.subscribe("enemy_pose", 10, &Ou::callback_enemy_pos,this);
        cmd_pub = nh.advertise<geometry_msgs::Twist>("ou_vel", 10);
	ROS_INFO("[Ou]START");

  ros::Rate loop_rate(20);
  while (ros::ok())
  {
	geometry_msgs::Twist output;
	double diff_time = ros::Time::now().toSec() - last_enemy_time.toSec();

	if(diff_time > 2.0)
	{
		//なにもしない
	}
	else if(diff_time > 1.0)
	{
		//止まる
		output.linear.x = 0.0;
		output.angular.z = 0.0;
		output.linear.y = 0;
		output.linear.z = 0;
		output.angular.x = 0;
		output.angular.y = 0;
		cmd_pub.publish(output);
	}
	else
	{
		if(last_enemy_pose_.y > 0.1)
		{
			output.linear.x = 0.5;
			output.angular.z = -(M_PI/180) * 45.0;
		}
		else if(last_enemy_pose_.y < -0.1)
		{
			output.linear.x = 0.5;
			output.angular.z = (M_PI/180) * 45.0;
		}
		else
		{
			output.linear.x = 0.5;
			output.angular.z = 0.0;
		}
		output.linear.y = 0;
		output.linear.z = 0;
		output.angular.x = 0;
		output.angular.y = 0;
		cmd_pub.publish(output);
	}
  ros::spinOnce();
  loop_rate.sleep();
  }
}

Ou::~Ou()
{

}

void Ou::callback_enemy_pos(const geometry_msgs::Point::ConstPtr &msg)
{
	last_enemy_pose_ = *msg;
	last_enemy_time = ros::Time::now();
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "ou");
	Ou ou_;
	return 0;
}
