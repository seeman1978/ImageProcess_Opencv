//
// Created by zhaoyue on 2019/9/22.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using namespace cv;

int computeOutput(int x, int r1, int s1, int r2, int s2)
{
    float result;
    if(0 <= x && x < r1)
    {
        result = s1/r1 * x;
    }
    else if(r1 <= x && x <= r2)
    {
        result = (s2 - s1)/(r2 - r1) * (x - r1);
    }
    else if(r2 < x && x <= 255)
    {
        result = ((255 - s2)/(255 - r2)) * (x - r2);
    }
    return (int)result;
}

int computeOutput(int x, int mean)
{
    int result;
    if(x < mean)
    {
        result = 0;
    }
    else
    {
        result = 255;
    }

    return result;
}

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat image = imread(parser.get<String>("@input"));
    if (image.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    double r_min, r_max;
    cv::minMaxLoc(image, &r_min, &r_max);
    double r1 = r_min, s1 = 0;
    double r2 = r_max, s2 = 255;

    Mat mat_mean, mat_stddev;
    meanStdDev(image, mat_mean, mat_stddev);
    double mean;
    mean = mat_mean.at<double>(0, 0);       //灰度均值

    Mat new_image = Mat::zeros(image.size(), image.type());
    Mat mean_image = Mat::zeros(image.size(), image.type());
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            for (int c = 0; c < image.channels(); ++c) {
                new_image.at<Vec3b>(y, x)[c] =
                        saturate_cast<uchar>(computeOutput(image.at<Vec3b>(y,x)[c], r1, s1, r2, s2));
                mean_image.at<Vec3b>(y, x)[c] =
                        saturate_cast<uchar>(computeOutput(image.at<Vec3b>(y,x)[c], mean));
            }
        }
    }

    normalize(new_image, new_image, 0, 255, NORM_MINMAX);
    convertScaleAbs(new_image, new_image);

    normalize(mean_image, mean_image, 0, 255, NORM_MINMAX);
    convertScaleAbs(mean_image, mean_image);

    imshow("Original Image", image);
    imshow("New Image", new_image);
    imshow("New Mean Image", mean_image);

    waitKey();
    return 0;
}
