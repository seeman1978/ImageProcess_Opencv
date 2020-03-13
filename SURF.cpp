//
// Created by 王强 on 2020/3/13.
//

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/xfeatures2d.hpp>

#include <iostream>

using namespace cv;
using namespace std;

static bool abs_compare(int a, int b)
{
    return (std::abs(a) < std::abs(b));
}

int  main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@image1 | ../data/lena.jpg | input image}{@image2 |<none>| image2 for compare  }");
    Mat image1 = imread(parser.get<String>("@image1"));
    if (image1.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    Mat image2 = imread(parser.get<String>("@image2"));
    if (image2.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }

    ///创建SIFT特征检测器
    cv::Ptr<cv::xfeatures2d::SURF> mySURF = cv::xfeatures2d::SURF::create();

    ///特征点提取与描述子生成
    //特征点
    std::vector<cv::KeyPoint> keyPoint1, keyPoint2;
    //特征描述子
    cv::Mat desp1, desp2;

    mySURF->detectAndCompute(image1, cv::Mat(), keyPoint1, desp1);
    mySURF->detectAndCompute(image2, cv::Mat(), keyPoint2, desp2);

    /// 暴力匹配
    cv::Ptr<cv::DescriptorMatcher> myFB = cv::DescriptorMatcher::create("FlannBased");
    std::vector<cv::DMatch> matches;
    myFB->match(desp1, desp2, matches);

    //计算特征点距离的最大值
    std::vector<cv::DMatch>::iterator maxMatch =  std::max_element(matches.begin(), matches.end(), [](const DMatch& a, const DMatch& b) { return a.distance < b.distance ; });
    double dMaxDistance = maxMatch->distance;

    //挑选好的匹配点
    int iCount = 0;
    std::vector< cv::DMatch > good_matches;
    for (auto distantce : matches)
    {
        if (distantce.distance < 0.0001*dMaxDistance)
        {
            good_matches.push_back(distantce);
            if(++iCount == 10)
            {
                break;
            }
        }
    }

    /// 绘制最佳匹配
    cv::Mat imageOutput;
    cv::drawMatches(image1, keyPoint1, image2, keyPoint2, good_matches, imageOutput);

    cv::namedWindow("picture of matching", 0);
    cv::imshow("picture of matching", imageOutput);
    cv::waitKey(0);
    return 0;
}