//
// Created by zhaoyue on 2019/12/26.
//

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat image_add_border( Mat &src )
{
    //int w=2*src.cols;
    //int h=2*src.rows;

    int h = getOptimalDFTSize(src.rows);
    int w = getOptimalDFTSize(src.cols);

    std::cout << "src: " << src.cols << "*" << src.rows << std::endl;

    cv::Mat padded;
    copyMakeBorder( src, padded, 0, h-src.rows, 0, w-src.cols,
                    cv::BORDER_CONSTANT, cv::Scalar::all(0));
    padded.convertTo(padded,CV_32FC1);
    std::cout << "opt: " << padded.cols << "*" << padded.rows << std::endl;
    return padded;
}

//transform to center 中心化
void center_transform( cv::Mat &src )
{
    for(int i=0; i<src.rows; i++){
        float *p = src.ptr<float>(i);
        for(int j=0; j<src.cols; j++){
            p[j] = p[j] * pow(-1, i+j);
        }
    }
}

//对角线交换内容
void zero_to_center(cv::Mat &freq_plane)
{
//    freq_plane = freq_plane(Rect(0, 0, freq_plane.cols & -2, freq_plane.rows & -2));
    //这里为什么&上-2具体查看opencv文档
    //其实是为了把行和列变成偶数 -2的二进制是11111111.......10 最后一位是0
    int cx=freq_plane.cols/2;int cy=freq_plane.rows/2;//以下的操作是移动图像  (零频移到中心)
    cv::Mat part1_r(freq_plane, cv::Rect(0,0,cx,cy)); //元素坐标表示为(cx,cy)
    cv::Mat part2_r(freq_plane, cv::Rect(cx,0,cx,cy));
    cv::Mat part3_r(freq_plane, cv::Rect(0,cy,cx,cy));
    cv::Mat part4_r(freq_plane, cv::Rect(cx,cy,cx,cy));

    cv::Mat tmp;
    part1_r.copyTo(tmp); //左上与右下交换位置(实部)
    part4_r.copyTo(part1_r);
    tmp.copyTo(part4_r);

    part2_r.copyTo(tmp); //右上与左下交换位置(实部)
    part3_r.copyTo(part2_r);
    tmp.copyTo(part3_r);
}


void show_spectrum( cv::Mat &complexI )
{
    cv::Mat temp[] = {cv::Mat::zeros(complexI.size(),CV_32FC1),
                      cv::Mat::zeros(complexI.size(),CV_32FC1)};
    //显示频谱图
    cv::split(complexI, temp);
    cv::Mat aa;
    cv::magnitude(temp[0], temp[1], aa);
//    zero_to_center(aa);
    cv::divide(aa, aa.cols*aa.rows, aa);
    double min, max;
    cv::Point min_loc, max_loc;
    cv::minMaxLoc(aa, &min, &max, &min_loc, &max_loc );
    std::cout << "min: " << min << " max: " << max << std::endl;
    std::cout << "min_loc: " << min_loc << " max_loc: " << max_loc << std::endl;
    cv::circle( aa, min_loc, 20, cv::Scalar::all(1), 3);
    cv::circle( aa, max_loc, 20, cv::Scalar::all(1), 3);

    cv::imshow("src_img_spectrum",aa);
}

//频率域滤波
cv::Mat frequency_filter(cv::Mat &padded,cv::Mat &blur)
{
    cv::Mat plane[]={padded, cv::Mat::zeros(padded.size(), CV_32FC1)};
    cv::Mat complexIm;

    cv::merge(plane,2,complexIm);
    cv::dft(complexIm,complexIm);//fourior transform
    show_spectrum(complexIm);

    cv::multiply(complexIm, blur, complexIm);
    cv::idft(complexIm, complexIm, DFT_INVERSE);     ///idft
    cv::Mat dst_plane[2];
    cv::split(complexIm, dst_plane);
    center_transform(dst_plane[0]);
//    center_transform(dst_plane[1]);

    cv::magnitude(dst_plane[0],dst_plane[1],dst_plane[0]); ///求幅值(模)
//    center_transform(dst_plane[0]);        //center transform

    return dst_plane[0];
}

