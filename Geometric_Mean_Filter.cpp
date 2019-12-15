//
// Created by wq on 2019/12/15.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;

void GeometricMeanFilter(const Mat& input, Mat& output)
{
    double power = 1.0 / 9.0, geo = 1.0;

    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            for (int c = 0; c < input.channels(); ++c) {    //必须有channel，否则目标图像只有原图像的三分之一
                if (y == 0 || x == 0 || y == input.rows-1 || x == input.cols - 1){
                    output.at<Vec3b>(y, x)[c] = input.at<Vec3b>(y, x)[c];
                } else{
                    if (input.at<Vec3b>(y, x)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y, x)[c];
                    }
                    if (input.at<Vec3b>(y+1, x+1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y+1, x+1)[c];
                    }
                    if (input.at<Vec3b>(y+1, x)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y+1, x)[c];
                    }
                    if (input.at<Vec3b>(y, x+1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y, x+1)[c];
                    }
                    if (input.at<Vec3b>(y+1, x-1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y+1, x-1)[c];
                    }
                    if (input.at<Vec3b>(y-1, x+1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y-1, x+1)[c];
                    }
                    if (input.at<Vec3b>(y-1, x)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y-1, x)[c];
                    }
                    if (input.at<Vec3b>(y, x-1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y, x-1)[c];
                    }
                    if (input.at<Vec3b>(y-1, x-1)[c] != 0) {
                        geo = geo * input.at<Vec3b>(y-1, x-1)[c];
                    }
                    output.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(pow(geo, power));
                    geo = 1;
                }
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
    GeometricMeanFilter(image, GeoMeanImg);

    Mat ArthMeanImg;    //算术均值滤波后的图像
    blur(image, ArthMeanImg, Size(3, 3));

    imshow("Gaussian Noise", image);
    imshow("Arithmetric Mean Filter", ArthMeanImg);
    imshow("Geometric Mean Filter", GeoMeanImg);

    waitKey();
    return 0;
}