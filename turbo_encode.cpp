#include "Turbo.h"
#include <math.h>
#include <itpp/comm/turbo.h>
#include "CodeBlock_segment.h"
#include "error_log.h"
#include "Constant.h"
#include "MatOperate.h"
using namespace itpp;
using std::cout;
using std::endl;
//根据输入比特的长度，寻找leaver参数表来获得扰码序列
ivec gen_leaver_par(int code_length)
{
	int index;//用来指示扰码表序列的序号
	int k=0;
	float f1,f2;//扰码序列生成的两个参量
	ivec inLeaver_par=zeros_i(code_length);//扰码序列
	float Leaver_par[188][3];
	if (!load_LeaverPar(Leaver_par,"LeaverPar.txt"))                                                        //载入Turbo交织表
	{
		char* err_msg="fatal error! Failure to open the leaverParam config file!";
		loger(err_msg);
		return NULL;
	}
	for (index=0;index<188;index++)
	{
		if (Leaver_par[index][0]==code_length)
			break;
	}
	f1=Leaver_par[index][1];
	f2=Leaver_par[index][2];
	while(k<code_length)
	{
		inLeaver_par.set(k,mod(f1*k+f2*k*k,code_length));
		k++;
	}
#ifdef _LTEA_DEBUG
	std::cout<<扰码序列是:<<inLeaver_par<<std::endl;
#endif
	return inLeaver_par;

}
/*
turbo 编码函数
码率为1/3，两个RSC编码器，附加输出12个尾比特
*/
Array<Array<bvec> > turbo_encode(Array<bvec> segment_code)
{
	int nBlock=segment_code.length();//获得分块的码块数
	Array<Array<bvec>> turbo_code;    //第一维为块数，第二维为三个输出
	turbo_code.set_size(nBlock);
	Turbo_Codec turbo_coder;
	ivec gen(2);
	gen(0)=013;
	gen(1)=015;
	int constraint_length=4;                                            //约束长度
	int i;
	bvec coded__bits;

	for (i=0;i<nBlock;i++)
		{
			turbo_code(i).set_size(3);//一个codeblock对应三个输出,信息比特还有两个校验比特
			ivec leaver_sequence=gen_leaver_par(segment_code(i).length());//获得扰码序列
			turbo_coder.set_parameters(gen,gen,constraint_length,leaver_sequence);
			turbo_coder.encode(segment_code(i),coded__bits);
			turbo_code(i)=dispatch_turboCode(coded__bits);
	}

return turbo_code;
}

