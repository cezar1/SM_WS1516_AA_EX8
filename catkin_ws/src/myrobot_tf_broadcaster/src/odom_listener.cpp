#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <tf/transform_broadcaster.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

/**
 * This tutorial demonstrates simple receipt of position and speed of the Evarobot over the ROS system.
 */

/**
 * Callback function executes when new topic data comes.
 * Task of the callback function is to print data to screen.
 */
#define ODO_TRANSLATION_SCALING 0.75
#define ODO_ANGLE_SCALING 1

# define M_PI       3.14159265358979323846  /* pi */

double myDeltaX,myDeltaY,myDeltaZ,myDeltaTheta;
double myLastX,myLastY,myLastZ,myLastTheta;
bool myCreatedOdo;

void mySendTransform(const char* vSource,const char* vDestination, geometry_msgs::TransformStamped vTransform)
{
	static tf::TransformBroadcaster broadcaster;
	vTransform.header.frame_id = vSource;
	vTransform.child_frame_id = vDestination;
	broadcaster.sendTransform(vTransform);
	ROS_INFO("Broadcasting this as TF from frame ID: [%s] to frame ID: [%s]",vSource, vDestination);


}

void chatterCallback2(const geometry_msgs::PoseWithCovarianceStamped & pose)

{
	//fprintf(stderr,"Received pose");
	//fprintf(stderr,"Received pose position %f, %f, %f\n", pose.pose.pose.position.x,pose.pose.pose.position.y, pose.pose.pose.position.z);
	//fprintf(stderr,"Received pose orientation %f, %f, %f\n", pose.pose.pose.orientation.x,pose.pose.pose.orientation.y, pose.pose.pose.orientation.z);

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	geometry_msgs::TransformStamped odom_trans;
	geometry_msgs::Quaternion odom_quat;
	odom_trans.header.stamp = current_time;
	odom_trans.transform.translation.x = 0;
  odom_trans.transform.translation.y = 0;
  odom_trans.transform.translation.z = 0;
	odom_quat = tf::createQuaternionMsgFromYaw((pose.pose.pose.orientation.z)*ODO_ANGLE_SCALING);
  odom_trans.transform.rotation = odom_quat;
	mySendTransform("odom_combined","base_link",odom_trans);
}
void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Seq: [%d]", msg->header.seq);
  ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  ROS_INFO("Orientation-> x: [%f], y: [%f], z: [%f], w: [%f]", msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
  ROS_INFO("Vel-> Linear: [%f], Angular: [%f]", msg->twist.twist.linear.x,msg->twist.twist.angular.z);
	ROS_INFO("Frame ID: [%s] Child frame ID: [%s]",msg->header.frame_id.c_str(), msg->child_frame_id.c_str());

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	geometry_msgs::TransformStamped odom_trans;
	geometry_msgs::Quaternion odom_quat;
	odom_trans.header.stamp = current_time;
	odom_trans.transform.translation.x = msg->pose.pose.position.x*ODO_TRANSLATION_SCALING;
  odom_trans.transform.translation.y = msg->pose.pose.position.y*ODO_TRANSLATION_SCALING;
  odom_trans.transform.translation.z = msg->pose.pose.position.z*ODO_TRANSLATION_SCALING;
	//odom_quat = tf::createQuaternionMsgFromYaw((msg->pose.pose.orientation.z+ODO_ANGLE_SHIFT*M_PI/180)*ODO_ANGLE_SCALING);
	odom_quat = tf::createQuaternionMsgFromYaw(0);
  odom_trans.transform.rotation = odom_quat;
	
	
	mySendTransform("odom_combined","base_link",odom_trans);
	//mySendTransform("map","front_laser_gui",odom_trans);
	//mySendTransform("map","/front_laser",odom_trans);
/*
	odom_trans.transform.translation.x = -msg->pose.pose.position.x;
  odom_trans.transform.translation.y = -msg->pose.pose.position.y;
  odom_trans.transform.translation.z = -msg->pose.pose.position.z;
	odom_quat = tf::createQuaternionMsgFromYaw(-msg->pose.pose.orientation.z);
  odom_trans.transform.rotation = odom_quat;
*/

	//mySendTransform("odom_combined","map",odom_trans);
	
}
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "odom_listener");


  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;



  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("odom", 1000, chatterCallback);
	ros::Subscriber sub2 = n.subscribe("amcl_pose", 1000, chatterCallback2);
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
