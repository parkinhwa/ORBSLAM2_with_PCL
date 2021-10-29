#ifndef PCL_MODIFIER_INSRC
#define PCL_MODIFIER_INSRC

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>  //`pcl::PointXYZ`을 포함한 포인트 type 구조체 정의 
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>
#include <pcl/visualization/cloud_viewer.h> //SKS: viewer added
#include "Converter.h"
#include "pointcloudmapping.h"
#include "System.h"

using namespace ORB_SLAM2;
using namespace std;
using namespace pcl;


// pcl::PointCloud< PointCloudMapping::PointT >::Ptr addSquare(double x, double y, double z);
PointCloudMapping::PointCloud::Ptr addSquare(double x, double y, double z, int cls);
PointCloud<PointXYZRGBA>::Ptr generatePoint(vector<float> pose, cv::Mat& depth, int mid_x, int mid_y);
void loadKeyframe(vector<vector<float>> &poses, vector<double> &vTimestamps);


#endif