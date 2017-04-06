#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
#ifndef MYSPARSE_H
#define MYSPARSE_H
//稀疏矩阵
class mysparse
{
public:
    mysparse(int a, int b):rows(a),cols(b){}//构造函数
    void initializeFromVector(const vector<int> rows, const vector<int> cols,const vector<int> vals);//初始化
    float at(int row, int col) const;//at
    void insert(float val, int row, int col);// 将val插入到（row, col）这个位置去
    void print();
    int rows, cols;
    friend mysparse operator*(const float& m, const mysparse& sp);
    friend mysparse operator*(const mysparse& sp, const float& m);
private:
    vector<float> val;
    vector<int> col_ind, row_ptr;
};
//重载乘法
 cv::Mat operator*(const mysparse& sp, const cv::Mat& m);
cv::Mat operator*(const cv::Mat& m, const mysparse& sp);


#endif
