#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <fstream>
#include "Mysparse.h"
using namespace cv;
extern Mat GaussSeidel(mysparse sp, Mat b);
extern Mat Conjugate(mysparse sp, Mat b);
int main(int argc,char* argv[])
{
    //mysparse m(4,4);
////    vector<int> row, col, val;
////    row.push_back(0);
////    col.push_back(1);
////    val.push_back(2);
////    row.push_back(1);
////    col.push_back(2);
////    val.push_back(3);
////    m.initializeFromVector(row,col,val);
////    m.print();
////    for(int i=0;i<4;i++){
////        for(int j=0;j<4;j++){
////            std::cout<<m.at(1,2)<<std::endl;
////        }
////    }
////    m.insert(1,0,0);
////    m.print();
    //std::cout<<m.at(0,0)<<std::endl;
    vector<int> col, row, val;
    row.push_back(0);
    col.push_back(0);
    val.push_back(10);

    row.push_back(0);
    col.push_back(1);
    val.push_back(-1);

    row.push_back(0);
    col.push_back(2);
    val.push_back(2);

    row.push_back(1);
    col.push_back(0);
    val.push_back(-1);

    row.push_back(1);
    col.push_back(1);
    val.push_back(11);

    row.push_back(1);
    col.push_back(2);
    val.push_back(-1);

    row.push_back(1);
    col.push_back(3);
    val.push_back(3);

    row.push_back(2);
    col.push_back(0);
    val.push_back(2);

    row.push_back(2);
    col.push_back(1);
    val.push_back(-1);

    row.push_back(2);
    col.push_back(2);
    val.push_back(10);
    row.push_back(2);
    col.push_back(3);
    val.push_back(-1);

    row.push_back(3);
    col.push_back(1);
    val.push_back(3);

    row.push_back(3);
    col.push_back(2);
    val.push_back(-1);

    row.push_back(3);
    col.push_back(3);
    val.push_back(8);

    mysparse a(4,4);
    a.initializeFromVector(row, col, val);

    cv::Mat b(1, 4, CV_32FC1);
    b.at<float>(0,0) = 6;
    b.at<float>(0,1) = 25;
    b.at<float>(0,2) = -11;
    b.at<float>(0,3) = 15;
    std::cout<<"GaussSeide:"<<std::endl;
    //cv::Mat ans =  GaussSeidel(a, b);
    //std::cout<<"GaussSeide:"<<std::endl;
    cv::Mat b2(4, 1, CV_32FC1);
    b2.at<float>(0,0) = 6;
    b2.at<float>(1,0) = 25;
    b2.at<float>(2,0) = -11;
    b2.at<float>(3,0) = 15;

    cv::Mat ans = Conjugate(a,b2);
    std::cout<<"final ans:"<<ans<<std::endl;

    return 0;
}

