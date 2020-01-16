//
// Created by zhaoyue on 2019/8/24.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main( int argc, char** argv ) {

    cv::Mat imgSrc;
    /// Load 图像
    imgSrc = imread( argv[1] );

    cv::namedWindow("Source Image", 0);
    cv::imshow("Source Image", imgSrc);

    cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat imgOpening;
    cv::morphologyEx(imgSrc, imgOpening, cv::MORPH_OPEN, se); //开运算

    cv::namedWindow("Opening", 0);
    cv::imshow("Opening", imgOpening);

    cv::Mat imgClose;
    cv::morphologyEx(imgOpening, imgClose, cv::MORPH_CLOSE, se); //闭运算

    cv::namedWindow("Close", 0);
    cv::imshow("Close", imgClose);
    cv::waitKey();

    return 0;
}