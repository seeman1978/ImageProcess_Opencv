//
// Created by 王强 on 2020/3/11.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

//定义全局变量
Mat srcImage, grayImage;
int thresh = 100;
const int threshMaxValue = 255;
RNG rng(12345);

//声明回调函数
void thresh_callback(int, void*);

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



    //图像转化为灰度图并平滑
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    blur(grayImage, grayImage, Size(3, 3));

    imshow("source", srcImage);
    imshow("gray", grayImage);

    //创建轨迹条
    createTrackbar("Thresh:", "gray", &thresh, threshMaxValue, thresh_callback);
    thresh_callback(thresh, 0);
    waitKey(0);

    return 0;
}

void thresh_callback(int, void*)
{
    Mat canny_output;
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //canny边缘检测
    Canny(grayImage, canny_output, thresh, thresh * 2, 3);
    //轮廓提取
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    //计算图像矩
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i], false);
    }

    //计算图像的质心
    vector<Point2f>mc(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
    }

    //绘制轮廓
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 8, 0);
    }

    namedWindow("moments", 0);
    imshow("moments", drawing);

    //用moments矩集计算轮廓面积并与opencv函数计算结果进行比较
    printf("\t Info: Area and Contour Length \n");
    for (int i = 0; i < contours.size(); i++)
    {
        printf("* Contour[%d] - Area(M_00)=%.2f-Area OpenCV:%.2f - Length:%.2f\n", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 8, 0);
    }
}
