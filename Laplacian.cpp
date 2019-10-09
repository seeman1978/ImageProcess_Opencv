//
// Created by zhaoyue on 2019/10/6.
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
    GaussianBlur( image, image_blur, Size(3,3), 0, 0, BORDER_DEFAULT );
    Laplacian(image_blur, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(dst, abs_dst);

    Mat sharpenDst, new_image;
    abs_dst.convertTo(new_image, -1, -1, 0);
    sharpenDst = image + new_image;

    imshow("Original Image", image);
    imshow("Laplacian Image", abs_dst);
    imshow("Sharpen Image", sharpenDst);
    waitKey();
    return 0;
}