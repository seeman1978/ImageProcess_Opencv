//
// Created by 王强 on 2020/3/10.
//

// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
static void help(const char* programName)
{
    cout <<
         "\nA program using pyramid scaling, Canny, contours and contour simplification\n"
         "to find squares in a list of images (pic1-6.png)\n"
         "Returns sequence of squares detected on the image.\n"
         "Call:\n"
         "./" << programName << " [file_name (optional)]\n"
                                "Using OpenCV version " << CV_VERSION << "\n" << endl;
}
int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";
// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
// returns sequence of squares detected on the image.
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;
    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }
            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(contours[i], approx, 1, true);
                squares.push_back(approx);
            }
        }
    }
}
// the function draws all the squares in the image
static void drawSquares(const Mat& image, const vector<vector<Point> >& squares )
{
    /// create hull array for convex hull points
    std::vector<std::vector<Point> > hull(squares.size());
    for(int i=0; i<squares.size(); ++i)
    {
        convexHull(Mat(squares[i]), hull[i], false);
    }

    /// create a blank image(black image)
    Mat drawing = Mat::zeros(image.size(), CV_8UC3);
    for (int j = 0; j < squares.size(); ++j) {
        Scalar color_contours = Scalar(0, 255, 0);  ///green - color for contours
        Scalar color = Scalar(255, 0, 0);   ///blue - color for convex hull
        /// draw ith contour
        drawContours(drawing, squares, j, color_contours, 1, 8, std::vector<Vec4i>(), 0, Point());
        /// draw ith contour
        drawContours(drawing, hull, j, color, 1, 8, std::vector<Vec4i>(), 0, Point());
    }
    //显示
    namedWindow( "Hull demo", 0 );
    imshow( "Hull demo", drawing );

}
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat image = imread(parser.get<String>("@input"));
    if (image.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    vector<vector<Point> > squares;

    findSquares(image, squares);
    drawSquares(image, squares);
    waitKey();
    return 0;
}