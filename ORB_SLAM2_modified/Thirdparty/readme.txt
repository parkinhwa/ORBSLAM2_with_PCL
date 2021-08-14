cd DBoW2
mkdir build && cd build
cmake ..
make -j4

cd ../g2o
mkdir build && cd build
cmake ..
make -j4

cd ../Pangolin
mkdir build && cd build
cmake ..
make -j4
sudo make install
