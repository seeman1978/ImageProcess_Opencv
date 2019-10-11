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
    resize(image, image, cv::Size(), 0.4, 0.4);
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


    imshow("Original Image", image);
    imshow("Sobel Image", grad_Sobel);
    imshow("Scharr Image", grad_Scharr);
    waitKey();
    return 0;
}