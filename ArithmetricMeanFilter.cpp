//
// Created by wq on 2019/12/11.
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

    namedWindow("Original", 0);
    namedWindow("Arithmetric Mean Filter", 0);

    Mat meanImg;    //均值滤波后的图像
    blur(image, meanImg, Size(3, 3));

    imshow("Original", image);
    imshow("Arithmetric Mean Filter", meanImg);

    waitKey();
    return 0;
}