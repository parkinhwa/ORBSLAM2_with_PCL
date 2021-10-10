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

// typedef pcl::PointXYZRGBA PointT;
// typedef pcl::PointCloud<PointT> PointCloud;

PointCloudMapping::PointCloud::Ptr addSquare(double x, double y, double z){
  // typedef pcl::PointXYZRGBA PointT;
  // typedef pcl::PointCloud<PointT> PointCloud;

  PointCloudMapping::PointCloud::Ptr cloud ( new PointCloudMapping::PointCloud() );
  // PointT p;

  int cnt=0;
  // double _x= 0.05, _y=0.05 , _z=0.05;
  double size = 1e-2;  
  double points_gap = 7e-4;
  cout<<"add square to ["<<x<< ", "<<y<<", "<<z*2.0<<"]"<<endl;

  for(float i = (x-size) ; i < x+size ; i+=points_gap){
    for(float j = (y-size) ; j < y+size ; j+=points_gap){
      for(float k = (z-size) ; k < z+size ; k+=points_gap){
        // cout<<"push back"<<endl;
        PointCloudMapping::PointT p;
        p.x = i;
        p.y = j;
        p.z = k;
        p.r = 0;
        p.g =255;
        p.b=0;
        cloud ->points.push_back(p);
        cnt ++;
      }
    } 
  }
  
  cloud -> resize(cnt);
  cout<<"cnt = "<< cnt<< " size = "<< cloud -> size()<< "capa = "<< endl;

  //정보 출력 
  std::cerr << "Saved " << cloud-> points.size() << " data points to test_pcd.pcd." << std::endl;

  for (std::size_t i = 0; i < 10; ++i)
    std::cerr << "    " << cloud -> points[i].x << " " << cloud -> points[i].y << " " << cloud->points[i].z << std::endl;

  return cloud;

}

PointCloud<PointXYZRGBA>::Ptr generatePoint(vector<float> pose, cv::Mat& depth, int mid_x, int mid_y){
    cout<<"generate point in func2"<<endl;
    typedef pcl::PointXYZRGBA PointT;
    typedef pcl::PointCloud<PointT> PointCloud;
    cout<<"mid poinst:"<<mid_x<< " "<< mid_y<<",   depth.size = "<<depth.rows<< " "<< depth.cols<<endl;
    PointCloud::Ptr tmp( new PointCloud() );

    float cx = 318.874;
    float cy = 243.171;
    float fx = 616.213;
    float fy = 616.282;
    
    float x=0.0, y=0.0, z=0.0;
    pcl::PointXYZRGB p;
    int m2=0, n2=0;
    
    for ( int m=0; m<depth.rows; m+=3 )
    {
        for ( int n=0; n<depth.cols; n+=3 )
        {
          float d = depth.ptr<float>(m)[n];
          if (d < 0.01 || d>10)
              continue;
          else{
            cout<< "안걸림"<<endl;
          }
          // if( m== mid_x - mid_x%3 ){
          //   cout<< "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx match\n";
          // }
          // if( n==mid_y - mid_y%3){
          //   cout<< "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy match\n";
          // }

          if( (m== mid_x - mid_x%3) && (n==mid_y - mid_y%3)){
            z = d;
            x = (n - cx) * z / fx;;
            y = (m - cy) * z / fy;;
            cout<<"!!find!!  ["<<x<< ", "<< y<<", "<<z<<"]\n";
            tmp = addSquare(x, y, z);
          }else{
            // cout<<"pass  ["<<m<< ", "<< n<<", "<<d<<"] when mid is ["<<mid_x<<", "<<mid_y<<"]\n";
          }
        }
    }


    cv::Mat mat = cv::Mat(pose, false);

    Eigen::Isometry3d T = ORB_SLAM2::Converter::toSE3Quat( mat );
    PointCloud::Ptr cloud(new PointCloud);
    pcl::transformPointCloud( *tmp, *cloud, T.inverse().matrix());
    cloud->is_dense = false;

    //    cout<<"generate point cloud for kf "<<kf->mnId<<", size="<<cloud->points.size()<<endl;
    return tmp;
}

