#include<iostream>
#include<pcl/io/pcd_io.h>
#include<pcl/point_types.h>
#include<pcl/filters/voxel_grid.h>

//Downsampling a PointCloud using a VoxelGrid filter
//http://pointclouds.org/documentation/tutorials/voxel_grid.php#voxelgrid
using namespace pcl;

int main(int argc, char** argv){
    std::string filename = argv[1];
    PointCloud<PointXYZ>::Ptr cloud (new PointCloud<PointXYZ>);
    PointCloud<PointXYZ>::Ptr cloud_filtered (new PointCloud<PointXYZ>);
    
    io::loadPCDFile<PointXYZ>(filename, *cloud);
    
    //print number of points
    std::cout<< "Input : " << cloud->points.size () << " (" << pcl::getFieldsList (*cloud) <<")"<< std::endl;
    
    //create object
    VoxelGrid<PointXYZ> sor;
    sor.setInputCloud(cloud);           //input
    sor.setLeafSize(0.01f,0.01f,0.01f); //leaf size 1cm
    sor.filter(*cloud_filtered);
    
    //number of created cloudpoints
    std::cout << "Output : " << cloud_filtered->points.size () << " (" << pcl::getFieldsList (*cloud_filtered) <<")"<< std::endl;

    //save created cloudPoints
    io::savePCDFile<pcl::PointXYZ>(filename+"downsampled.pcd",*cloud_filtered);
    return(0);
}