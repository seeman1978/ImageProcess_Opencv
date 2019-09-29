//
// Created by zhaoyue on 2019/9/26.
//

#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int _brightness = 100;
int _contrast = 100;

Mat image;
/* brightness/contrast callback function */
static void updateBrightnessContrast(int /*arg*/, void*)
{
    int histSize = 64;
    int brightness = _brightness - 100;
    int contrast = _contrast - 100;
    /*
     * The algorithm is by Werner D. Streidt
     * (http://visca.com/ffactory/archives/5-99/msg00021.html)
     */
    double a, b;
    if (contrast > 0)
    {
        double delta = 127. * contrast / 100;
        a = 255. / (255.-delta*2);
        b = a * (brightness - delta);
    } else{
        double delta = -128. * contrast / 100;
        a = (256. - delta*2)/255.;
        b = a * brightness + delta;
    }
    Mat dst, hist;
    image.convertTo(dst, CV_8U, a, b);
    imshow("image", dst);

    calcHist(&dst, 1, 0, Mat(), hist, 1, &histSize, 0);
    Mat histImage = Mat::ones(200, 320, CV_8U)*255;
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, CV_32F);
    histImage = Scalar::all(255);
    int binW = cvRound((double)histImage.cols/histSize);
    for (int i = 0; i < histSize; ++i) {
        rectangle(histImage, Point(i*binW, histImage.rows), Point((i+1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
                  Scalar::all(0), -1, 8, 0);
    }
    imshow("histogram", histImage);
}

static void help()
{
    std::cout << "\nThis program demonstrates the use of calcHist() -- histogram creation.\n"
              << "Usage: \n" << "demhist [image_name -- Defaults to baboon.jpg]" << std::endl;
}

const char* keys =
        {
                "{help h || }{@image|baboon.jpg|input image file}"
        };

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
    image = imread(samples::findFile(inputImage), IMREAD_GRAYSCALE);
    if (image.empty())
    {
        std::cerr << "Cannot read image file: " << inputImage << std::endl;
        return -1;
    }

    Mat dest, orgHist, equHist;
    equalizeHist(image, dest);

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
    imshow("histogram", histImageOrg);

    calcHist(&dest, 1, 0, Mat(), equHist, 1, &histSize, 0);
    Mat histImageEqu = Mat::ones(200, 320, CV_8U)*255;
    normalize(equHist, equHist, 0, histImageEqu.rows, NORM_MINMAX, CV_32F);
    histImageEqu = Scalar::all(255);
    binW = cvRound((double)histImageEqu.cols/histSize);
    for (int i = 0; i < histSize; ++i) {
        rectangle(histImageEqu, Point(i*binW, histImageEqu.rows), Point((i+1)*binW, histImageEqu.rows - cvRound(equHist.at<float>(i))),
                  Scalar::all(0), -1, 8, 0);
    }
    imshow("histogram Equalization", histImageEqu);

    imshow("Original Image", image);
    imshow("Equalization Image", dest);
    waitKey();
    return 0;
}

