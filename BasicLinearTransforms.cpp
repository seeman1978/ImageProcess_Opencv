//
// Created by wq on 2019/9/4.
//

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
    double alpha = 1.0, gamma_ = 1.0; /*< Simple contrast control */
    int beta = 0;   /*< Simple brightness control */

    cout << " Basic Linear Transforms " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
    cout << "* Enter the beta value [0-100]: ";    cin >> beta;
    cout << "* Enter the gamma value [0-100]: ";   cin >> gamma_;
    /*
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            for (int c = 0; c < image.channels(); ++c) {
                new_image.at<Vec3b>(y, x)[c] =
                        saturate_cast<uchar>(alpha*image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
     */
    image.convertTo(new_image, -1, alpha, beta);    //linear transform

    //gamma correction
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i) {
        p[i] = saturate_cast<uchar>(pow(i/255.0, gamma_)*255.0);
    }
    Mat res = image.clone();
    LUT(image, lookUpTable, res);
    imshow("Original Image", image);
    imshow("New Image", new_image);
    imshow("Gamma New Image", res);

    waitKey();
    return 0;
}
