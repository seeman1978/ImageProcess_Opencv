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

    namedWindow("Original Image", 0);
    namedWindow("Resample Image", 0);
    namedWindow("Blur Resample Image", 0);

    Mat imgZoomOut;
    resize(image, imgZoomOut, cv::Size(), 0.45, 0.45,  INTER_AREA );
    //resize(imgZoomOut, imgZoomOut, cv::Size(), 1.33, 1.33);

    Mat imgZoomOutBlur;
    blur(image, imgZoomOutBlur, Size(5, 5));
    resize(imgZoomOutBlur, imgZoomOutBlur, cv::Size(), 0.75, 0.75,  INTER_AREA );
    //resize(imgZoomOutBlur, imgZoomOutBlur, cv::Size(), 1.33, 1.33);

    imshow("Original Image", image);
    imshow("Resample Image", imgZoomOut);
    imshow("Blur Resample Image", imgZoomOutBlur);
    waitKey();
    return 0;
}