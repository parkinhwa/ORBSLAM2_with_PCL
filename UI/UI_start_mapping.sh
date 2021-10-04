file_name=$2
file_full_directory=$1

if test -f $file_full_directory; then
    echo "SYSTEM:::::::::::::::::::::: $file_name 유무확인완료"
else 
    echo "SYSTEM:::::::::::::::::::::: $file_name 없음.."
    exit
fi

if [[ -d "dataset/$file_name/" ]]
then
    echo "SYSTEM:::::::::::::::::::::: 필요파일 이미 준비되어있음... "
else
    echo "SYSTEM:::::::::::::::::::::: .bag 파일 준비중..."
    sudo python2.7 realsense_resolver.py -i $file_full_directory
fi
echo "SYSTEM:::::::::::::::::::::: .bag 파일 준비완료"



echo "SYSTEM:::::::::::::::::::::: 지도제작 시작..."
echo "$> ./ORB_SLAM2_modified/Examples/RGB-D/rgbd_tum ORB_SLAM2_modified/Vocabulary/ORBvoc.txt ORB_SLAM2_modified/d435i.yaml dataset/${file_name} dataset/${file_name}/association.txt
"
./ORB_SLAM2_modified/Examples/RGB-D/rgbd_tum ORB_SLAM2_modified/Vocabulary/ORBvoc.txt ORB_SLAM2_modified/d435i.yaml dataset/${file_name} dataset/${file_name}/association.txt

echo "SYSTEM:::::::::::::::::::::: 지도 제작 완료"
echo "SYSTEM:::::::::::::::::::::: 지도 이름은 현재시간.pcd"

echo "SYSTEM:::::::::::::::::::::: 지도 로딩 시작 "
./pcl/build/pcd_viewer_test result
