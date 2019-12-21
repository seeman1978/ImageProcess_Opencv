//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

double filter_max(const Mat& src)
{
    //最大值滤波
    double har = 0.0;
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j] > har) {
                har = data[j];
            }
        }
    }
    return har;
}

double filter_min(const Mat& src)
{
    //最小值滤波
    double har = src.at<uchar>(0, 0);
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j] < har) {
                har = data[j];
            }
        }
    }
    return har;
}

void MaxFilter(const Mat& input, const Size kernalSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = filter_max(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height)));
            }
        }
    }
}

void MinFilter(const Mat& input, const Size kernalSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = filter_min(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height)));
            }
        }
    }
}

int  main(int argc, char** argv)
{
    Mat imagePepper = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0508(a)(circuit-board-pepper-prob-pt1).tif");
    if (imagePepper.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    Mat imageSalt = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0508(b)(circuit-board-salt-prob-pt1).tif");
    if (imageSalt.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    namedWindow("Pepper Noise", 0);
    namedWindow("Salt Noise", 0);
    namedWindow("Max Filter", 0);
    namedWindow("Min Filter", 0);

    Mat MaxFilterImg = Mat::zeros(imagePepper.size(), imagePepper.type());   //最大值滤波后的图像
    MaxFilter(imagePepper, Size(3, 3), MaxFilterImg);

    Mat MinFilterImg = Mat::zeros(imageSalt.size(), imageSalt.type());   //最大值滤波后的图像
    MinFilter(imageSalt, Size(3, 3), MinFilterImg);

    imshow("Pepper Noise", imagePepper);
    imshow("Salt Noise", imageSalt);
    imshow("Max Filter", MaxFilterImg);
    imshow("Min Filter", MinFilterImg);

    waitKey();
    return 0;
}