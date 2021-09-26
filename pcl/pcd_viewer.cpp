#include <iostream>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h> //SKS: viewer added
#include <typeinfo>
#include <pcl/io/pcd_io.h>
//SKS 4번줄 추가 후 파일 입력 하여 viewer생성하게 수정했음.

int
main (int argc, char** argv)
{
    std::string filename = argv[1];
    std::cout<<"filename = "<<filename<<endl;
    
    //READ #1 
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::io::loadPCDFile<pcl::PointXYZRGB> (filename, *cloud); //내부적으로 reader.read() 호출 
    
    //READ #2
    //pcl::PointCloud<pcl::PointXYZ> cloud;
    //pcl::io::loadPCDFile<pcl::PointXYZ>("tabletop.pcd", cloud) //내부적으로 reader.read() 호출 
    
    //READ #3
    //pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    //pcl::PCDReader reader;
    //reader.read<pcl::PointXYZ>("tabletop.pcd", cloud);
    
    std::cout << "Loaded " << cloud->width * cloud->height  << std::endl; //cloud_filtered->points.size()
    for (int i =0; i<10 ; i++){
        printf("x,y,z = [ %f, %f, %f]\n",cloud->points[i].x, cloud->points[i].y, cloud->points[i].z);
    }
    
    //viewer added : HSJ
    pcl::visualization::CloudViewer viewer("cloud viewer : "+filename);
    //std::cout << "viewer type " << typeid(int).name()  << std::endl; //cloud_filtered->points.size()
    
    viewer.showCloud(cloud);

    while (!viewer.wasStopped ()){
    }
    return (0);
}