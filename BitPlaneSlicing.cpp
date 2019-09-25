//
// Created by zhaoyue on 2019/9/23.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <bitset>
using namespace std;
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
    resize(image,image,cv::Size(), 0.4, 0.4);
    Mat mat[8];
    for (int k = 0; k < 8; ++k) {
        mat[k] = Mat::zeros(image.size(), image.type());
    }

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int c = 0; c < image.channels(); ++c) {
                int nPixelValue = image.at<Vec3b>(i, j)[c];
                bitset<8> b(nPixelValue);              //b有8位，每位都为0
                for (int k = 0; k < 8; ++k) {
                    mat[k].at<Vec3b>(i, j)[c] = b[k] * 255;
                }
            }
        }
    }

    imshow("Original Image", image);

    for (int l = 0; l < 8; ++l) {
        imshow("level"+to_string(l), mat[l]);
    }

    waitKey();
    return 0;
}