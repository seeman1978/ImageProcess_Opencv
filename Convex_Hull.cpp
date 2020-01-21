//
// Created by 王强 on 2020/1/21.
//


#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

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

    std::vector< std::vector<Point> > contours;     ///list of contours points
    std::vector<Vec4i> hierarchy;

    //find contours
    findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// create hull array for convex hull points
    std::vector<std::vector<Point> > hull(contours.size());
    for(int i=0; i<contours.size(); ++i)
    {
        convexHull(Mat(contours[i]), hull[i], false);
    }

    /// create a blank image(black image)
    Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
    for (int j = 0; j < contours.size(); ++j) {
        Scalar color_contours = Scalar(0, 255, 0);  ///green - color for contours
        Scalar color = Scalar(255, 0, 0);   ///blue - color for convex hull
        /// draw ith contour
        drawContours(drawing, contours, j, color_contours, 1, 8, std::vector<Vec4i>(), 0, Point());
        /// draw ith contour
        drawContours(drawing, hull, j, color, 1, 8, std::vector<Vec4i>(), 0, Point());
    }
    //显示
    namedWindow( "Hull demo", 0 );
    imshow( "Hull demo", drawing );
    waitKey();
    return 0;
}