Array<bvec> dispatch_turboCode(bvec turbo_code)
{
	Array<bvec> dispatched_code;
	int turbo_length=turbo_code.length();
	dispatched_code.set_size(3);
	dispatched_code(0).set_size(turbo_length/3);
	dispatched_code(1).set_size(turbo_length/3);
	dispatched_code(2).set_size(turbo_length/3);
	int info_pos=0;int other1_pos=0;int other2_pos=0;
	for (int i=0;i<turbo_code.length();i++)
	{
		switch (i%3)
		{
		case 0:
			dispatched_code(0)(info_pos)=turbo_code.get(i);
			info_pos++;
			break;
		case 1:
			dispatched_code(1)(other1_pos)=turbo_code.get(i);
			other1_pos++;
			break;
		case 2:
			dispatched_code(2)(other2_pos)=turbo_code.get(i);
			other2_pos++;
			break;
		}
	}
	return dispatched_code;
//#ifdef _LTEA_DEBUG
	//printf("the length of info bit is :%d\n",(turbo_length-12)/3+4);
	//printf("the value of infoBit_length is:%d-----%d-------%d\n",info_pos,other1_pos,other2_pos);
//#endif

}
inline int LTEA_min(int a,int b)
{
	return(a>=b?b:a);
}
/*
速率匹配实现，包括子块交织，比特收集，比特选取和凿孔,速率匹配就是从编码后的比特中通过打孔或者重传比特来实现不同速率的打孔图样,使
之适应可以使用的物理资源
*/
Array<bvec> rate_match(Array<Array<bvec>>turbo_code,iUE* m_iUE,int iTB)
{

	//===================== 子块交织 ==================================//
	ivec inLeaver ="0 16 8 24 4 20 12 28 2 18 10 26 6 22 14 30 1 17 9 25 5 21 13 29 3 19 11 27 7 23 15 31";
	ivec alphaPi;//对第三个数据流的处理
	ivec nanPosition;//记录NaN比特位置的向量
	const int cSubTC=32;
	int rSubTC,nLeaverLen,t, Ncb;
	int iCode=turbo_code.size();//获得用户i的码块数，这是经过码块分割后的码块数
	mat v,d;//储存字块交织前后的三路流数据
	int F;
	Array<bvec> output;
	output.set_size(iCode);

    m_iUE->nUE_param.m_UE_param.TB.TB1.Ncb.set_size(iCode);
    m_iUE->nUE_param.m_UE_param.TB.TB1.rSubTC.set_size(iCode);
    m_iUE->nUE_param.m_UE_param.TB.TB1.nanPosition.set_size(iCode);
    m_iUE->nUE_param.m_UE_param.TB.TB1.turboOutputSize.set_size(iCode);

	//////////////////////////////////////////////////////////////////////////
	for (int i=0;i<iCode;i++)
	{
		//turbo(i)是一个3行 有限长的矩阵
		int colLen=turbo_code(i)(0).length();//turbo码编码其输出的码片长度
		d.set_size(3,colLen);
		for (int index=0;index<3;index++)             //将比特流填入d
		{
			vec vTurbo_code=to_vec(turbo_code(i)(index));
			d.set_row(index,vTurbo_code);
		}
		rSubTC=ceilf(colLen/cSubTC)+1;//获得子块交织器的行数

		nLeaverLen=rSubTC*cSubTC;//子块交织器输出向量长度（行数×列数）
		v.set_size(3,nLeaverLen);        //确定交织后v的大小
		v.zeros();
		if (nLeaverLen>colLen)          //矩阵元素数目大于比特流数目时要添加虚假比特
		{
			F=nLeaverLen-colLen;
			mat F_mat=ones(3,F);
			F_mat.zeros();
			F_mat=F_mat*=d_NaN;
			mat temp=d;
			d.set_size(3,nLeaverLen);
			d=concat_horizontal(F_mat,temp);             //水平串联矩阵
		}


		for (int iRow=0;iRow<2;iRow++)//先对前两个数据流处理
		{

			mat temp_leaver;
			temp_leaver.set_size(rSubTC,cSubTC);
			temp_leaver=transpose(reshape(d.get_row(iRow),cSubTC,rSubTC));
			temp_leaver=temp_leaver.get_cols(inLeaver);//扰码
			vec v_temp;
            v_temp.set_length(v.get_row(iRow).length());
			//按列优先读取
			for (int cols=0;cols<temp_leaver.cols();cols++)
			{
				assert(cols*rSubTC+cSubTC<=v.cols());
				v_temp.set_length(v.get_row(iRow).length());
				v_temp.set_subvector(cols*rSubTC,temp_leaver.get_col(cols));
			}
			v.set_row(iRow,v_temp);
			v_temp.zeros();
		}
		alphaPi=zeros_i(nLeaverLen);
		for (int k=0;k<nLeaverLen;k++)
		{
			t=floor_i(k/rSubTC);
			alphaPi(k)=mod(inLeaver(t)+cSubTC*mod(k,rSubTC),nLeaverLen);

		}
		v.set_row(2,d.get_row(2)(alphaPi));

		//===================== 比特收集 ===================================//
		int CircularBufLen=d.rows()*nLeaverLen;//比特收集缓冲区大小
		ivec info_bit_pos,other_pos1,other_pos2;
		info_bit_pos.set_size(nLeaverLen);
		other_pos1.set_size(nLeaverLen);
		other_pos2.set_size(nLeaverLen);
		vec Buf=zeros(CircularBufLen);
		vec Buf_temp=zeros(2*nLeaverLen);     //存储后两路数据
		vec row0=v.get_row(0);
	    vec row1=v.get_row(1);
	    vec row2=v.get_row(2);
	    Buf.set_subvector(0,row0);
		int info=0,other1=0,other2=0;
		for(int index=0,index1=0;index<2*nLeaverLen;index++)
        {
             if(index%2==0)
                Buf_temp(index)=row1(index/2);
             else
                Buf_temp(index)=row2(index/2);
        }

		Buf.set_subvector(nLeaverLen,Buf_temp);
	    nanPosition.set_size(Buf.length());
	    LTE_isnan(nanPosition,Buf);


	//===================== 比特选取和凿孔 ==============================//
	vec Nsoft ="250368 1237248 1237248 1827072 3667200"; //% 软信道比特总和 36.306 V9.1.0,注意这里超出了int
																									//的表示范围65536，故要用double
	const int Mlimit=8;
	int E;
	int M_DL_HARQ=8;//这个参数根据上下行时隙配置可变
	int KMIMO=m_iUE->nUE_param.m_UE_param.KMIMO;
	int catgory=m_iUE->nUE_param.m_UE_param.LTEA_category;
	int nTB=2;//双层传输
	int nLayer=m_iUE->nUE_param.m_UE_param.nLayer;
	int nRE=m_iUE->nUE_param.m_UE_param.nRE;
	int Qm=m_iUE->nUE_param.m_UE_param.Qm;//调制方式
	int C, rvidx;
	switch(iTB)
	{
	case 1:
		C=m_iUE->nUE_param.m_UE_param.TB.TB1.C;
		rvidx=m_iUE->nUE_param.m_UE_param.TB.TB1.rvidx;
		break;
	case 2:
		C=m_iUE->nUE_param.m_UE_param.TB.TB2.C;
		rvidx=m_iUE->nUE_param.m_UE_param.TB.TB2.rvidx;
		break;
	}
	int NL,G;
	if (nLayer==2&&nTB==1)
	{
		NL=2;
		G=2*nRE;
	}
	else if(nLayer==3&&nTB==2)
	{
		if (iTB==1)
		{
			NL=1;
			G=nRE;
		}
		NL=2;
		G=2*nRE;
	}
	else if (nLayer==4&&nTB==2)
	{
		NL = 2;
		G = 2*nRE;

	}
	else
	{
		NL = 1;
		G = nRE;
	}
	int NIR=floor_i(Nsoft(catgory)/(KMIMO*LTEA_min(M_DL_HARQ,Mlimit)));
	Ncb=LTEA_min(floor_i(NIR/C),CircularBufLen);
	int GG=G/NL;
	int gama=mod(GG,C);
	if (i<C-gama)//前gama个分割后的码块//这个E很关键影响速率匹配的输出比特数在目前情况下输出4416
		E=NL*Qm*floor_i(GG/C);
	else
		E=NL*Qm*ceil_i(GG/C);
		int k0=rSubTC*(2*ceil_i(Ncb/(8*rSubTC))*rvidx+2);
	output(i).set_size(E);
	output(i).zeros();
	int kk=0,j=0;
	while(kk<E)
	{
		if (!std::isnan(Buf(mod(k0+j,Ncb))))
		{
			output(i)(kk)=(bin)Buf(mod(k0+j,Ncb));
			kk++;
		}
		j++;
	}
	m_iUE->nUE_param.m_UE_param.E.set_size(2);
	switch(iTB)
	{
	case 1:
		m_iUE->nUE_param.m_UE_param.TB.TB1.nanPosition(i)=nanPosition;
		m_iUE->nUE_param.m_UE_param.TB.TB1.Ncb(i)=Ncb;
		m_iUE->nUE_param.m_UE_param.TB.TB1.rSubTC(i)=rSubTC;
		m_iUE->nUE_param.m_UE_param.E(0)=E;
		break;
	case 2:
		m_iUE->nUE_param.m_UE_param.TB.TB2.Ncb(i)=Ncb;
		m_iUE->nUE_param.m_UE_param.TB.TB2.rSubTC(i)=rSubTC;
		m_iUE->nUE_param.m_UE_param.TB.TB2.nanPosition(i)=nanPosition;
		m_iUE->nUE_param.m_UE_param.E(1)=E;
		break;

	}
}

return output;



}

 bvec BitCat(Array<bvec> rate_match_code)
{
#ifdef _LTEA_MULTI_THREAD

#else ifdef _LTEA_NO_MULTI_THREAD
	bvec Output_BitCat;
	int length=rate_match_code.length();
	for (int i=0;i<length;i++)
	{
		int temp_length=rate_match_code(i).length();
		int output_length=Output_BitCat.length();

		//Output_BitCat.set_length(temp_length+output_length);
		Output_BitCat=concat(Output_BitCat,rate_match_code(i));
		int concat_length=Output_BitCat.length();
#ifdef _LTEA_DEBUG
		printf("the length of rate_match_code(%d) is :%d\n",i,temp_length);
		printf("the length of output now is:%d\n",output_length);
		printf("the length of output now is:%d\n",concat_length);
		printf("the total is:%d\n",output_length+temp_length);

#endif
	}

	return Output_BitCat;

#endif

}







