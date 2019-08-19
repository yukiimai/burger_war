/*
 *  Copyright (c) 2019,  Corporation
 *  All rights reserved.
 */

#include "enemy_position.h"

EnemyPosition::EnemyPosition()
{
	ros::NodeHandle nh;

    ROS_INFO("[enemy_position]START");

    image_sub = nh.subscribe("image_raw", 1, &EnemyPosition::image_callback, this);

    enemy_pub = nh.advertise<geometry_msgs::Point>("enemy_pose" , 10);

}

EnemyPosition::~EnemyPosition()
{

}

void EnemyPosition::image_callback(const sensor_msgs::ImageConstPtr& msg)
{

    //input convert
     cv_bridge::CvImagePtr cv_ptr;
     cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
     inImage = cv_ptr->image;

      //sagyo
      double position_y = 0.0;
      double row_num = inImage.rows;
      double cols_num = inImage.cols;
      cv::Vec3b color;

      int posi_y_min = -100;
      int posi_y_max = 100;

      int th_red = 100;

      double th_left = cols_num/3;
      double th_right = cols_num - th_left;

      int col;
      int red_cnt_l = 0;
      int red_cnt_c = 0;
      int red_cnt_r = 0;

      for (int row; row <= row_num; row++){
            for (col = 0; col < th_left; col++)
            {
              color = inImage.at<cv::Vec3b>(row, col);
                if (color[2] > th_red){
                    red_cnt_l++;
                }
            }
            for (col = th_left; col <= th_right; col++)
            {
              color = inImage.at<cv::Vec3b>(row, col);
                if (color[2] > th_red){
                    red_cnt_c++;
                }
            }
            for (col = th_right; col <= cols_num; col++)
            {
              color = inImage.at<cv::Vec3b>(row, col);
                if (color[2] > th_red){
                    red_cnt_r++;
                }
            }
      }

      if ((red_cnt_l ==0) && (red_cnt_c ==0) && (red_cnt_r ==0)) {

      } else {
             if ((red_cnt_l > red_cnt_c) && (red_cnt_l > red_cnt_r)){
                position_y = posi_y_min;
            } else if ((red_cnt_c >= red_cnt_l) && (red_cnt_c >= red_cnt_r)) {
                position_y = 0;
            } else if ((red_cnt_r > red_cnt_l) && (red_cnt_r > red_cnt_c)) {
                position_y = posi_y_max;
            }

         //output
         geometry_msgs::Point output;

         output.x = 0.0;
         output.y = position_y;
         output.z = 0.0;

         enemy_pub.publish(output);
      }
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "enemy_position");
	EnemyPosition enemy_position_;
	ros::spin();

	return 0;
}
