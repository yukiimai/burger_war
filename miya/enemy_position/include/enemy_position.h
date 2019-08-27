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
#include <sensor_msgs/Image.h>
#include <geometry_msgs/Point.h>

#include <cv_bridge/cv_bridge.h>


/**
 * @class EnemyPosition
 * @copyright &copy; 2019,  Corporation
 */
class EnemyPosition
{
public:
	EnemyPosition();///<コンストラクタ
	~EnemyPosition();///<デストラクタ
protected:
        //ros::Subscriber sub_gazebo;///<gazebo_msgs入力ハンドラ
        //ros::Publisher pub_pose;///<ロボット位置Pose 発行ハンドラ

        ros::Publisher enemy_pub;
        ros::Subscriber image_sub;

         void image_callback(const sensor_msgs::ImageConstPtr& msg);
         cv::Mat inImage;
	cv::Mat maskImage0;
	cv::Mat maskImage1;
	cv::Mat maskImage2;
};

#endif //HANDAN_H_
