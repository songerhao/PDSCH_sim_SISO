#ifndef _MAT_OPERATE
#define _MAT_OPERATE
#include "common.h"
/*
 *	这个文件主要是一些对矩阵的常用的操作，比如说slice分割等~~
 */
 cvec slice(cvec input_cvec,int start,int delta,int end);//重载括号运算实现slice功能
 bvec slice(bvec input_cvec,int start,int delta,int end);//重载括号运算实现slice功能
 ivec slice(ivec input_cvec,int start,int delta,int end);//重载括号运算实现slice功能
double LTE_norm(cvec cv);
 vec LTE_abs(const cvec& cv);
 vec LTE_pow(vec v);
 cmat LTE_eye(int rows,int cols);
  cmat multi_cmat_m( const cmat &m1, const cmat& m2);//矩阵相乘
  cvec mutil_cmat_v( const cmat&m1, const cvec&v2);
  /*
   *	这个函数的功能是返回指定多维矩阵中指定位置的元素，通常将构成一个向量
   * 比如说三维矩阵A(3,4,5) 则a=get_Ceil_elem(2,2,A)后将返回5个二维矩阵中处在(2,2)位置的元素
   * 他将构成一个向量
   */
  cvec get_Ceil_elem(Array<cmat>input,int dest_row,int dest_col);
  /*
   *	与上面的函数恰好相对应
   *其中cv就是要插入到矩阵中的数据的集合
   */
  void set_Ceil_elem(Array<cmat>input,int dest_row,int dest_col,cvec cv);
  //cvec get_Ceil_elem(Array<cmat>input,int dest_row,int dest_col);

  void LTE_isnan(ivec& output,vec v);

 #endif
