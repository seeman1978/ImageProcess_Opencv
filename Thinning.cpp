//
// Created by 王强 on 2020/1/21.
//


#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ximgproc.hpp>
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

    Mat gray, blur_image, threshold_output;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    blur(gray, blur_image, Size(3, 3));
    threshold(blur_image, threshold_output, 50, 255, THRESH_BINARY);

    Mat drawing;
    ximgproc::thinning(threshold_output, drawing, ximgproc::THINNING_ZHANGSUEN);
    //显示
    namedWindow( "Thinning image", 0 );
    imshow( "Thinning image", drawing );
    waitKey();
    return 0;
}