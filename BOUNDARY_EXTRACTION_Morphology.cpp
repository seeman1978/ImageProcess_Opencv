//
// Created by zhaoyue on 2020/1/12.
//

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// 全局变量
Mat src, erosion_dst;

int erosion_elem = 0;
int erosion_size = 0;

int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
void Erosion( int, void* );

/** @function main */
int main( int argc, char** argv )
{
    /// Load 图像
    src = imread( argv[1] );

    if( !src.data )
    {
        return -1;
    }

    /// 创建显示窗口
    namedWindow( "Erosion Demo", WINDOW_AUTOSIZE );

    /// 创建腐蚀 Trackbar
    createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
                    &erosion_elem, max_elem,
                    Erosion );

    createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
                    &erosion_size, max_kernel_size,
                    Erosion );



    /// Default start
    Erosion( 0, 0 );

    waitKey(0);
    return 0;
}

/**  @function Erosion  */
void Erosion( int, void* )
{
    int erosion_type;
    if( erosion_elem == 0 )
    {
        erosion_type = MORPH_RECT;
    }
    else if( erosion_elem == 1 )
    {
        erosion_type = MORPH_CROSS;
    }
    else if( erosion_elem == 2)
    {
        erosion_type = MORPH_ELLIPSE;
    }

    Mat element = getStructuringElement( erosion_type,
                                         Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                         Point( erosion_size, erosion_size ) );

    /// 腐蚀操作
    erode( src, erosion_dst, element );
    Mat matBoundary = src - erosion_dst;
    imshow( "Erosion Demo", matBoundary );
}