//
// Created by 王强 on 2020/3/10.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace cv;

/*
对输入图像作移动平均分割
src为单通道灰度图像
num表示计算平均的点数
*/
Mat MovingThresh(const Mat* src, int n, float b)
{
    int w = src->cols, h = src->rows;

    std::vector<uchar> data(src->rows * src->cols);
    if (src->isContinuous()){
        data.assign(src->datastart, src->dataend);
    }

    float m_pre = data[0]/n, m_now;

    std::vector<uchar> dstdata(src->rows * src->cols);

    for(int i=0; i<h; i++)
    {
        for(int j=0; j<w; j++)
        {
            int index = i + j;
            float dif;
            if(index < n+1){
                dif = data[index];
            }
            else{
                dif = data[index]-data[index-n-1];
            }


            dif *= 1/n;
            m_now = m_pre + dif;
            m_pre = m_now;

            if(data[index] > cvRound(b * m_now)){
                dstdata[index] = 255;
            }
            else{
                dstdata[index] = 0;
            }
        }
    }
    return Mat(h, w, src->type(), dstdata.data());
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
    int nChannels = image.channels();

    std::vector<cv::Mat> SrcMatpart(nChannels);      //生成与通道数数目相等的图像容器

    cv::split(image, SrcMatpart);                    //分解与通道数数目相等的图像容器

    namedWindow("Original Image", 0);
    namedWindow("Dest Image", 0);

    Mat dst;
    dst = MovingThresh(&SrcMatpart[0], 20, 0.5);

    imshow("Original Image", image);
    imshow("Dest Image", dst);

    waitKey();
    return 0;
}