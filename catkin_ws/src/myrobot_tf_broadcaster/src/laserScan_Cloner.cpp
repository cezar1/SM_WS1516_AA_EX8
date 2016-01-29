#include <stdlib.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

double myRangeDurationSums[50000];
int myTotalRangesDurations=0;
long myTime;
const int myTimeStepSize=0;//[seconds]
const int myRangeSelect=541;//[limit the number of ranges sumed]
int myRangesSumed;
std::string myOutputFileName; 
//const char* args[] = {"/LMS_irma","/LMS_achim","/LMS_lars"};
//std::vector<std::string> myTopics(args, args + 4);
std::vector<std::string> myTopics;

FILE * myOutputFile;

void scanValues(const sensor_msgs::LaserScan laser,std::string vTopic)
{
  double myCurrentSum=0;
  for (unsigned int i=0; i<laser.ranges.size();i++)
    {
	myCurrentSum+=laser.ranges[i];
    }
  myRangeDurationSums[myTotalRangesDurations]=myCurrentSum;
  myOutputFile=fopen(myOutputFileName.c_str(),"a");
  myTime=laser.header.stamp.sec;
  fprintf(stderr,"%s : Sum [%d]=[%f] from first %d ranges, at seconds %d and ns %d \n",vTopic.c_str(), myCurrentSum,myTotalRangesDurations,myRangeSelect,laser.header.stamp.sec,laser.header.stamp.nsec);
  fprintf(myOutputFile,"# sum %s %d %f %d %d\n",vTopic.c_str(),myTotalRangesDurations, myRangeDurationSums[myTotalRangesDurations],laser.header.stamp.sec,laser.header.stamp.nsec);
  for (unsigned int i=0; i<laser.ranges.size();i++)
    {
	fprintf(myOutputFile,"%d %f\n",i,laser.ranges[i]);
    }
  fclose (myOutputFile);
  myTotalRangesDurations++;
}
void myCallBack1(const sensor_msgs::LaserScan laser)
{
  //fprintf(stderr,"Callback1\n");
  scanValues(laser,myTopics[0]);
  
}
void myCallBack2(const sensor_msgs::LaserScan laser)
{
  //fprintf(stderr,"Callback2\n");
  scanValues(laser,myTopics[1]);
  
}
void myCallBack3(const sensor_msgs::LaserScan laser)
{
  //fprintf(stderr,"Callback3\n");
  scanValues(laser,myTopics[2]);
  
}


int main(int argc, char **argv)
{
  myTopics.push_back(std::string("/LMS_irma"));
  myTopics.push_back(std::string("/LMS_achim"));
  myTopics.push_back(std::string("/LMS_lars"));
  
  ros::init(argc, argv, "hLaserReader");
  ros::NodeHandle n;
  
  myOutputFileName="LaserScan_ts2.txt";
  myOutputFile=fopen(myOutputFileName.c_str(),"w");
  
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
    if (i==0) mySubscriber1=n.subscribe(myTopics[i].c_str(), 1, myCallBack1);
    else if (i==1) mySubscriber2=n.subscribe(myTopics[i].c_str(), 1, myCallBack2);
    else if (i==2) mySubscriber3=n.subscribe(myTopics[i].c_str(), 1, myCallBack3);  
    fprintf(stderr,"Laser proc 2.Subscribed to %s. Printing to %s. \n",myTopics[i].c_str(), myOutputFileName.c_str());  
    fprintf(myOutputFile,"# topic %s 0 0 0\n",myTopics[i].c_str());
  }
  fclose (myOutputFile);
  ros::spin();
  return 0;
}
