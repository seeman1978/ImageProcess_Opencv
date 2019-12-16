//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

double filter_contra_har(const Mat& src, double Q)
{
    //谐波滤波
    double contra_har = 0.0, denominator = 0.0, numerator = 0.0;

    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            denominator += pow(data[j], Q);
            numerator += pow(data[j], Q+1);
        }
    }
    if (denominator != 0)
    {//分母为0，返回0，不知道对不对？
        contra_har = (double)numerator/(double)denominator;
    }

    return contra_har;
}

void ContraHarmonicMeanFilter(const Mat& input, const Size& kernelSize, double Q, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernelSize.height-1)/2;
    int w = (kernelSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_contra_har(rgbChannels[ii](Rect(j-w, i-l, kernelSize.width, kernelSize.height)), Q));
            }
        }
    }
}

int  main(int argc, char** argv)
{
    Mat imagePepper = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0508(a)(circuit-board-pepper-prob-pt1).tif");
    if (imagePepper.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        return -1;
    }

    Mat imageSalt = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0508(b)(circuit-board-salt-prob-pt1).tif");
    if (imageSalt.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        return -1;
    }

    namedWindow("Pepper Noise", 0);
    namedWindow("Salt Noise", 0);
    namedWindow("ContraHarmonic Mean Filter Q = 1.5", 0);
    namedWindow("ContraHarmonic Mean Filter Q = -1.5", 0);

    Mat ContraHarmonicMeanImgPositive = Mat::zeros(imagePepper.size(), imagePepper.type());   //逆谐波均值滤波后的图像
    double Q = 1.5;
    ContraHarmonicMeanFilter(imagePepper, Size(3, 3), Q, ContraHarmonicMeanImgPositive);

    Mat ContraHarmonicMeanImgNegative = Mat::zeros(imageSalt.size(), imageSalt.type());   //逆谐波均值滤波后的图像
    Q = -1.5;
    ContraHarmonicMeanFilter(imageSalt, Size(3, 3), Q, ContraHarmonicMeanImgNegative);
    
    imshow("Pepper Noise", imagePepper);
    imshow("Salt Noise", imageSalt);
    imshow("ContraHarmonic Mean Filter Q = 1.5", ContraHarmonicMeanImgPositive);
    imshow("ContraHarmonic Mean Filter Q = -1.5", ContraHarmonicMeanImgNegative);

    waitKey();
    return 0;
}