//
// Created by 王强 on 2020/3/11.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <tuple>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int edgeThresh = 1;
Mat srcImage;

///求最大连通域的中心点坐标
std::tuple<int,int> centroid(const std::vector<Point>& contour)
{
    Moments aMoment = moments(contour); /// 矩
    if (aMoment.m00 != 0){
        int cx = int(aMoment.m10 / aMoment.m00);
        int cy = int(aMoment.m01 / aMoment.m00);
        return std::make_tuple(cy, cy);
    }
    else{
        return {0, 0};
    }
}

void LargestConnecttedComponent(const Mat& src, Mat &dstImage)
{
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    Mat drawing = Mat::zeros(src.size(), CV_8UC3);
    drawContours(drawing, contours, -1, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());      /// -1表示画所有的contour

    ///找到最大区域并填充
    vector<double> vecArea;
    for(auto aContour : contours){
        vecArea.push_back(contourArea(aContour));
    }

    std::vector<double>::iterator result = std::max_element(vecArea.begin(), vecArea.end());
    int max_idx = std::distance(vecArea.begin(), result);

    for(int i=0; i<contours.size(); ++i){
        if (i != max_idx){
            fillConvexPoly(drawing, contours[i], Scalar(0, 0, 0));
        } else{
            fillConvexPoly(drawing, contours[max_idx], Scalar(255, 255, 255));
        }
    }

    /// 求最大连通域的中心坐标
    /*auto [ cx, cy ] = centroid(contours[max_idx]);
    circle(drawing, Point(cx, cy), 5, Scalar(255,0,255));*/

    drawing.copyTo(dstImage);
}

void LargestConnecttedComponent2(const Mat& src, Mat &dstImage)
{
    int nChannels = srcImage.channels();
    std::vector<cv::Mat> SrcMatpart(nChannels);      //生成与通道数数目相等的图像容器
    cv::split(src, SrcMatpart);                 //分解与通道数数目相等的图像容器
    Mat temp = SrcMatpart[0];
    Mat labels;

    //1. 标记连通域
    int n_comps = connectedComponents(temp, labels, 8, CV_16U);
    vector<int> histogram_of_labels(n_comps, 0);    ///初始化labels的个数为0

    for (int row = 0; row < labels.rows; row++) //计算每个labels的个数
    {
        for (int col = 0; col < labels.cols; col++)
        {
            histogram_of_labels.at(labels.at<unsigned short>(row, col)) += 1;
        }
    }
    histogram_of_labels.at(0) = 0; //将背景的labels个数设置为0

    //2. 计算最大的连通域labels索引
    std::vector<int>::iterator result = std::max_element(histogram_of_labels.begin(), histogram_of_labels.end());
    int max_idx = std::distance(histogram_of_labels.begin(), result);

    //3. 将最大连通域标记为1
    for (int row = 0; row < labels.rows; row++)
    {
        for (int col = 0; col < labels.cols; col++)
        {
            if (labels.at<unsigned short>(row, col) == max_idx)
            {
                labels.at<unsigned short>(row, col) = 255;
            }
            else
            {
                labels.at<unsigned short>(row, col) = 0;
            }
        }
    }

    //4. 将图像更改为CV_8U格式
    labels.convertTo(dstImage, CV_8U);
}

// define a trackbar callback
/// Canny 效果
static void onTrackbar(int, void*)
{
    ///图像转化为灰度图并平滑
    Mat grayImage;
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    GaussianBlur(grayImage, grayImage, Size(5, 5), 0, 0);

    //canny边缘检测
    Mat canny_output;
    Canny(grayImage, canny_output, edgeThresh, edgeThresh * 2, 3);

    /// Canny detector with scharr
    Mat dx, dy;
    Scharr(grayImage, dx, CV_16S, 1, 0);
    Scharr(grayImage, dy, CV_16S, 0, 1);
    Canny(dx, dy, canny_output, edgeThresh, edgeThresh*3);

    //轮廓提取
    Mat dstImage;
    LargestConnecttedComponent2(canny_output, dstImage);

    imshow("source", srcImage);
    imshow("gray", dstImage);
}

void onTrackbar2( int, void* )
{
    ///图像转化为灰度图并平滑
    Mat grayImage;
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    GaussianBlur(grayImage, grayImage, Size(5, 5), 0, 0);

    std::vector<cv::Mat> SrcMatpart(grayImage.channels());

    cv::split(grayImage, SrcMatpart);
    Mat dst;
    adaptiveThreshold(SrcMatpart[0], dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, edgeThresh);

    //轮廓提取
    Mat dstImage;
    LargestConnecttedComponent(dst, dstImage);

    imshow("source", srcImage);
    imshow("gray", dstImage);
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    srcImage = imread(parser.get<String>("@input"));
    if (srcImage.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    namedWindow("source", 0);
    namedWindow("gray", 0);

    // create a toolbar
    createTrackbar("Adaptive Threshold default", "gray", &edgeThresh, 10, onTrackbar2);

    // Show the image
    onTrackbar2(0, 0);

    waitKey(0);

    return 0;
}