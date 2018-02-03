#include "OFDM.h"
#include "MatOperate.h"
#include "error_log.h"
using namespace itpp;
cvec LTE_Ofdm(cmat SubCarrierMapped_Code,iUE* m_iUE)
{

	 ivec CP;
	//int iOfdmSybol=14;//一个子帧中包含14个OFDM符号
	int nTx=m_iUE->nUE_param.m_BS_param.nAnt;
	int cyclicPrefix=m_iUE->nUE_param.cyclicPrefix;//循环前缀的指示比特
	assert(cyclicPrefix==1||cyclicPrefix==0);
	if (cyclicPrefix==normal)//normal:1extend 0
	{
		CP.set("160 144 144 144 144 144 144  160 144	144	144	144	144	144");

	}

	else if (cyclicPrefix==extend)
	{
		printf("extend CP is not realized now......\n");
		char* err="系统错误，扩展CP未定义";
		loger(err);
		printf("press any key to continue...");
		getchar();
		exit(-1);
	}
	int nOfdmSybol=CP.length();         //14个OFDM符号
	cvec OFDM_Code=zeros_c(FFT_SIZE*nOfdmSybol+sum(CP));
		int start=0;
		for (int iOfdm=0;iOfdm<nOfdmSybol;iOfdm++)
		{
			int end=start+FFT_SIZE+CP(iOfdm)-1;//一个OFDM符号中总共有FFT_SIZE+CP(iOfdm)个符号
			OFDM ofdm_coder(FFT_SIZE,CP(iOfdm));                                                                              //设置参数
			cvec ofdm_out=ofdm_coder.modulate(SubCarrierMapped_Code.get_col(iOfdm));           //OFDM调制
           OFDM_Code.set_subvector(start,end,ofdm_out);                                                                     //并串转换
			start=end+1;
		}

	return OFDM_Code;

}
