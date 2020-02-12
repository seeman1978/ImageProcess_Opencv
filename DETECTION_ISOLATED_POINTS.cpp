//
// Created by 王强 on 2020/2/12.
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

    namedWindow("Original Image", 0);
    namedWindow("Laplacian Image", 0);
    namedWindow("Isolated Points Image", 0);

    Mat image_blur, dst, abs_dst;
    int kernel_size = 3, scale = 1, delta = 0, ddepth = CV_16S;

    /// 使用高斯滤波消除噪声
    GaussianBlur( image, image_blur, Size(3,3), 0, 0, BORDER_DEFAULT );
    Laplacian(image_blur, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(dst, abs_dst);

    //取像素灰度的最大值
    double dMax, dMin;
    minMaxIdx(abs_dst, &dMin, &dMax);
    double T = dMax * 0.9;

    //根据T，求二值图像
    Mat new_image = Mat::zeros(abs_dst.size(), abs_dst.type());
    for (int i = 0; i < abs_dst.rows; ++i){
        for (int j = 0; j < abs_dst.cols; ++j){
            for (int ii =0; ii < abs_dst.channels(); ++ii){
                if (abs_dst.at<Vec3b>(i,j)[ii] >= T){
                    new_image.at<Vec3b>(i,j)[ii] = 255;
                }
            }
        }
    }

    imshow("Original Image", image);
    imshow("Laplacian Image", abs_dst);
    imshow("Isolated Points Image", new_image);
    waitKey();
    return 0;
}