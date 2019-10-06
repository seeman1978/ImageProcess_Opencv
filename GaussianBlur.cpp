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

    Mat GaussianBlurImg1, GaussianBlurImg2;
    GaussianBlur(image, GaussianBlurImg1, Size(21, 21), 0, 0);
    GaussianBlur(image, GaussianBlurImg2, Size(43, 43), 0, 0);

    imshow("Original", image);
    imshow("Gaussian Filter1", GaussianBlurImg1);
    imshow("Gaussian Filter2", GaussianBlurImg1);

    waitKey();
    return 0;
}