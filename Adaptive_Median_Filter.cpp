//
// Created by zhaoyue on 2019/12/22.
//

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

void MatToVector(const Mat& src, std::vector<uchar>& V)
{
    if (src.isContinuous()) {
        // array.assign(mat.datastart, mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
        V.assign(src.data, src.data + src.total());
    } else {
        for (int i = 0; i < src.rows; ++i) {
            V.insert(V.end(), src.ptr<uchar>(i), src.ptr<uchar>(i)+src.cols);
        }
    }
}

/// g:坐标（x,y)处的灰度值
/// nMinSize: 最小窗口尺寸
/// kernalSize：最大核的尺寸
double Adaptive(double g, int nMinSize, const Size& kernelSize, const Mat& input)
{
    double Zxy = g;
    int l = (kernelSize.height-1)/2;
    int w = (kernelSize.width-1)/2;
    int i = 0, j = 0;
    Mat matTemp;
    do
    {
        ++i, ++j;
        matTemp = input(Rect(w-j, l-i, nMinSize, nMinSize));
        std::vector<uchar> V;
        MatToVector(matTemp, V);
        int Zmin = *std::min_element(V.begin(), V.end()); /// Sxy中的最小灰度值
        int Zmax = *std::max_element(V.begin(), V.end()); /// Sxy中的最大灰度值
        std::sort(V.begin(), V.end());
        int nIndex = (V.size()-1)/2;
        int Zmed = V[nIndex];        /// Sxy中的中值

        int A1 = Zmed - Zmin;
        int A2 = Zmed - Zmax;
        if (A1 > 0 && A2 < 0){
            int B1 = Zxy - Zmin;
            int B2 = Zxy - Zmax;
            if (B1>0 && B2<0){
                return Zxy;
            }
            else{
                return Zmed;
            }
        } else{
            nMinSize += 2;     ///增大窗口尺寸
            if(nMinSize <= kernelSize.height){
                continue;
            }
            else{
                return Zmed;
            }
        }
    }while(true);
}

void AdaptiveMedianFilter(const Mat& input, const Size& kernelSize, int nMinSize, Mat& output)
{
    std::vector<cv::Mat> rgbChannels(input.channels());
    split(input, rgbChannels);

    int l = (kernelSize.height-1)/2;
    int w = (kernelSize.width-1)/2;
    for (int i = l; i < input.rows-l; ++i){
        for (int j =w; j < input.cols-w; ++j){
            for (int ii =0;ii < input.channels(); ++ii){
                output.at<Vec3b>(i,j)[ii] = saturate_cast<uchar>(Adaptive(input.at<Vec3b>(i, j)[ii], nMinSize, kernelSize, rgbChannels[ii](Rect(j-w, i-l, kernelSize.width, kernelSize.height))));
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

    namedWindow("PepperSalt Noise", 0);
    namedWindow("Median Filter", 0);
    namedWindow("Adaptive Median Blur", 0);

    Mat MedianImg = Mat::zeros(image.size(), image.type());   //自适应中值滤波后的图像
    AdaptiveMedianFilter(image, Size(7, 7), 3, MedianImg);

    Mat medianBlurImg;
    medianBlur(image, medianBlurImg, 7);

    imshow("PepperSalt Noise", image);
    imshow("Median Filter", medianBlurImg);
    imshow("Adaptive Median Blur", MedianImg);

    waitKey();
    return 0;
}