/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */
#ifndef HANDAN_H_
#define HANDAN_H_

#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Bool.h>
/**
 * @class Handan
 * @brief 
 * @authors Yuki Imai(2019)
 * @copyright &copy; 2019,  Corporation
 */
class Handan
{
public:
	Handan();///<コンストラクタ
	~Handan();///<デストラクタ
protected:
	ros::Subscriber sub_enemy_pos;

	ros::Publisher pub_score_command;
	ros::Publisher pub_enemyfollow_command;

	void callback_enemy_pos(const geometry_msgs::Point::ConstPtr &msg);
	geometry_msgs::Point last_enemy_pose_;
	ros::Time last_enemy_time;
};	

#endif //HANDAN_H_
