#include "system_init.h"
#include "CRC.h"
#include "CodeBlock_segment.h"
#include "Turbo.h"
#include "run_time.h"
#include "Scramble.h"
#include "modulation.h"
#include "SubCarrier_Mapping.h"
#include "OFDM.h"
#include "SendProcess.h"
#include "Constant.h"
using namespace itpp;
using std::cout;
using std::endl;
/*
发端处理流程
基站的基带处理函数，该函数仅用户数据信道，PDSCH处理流程为：
产生信息-->附加CRC-->码块分割-->Turbo编码-->速率匹配-->码块级联
-->加扰-->层映射-->预编码-->资源映射-->OFDM调制
*/
cvec Send(iUE*m_iUE)
{
	param_init(m_iUE);
	int nOfdm=14;
	int nTx=m_iUE->nUE_param.m_BS_param.nAnt;
	int nTB=1;
	int RNIT=m_iUE->nUE_param.m_UE_param.RNIT;
	int nLayer=m_iUE->nUE_param.m_UE_param.nLayer;
	int sizeTB1=m_iUE->nUE_param.m_UE_param.TB.TB1.sizeTB;
	int CellID=m_iUE->nUE_param.m_BS_param.cellID;
	int iSubFrame=m_iUE->nUE_param.iSubFrame;
	 int PrecodingMode=m_iUE->nUE_param.m_UE_param.preCoding_m;//预编码模式
	 int PMI=m_iUE->nUE_param.m_UE_param.PMI;
	 /*
	  *	处理开始
	  */
	    bvec TB;
        bvec CRC_TB;
	    cvec modulated_code;                                                                       //承载调制后的符号

		TB=bit_init(*m_iUE);                                                                        //初始化数据,size=6200
		//std::cout<<"size after bit_init: "<<TB.size()<<std::endl;
		CRC_TB=CRC24a(TB);                                                           //传输块TB的尾部添加24bit校验位，size=6224
		 //std::cout<<"size after CRC24a: "<<CRC_TB.size()<<std::endl;
		Array<bvec> segment_code;
		segment_code=code_segment(m_iUE,CRC_TB,1);                   //码块分段,分为两段，size分别为3072和3136
        //std:cout<<"size of segment_code: "<<segment_code.size()<<std::endl;
       //std::cout<<"size of first CB: "<<segment_code(0).size()<<std::endl;
       // std::cout<<"size of second CB: "<<segment_code(1).size()<<std::endl;
       Array<Array<bvec>> turbo_code=turbo_encode(segment_code);   //第一维为2个码块，第二维为3路输出,码块1(size=3*3072+12),码块2(size=3*3136+12)
       //std::cout<<"size of first CB after turbo: "<<turbo_code(0)(0).size()+turbo_code(0)(1).size()+turbo_code(0)(2).size()<<std::endl;
       //std::cout<<"size of second CB after turbo: "<<turbo_code(1)(0).size()+turbo_code(1)(1).size()+turbo_code(1)(2).size()<<std::endl;
       Array<bvec> RateMatch_code=rate_match(turbo_code,m_iUE,1);           //速率匹配    码块1(size=4416)，码块2(size=4416)    *errors maybe*
       //std::cout<<"size of first CB after RateMatch: "<<RateMatch_code(0).size()<<std::endl;
       //std::cout<<"size of second CB after RateMatch: "<<RateMatch_code(1).size()<<std::endl;
       bvec BitCat_code=BitCat(RateMatch_code);                                         //码块级联   size=8832
       //std::cout<<"size of vector after BitCat: "<<BitCat_code.size()<<std::endl;
       bvec scramble_code=LTEA_scramble(BitCat_code,m_iUE,1);           //加扰
       //std::cout<<"size of vector after scramble: "<<scramble_code.size()<<std::endl;
       modulated_code=modulate(scramble_code,m_iUE);                //16QAM调制    size=2208
       //std::cout<<"size complex vector after modulate: "<<modulated_code.size()<<std::endl;
	m_iUE->nUE_param.m_UE_param.M_0_SYMB=modulated_code.length();

	    cmat SubCarrierMapped_Code=SubCarrierMapping(modulated_code,m_iUE);           //资源映射    (将预编码后的数据映射到2048*14的矩阵中,加入了导频)
	    //std::cout<<"size of cmat after  SubCarrierMapping: "<<SubCarrierMapped_Code.size()<<std::endl;        //28672(2048*14)
		cvec OFDM_Code=LTE_Ofdm(SubCarrierMapped_Code,m_iUE);                                  //OFDM调制
		//std::cout<<"size of OFDM_Code: "<<OFDM_Code.size()<<std::endl;                                //30720

	return OFDM_Code;                                                                 //bug free&&understand  2018/1/31

	}



