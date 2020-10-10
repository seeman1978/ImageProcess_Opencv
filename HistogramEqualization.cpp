//
// Created by wq on 2020/10/9.
//

//
// Created by zhaoyue on 2019/10/6.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <iostream>
#include <opencv2/core/opengl.hpp>

using namespace cv;

int  main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat image = imread(parser.get<String>("@input"), CV_8UC1);
    if (image.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    namedWindow("Original", cv::WINDOW_OPENGL);
    namedWindow("equalizeHist", cv::WINDOW_OPENGL);

    cuda::GpuMat g_dst, g_src;
    g_src.upload(image);
    cuda::equalizeHist(g_src, g_dst);

    /*Mat dest;
    g_dst.download(dest);*/

    imshow("Original", g_src);
    imshow("equalizeHist", g_dst);

    waitKey();
    return 0;
}