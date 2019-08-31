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
      double position_x = 0.0;
      double position_y = 0.0;
      double position_z = 0.0;

      //画像の行数列数
      int row_num = inImage.rows;
      int cols_num = inImage.cols;

      // データ送信の最大値（最小値-100）
      double posi_x_max = 100.0;
      double posi_y_max = 100.0;

      //
#if 1
      // RBGの閾値
      int b_min = 0;
      int b_max = 90;
      int g_min = 0;
      int g_max = 90;
      int r_min = 100;
      int r_max = 255;

      int open_num = 1;         // 画像膨張回数
      int close_num = 2;        // 画像縮小回数

      int th_ditect = 3;       // 敵検知最小pixcel
//      float th_unditect_rate = 0.5;

      int cnt_black = 0;        // 閾値値超えpixcelカウント

      //output
      geometry_msgs::Point output;

      cv::Scalar s_min = cv::Scalar(b_min, g_min, r_min);
      cv::Scalar s_max = cv::Scalar(b_max, g_max, r_max);

      // mask
      cv::inRange(inImage, s_min, s_max, maskImage0);

      // 膨張と収縮
      cv::morphologyEx(maskImage0, maskImage1, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1,1), open_num);
      cv::morphologyEx(maskImage1, maskImage2, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1,1), close_num);

      for (int row = 0; row < row_num; row++) {
            for (int col = 0; col < cols_num; col++) {
                // 二値化したマスク画像の255のpixcelを数える
                if (maskImage2.at<cv::Scalar>(row, col) != cv::Scalar(0)) {
                    cnt_black++;
                }
            }
      }

        // 未検出かどうか
        if (cnt_black < th_ditect) {
            ;                                               //未検出の時は何もしない
        } else {
            // Center of Moments
            cv::Moments mmt = moments(maskImage2, false);
            cv::Point2d center = cv::Point2d(mmt.m10/mmt.m00, mmt.m01/mmt.m00);

            position_y = posi_y_max * (center.x - cols_num/2)/(cols_num/2);
            position_x = posi_x_max * ((center.y - row_num/2) * (-1))/(row_num/2);

            output.x = position_x;        // 画角の右が正 (max:100,min:-100)
            output.y = position_y;        // 画角の上が正 (max:100,min:-100)

//            // 画面の大部分が赤の時
//            if (cnt_black > (row_num * cols_num * th_unditect_rate)) {
//                    double position_z = 1.0;
//            }
            output.z = 0.0;

            enemy_pub.publish(output);
        }

#else
      // ---------- //
      // 3Partition
      // ---------- //

      int posi_y_min = -100;

      cv::Vec3b color;
      double th_left = cols_num/4;
      double th_right = cols_num - th_left;

      int th_red = 100;

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

      //output
      geometry_msgs::Point output;

      if ((red_cnt_l ==0) && (red_cnt_c ==0) && (red_cnt_r ==0)) {
            ;
      } else {
             if ((red_cnt_l > red_cnt_c) && (red_cnt_l > red_cnt_r)){
                position_y = posi_y_min;
            } else if ((red_cnt_c >= red_cnt_l) && (red_cnt_c >= red_cnt_r)) {
                position_y = 0;
            } else if ((red_cnt_r > red_cnt_l) && (red_cnt_r > red_cnt_c)) {
                position_y = posi_y_max;
            }

         output.x = 0.0;
         output.y = position_y;
         output.z = 0.0;

         enemy_pub.publish(output);
      }
#endif
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "enemy_position");
    EnemyPosition enemy_position_;
    ros::spin();

    return 0;
}
