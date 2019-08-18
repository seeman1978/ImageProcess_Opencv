#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main() {
    std::cout << "Hello, World!" << std::endl;
    cv::Mat imgSrc, imgDest;
    imgSrc = cv::imread("/Users/zhaoyue/Pictures/WechatIMG68.jpeg", 0);
    cv::namedWindow("Source", 1);
    cv::imshow("Source", imgSrc);
    imgDest = imgSrc.clone();
    cv::threshold(imgSrc, imgDest, 30, 200.0, cv::THRESH_BINARY);
    cv::namedWindow("Destination");
    cv::imshow("Destination", imgDest);
    cv::waitKey();

    return 0;
}