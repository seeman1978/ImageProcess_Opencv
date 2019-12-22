//
// Created by 王强 on 2019/12/22.
//


#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace cv;

double filter_geo(const Mat& src)
{
    //几何均值滤波
    double geo = 1.0;
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j]!=0) {
                geo *= (double)(data[j]);
            }
        }
    }
    double result = pow(geo, (double)1/(src.rows*src.cols));
    return result;
}

void GeometricMeanFilter(const Mat& input, const Size kernalSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_geo(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height))));
            }
        }
    }
}

double filter_AdaptiveLocal(double g, const Mat& src, double dVariance)
{
    std::vector<uchar> V;
    if (src.isContinuous()) {
        // array.assign(mat.datastart, mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
        V.assign(src.data, src.data + src.total());
    } else {
        for (int i = 0; i < src.rows; ++i) {
            V.insert(V.end(), src.ptr<uchar>(i), src.ptr<uchar>(i)+src.cols);
        }
    }

    double sum = std::accumulate(V.begin(), V.end(), 0.0);  //求和
    double mean = sum / V.size();   //计算局部均值

    std::vector<double> diff(V.size());
    std::transform(V.begin(), V.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    //double stdev = std::sqrt(sq_sum / (V.size()-1));    //计算局部方差
    double stdev = sq_sum / (V.size()-1);    //计算局部方差
    double result;
    if (dVariance > stdev) {
        result = mean;
    }
    else {
        result = g - dVariance * (g - mean)/stdev;
    }

    return result;
}

void AdaptiveLocalFilter(const Mat& input, const Size kernalSize, double dVariance, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_AdaptiveLocal(input.at<Vec3b>(i, j)[ii], rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height)), dVariance));
            }
        }
    }
}

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

    namedWindow("Gaussian Noise", 0);
    namedWindow("Geo Mean Filter", 0);
    namedWindow("AdaptiveLocal Mean Blur", 0);

    Mat MeanImg = Mat::zeros(image.size(), image.type());   //自适应局部降低噪声均值滤波后的图像
    AdaptiveLocalFilter(image, Size(7, 7), 1000.0, MeanImg);

    Mat geoBlurImg = Mat::zeros(image.size(), image.type());   //几何均值滤波后的图像
    GeometricMeanFilter(image, Size(7, 7), geoBlurImg);

    imshow("Gaussian Noise", image);
    imshow("Geo Mean Filter", geoBlurImg);
    imshow("AdaptiveLocal Mean Blur", MeanImg);

    waitKey();
    return 0;
}