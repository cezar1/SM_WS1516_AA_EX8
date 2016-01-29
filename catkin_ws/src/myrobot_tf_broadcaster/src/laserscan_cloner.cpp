#include <stdlib.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"


std::string myOutputFileName; 
std::vector<std::string> myTopics;


void myCallBack3(const sensor_msgs::LaserScan laser)
{
  //fprintf(stderr,"Callback3\n");
  //scanValues(laser,myTopics[2]);
  //fprintf(stderr,"Received laser hit");
	sensor_msgs::LaserScan myLaser=laser;
	myLaser.header.frame_id="front_laser_gui";
ros::NodeHandle n;
ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 50);
	scan_pub.publish(myLaser);
}


int main(int argc, char **argv)
{
  myTopics.push_back(std::string("/LMS_irma"));
  myTopics.push_back(std::string("/LMS_achim"));
  myTopics.push_back(std::string("/LMS_lars"));
  
  ros::init(argc, argv, "hLaserReader");
  ros::NodeHandle n;
  
  myOutputFileName="LaserScan_ts2.txt";
  //myOutputFile=fopen(myOutputFileName.c_str(),"w");
  
  ros::Subscriber mySubscriber1,mySubscriber2,mySubscriber3;
  
//   for (int i=0;i<1;i++)
//   {
//     
//     if (i==0) 
//     
//     else if (i==1) ros::Subscriber mySubscriber=n.subscribe(myTopics[i].c_str(), 1, myCallBack2);
//     else if (i==2) ros::Subscriber mySubscriber=n.subscribe(myTopics[i].c_str(), 1, myCallBack3);
//     //fprintf(stderr,"Try %d: Result %d\n",i,mySubscriber);
//     fprintf(stderr,"Laser proc 2.Subscribed to %s. Printing to %s. \n",myTopics[i].c_str(), myOutputFileName.c_str());
//     fprintf(myOutputFile,"# topic %s\n",myTopics[i].c_str());
//   }
  
  for (int i=0;i<myTopics.size();i++)
  {
    //if (i==0) mySubscriber1=n.subscribe(myTopics[i].c_str(), 1, myCallBack1);
    //else if (i==1) mySubscriber2=n.subscribe(myTopics[i].c_str(), 1, myCallBack2);
    if (i==2) mySubscriber3=n.subscribe(myTopics[i].c_str(), 1, myCallBack3);  
    fprintf(stderr,"Laser proc 2.Subscribed to %s. Printing to %s. \n",myTopics[i].c_str(), myOutputFileName.c_str());  
    //fprintf(myOutputFile,"# topic %s 0 0 0\n",myTopics[i].c_str());
  }
	ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 50);
  //fclose (myOutputFile);
  ros::spin();
  return 0;
}