/*
void viewer(PointCloud<PointXYZRGBA>::Ptr globalMap){
  typedef pcl::PointXYZRGBA PointT;
  typedef pcl::PointCloud<PointT> PointCloud;
  
  pcl::visualization::CloudViewer viewer("viewer");
  viewer.showCloud( globalMap);
  while (!viewer.wasStopped ()){
  }

}
*/
void loadKeyframe(vector<vector<float>> &poses, vector<double> &vTimestamps){
    std::ifstream kfinfo;
    cout <<"load keyframe"<<endl;
    kfinfo.open("/home/skennetho/ORBSLAM2_with_PCL/kfinfo.txt");     
    cout <<"opened keyframe"<<endl;
                                                                                      
    while(!kfinfo.eof()){

      string buffer;
      if(!getline(kfinfo, buffer)){
        break;
      };
      stringstream ss ;
      ss << buffer;
      
      double timestamp;
      ss >> timestamp;
      // cout<< "reading... "<<timestamp<< '\n';
      // if(!vTimestamps.empty() && (vTimestamps.back() == t)){
      //   break;
      // }
      vTimestamps.push_back(timestamp);
      // cout <<"timestamp = "<<t<<endl;

      vector<float> ps;

      for(int i=0; i<4; i++){
        string pose;
        if(!getline(kfinfo, pose)){
          break;
        };  
        stringstream sss ;
        sss << pose;
        
        for(int j=0; j<4; j++){
          float num;
          sss >> num;
          ps.push_back(num);
        }
      }

      poses.push_back(ps);

    }
    cout<<"end loading keyframe"<<endl;
}

int main (int argc, char** argv)
{
  typedef pcl::PointXYZRGBA PointT;
  typedef pcl::PointCloud<PointT> PointCloud;
  if(argc!= 3 ) {
    cout<<"인자 부족함\n";
  }
  string filename = argv[1]; //생성된 지도
  string depthPath = argv[2];
  vector<vector<float>> pose;
  vector<double> vTimestamps;

  loadKeyframe(pose, vTimestamps);
  std::ifstream mid;

  // PointCloud<PointXYZRGB>::Ptr globalMap (new PointCloud<PointXYZRGB>);

  // PointCloud::Ptr globalMap;
  pcl::PointCloud<PointT> globalMap;
  cout<<"define"<<endl;

  io::loadPCDFile<PointXYZRGBA>(filename, globalMap);
  cout<<"pcl file loaded"<<endl;

  //mid포인트마다  루프
  mid.open("/home/skennetho/ORBSLAM2_with_PCL/mid_pose.txt");
  cout<<"mid_pose open"<<endl;
  while(!mid.eof()){
    double ts;
    string imgname;

    string s;
    getline(mid,s); //read ts, filename
    if(!s.empty()) 
    {
        stringstream ss;
        ss << s;
        ss >> imgname;
        ss >> ts;
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
        
    }
    // cout<<"read mid_pose " << ts<< " "<< x << " " << y<<endl;
   
    // cout<<"vector size "<< vTimestamps.size();
    for(int idx=0; idx<vTimestamps.size(); idx++){
      // cout<< "timestamp check ts= "<< ts<< "tsvector = " << vTimestamps[idx]<< endl;
      if(ts == vTimestamps[idx]){
        vector<float> ps = pose[idx];

        // cout<< "timestamp check ts= "<< ts<< "tsvector = " << vTimestamps[idx]<< endl;
        // cout<<"pose = " << ps[0] <<endl; 
        cout<<"img = "<< argv[2]<<"/"<<imgname<<endl;

        cv::Mat imD = cv::imread(string(argv[2])+"/"+imgname,CV_LOAD_IMAGE_UNCHANGED);
        pcl::PointCloud<pcl::PointXYZRGBA> ::Ptr p = generatePoint(ps, imD, x, y);
        
        globalMap += *p;
        io::savePCDFileASCII ("square_mapping.pcd", globalMap);
        break;
      }
    }

  }

  cout<<"out of while"<<endl;
  io::savePCDFileASCII ("square_mapping.pcd", globalMap);
  cout<<"save!!"<< endl;

  // std::cout<< "Input : " << globalMap->points.size () << " (" << pcl::getFieldsList (*globalMap) <<")"<< std::endl;

  // viewer(globalMap);

  return 0;
}