//
// Created by zhaoyue on 2019/8/24.
//

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main() {
    std::cout << "Hello, World!" << std::endl;
    cv::Mat imgSrc, imgBinary;
    imgSrc = cv::imread("/Users/zhaoyue/Pictures/WechatIMG68.jpeg", 0);
    cv::threshold(imgSrc, imgBinary, 30, 200.0, cv::THRESH_BINARY);
    cv::namedWindow("Binary");
    cv::imshow("Binary", imgBinary);

    cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Mat imgOpening;
    cv::morphologyEx(imgBinary, imgOpening, cv::MORPH_OPEN, se); //开运算

    cv::namedWindow("Opening");
    cv::imshow("Opening", imgOpening);
    cv::waitKey();

    return 0;
}