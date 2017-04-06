#include <iostream>
#include<cstdlib>
#include<cstring>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include "Mysparse.h"
//快排
inline void swap(int &a, int &b)
{ int t = a; a = b; b = t; }
int partition(vector<int>& a, const vector<int>&cols, int p, int r) {
    //对 a[p, r) 原址重排
    int x = cols[a[r-1]];//a[r] ==> a[r-1]
    int i = p - 1;
    for (int j = p; j < r - 1; ++j)
        if (cols[a[j]] <= x) {
            ++i;
            swap(a[i], a[j]);
        }
    swap(a[i+1], a[r-1]);
    return i + 1;
}
void quicksort(vector<int>& a,const vector<int>& cols, int p, int r) {
    //调用quicksort(a, 0, n),不是(a, 0, n-1)，区间a[0, n)
    if (p < r - 1) {
        //(p < r) ==>  (p < r - 1)
        int q = partition(a, cols, p, r);
        quicksort(a, cols, p, q);
        //(a, p, q-1)  ==> (a, p, q)
        quicksort(a, cols, q+1, r);
    }
}
//初始化稀疏矩阵
void mysparse::initializeFromVector(const vector<int> rows, const vector<int> cols, const vector<int> vals)
{
    vector<int> Index[this->rows];
    int Index_count[this->rows];
    memset(Index_count,0,this->rows*sizeof(int));
    //桶排序
    for(int i=0;i<rows.size();i++){
        Index[rows[i]].push_back(i);
        Index_count[rows[i]]++;
    }
    //桶内排序
    for(int i=0;i< this->rows;i++){
        if(Index_count[i]==0) continue;
        quicksort(Index[i], cols, 0, Index_count[i]);
    }
    bool isfirst;//是否该行第一个
    int count = 0;//记录在col的index

    for(int i=0;i< this->rows;i++){
        //空行加入-1,
        if(Index[i].size()==0){
             row_ptr.push_back(-1);
            continue;
        }
        //不是空行
        //对于每个桶/row
        isfirst = true;
        vector<int>::iterator it = Index[i].begin();
        for(;it!=Index[i].end();it++){
             val.push_back(vals[*it]);
             col_ind.push_back(cols[*it]);
            if(isfirst){
                row_ptr.push_back(count);
                isfirst = false;
            }
            count++;
        }
    }
}
float mysparse::at(int row, int col) const
{
    if(col> cols-1 || row> rows-1){
        cout<<"col > cols or row> rows in function at";
        return 0;
    }
    if(row >=  row_ptr.size()||row_ptr[row]==-1)
        return 0;
    int fircol_ptr =  row_ptr[row];//取出该行第一个col的下标
    //算出这一行有多少元素
    int col_num = 0;
    //如果是最后一行
    if(row ==  row_ptr.size()-1)
        col_num =  col_ind.size()-fircol_ptr;

    else if(row_ptr[row+1]==-1){
            //找到第一个非0行
        int first_not_zero_col;
        bool isfirst = true;
        for(int j=row;j<rows;j++){
             if( row_ptr[row+1]!=-1 && isfirst){
                 first_not_zero_col =  row_ptr[row+1];
                 isfirst = false;
                 break;
             }
         }
        if(!isfirst) col_num = first_not_zero_col - fircol_ptr;
        else col_num = col_ind.size()-fircol_ptr;
    }

    //如果不是最后一行
    else
        col_num =  row_ptr[row+1] - fircol_ptr;
    for(int i=0;i<col_num;i++){
        if( col_ind[fircol_ptr] == col)
            return  val[fircol_ptr];
        fircol_ptr++;
    }
    return 0;
}
void mysparse::insert(float val, int row, int col)
{
    //用户傻逼
    if(col> cols-1 || row> rows-1){
        cout<<"col > cols or row> rows in function insert";
        return;
    }

    int fircol_ptr =  row_ptr[row];//取出该行第一个col的下标
    //算出这一行有多少元素
    int col_num = 0;
    //如果是最后一行
    if(row ==  row_ptr.size()-1)
        col_num =  col_ind.size()-fircol_ptr;

    else if(row_ptr[row+1]==-1){
            //找到第一个非0行
        int first_not_zero_col;
        bool isfirst = true;
        for(int j=row;j<rows;j++){
             if( row_ptr[row+1]!=-1 && isfirst){
                 first_not_zero_col =  row_ptr[row+1];
                 isfirst = false;
                 break;
             }
         }
        if(!isfirst) col_num = first_not_zero_col - fircol_ptr;
        else col_num = col_ind.size()-fircol_ptr;
    }

    //如果不是最后一行
    else
        col_num =  row_ptr[row+1] - fircol_ptr;
    for(int i=0;i<col_num;i++){
        if(fircol_ptr == -1){
           row_ptr[row]= col;
           //找到第一个非0行
           int first_not_zero_col;
           bool isfirst = true;
           for(int j=row;j<rows;j++){
                if( row_ptr[row+1]!=-1 && isfirst){
                    first_not_zero_col =  row_ptr[row+1];
                    row_ptr[row+1]++;//整体后移
                    isfirst = false;
                }
                else if(row_ptr[row+1] != -1)
                    row_ptr[row+1]++;
            }
            col_ind.insert(col_ind.begin()+first_not_zero_col,col);
            this->val.insert(this->val.begin()+first_not_zero_col,val);
            return;

        }
        else if( col_ind[fircol_ptr]>col){
             col_ind.insert( col_ind.begin()+ fircol_ptr, col);
             this->val.insert( this->val.begin()+fircol_ptr, val);
             //整体后移
             for(int j=row;j<rows;j++){
                 if(row_ptr[j+1] != -1)
                      row_ptr[j+1]++;
              }
             return;
        }
    }
}
//debug用，印出三个成员的内容
void mysparse::print()
{
    std::cout<<"val:"<<std::endl;
    for(int i=0;i<val.size();i++){
        std::cout<<val[i]<<' ';
    }
    std::cout<<std::endl;
    std::cout<<"col_ind:"<<std::endl;
    for(int i=0;i<col_ind.size();i++){
        std::cout<<col_ind[i]<<' ';
    }
    std::cout<<std::endl;
    std::cout<<"row_ptr:"<<std::endl;
    for(int i=0;i<row_ptr.size();i++){
        std::cout<<row_ptr[i]<<' ';
    }
    std::cout<<std::endl;
}
//与mat相乘
cv::Mat operator * (const mysparse& sp, const cv::Mat& m)
{
    cv::Mat ans = cv::Mat::zeros(sp.rows, m.cols, CV_32FC1);
    for(int i=0; i<ans.rows;i++){
        for(int j = 0;j<ans.cols;j++){
            //行×列
            for(int k = 0;k<sp.cols;k++)
                ans.at<float>(i,j) += sp.at(i,k) * m.at<float>(k,j);
        }
    }
    return ans;
}
cv::Mat operator*(const cv::Mat& m, const mysparse& sp)
{
    cv::Mat ans = cv::Mat::zeros(m.rows, sp.cols, CV_32FC1);
    for(int i=0; i<ans.rows;i++){
        for(int j = 0;j<ans.cols;j++){
            //行×列
            for(int k = 0;k<m.cols;k++)
                ans.at<float>(i,j) += m.at<float>(i,k) * sp.at(k,j);
        }
    }
    return ans;
}
mysparse operator*(const float& m, const mysparse& sp)
{
    mysparse ans(sp.rows,sp.cols);

    for(int i=0;i<sp.val.size();i++){
        ans.val.push_back(sp.val[i] * m);
        ans.col_ind.push_back(sp.col_ind[i]);
    }
    for(int i=0;i<sp.row_ptr.size();i++){
        ans.row_ptr.push_back(sp.row_ptr[i]);
    }

    return ans;
}
mysparse operator*(const mysparse& sp, const float& m)
{
    mysparse ans(sp.rows,sp.cols);

    for(int i=0;i<sp.val.size();i++){
        ans.val.push_back(sp.val[i] * m);
        ans.col_ind.push_back(sp.col_ind[i]);
    }
    for(int i=0;i<sp.row_ptr.size();i++){
        ans.row_ptr.push_back(sp.row_ptr[i]);
    }

    return ans;
}
