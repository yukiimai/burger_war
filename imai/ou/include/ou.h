/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */
#ifndef HANDAN_H_
#define HANDAN_H_

#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include "geometry_msgs/Twist.h"


/**
 * @class Ou
 * @brief 
 * @authors Yuki Imai(2019)
 * @copyright &copy; 2019,  Corporation
 */
class Ou
{
public:
	Ou();///<コンストラクタ
	~Ou();///<デストラクタ
protected:
	ros::Subscriber sub_enemy_pos;

	ros::Publisher cmd_pub;

	void callback_enemy_pos(const geometry_msgs::Point::ConstPtr &msg);

	geometry_msgs::Point last_enemy_pose_;
	ros::Time last_enemy_time;
};	

#endif //HANDAN_H_
