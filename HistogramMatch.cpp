//
// Created by zhaoyue on 2019/9/27.
//

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
    std::cout << "\nThis program demonstrates the use of calcHist() -- histogram creation.\n"
              << "Usage: \n" << "demhist [image_name -- Defaults to baboon.jpg]" << std::endl;
}

const char* keys =
        {
                "{help h || }{@image|baboon.jpg|input image file}"
        };

bool use_mask;
Mat img, templ, mask, result;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

void MatchingMethod(int, void*);

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n" << argv[0] << " <image_name> <template_name> [<mask_name>]" << endl;
        return -1;
    }

    img = imread(argv[1], IMREAD_COLOR);
    templ = imread(argv[2], IMREAD_COLOR);
    if(argc > 3) {
        use_mask = true;
        mask = imread( argv[3], IMREAD_COLOR );
    }

    if (img.empty() || templ.empty() || (use_mask && mask.empty()))
    {
        cout << "Can't read one of the images" << endl;
        return EXIT_FAILURE;
    }

    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
    MatchingMethod(0, 0);
    waitKey(0);
    return EXIT_SUCCESS;
}

void MatchingMethod(int, void*)
{
    Mat img_display;
    img.copyTo(img_display);
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    bool method_accepts_mask = (TM_SQDIFF == match_method || match_method == TM_CCORR_NORMED);
    if (use_mask && method_accepts_mask)
    {
        matchTemplate(img, templ, result, match_method);
    } else{
        matchTemplate(img, templ, result, match_method);
    }
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
    } else
    {
        matchLoc = maxLoc;
    }
    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y+templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x+templ.cols, matchLoc.y+templ.rows), Scalar::all(0), 2, 8, 0);

    imshow(image_window, img_display);
    imshow(result_window, result);
    return;
}

/*
int main(int argc, const char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    string inputImage = parser.get<string>(0);
    // Load the source image . HighGUI use.
    Mat image = imread(samples::findFile(inputImage), IMREAD_GRAYSCALE);
    if (image.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        return -1;
    }

    Mat orgHist;
    int histSize = 64;
    calcHist(&image, 1, 0, Mat(), orgHist, 1, &histSize, 0);
    Mat histImageOrg = Mat::ones(200, 320, CV_8U)*255;
    normalize(orgHist, orgHist, 0, histImageOrg.rows, NORM_MINMAX, CV_32F);
    histImageOrg = Scalar::all(255);
    int binW = cvRound((double)histImageOrg.cols/histSize);
    for (int i = 0; i < histSize; ++i) {
        rectangle(histImageOrg, Point(i*binW, histImageOrg.rows), Point((i+1)*binW, histImageOrg.rows - cvRound(orgHist.at<float>(i))),
                  Scalar::all(0), -1, 8, 0);
    }

    imshow("Original Image", image);
    imshow("histogram", histImageOrg);

    waitKey();
    return 0;
}
*/


