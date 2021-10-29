/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "pointcloudmapping.h"
#include <KeyFrame.h>
#include <opencv2/highgui/highgui.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include "Converter.h"
#include <pcl/io/pcd_io.h>                  //SKS: this is added for saving map
#include "iostream"
#include "fstream"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include "pcl_modifier_inSRC.h"

#include<ctime>

using std::cout; using std::cin;
using std::endl; using std::string;
using std::vector;
using std::istringstream;

PointCloudMapping::PointCloudMapping(double resolution_)
{
    this->resolution = resolution_;
    voxel.setLeafSize( resolution, resolution, resolution);
    globalMap = boost::make_shared< PointCloud >( );

    viewerThread = make_shared<thread>( bind(&PointCloudMapping::viewer, this ) );
}

void PointCloudMapping::shutdown()
{
    {
        unique_lock<mutex> lck(shutDownMutex);
        shutDownFlag = true;
        keyFrameUpdated.notify_one();
    }
    viewerThread->join();
}

void PointCloudMapping::insertKeyFrame(KeyFrame* kf, cv::Mat& color, cv::Mat& depth)
{
    cout<<"receive a keyframe, id = "<<kf->mnId<<endl;
    unique_lock<mutex> lck(keyframeMutex);
    keyframes.push_back( kf );
    colorImgs.push_back( color.clone() );
    depthImgs.push_back( depth.clone() );

    keyFrameUpdated.notify_one();
}

pcl::PointCloud< PointCloudMapping::PointT >::Ptr PointCloudMapping::generatePointCloud(KeyFrame* kf, cv::Mat& color, cv::Mat& depth)
{
    PointCloud::Ptr tmp( new PointCloud() );
    std::ofstream fout("kfinfo.txt",std::ios_base::out | std::ios_base::app);
    long double ts;
    char line[100];
    string rgbname;
    string depthname;

    std::ifstream mid;
    ts = kf->mTimeStamp;

    vector<int> vector_x;
    vector<int> vector_y;
    vector<int> vector_cls;
    
    vector<double> vTimestamps;

    // mid.open("/home/hansujin/Desktop/mid_pose.txt");
    mid.open("/home/skennetho/ORBSLAM2_with_PCL/mid_pose.txt");

    while(!mid.eof()){
        // cout<<"in while"<<endl;
        // cout<<"1. m ="<<m1<<" n="<<n1<<endl;

        double mid_ts;
        string imgname;

        string s;
        getline(mid,s); //read ts, filenamecd ..
        if(!s.empty()) 
        {
            stringstream ss;
            ss << s;
            ss >> imgname;
            ss >> mid_ts;
            vTimestamps.push_back(mid_ts);
        }
        string s2;
        int cls, x, y;

        getline(mid,s2); //read cls x y
            if(!s2.empty())
        {
            stringstream ss;
            ss << s2;
            ss >> cls;
            ss >> x;
            ss >> y;
            vector_x.push_back(x);
            vector_y.push_back(y);
            vector_cls.push_back(cls);
        }
    }
    
    int find_x = 0;
    int find_y = 0;
    int find_cls = 0;
    for(int i=0; i<vTimestamps.size(); i++){
        if(ts == vTimestamps[i]){
            find_x = vector_x[i];
            find_y = vector_y[i];
            find_cls = vector_cls[i];
        }
    }
    printf("current find[x,y] is [%d, %d]\n", find_x, find_y);
    
    int idx=0;
    int m=0, n=0;
    for (m=0; m<depth.rows; m+=3 )
    {
        for (n=0; n<depth.cols; n+=3 )
        {
            float d = depth.ptr<float>(m)[n];
            if (d < 0.01 || d>10)
                continue;
            PointT p;
            p.z = d;
            p.x = ( n - kf->cx) * p.z / kf->fx;
            p.y = ( m - kf->cy) * p.z / kf->fy;

            p.b = color.ptr<uchar>(m)[n*3];
            p.g = color.ptr<uchar>(m)[n*3+1];
            p.r = color.ptr<uchar>(m)[n*3+2];

            tmp->points.push_back(p);

            if(find_x != 0 && find_y != 0){
                if(n == (find_x - find_x%3)&& m==(find_y - find_y%3)){
                    printf("Add point to [%d, %d] while MAXSIZE is [%d,%d]\n", find_x, find_y, n, m);
                    PointCloud::Ptr square_point = addSquare(p.x, p.y, p.z, find_cls);
                    *tmp += *square_point;
                }
            }
        }
    }
    
    fout<< ts<<'\n';
    for(int i=0; i<kf->GetPose().rows; i++)
    {
        for(int j=0; j<kf->GetPose().cols; j++)
        {
            fout<<kf->GetPose().at<float>(i,j)<<" ";
        }
        fout<<endl;
    }
    fout.close();

    Eigen::Isometry3d T = ORB_SLAM2::Converter::toSE3Quat( kf->GetPose() );
    PointCloud::Ptr cloud(new PointCloud);
    pcl::transformPointCloud( *tmp, *cloud, T.inverse().matrix());
    cloud->is_dense = false;

    cout<<"generate point cloud for kf "<<kf->mTimeStamp<<", size="<<cloud->points.size()<<endl;
    return cloud;
}


void PointCloudMapping::viewer()
{
    pcl::visualization::CloudViewer viewer("viewer");
    while(1)
    {
        {
            unique_lock<mutex> lck_shutdown( shutDownMutex );
            if (shutDownFlag)
            {
                break;
            }
        }
        {
            unique_lock<mutex> lck_keyframeUpdated( keyFrameUpdateMutex );
            keyFrameUpdated.wait( lck_keyframeUpdated );
        }

        // keyframe is updated
        size_t N=0;
        {
            unique_lock<mutex> lck( keyframeMutex );
            N = keyframes.size();
        }

        for ( size_t i=lastKeyframeSize; i<N ; i++ )
        {
            PointCloud::Ptr p = generatePointCloud( keyframes[i], colorImgs[i], depthImgs[i] );
            *globalMap += *p;
        }
//        PointCloud::Ptr tmp(new PointCloud());
//        voxel.setInputCloud( globalMap );
//        voxel.filter( *tmp );
//        globalMap->swap( *tmp );
        viewer.showCloud( globalMap );
        //cout<<"show global map, size="<<globalMap->points.size()<<endl;
        lastKeyframeSize = N;
    }

    cout<<"try to save file..."<<endl;
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y-%m-%d-%H-%M-%S",timeinfo);

    string str = buffer;
    str= "result/"+str+ ".pcd";

    pcl::io::savePCDFileASCII (str, *globalMap);
    cout<<"SKS: viewer end__ saving map as => "+str <<endl;
}