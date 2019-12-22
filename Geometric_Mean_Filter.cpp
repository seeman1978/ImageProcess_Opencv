//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

double filter_geo(const Mat& src)
{
    //谐波滤波
    double geo = 1.0;
    for (int i =0; i < src.rows; ++i){
        const uchar* data = src.ptr<uchar>(i);
        for (int j =0; j < src.cols; ++j){
            if (data[j]!=0) {
                geo *= (double)(data[j]);
            }
        }
    }
    return pow(geo, (double)1/(src.rows*src.cols));
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
    namedWindow("Arithmetric Mean Filter", 0);
    namedWindow("Geometric Mean Filter", 0);


    Mat GeoMeanImg = Mat::zeros(image.size(), image.type());   //几何均值滤波后的图像
    GeometricMeanFilter(image, Size(3, 3), GeoMeanImg);

    Mat ArthMeanImg;    //算术均值滤波后的图像
    blur(image, ArthMeanImg, Size(3, 3));

    imshow("Gaussian Noise", image);
    imshow("Arithmetric Mean Filter", ArthMeanImg);
    imshow("Geometric Mean Filter", GeoMeanImg);

    waitKey();
    return 0;
}