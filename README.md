# STILL WORKING ON PROGRESS
## Gaoxiang12's comment
This is a modified ORB_SLAM2 (from https://github.com/raulmur/ORB_SLAM2, thanks for Raul's great work!) with a online point cloud map module running in RGB-D mode. You can visualize your point cloud map during the SLAM process. 
## Skennetho's Comment
I and my team-mates has modified the code of Gaoxiang12. from the code of Gaoxiang12 we tried to include object detection which we used YOLOv5.

This project allows to make marker on the object detected in point cloud map.

And also we provided simple python made UI. which includes 

function that decompose .bag format file(RGB-D file) into Color images, Depth images and also txt files needed by ORB-SLAM2,

function that starts ORB-SLAM2 and put the result map into 'result'folder, and

function that opens the .pcd file which is the result map of ORB-SLAM2.

# How to install
this installation is based on https://github.com/gaoxiang12/ORBSLAM2_with_pointcloud_map and original ORB-SLAM2

requirements: pcl>=1.7,opencv>=3.4 other requirements for ORB-SLAM2

```
dpkg -i libeigen3-dev_3.2.0-8_all.deb

unzip g2o_with_orbslam2.zip
cd g2o_with_orbslam2
rm -rf build
mkdir build && cd build
cmake ..
make -j12

cd ../../
cd ORB_SLAM2_modified/Thirdparty
unzip DBoW2_Pangolin_g2o.zip
sudo sh quick_build.sh

cd ..
sudo sh build.sh
```

To run the program you also need to download the ORB vocabulary (which is a large file so I don't upload it) in the original ORB_SLAM2 repository.

# Run examples
Prepare a RGBD camera or dataset, give the correct parameters and you can get a ORB SLAM with point cloud maps.

# Build the unpacked modified repo 
please see this [README](./ORB_SLAM2_modified/README.md)





