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

    Mat meanImg1, meanImg2, meanImg3, meanImg4, meanImg5;    //均值滤波后的图像
    blur(image, meanImg1, Size(3, 3));
    blur(image, meanImg2, Size(5, 5));
    blur(image, meanImg3, Size(9, 9));
    blur(image, meanImg4, Size(15, 15));
    blur(image, meanImg5, Size(35, 35));

    imshow("Original", image);
    imshow("Mean Filter1", meanImg1);
    imshow("Mean Filter2", meanImg2);
    imshow("Mean Filter3", meanImg3);
    imshow("Mean Filter4", meanImg4);
    imshow("Mean Filter5", meanImg5);
    waitKey();
    return 0;
}