//陷波滤波器
cv::Mat notch_kernel( cv::Mat &scr, std::vector<cv::Point> &notch_pot, float D0 )
{
    cv::Mat notch_pass(scr.size(),CV_32FC2);
    int row_num = scr.rows;
    int col_num = scr.cols;
    int n = 4;
    for(int i=0; i<row_num; i++ ){
        float *p = notch_pass.ptr<float>(i);
        for(int j=0; j<col_num; j++ ){
            float h_nr = 1.0;
            for( unsigned int k = 0; k < notch_pot.size(); k++ ){
                int u_k = notch_pot.at(k).y;
                int v_k = notch_pot.at(k).x;
                float dk = sqrt( pow((i-u_k),2) + pow((j-v_k),2) );
                float d_k = sqrt( pow((i+u_k),2) + pow((j+v_k),2) );
                float dst_dk = 1.0/(1.0 + pow(D0/dk, 2*n));
                float dst_d_k = 1.0/(1.0 + pow(D0/d_k, 2*n));
                h_nr = dst_dk * dst_d_k * h_nr;
//                std::cout <<  "notch_pot: " << notch_pot.at(k) << std::endl;
            }
            p[2*j] = h_nr;
            p[2*j+1] = h_nr;

        }
    }

    cv::Mat temp[] = { cv::Mat::zeros(scr.size(), CV_32FC1),
                       cv::Mat::zeros(scr.size(), CV_32FC1) };
    cv::split(notch_pass, temp);

    std::string name = "notch滤波器d0=" + std::to_string(D0);
    cv::Mat show;
    cv::normalize(temp[0], show, 1, 0, NORM_MINMAX);
    namedWindow(name, 0);
    cv::imshow(name, show);
    return notch_pass;
}

std::string name_win("Notch_filter");
cv::Rect g_rectangle;
bool g_bDrawingBox = false;//是否进行绘制
cv::RNG g_rng(12345);

std::vector<cv::Point> notch_point;

void on_MouseHandle(int event, int x, int y, int flags, void*param);
void DrawRectangle(cv::Mat& img, cv::Rect box);

int main(int argc, char * argv[])
{
    if( argc != 2){
        std::cerr << "Usage: " << argv[0] << " <img_name> " << std::endl;
        return -1;
    }

    Mat srcImage = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if( srcImage.empty() )
        return -1;
    namedWindow("src_img", 0);
    imshow( "src_img", srcImage );
    Mat padded = image_add_border(srcImage);
    center_transform( padded );
    Mat plane[]={padded, cv::Mat::zeros(padded.size(), CV_32FC1)};
    Mat complexIm;

    merge(plane, 2, complexIm);
    cv::dft(complexIm, complexIm);//fourior transform
    Mat temp[] = {cv::Mat::zeros(complexIm.size(),CV_32FC1),
                  cv::Mat::zeros(complexIm.size(),CV_32FC1)};
    //显示频谱图
    split(complexIm, temp);
    Mat aa;
    magnitude(temp[0], temp[1], aa);
    divide(aa, aa.cols*aa.rows, aa);

    cv::namedWindow(name_win, 0);
    cv::imshow(name_win,aa);

    cv::setMouseCallback(name_win, on_MouseHandle, (void*)&aa);
    Mat tempImage = aa.clone();
    int key_value = -1;
    while (1){
        key_value = cv::waitKey(10);
        if( key_value == 27 ){//esc key,break
            break;
        }

    }
    if( !notch_point.empty() ){
        for( unsigned int i = 0; i < notch_point.size(); i++ ){
            cv::circle( tempImage, notch_point.at(i), 3, cv::Scalar(1), 2);
            std::cout << notch_point.at(i) << std::endl;
        }
    }
    cv::imshow(name_win, tempImage);

    Mat ker = notch_kernel( complexIm,notch_point, 30 );
    cv::multiply(complexIm, ker, complexIm);

    split(complexIm, temp);
    magnitude(temp[0], temp[1], aa);
    divide(aa, aa.cols*aa.rows, aa);
    namedWindow("aa", 0);
    imshow( "aa", aa );
    cv::idft(complexIm, complexIm, DFT_INVERSE);     //idft
    cv::Mat dst_plane[2];
    cv::split(complexIm, dst_plane);
    center_transform(dst_plane[0]);
//  center_transform(dst_plane[1]);

//  cv::magnitude(dst_plane[0],dst_plane[1],dst_plane[0]);  //求幅值(模)

    cv::normalize(dst_plane[0], dst_plane[0], 1, 0, NORM_MINMAX);

    /// 裁剪掉填充的部分
    Mat imgOutput = dst_plane[0];
    imgOutput = imgOutput( cv::Rect(0, 0, srcImage.cols, srcImage.rows) );

    namedWindow("dest", 0);
    imshow("dest", imgOutput);
    cv::waitKey(0);

    return 1;
}


