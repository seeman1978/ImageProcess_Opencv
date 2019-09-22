//
// Created by zhaoyue on 2019/9/22.
// 灰度级分层

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using namespace cv;

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

    Mat new_image = Mat::zeros(image.size(), image.type());
    int left = 1.0, right = 1.0, r_max = 255; /*< Simple contrast control */

    cout << " Intensity Level Slicing " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the left value [0-255]: "; cin >> left;
    cout << "* Enter the right value [0-255]: ";    cin >> right;
    cout << "* Enter the max value [0-255]: ";    cin >> r_max;

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            for (int c = 0; c < image.channels(); ++c) {
                if (image.at<Vec3b>(y, x)[c] >= left && image.at<Vec3b>(y, x)[c] <= right)
                {
                    new_image.at<Vec3b>(y, x)[c] = r_max;
                }
                else {
                    new_image.at<Vec3b>(y, x)[c] = image.at<Vec3b>(y, x)[c];
                }
            }
        }
    }

    imshow("Original Image", image);
    imshow("New Image", new_image);

    waitKey();
    return 0;
}


