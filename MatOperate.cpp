#include "MatOperate.h"
using namespace itpp;
cvec get_Ceil_elem(Array<cmat>input,int des_row,int dest_col)
{
	assert(input.length()>0);
	cvec output;
	output.set_size(input.length());
	for (int index=0;index<input.length();index++)
	{
		assert(des_row<input(index).rows()&&dest_col<input(index).cols());
		output.set(index,input(index)(des_row,dest_col));
	}
	return output;
}
 cvec slice(cvec input_cvec,int start,int delta,int end)
{
	assert(start>=0&&end<input_cvec.length()&&(start+delta)<end);
	int num=(end-start)/delta+1;
	/*
	0 1 2 3 4 5 6 7 8 9
	 start 0 delta 2 end 8   0 2 4 6 8
	*/
	cvec v;
	v.set_size(num);
	int index=0;
	for (int i=start;i<=end;i+=delta)
	{
		v.set(index,input_cvec(i));
		index++;
	}
	return v;
}
 bvec slice(bvec input_cvec,int start,int delta,int end)
 {
	 assert(start>=0&&end<input_cvec.length()&&(start+delta)<end);
	 int num=(end-start)/delta+1;
	 bvec v;
	 v.set_size(num);
	 int index=0;
	 for (int i=start;i<end;i+=delta)
	 {
		 v.set(index,input_cvec(i));
		 index++;
	 }
	 return v;
 }
 ivec slice(ivec input_cvec,int start,int delta,int end)
 {
	 assert(start>=0&&end<input_cvec.length()&&(start+delta)<end);
	 int num=(end-start)/delta+1;
	 ivec v;
	 v.set_size(num);
	 int index=0;
	 for (int i=start;i<end;i+=delta)
	 {
		 v.set(index,input_cvec(i));
		 index++;
	 }
	 return v;
 }
 double LTE_norm(cvec cv)
 {
	 vec abs_vec=LTE_abs(cv);
	 double temp=elem_mult_sum(abs_vec,abs_vec);
	 return std::sqrt(temp);
 }
  vec LTE_abs(const cvec& cv)
 {
	 vec output;
	 output.set_size(cv.size());
	 for (int index=0;index<output.size();index++)
	 {
		 output(index)=std::sqrt(std::norm(cv(index)));
	 }
	 return output;

 }

  vec LTE_pow(vec v)
  {
	  assert(v.size()>0);
	  vec output;
	  output.set_size(v.size());
	  for (int index=0;index<output.length();index++)
	  {
		  output(index)=std::pow(10.0,v(index)/=10);
	  }
	  return output;
  }

  cmat LTE_eye(int rows,int cols)
  {
	  assert(rows>=0&&cols>=0);
	  cmat output;
	  output.set_size(rows,cols);
	  output.zeros();
	  if (rows>cols)
		output.set_submatrix(0,cols-1,0,cols-1,1);
	  else if (rows<cols)
		output.set_submatrix(0,rows-1,0,rows-1,1);
	  else
		  eye(rows,output);
	  return output;


  }

   cmat multi_cmat_m(const cmat &m1,const cmat& m2)
  {
	  assert(m1.cols()==m2.rows());
	  cmat output;
	  output.set_size(m1.rows(),m2.cols());
	  for (int row=0;row<m1.rows();row++)
	  {
		  for (int col=0;col<m2.cols();col++)
		  {
			  output(row,col)=m1.get_row(row)*m2.get_col(col);
		  }
	  }
	  return output;

  }
   cvec mutil_cmat_v( const cmat&m1, const cvec&v2)
  {
	  assert(m1.cols()==v2.length());
	  cvec output;
	  output.set_size(m1.rows());
	  output.zeros();
	  for (int row=0;row<m1.rows();row++)
	  {
		  output.set(row,dot(m1.get_row(row),v2));
	  }
	  return output;
  }


//    cvec get_Ceil_elem(Array<cmat>input,int des_row,int dest_col)
//    {
// 	   cvec output;
// 	   output.set_size(input.length());
// 	   assert(input.length()>0);
// 	   for (int len=0;len<input.length();len++)
// 	   {
// 		   assert(dest_col<input(len).cols()&&des_row<input(len).rows());
// 		   for (int row=0;row<input(len).rows();row++)
// 		   {
// 			   for (int col=0;col<input(len).cols();col++)
// 			   {
//
// 				  output(len)=input(len).get(des_row,dest_col);
// 			   }
// 		   }
// 	   }
// 	   return output;
//
//    }

   void set_Ceil_elem(Array<cmat>input,int dest_row,int dest_col,cvec cv)
   {
	   assert(input.length()>0);
	   assert(input.length()==cv.length());
	   int len=input.length();
	   for (int len=0;len<input.length();len++)
	   {
		   assert(dest_col<input(len).cols()&&dest_row<input(len).rows());
		   for (int row=0;row<input(len).rows();row++)
		   {
			   for (int col=0;col<input(len).cols();col++)
			   {
				   input(len).set(dest_row,dest_col,cv(len));
			   }
		   }

	   }
   }

   void LTE_isnan(ivec& output,vec v)
   {
	   assert(v.length()==output.length());
	   assert(v.length()>0&&output.length()>=0);
	   for (int index=0;index<v.length();index++)
	   {
		   if (std::isnan(v(index)))
		   {
			   output(index)=1;
		   }
		   else
			   output(index)=0;
	   }

   }
