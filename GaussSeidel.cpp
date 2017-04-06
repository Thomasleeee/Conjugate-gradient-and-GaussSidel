#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Mysparse.h"
//高斯赛达尔迭代
cv::Mat GaussSeidel(mysparse sp, cv::Mat b)
{
   const int n = b.cols;
   //分别记录当前答案、前一个答案
   cv::Mat pre = cv::Mat::zeros(1, n, CV_32FC1),
           cur = cv::Mat::zeros(1, n, CV_32FC1);
   bool isfirst = true;
   float *b_ptr, *pre_ptr ,
           *cur_ptr;
   do{
       b_ptr = b.ptr<float>(0);
       cur_ptr = cur.ptr<float>(0);
       pre_ptr = pre.ptr<float>(0);
       if(!isfirst && norm(cur-pre)<10){
           break;
       }
       float dfr;
       cur.copyTo(pre);//复制上一次答案
       //计算当前x
       for(int i=0,k =0;i<n;i++,k++){
           dfr = 0;//要被减的数
           for(int j=0;j<n;j++){
               if(i==j) continue;
               dfr += pre_ptr[j] * sp.at(i,j);
           }
           *cur_ptr = (*b_ptr-dfr)/sp.at(i,k);
           b_ptr++;
           cur_ptr++;
        }
       std::cout<<cur<<std::endl;
       isfirst = false;
    }while(1);
   return cur;
}


