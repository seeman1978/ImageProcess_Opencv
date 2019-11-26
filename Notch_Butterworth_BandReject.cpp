//
// Created by zhaoyue on 2019/11/24.
//

/// Example : apply ideal low pass filtering to input image
/// usage: prog {<image_name>}

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/******************************************************************************/
// setup the cameras properly based on OS platform

// 0 in linux gives first camera for v4l
//-1 in windows gives first device or user dialog selection

#ifdef linux
#define CAMERA_INDEX 0
#else
#define CAMERA_INDEX -1
#endif
/******************************************************************************/

void fftshift(const Mat& inputImg, Mat& outputImg);
void filter2DFreq(const Mat& inputImg, Mat& outputImg, const Mat& H);
void synthesizeFilterH(Mat& inputOutput_H, Point center, int radius);
void calcPSD(const Mat& inputImg, Mat& outputImg, int flag = 0);

int main(int argc, char** argv)
{
    Mat imgIn = imread("/Users/zhaoyue/Downloads/DIP3E_Original_Images_CH04/period_input.jpg", IMREAD_GRAYSCALE);
    if (imgIn.empty()) //check whether the image is loaded or not
    {
        cout << "ERROR : Image cannot be loaded..!!" << endl;
        return -1;
    }
    imgIn.convertTo(imgIn, CV_32F);
    // it needs to process even image only
    Rect roi = Rect(0, 0, imgIn.cols & -2, imgIn.rows & -2);
    imgIn = imgIn(roi);
    // PSD calculation (start)
    Mat imgPSD;
    calcPSD(imgIn, imgPSD);
    fftshift(imgPSD, imgPSD);
    normalize(imgPSD, imgPSD, 0, 255, NORM_MINMAX);
    // PSD calculation (stop)
    //H calculation (start)
    Mat H = Mat(roi.size(), CV_32F, Scalar(1));
    const int r = 21;
    synthesizeFilterH(H, Point(705, 458), r);
    synthesizeFilterH(H, Point(850, 391), r);
    synthesizeFilterH(H, Point(993, 325), r);
    //H calculation (stop)
    // filtering (start)
    Mat imgOut;
    fftshift(H, H);
    filter2DFreq(imgIn, imgOut, H);
    // filtering (stop)
    imgOut.convertTo(imgOut, CV_8U);
    normalize(imgOut, imgOut, 0, 255, NORM_MINMAX);
    imshow("result.jpg", imgOut);
    imshow("PSD.jpg", imgPSD);
    fftshift(H, H);
    normalize(H, H, 0, 255, NORM_MINMAX);
    imshow("filter.jpg", H);
    cv::waitKey();
    return 0;
}
void fftshift(const Mat& inputImg, Mat& outputImg)
{
    outputImg = inputImg.clone();
    int cx = outputImg.cols / 2;
    int cy = outputImg.rows / 2;
    Mat q0(outputImg, Rect(0, 0, cx, cy));
    Mat q1(outputImg, Rect(cx, 0, cx, cy));
    Mat q2(outputImg, Rect(0, cy, cx, cy));
    Mat q3(outputImg, Rect(cx, cy, cx, cy));
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}
void filter2DFreq(const Mat& inputImg, Mat& outputImg, const Mat& H)
{
    Mat planes[2] = { Mat_<float>(inputImg.clone()), Mat::zeros(inputImg.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);
    dft(complexI, complexI, DFT_SCALE);
    Mat planesH[2] = { Mat_<float>(H.clone()), Mat::zeros(H.size(), CV_32F) };
    Mat complexH;
    merge(planesH, 2, complexH);
    Mat complexIH;
    mulSpectrums(complexI, complexH, complexIH, 0);
    idft(complexIH, complexIH);
    split(complexIH, planes);
    outputImg = planes[0];
}
void synthesizeFilterH(Mat& inputOutput_H, Point center, int radius)
{
    Point c2 = center, c3 = center, c4 = center;
    c2.y = inputOutput_H.rows - center.y;
    c3.x = inputOutput_H.cols - center.x;
    c4 = Point(c3.x,c2.y);
    circle(inputOutput_H, center, radius, 0, -1, 8);
    circle(inputOutput_H, c2, radius, 0, -1, 8);
    circle(inputOutput_H, c3, radius, 0, -1, 8);
    circle(inputOutput_H, c4, radius, 0, -1, 8);
}
// Function calculates PSD(Power spectrum density) by fft with two flags
// flag = 0 means to return PSD
// flag = 1 means to return log(PSD)
void calcPSD(const Mat& inputImg, Mat& outputImg, int flag)
{
    Mat planes[2] = { Mat_<float>(inputImg.clone()), Mat::zeros(inputImg.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);
    dft(complexI, complexI);
    split(complexI, planes);            // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))
    planes[0].at<float>(0) = 0;
    planes[1].at<float>(0) = 0;
    // compute the PSD = sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)^2
    Mat imgPSD;
    magnitude(planes[0], planes[1], imgPSD);        //imgPSD = sqrt(Power spectrum density)
    pow(imgPSD, 2, imgPSD);                         //it needs ^2 in order to get PSD
    outputImg = imgPSD;
    // logPSD = log(1 + PSD)
    if (flag)
    {
        Mat imglogPSD;
        imglogPSD = imgPSD + Scalar::all(1);
        log(imglogPSD, imglogPSD);
        outputImg = imglogPSD;
    }
}

