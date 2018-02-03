/*
% 功能：对已经添加CRC24A的数据b进行码块分割,
		并给分割后数据添加CRC24B
%      参照36.212 V870 5.1.2
*/

#include "CodeBlock_segment.h"
#include "CRC.h"
#include "Constant.h"
#include <io.h>
#include <stdio.h>
#include <math.h>
#include "error_log.h"
bool load_LeaverPar(float turbo_param[][3],const char* filepath)//可以将这个函数进行性能改进，不必每次都重新加载
{
	FILE* fp=fopen(filepath,"r");
	if (!fp)
	{
		return false;
	}
	int i=0;
	while (!feof(fp))
	{
		if(fscanf(fp,"%f %f %f",&turbo_param[i][0],&turbo_param[i][1],&turbo_param[i][2])==EOF)
			break;
		i++;
	}
	fclose(fp);
#ifdef _LTEA_DEBUG
	printf("load leaverPar successfully!\n");
#endif
	return true;
}
Array<bvec> code_segment(iUE* m_iUE,bvec CRC_24a_Bit,int iTB)
{
	float leaverParam[188][3];
	int index;//用来记录leaver表中的索引
	int L,F;//CRC校验位长度
	int Bz;//分块的码块大小
	int B=CRC_24a_Bit.length();
	Array<bvec> segment_data;
	int Kr,C,Ka,Ca,Kb,Cb,k,s=0;
	bvec c0;//填充比特
	if (B<40)                                    //如果B<40,填充比特添加到码块的开始位置
		{
			segment_data.set_size(1);
			F=40-B;
			c0=randb(F)*=NaN;
			segment_data(0)=concat(c0,CRC_24a_Bit);
			//储存分块信息

		}
	else
	{
		if(B<=MAX_TB)//当比特数大于40，且小于6144时
		{
			L=0;
			C=1;
			Bz=B;
		}
		else
		{
			L=24;
			C=ceilf(B/(MAX_TB-L))+1;
			Bz=B+C*L;
		}
		if (!load_LeaverPar(leaverParam,"LeaverPar.txt"))                                         //载入Turbo交织表
		{
			char* err_msg="fatal error! Failure to open the leaverParam config file!";
			loger(err_msg);
			return NULL;
		}
		for (int row=0;row<188;row++)
		{
			if (leaverParam[row][0]*C>=Bz)
			{
				Ka=leaverParam[row][0];
				index=row;
#ifdef _LTEA_DEBUG
				printf("Ka has been found :%d\n",Ka);
#endif
				break;
			}
		}
		if(C == 1)
		{
			Ca = 1;
			Kb = 0;
			Cb = 0;
		}
		else if (C>1)
		{
			Kb=leaverParam[index-1][0];
			int deltaK=Ka-Kb;
			Cb=floorf((C*Ka-Bz)/deltaK);
			Ca=C-Cb;
		}
		segment_data.set_size(C);
		 F = Ca*Ka+Cb*Kb-Bz;
		 if (F>0)
			 c0=ones_b(F)*=NaN;
		 k=F+1;
		 for (int iCode=0;iCode<C;iCode++)
		 {
			 if (iCode<=Cb)
				 Kr=Kb;
			 else
				 Kr=Ka;
			 segment_data(iCode)=zeros_b(Kr);

			 if (iCode==0&&F>0)
				 segment_data(iCode).set_subvector(0,F-1,c0);
			segment_data(iCode).set_subvector(k-1,Kr-L-1,CRC_24a_Bit.get(0,Kr-L-k));

			 if (C>1)
			 {

				 bvec temp=segment_data(iCode).get(0,Kr-L-1);
				 segment_data(iCode)=CRC24b(temp);
			 }
		 k=1;
		 }
	}
		/*---------------------------------------*/
	m_iUE->nUE_param.m_UE_param.nCB.set_size(2);//两个传输块
	if(iTB==1)
	{
	m_iUE->nUE_param.m_UE_param.TB.TB1.Ka=Ka;
	m_iUE->nUE_param.m_UE_param.TB.TB1.C=C;
	m_iUE->nUE_param.m_UE_param.TB.TB1.Ca=Ca;
	m_iUE->nUE_param.m_UE_param.TB.TB1.Cb=Cb;
	m_iUE->nUE_param.m_UE_param.TB.TB1.Kb=Kb;
	m_iUE->nUE_param.m_UE_param.nCB.set(0,C);

	}
	if (iTB==2)
	{
	m_iUE->nUE_param.m_UE_param.TB.TB2.Ka=Ka;
	m_iUE->nUE_param.m_UE_param.TB.TB2.C=C;
	m_iUE->nUE_param.m_UE_param.TB.TB2.Ca=Ca;
	m_iUE->nUE_param.m_UE_param.TB.TB2.Cb=Cb;
	m_iUE->nUE_param.m_UE_param.TB.TB2.Kb=Kb;
	m_iUE->nUE_param.m_UE_param.nCB.set(1,C);

	}

#ifdef _LTEA_DEBUG
	printf("the length of segment_data is:%d\n",segment_data.length());
	printf("the length of segment_data(0) is:%d\n",segment_data(0).length());
    printf("the length of segment_data(1) is:%d\n",segment_data(1).length());
#endif
	return segment_data;

	}









