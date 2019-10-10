//
// Created by wq on 2019/10/10.
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

    Mat image_blur, dst, abs_dst;
    int kernel_size = 3, scale = 1, delta = 0, ddepth = CV_16S;

    /// 使用高斯滤波消除噪声
    GaussianBlur( image, image_blur, Size(31,31), 5, 5, BORDER_DEFAULT );
    Mat g_mask;
    g_mask = image - image_blur;

    Mat result1, result2;
    result1 = image + g_mask;

    g_mask.convertTo(result2, -1, 4.5, 0);
    Mat Unsharp;
    Unsharp = image + result2;

    imshow("Original Image", image);
    imshow("Gaussian Blur Image", image_blur);
    imshow("Unsharp mask template Image", g_mask);
    imshow("Unsharp mask k=1 Image", result1);
    imshow("Unsharp mask k=4.5 Image", Unsharp);
    waitKey();
    return 0;
}