//
// Created by 王强 on 2020/2/19.
//

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace cv;
using namespace std;

/**
 * @function help
 * @brief Indications of how to run this program and why is it for
 */
void help()
{
    printf("\t Hough Transform to detect lines \n ");
    printf("\t---------------------------------\n ");
    printf(" Usage: ./HoughLines_Demo <image_name> \n");
}

/**
 * @function main
 */
int main( int argc, char** argv ) {
    // Read the image
    std::string imageName("../data/building.jpg"); // by default
    if (argc > 1) {
        imageName = argv[1];
    }
    Mat src = imread(imageName, IMREAD_COLOR);

    if (src.empty()) {
        help();
        return -1;
    }

    namedWindow("Source Image", 0);
    namedWindow("Global Threshold", 0);

    double T = 0.0;
    /// 计算图像的平均灰度
    cv::Mat img;    /// 灰度图
    if (src.channels() == 3) {
        cv::cvtColor(src, img, COLOR_BGR2GRAY);
    }
    else {
        img = src;
    }
    Scalar myScalar = cv::mean(img);
    T = myScalar[0];
    double Ttemp = 0.01;
    ///全局阈值处理
    do
    {
        std::vector<int> G1, G2;
        for (int i = 0; i < img.rows; ++i){
            for (int j = 0; j < img.cols; ++j){
                for (int ii =0; ii < img.channels(); ++ii){
                    if (img.at<Vec3b>(i,j)[ii] > T){
                        G1.push_back(img.at<Vec3b>(i,j)[ii]);
                    }
                    else{
                        G2.push_back(img.at<Vec3b>(i,j)[ii]);
                    }
                }
            }
        }

        double sum1 = std::accumulate(G1.begin(), G1.end(), 0.0);  //求和
        double mean1 = sum1 / G1.size();   //计算局部均值

        double sum2 = std::accumulate(G2.begin(), G2.end(), 0.0);  //求和
        double mean2 = sum2 / G2.size();   //计算局部均值

        double T1 = (mean1 + mean2)/2;
        if (abs(T1-T) <= Ttemp){
            T = T1;
            break;
        }
        T = T1;
    }while(true);
    Mat dst;
    threshold( img, dst, T, 255, 0 );

    imshow("Source Image", src);
    imshow("Global Threshold", dst);
    waitKey();
    return 0;
}