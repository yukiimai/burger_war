/*
 *  Copyright (c) 2019, Panasonic Corporation
 *  All rights reserved.
 */

#include "handan.h"

Handan::Handan()
{
	ros::NodeHandle nh;
	sub_gazebo = nh.subscribe("/gazebo/link_states" , 1 , &Handan::callback_gazebo , this );
	pub_pose = nh.advertise<geometry_msgs::PoseStamped>("robot_pose" , 10 , true);
	tf2_brocast = new tf2_ros::TransformBroadcaster();
}

Handan::~Handan()
{

}

void Handan::callback_gazebo(const gazebo_msgs::LinkStates::ConstPtr &msg)
{
	ros::Time msg_time = ros::Time::now();
	if(msg->name.size() == 0)return;

	int link_no = -1;
	//	int base_link_no = -1;

	for(int i = 0; i < msg->name.size() ; i++)
	{
		if(msg->name[i] == "fctr_robo::base_footprint")
		{
			link_no = i;
		}
	}
	if(link_no == -1)
	{
		return;
	}
	//PoseStamped
	geometry_msgs::PoseStamped pose_stamp;
	pose_stamp.pose = msg->pose[link_no];
	pose_stamp.header.stamp = msg_time;
	pose_stamp.header.frame_id = "map";
	pub_pose.publish(pose_stamp);

	//Transform
	tf2_ros::Buffer tf2Buffer;
	tf2_ros::TransformListener tf2Listener(tf2Buffer);
	geometry_msgs::TransformStamped odom_tf;

	try{
		while(!tf2Buffer.canTransform("odom", "base_footprint", ros::Time(0)))
		{
			ros::Duration(0.01).sleep();
		}
		odom_tf = tf2Buffer.lookupTransform("odom", "base_footprint", ros::Time(0));
		msg_time = ros::Time::now();
	}
	catch (tf2::TransformException &ex) {
		ROS_WARN("%s",ex.what());
		return;
	}

	//True Position (Not publish)
	geometry_msgs::TransformStamped true_tf;
	true_tf.header.stamp = msg_time;
	true_tf.header.frame_id = "map";
	true_tf.child_frame_id = "base_footprint";
	true_tf.transform.rotation = pose_stamp.pose.orientation;
	true_tf.transform.translation.x = pose_stamp.pose.position.x;
	true_tf.transform.translation.y = pose_stamp.pose.position.y;
	true_tf.transform.translation.z = pose_stamp.pose.position.z;

	//error_tf = true_tf - odom_tf
	//**** tf"map to odom" means Odom error value
	geometry_msgs::TransformStamped error_tf = diff_geo_tf( true_tf , odom_tf );
	error_tf.header.stamp = msg_time;
	error_tf.header.frame_id = "map";
	error_tf.child_frame_id = "odom";
	tf2_brocast->sendTransform(error_tf);
}

geometry_msgs::TransformStamped Handan::diff_geo_tf(const geometry_msgs::TransformStamped tf1 , const geometry_msgs::TransformStamped tf2)
{
	//tf1(map to base_footprint) - tf2(odom to base_footprint) = return(map to odom)
	//tf1.header.frame_id = map , tf1.child_frame_id = base_footprint
	//tf2.header.frame_id = odom , tf2.child_frame_id = base_footprint
	//ret_tf.header.frame_id = map , ret_tf.child_frame_id = odom
	tf2::Stamped<tf2::Transform> transform1;
	tf2::Stamped<tf2::Transform> transform2;
	tf2::fromMsg(tf1,transform1);
	tf2::fromMsg(tf2,transform2);
	tf2::Transform tf_delta = transform1 * transform2.inverse();
	tf2::Stamped<tf2::Transform> tf_delta_stamped(tf_delta,tf2.header.stamp,tf2.header.frame_id);
	//return
	geometry_msgs::TransformStamped ret_tf = tf2::toMsg(tf_delta_stamped);
	ret_tf.child_frame_id = tf1.header.frame_id;
	return  ret_tf;
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "handan");
	Handan handan_;
	ros::spin();
}
