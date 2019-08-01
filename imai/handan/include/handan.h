/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */
#ifndef HANDAN_H_
#define HANDAN_H_

#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/convert.h>
#include <gazebo_msgs/LinkStates.h>

/**
 * @class Handan
 * @brief Gazeboのメッセージを読み取り、Robot位置(tf"base_footprint")をpublishする。
 * @authors Yuki Imai(2019)
 * @copyright &copy; 2019,  Corporation
 */
class Handan
{
public:
	Handan();///<コンストラクタ
	~Handan();///<デストラクタ
protected:
	ros::Subscriber sub_gazebo;///<gazebo_msgs入力ハンドラ
	ros::Publisher pub_pose;///<ロボット位置Pose 発行ハンドラ
	tf2_ros::TransformBroadcaster *tf2_brocast;///< ロボット位置tf発行ハンドラ
	void callback_gazebo(const gazebo_msgs::LinkStates::ConstPtr &msg);///<gazebo_msgs入力コールバック

	/**
	 * @brief from tf1 to tf2 、frame_id tfを求める。
	 * @param[in] tf1 元tf
	 * @param[in] tf2 先tf
	 * @return tf1からtf2へ至るtf
	 */
	geometry_msgs::TransformStamped diff_geo_tf(const geometry_msgs::TransformStamped tf1 , const geometry_msgs::TransformStamped tf2);
};

#endif //HANDAN_H_
