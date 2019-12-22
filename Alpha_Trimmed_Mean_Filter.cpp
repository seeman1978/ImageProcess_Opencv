//
// Created by zhaoyue on 2019/12/21.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace cv;

double filter_alpha(const Mat& src, int d)
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

    std::sort(V.begin(), V.end());
    int nCount = d/2;
    std::vector<uchar> V2;
    std::copy(V.begin()+nCount, V.end()-nCount, std::back_inserter(V2)); //全部拷贝

    return (double)std::accumulate(V2.begin(), V2.end(), 0.0) / V2.size();
}

void AlphaTrimMeanFilter(const Mat& input, const Size kernalSize, int d, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernalSize.height-1)/2;
    int w = (kernalSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(filter_alpha(rgbChannels[ii](Rect(j-w, i-l, kernalSize.width, kernalSize.height)), d));
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
    namedWindow("Alpha-Trimmed Mean Filter", 0);
    namedWindow("Median Blur", 0);

    Mat MeanImg = Mat::zeros(image.size(), image.type());   //修正的阿尔法均值滤波后的图像
    AlphaTrimMeanFilter(image, Size(5, 5), 6, MeanImg);

    Mat medianBlurImg;  //中值滤波
    medianBlur(image, medianBlurImg, 5);

    imshow("Gaussian Noise", image);
    imshow("Alpha-Trimmed Mean Filter", MeanImg);
    imshow("Median Blur", medianBlurImg);

    waitKey();
    return 0;
}