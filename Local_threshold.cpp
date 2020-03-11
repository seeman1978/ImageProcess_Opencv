//
// Created by 王强 on 2020/2/20.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;

int b = 2;
int a = 29;
int const max_value = 255;
int const max_a = 100;
int const max_b = 255;

Mat src, src_gray, dst;
std::string window_name = "Threshold Demo";
std::string trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
std::string trackbar_value = "Value";

/// Function headers
void Threshold_Demo( int, void* );

int main(int argc, char** argv)
{
    src = imread(argv[1], 1);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    namedWindow(window_name, WINDOW_AUTOSIZE);
    createTrackbar( trackbar_type,
                    window_name, &a,
                    max_a, Threshold_Demo );

    createTrackbar( trackbar_value,
                    window_name, &b,
                    max_b, Threshold_Demo );

    /// Call the function to initialize
    Threshold_Demo( 0, 0 );

    /// Wait until user finishes program
    while(true)
    {
        int c;
        c = waitKey( 20 );
        if( (char)c == 27 )
        {
            break;
        }
    }
}

/**
 * @function Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
    std::vector<cv::Mat> SrcMatpart(src.channels());

    cv::split(src, SrcMatpart);
    adaptiveThreshold(SrcMatpart[0], dst, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, a, b);

    imshow( window_name, dst );
}
