//
// Created by zhaoyue on 2019/10/16.
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

    Mat resample;
    resize(image, resample, cv::Size(), 0.3, 0.3);

    Mat imgBlur;
    blur(image, imgBlur, Size(3, 3));
    resize(imgBlur, imgBlur, cv::Size(), 0.3, 0.3);

    imshow("Original Image", image);
    imshow("Resample Image", resample);
    imshow("Blur Resample Image", imgBlur);
    waitKey();
    return 0;
}