echo "g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o_g2o"
cd g2o
rm -rf build
mkdir build && cd build
cmake ..
make -j12
cd ../../

echo "DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2_DBoW2"
cd DBoW2
rm -rf build
mkdir build && cd build
cmake ..
make -j12
cd ../../


ehco "Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin_Pangolin"
cd Pangolin
rm -rf build
mkdir build && cd build
cmake ..
make -j12
sudo make install
cd ../../

echo "Hew~~ build has done... GoGoGo"

