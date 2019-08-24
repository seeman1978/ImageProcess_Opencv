//
// Created by zhaoyue on 2019/8/24.
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int i = 7;//滑动条初始值
int maxnum = 20;//滑动条最大值
void text(int,void*);//声明回调函数

int main(int argc, char** argv)
{
    Mat src = imread("/Users/zhaoyue/Pictures/truck.png");
//判断图片是否载入成功
    if (src.empty())
    {
        printf("图片加载失败\n");
        system("pause");
        //return -1;
    }
    //新建一个窗口
    namedWindow("test",WINDOW_AUTOSIZE);
    //创建滑动条
    //注意：i是变量，滑动条擦改变后i改变。
    createTrackbar("digit：","test",&i,maxnum,text);

    text(0,0);
    imshow( "test", src );

    waitKey(0);
    return 0;
}
//回调函数
void text(int,void*)
{
    cout<<"数字i的值为:"<<i<<endl;
}


