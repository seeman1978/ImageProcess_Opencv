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

    Mat boxFilterImg1, boxFilterImg2, boxFilterImg3;
    boxFilter(image, boxFilterImg1, -1, Size(3, 3));
    boxFilter(image, boxFilterImg2, -1, Size(11, 11));
    boxFilter(image, boxFilterImg3, -1, Size(21, 21));

    imshow("Original", image);
    imshow("Box Filter1", boxFilterImg1);
    imshow("Box Filter2", boxFilterImg2);
    imshow("Box Filter3", boxFilterImg3);
    waitKey();
    return 0;
}