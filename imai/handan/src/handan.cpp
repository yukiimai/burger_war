/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */

#include "handan.h"

Handan::Handan()
{
	ros::NodeHandle nh;
	pub_score_command = nh.advertise<std_msgs::Bool>("scoreget_activate" , 10 , true);
	pub_enemyfollow_command = nh.advertise<std_msgs::Bool>("enemyfollow_activate" , 10 , true);
	ros::Subscriber sub_enemy_pos = nh.subscribe("enemy_pose", 10, &Handan::callback_enemy_pos,this);

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    std_msgs::Bool output;
	double diff_time = ros::Time::now().toSec() - last_enemy_time.toSec();
bool handan = false;
if(handan)
{
	output.data = true;
	pub_score_command.publish(output);
	output.data = false;
	pub_enemyfollow_command.publish(output);
}
else
{
	output.data = false;
	pub_score_command.publish(output);
	output.data = true;
	pub_enemyfollow_command.publish(output);
}

    ros::spinOnce();

    loop_rate.sleep();
  }
}

Handan::~Handan()
{

}

void Handan::callback_enemy_pos(const geometry_msgs::Point::ConstPtr &msg)
{
	last_enemy_pose_ = *msg;
	last_enemy_time = ros::Time::now();
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "handan");
	Handan handan_;
	return 0;
}
