#include<iostream>
#include<sstream>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std; 

void test(){

    cv::Mat depth = cv::imread("test2.png",CV_LOAD_IMAGE_UNCHANGED);

    depth.convertTo(depth, CV_32F, 5000.0);
    float cx = 318.874;
    float cy = 243.171;
    float fx = 616.213;
    float fy = 616.282;
    
    float x=0.0, y=0.0, z=0.0;
        
    for ( int m=0; m<depth.rows; m+=3 )
    {
        for ( int n=0; n<depth.cols; n+=3 )
        {
            float d = depth.ptr<float>(m)[n];
            cout<< d<<endl;
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

            z = d;
            x = (n - cx) * z / fx;;
            y = (m - cy) * z / fy;;
            cout<<"!!find!!  ["<<x<< ", "<< y<<", "<<z<<"]\n";
            // cout<<"pass  ["<<m<< ", "<< n<<", "<<d<<"] when mid is ["<<mid_x<<", "<<mid_y<<"]\n";
            
        }
    }
}

int main(){
    double num = 6.975e+06;
    cout<<setprecision(0)<<fixed<< num<<endl;
}
