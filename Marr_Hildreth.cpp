//
// Created by 王强 on 2020/2/18.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void marrEdge(const Mat src, Mat& result, Mat& laplacian, int kerValue, double delta)
{
    //计算LoG算子
    Mat kernel;
    //半径
    int kerLen = kerValue / 2;
    kernel = Mat_<double>(kerValue, kerValue);
    //滑窗
    for (int i = -kerLen; i <= kerLen; i++)
    {
        for (int j = -kerLen; j <= kerLen; j++)
        {
            //生成核因子
            kernel.at<double>(i + kerLen, j + kerLen) = exp(-((pow(j, 2) + pow(i, 2)) / (pow(delta, 2) * 2)))
                                                        *((pow(j, 2) + pow(i, 2) - 2 * pow(delta, 2)) / (2 * pow(delta, 4)));
        }
    }
    //设置输入参数
    int kerOffset = kerValue / 2;
    laplacian = (Mat_<double>(src.rows - kerOffset * 2, src.cols - kerOffset * 2));
    result = Mat::zeros(src.rows - kerOffset * 2, src.cols - kerOffset * 2, src.type());
    double sumLaplacian;
    //遍历计算卷积图像的拉普拉斯算子
    for (int i = kerOffset; i < src.rows - kerOffset; ++i)
    {
        for (int j = kerOffset; j < src.cols - kerOffset; ++j)
        {
            sumLaplacian = 0;
            for (int k = -kerOffset; k <= kerOffset; ++k)
            {
                for (int m = -kerOffset; m <= kerOffset; ++m)
                {
                    //计算图像卷积
                    sumLaplacian += src.at<uchar>(i + k, j + m)*kernel.at<double>(kerOffset + k, kerOffset + m);
                }
            }
            //生成拉普拉斯结果
            laplacian.at<double>(i - kerOffset, j - kerOffset) = sumLaplacian;
        }
    }
    for (int y = 1; y < result.rows - 1; ++y)
    {
        for (int x = 1; x < result.cols-1; ++x)
        {
            result.at<uchar>(y, x) = 0;
            //领域判定
            if (laplacian.at<double>(y - 1, x)*laplacian.at<double>(y + 1, x) < 0)
            {
                result.at<uchar>(y, x) = 255;
            }
            if (laplacian.at<double>(y, x - 1)*laplacian.at<double>(y, x + 1) < 0)
            {
                result.at<uchar>(y, x) = 255;
            }
            if (laplacian.at<double>(y + 1, x - 1)*laplacian.at<double>(y - 1, x + 1) < 0)
            {
                result.at<uchar>(y, x) = 255;
            }
            if (laplacian.at<double>(y - 1, x - 1)*laplacian.at<double>(y + 1, x + 1) < 0)
            {
                result.at<uchar>(y, x) = 255;
            }
        }
    }
}
int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@input | ../data/lena.jpg | input image}");
    Mat srcImage = imread(parser.get<String>("@input"));
    if (srcImage.empty())
    {
        std::cout << "Could not open or find the image!\n" << std::endl;
        std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
        return -1;
    }
    Mat edge, srcGray, laplacian;
    cvtColor(srcImage, srcGray, COLOR_BGR2GRAY);
    marrEdge(srcGray, edge, laplacian, 25, 4);
    imshow("srcImage", srcImage);
    imshow("laplacian Image", laplacian);
    imshow("edge", edge);
    waitKey(0);
    return 0;

}