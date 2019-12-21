//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

double filter_mid(const Mat& src)
{
    //中点滤波
    double minFilter = src.at<uchar>(0, 0), maxFilter = 0, midFilter;
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j] < minFilter) {
                minFilter = data[j];
            }
            else if (data[j] > maxFilter) {
                maxFilter = data[j];
            }
        }
    }
    midFilter = (double)(maxFilter + minFilter)/2.0;
    return midFilter;
}

void MidFilter(const Mat& input, const Size kernalSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_mid(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height))));
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
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    Mat imageSalt = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0508(b)(circuit-board-salt-prob-pt1).tif");
    if (imageSalt.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    Mat imageGaussian = imread("/Users/zhaoyue/Downloads/DIP3E_CH05_Original_Images/Fig0507(b)(ckt-board-gauss-var-400).tif");
    if (imageGaussian.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    namedWindow("Pepper Noise", 0);
    namedWindow("Salt Noise", 0);
    namedWindow("Gaussian Noise", 0);
    namedWindow("Mid Filter For Pepper", 0);
    namedWindow("Mid Filter For Salt", 0);
    namedWindow("Mid Filter For Gaussian", 0);

    Mat MidFilterImgPepper = Mat::zeros(imagePepper.size(), imagePepper.type());   //中点滤波后的图像
    MidFilter(imagePepper, Size(3, 3), MidFilterImgPepper);

    Mat MidFilterImgSalt = Mat::zeros(imageSalt.size(), imageSalt.type());   //中点滤波后的图像
    MidFilter(imageSalt, Size(3, 3), MidFilterImgSalt);

    Mat MidFilterImgGaussian = Mat::zeros(imageSalt.size(), imageGaussian.type());   //中点滤波后的图像
    MidFilter(imageGaussian, Size(3, 3), MidFilterImgGaussian);

    imshow("Pepper Noise", imagePepper);
    imshow("Salt Noise", imageSalt);
    imshow("Gaussian Noise", imageGaussian);

    imshow("Mid Filter For Pepper", MidFilterImgPepper);
    imshow("Mid Filter For Salt", MidFilterImgSalt);
    imshow("Mid Filter For Gaussian", MidFilterImgGaussian);
    waitKey();
    return 0;
}