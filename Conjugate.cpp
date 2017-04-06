#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Mysparse.h"
//共軛梯度法
cv::Mat Conjugate(mysparse sp, cv::Mat b)
{
   const int n = b.rows;
   cv::Mat pre(n, 1 , CV_32FC1),
           cur(n, 1 , CV_32FC1),
           curr, p, a_mat, prer, p_t, prer_t,  curr_t, beta_mat;
   float a, beta;
   bool isfirst = true;
   for(int i=b.rows-1;i>=0;i--){
       cur.at<float>(cur.rows-i-1, 0) = b.at<float>(i,0);
   }
//   std::cout<<cur<<std::endl;
   curr = (b - sp * cur);
   curr.copyTo(p);
   cv::Mat balala = cv::Mat::zeros(n,1,CV_32FC1);
   do{
       //记录上一次结果和上一次残差
       cur.copyTo(pre);
       curr.copyTo(prer);
       //转置矩阵用于相乘
       cv::transpose(prer,prer_t);
       cv::transpose(p, p_t);
       //公式计算
       cv::Mat a_mat1 = prer_t*prer, a_mat2 = p_t*sp*p;
       //a_mat = prer_t*prer./(p_t*sp*p);
       std::cout<<"prer:"<<prer<<std::endl;
       std::cout<<"p:"<<p<<std::endl;
       //a = a_mat.at<float>(0,0);
       a = a_mat1.at<float>(0,0)/a_mat2.at<float>(0,0);
       std::cout<<"a:"<<a<<std::endl;
       cur = pre + a*p;
       std::cout<<"prer:"<<prer<<std::endl;
       std::cout<<"cur:"<<cur<<std::endl;
       curr = prer - a*sp*p;
       //收敛就停
       if(!isfirst && norm(curr,balala)<1e-5)
           break;
       cv::transpose(curr, curr_t);
       beta_mat = curr_t * curr/(prer_t*prer);
       beta = beta_mat.at<float>(0,0);
       p = curr + beta*p;
       isfirst = false;
    }while(1);
   cur.at<float>(0,0)*=2;
   return cur;//返回结果
}


