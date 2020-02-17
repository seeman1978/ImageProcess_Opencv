//
// Created by zhaoyue on 2019/10/11.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

int  main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat image = imread(parser.get<String>("@input"));
    if (image.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }
    /// 创建显示窗口
    namedWindow( "Original Image", 0 );
    namedWindow( "Sobel X Image", 0 );
    namedWindow( "Sobel Y Image", 0 );
    namedWindow( "Sobel grad Image", 0 );
    namedWindow( "Sobel grad threshold Image", 0 );
    namedWindow( "Scharr X Image", 0 );
    namedWindow( "Scharr Y Image", 0 );
    namedWindow( "Scharr grad Image", 0 );
    namedWindow( "Scharr grad threshold Image", 0 );

    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Mat imageBlur;
    GaussianBlur( image, imageBlur, Size(3,3), 0, 0, BORDER_DEFAULT );

    /// 创建 grad_x 和 grad_y 矩阵
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// 求 X方向梯度
    Sobel( imageBlur, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// 求Y方向梯度
    Sobel( imageBlur, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// 合并梯度(近似)
    Mat grad_Sobel;
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad_Sobel );

    /// 阈值处理
    //取像素灰度的最大值
    double dMax, dMin;
    minMaxIdx(grad_Sobel, &dMin, &dMax);
    double T = dMax * 0.33;
    Mat grad_Sobel_t;   //阈值处理后的图像
    threshold( grad_Sobel, grad_Sobel_t, T, 255, 0);

    /// 创建 grad_x 和 grad_y 矩阵
    Mat grad_x_Scharr, grad_y_Scharr;
    Mat abs_grad_x_Scharr, abs_grad_y_Scharr;

    /// 求 X方向梯度
    Scharr( imageBlur, grad_x_Scharr, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x_Scharr, abs_grad_x_Scharr );

    /// 求Y方向梯度
    Scharr( imageBlur, grad_y_Scharr, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y_Scharr, abs_grad_y_Scharr );

    /// 合并梯度(近似)
    Mat grad_Scharr;
    addWeighted( abs_grad_x_Scharr, 0.5, abs_grad_y_Scharr, 0.5, 0, grad_Scharr );

    /// 阈值处理
    //取像素灰度的最大值
    minMaxIdx(grad_Sobel, &dMin, &dMax);
    T = dMax * 0.63;
    Mat grad_Scharr_t;   //阈值处理后的图像
    threshold( grad_Scharr, grad_Scharr_t, T, 255, 0);

    imshow("Original Image", image);
    imshow("Sobel X Image", abs_grad_x);
    imshow("Sobel Y Image", abs_grad_y);
    imshow("Sobel grad Image", grad_Sobel);
    imshow("Sobel grad threshold Image", grad_Sobel_t);
    imshow("Scharr X Image", abs_grad_x_Scharr);
    imshow("Scharr Y Image", abs_grad_y_Scharr);
    imshow("Scharr grad Image", grad_Scharr);
    imshow("Scharr grad threshold Image", grad_Scharr_t);
    waitKey();
    return 0;
}