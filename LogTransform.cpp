//
// Created by zhaoyue on 2019/9/21.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using namespace cv;

int main(int argc, char** argv) {
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat image = imread(parser.get<String>("@input"));
    if (image.empty()) {
        cout << "Could not open or find the image!\n" << endl;
        cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }

    Mat new_image = Mat::zeros(image.size(), image.type());
    image.convertTo(new_image, CV_32F);

    std::cout<<new_image<<std::endl;

    int c = 0;   /*< constant */

    cout << " Log Transforms " << endl;
    cout << "-------------------------" << endl;
    cout << "* Enter the constant value [0-100]: ";
    cin >> c;

    new_image.convertTo(new_image, -1, 1, 1);   //new_image = new_image + 1;

    std::cout<<new_image<<std::endl;
    log(new_image, new_image);

    std::cout<<new_image<<std::endl;

    new_image.convertTo(new_image, -1, c, 0);   //new_image = c * new_image;

    normalize(new_image, new_image, 0, 255, NORM_MINMAX);

    std::cout<<new_image<<std::endl;

    convertScaleAbs(new_image, new_image);

    imshow("Original Image", image);
    imshow("resultImage", new_image);
    waitKey();
    return 0;
}