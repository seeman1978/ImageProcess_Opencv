//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

double filter_har(const Mat& src)
{
    //谐波滤波
    double har = 0.0;
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j]!=0) {
                har += 1/(double)(data[j]);
            }
        }
    }
    return (src.cols*src.rows)/har;
}

void HarmonicMeanFilter(const Mat& input, const Size kernalSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_har(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height))));
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
    namedWindow("Harmonic Mean Filter", 0);

    Mat HarmonicMeanImg = Mat::zeros(image.size(), image.type());   //谐波均值滤波后的图像
    HarmonicMeanFilter(image, Size(3, 3),HarmonicMeanImg);

    imshow("Gaussian Noise", image);
    imshow("Harmonic Mean Filter", HarmonicMeanImg);

    waitKey();
    return 0;
}