void on_MouseHandle(int event, int x, int y, int falgs, void* param)
{
    Mat& image = *(cv::Mat*)param;

    switch (event){                 //鼠标移动消息
        case cv::EVENT_MOUSEMOVE:{
            if (g_bDrawingBox){         //标识符为真，则记录下长和宽到Rect型变量中

                g_rectangle.width = x - g_rectangle.x;
                g_rectangle.height = y - g_rectangle.y;
            }
        }
            break;

        case cv::EVENT_LBUTTONDOWN:{
            g_bDrawingBox = true;
            g_rectangle = cv::Rect(x, y, 0, 0);//记录起点
            std::cout << "start point( " << g_rectangle.x << "," << g_rectangle.y << ")" << std::endl;
        }
            break;

        case cv::EVENT_LBUTTONUP: {
            g_bDrawingBox = false;
            bool w_less_0 = false, h_less_0 = false;

            if (g_rectangle.width < 0){     //对宽高小于0的处理
                g_rectangle.x += g_rectangle.width;
                g_rectangle.width *= -1;
                w_less_0 = true;

            }
            if (g_rectangle.height < 0){
                g_rectangle.y += g_rectangle.height;
                g_rectangle.height *= -1;
                h_less_0 = true;
            }
            std::cout << "end Rect[ " << g_rectangle.x << "," << g_rectangle.y << "," << g_rectangle.width<< ","
                      << g_rectangle.height << "]" <<std::endl;

            if( g_rectangle.height > 0 && g_rectangle.width > 0 ){
                Mat imageROI = image(g_rectangle).clone();
                double min, max;
                cv::Point min_loc, max_loc;
                cv::minMaxLoc( imageROI, &min, &max, &min_loc, &max_loc);
                cv::circle(imageROI, max_loc, 10, 1);
                max_loc.x += g_rectangle.x;
                max_loc.y += g_rectangle.y;

                notch_point.push_back(max_loc);

                cv::circle(image, max_loc, 10, 1);
//            cv::imshow( "ROI", imageROI );
                namedWindow("src", 0);
                cv::imshow( "src", image );
            }

        }
            break;
    }
}

cv::Mat notchFilter_BTW(int rows,int cols,std::vector<cv::Point> np,
                        float* D,int n=1,int cvtype=CV_32FC2)
{
    cv::Mat filt(rows,cols,cvtype,cv::Scalar::all(0));
    int cx=cols/2,cy=rows/2;
    int numNotch=np.size();
    float* D2=D;
    for(int i=0;i<numNotch;i++)
    {
        D2[i]=D[i]*D[i];
    }
    int uk[numNotch],vk[numNotch];//在画面上的实际陷波坐标点
    int u_k[numNotch],v_k[numNotch];//陷波共轭点
    float Dk[numNotch],D_k[numNotch];//陷波半径r
    float Hk[numNotch],H_k[numNotch];

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            int u=cx-j,v=cy-i;//中心坐标
            for(int s=0;s<numNotch;s++)
            {
                uk[s]=u+np[s].x,vk[s]=v+np[s].y;
                Dk[s]=uk[s]*uk[s]+vk[s]*vk[s];//距离中心半径的平方
                Hk[s]=1-1/(1+pow(Dk[s]/D2[s],n));

                u_k[s]=u-np[s].x,v_k[s]=v-np[s].y;
                D_k[s]=u_k[s]*u_k[s]+v_k[s]*v_k[s];
                H_k[s]=1-1/(1+pow(D_k[s]/D2[s],n));
            }
            //.data返回的是uchar*型指针，所以要强制转换成浮点数型
            float* p=(float*)(filt.data+i*filt.step[0]+j*filt.step[1]);

            for(int c=0;c<filt.channels();c++)
            {
                p[c]=Hk[0]*H_k[0];
                for(int k=1;k<numNotch;k++)
                {
                    p[c]*=Hk[k]*H_k[k];
                }
            }

        }
    }
    return filt;
}
