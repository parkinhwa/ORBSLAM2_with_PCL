#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>  //`pcl::PointXYZ`을 포함한 포인트 type 구조체 정의 


using namespace std;
using namespace pcl;
string filename;


void test3(){
  PointCloud<PointXYZRGB>::Ptr tmp( new PointCloud<PointXYZRGB> );
  PointXYZRGB p;
  p.x = 1;
  p.y = 1;
  p.z = 1;
  p.r = 255;
  p.g =0;
  p.b=0;

  tmp->points.push_back(p);
  cout << tmp->points.size() << " <- 점의 개수\n";
  cout << tmp->points.capacity() << " <- tmp 용량\n";
  tmp->height= tmp->points.size();
  tmp->width = 1;
  tmp->is_dense = false;
  tmp->resize( tmp->points.size());s

  // pcl::PointCloud<pcl::PointXYZRGB> cloud; //생성할 PointCloud structure구조체(x,y,z) 정의 
  // cloud.is_dense =false;
  // cloud.width = 1;
  // cloud.height =1;

  // cloud.resize(tmp->points.size());

  // for(int i =0 ; i < tmp->points.size() ; i++){
  //   cloud.points[i] = tmp->points[i];
  // }

  pcl::io::savePCDFileASCII ("test_pcd_mod.pcd", *tmp);
  cout<< " saved!\n";
}

void test(){
  pcl::PointCloud<pcl::PointXYZRGB> cloud; //생성할 PointCloud structure구조체(x,y,z) 정의 

  // 포인트클라우드의 파라미터 설정 : width, height, is_dense
  cloud.width    = 5;
  cloud.height   = 5;
  cloud.is_dense = false;
  cloud.resize( cloud.width* cloud.height);

  //랜덤한 위치 정보를 각 포인트에 지정  
  for (std::size_t i = 0; i < cloud.points.size (); ++i)
  {
    cloud.points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].r = 0;
    cloud.points[i].g = 255;
    cloud.points[i].b = 0;
  }

  //test_pcd.pcd이름으로 저장 
  pcl::io::savePCDFileASCII ("test_pcd.pcd", cloud);

  //정보 출력 
  std::cerr << "Saved " << cloud.points.size () << " data points to test_pcd.pcd." << std::endl;

  for (std::size_t i = 0; i < cloud.points.size (); ++i)
    std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;

}

void test2(string filename){
  PointCloud<PointXYZRGB>::Ptr cloud (new PointCloud<PointXYZRGB>);

  //지도 가져오기
  io::loadPCDFile<PointXYZRGB>(filename, *cloud);
  std::cout<< "Input : " << cloud->points.size () << " (" << pcl::getFieldsList (*cloud) <<")"<< std::endl;


  int x=10 , y = 20;
  //-0.004163, -0.006593, 0.015778]
  for(int i =0 ; i< cloud->points.size() ;i++ ){
    if( i< 10 ){
      printf("x,y,z = [ %f, %f, %f]\n",cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
    }
    if(i ==1 ){
      cloud->points[i].x=10;
      cloud->points[i].y=10;
      cloud->points[i].z = 10;
      cloud->points[i].r = 0;
      cloud->points[i].g = 0;
      cloud->points[i].b = 255;
      cout<<"detected!!\n";
    }
  }
  PointXYZRGB p;
  p.x = 1;
  p.y = 1;
  p.z = 1;
  p.r = 255;
  p.g =0;
  p.b=0;

  // cloud->points.push_back( p);
  io::savePCDFileASCII ("test_pcd_mod.pcd", *cloud);

}

int main (int argc, char** argv)
{
  if(argc!= 2 ) {
    cout<<"인자 부족함\n";
  }
  filename = argv[1];

  test3();
  // test2(filename);

  return 0;
}