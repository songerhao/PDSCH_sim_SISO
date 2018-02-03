/*
记录了整个lte-advanced仿真的重要配置参数
结构体ltea_param尤为重要
*/
#ifndef _LTEA_PARAM
#define _LTEA_PARAM
#include "common.h"
  struct BS_param
{
	int cellID;//小区号
	int nAnt;//发射端天线数
};

#ifdef _SCHEDUL
	  struct Schedule_param//系统调度参数，现在不用，今后可以扩展
	{
		int indexUE;//用户索引
	};
#endif

 struct UE_param
{
	int startCarrPos;
	int DetectAlgorithm;//1 for MMSE 2 for ZF
	int M_0_SYMB;//码字0上的复值符号数,用于传输分集中
	int nRx;//接受天线数
	int iCW;//用户可用的码字，1或2
	int KMIMO;//
	int RNIT;//用户扰码
	int Qm;//调制方式
	ivec nCB;//记录两个传输块的分块数
	ivec E;//记录跟传输块相关的E信息。。
	int preCoding_m;//预处理方式1:singleAnt,2:tranmitDiversity or 3:spatialMultiplexing}
	int nLayer;//系统处理层数(1-8)层是合法的
	int LTEA_category;//用户终端等级
#ifdef _CODE_BASED
	int PMI;//仅在基于码本反馈的系统中这个参数才有效
#endif
	struct TB{
		struct TB1
		{
			int sizeTB;
			int C;//码块分割时的码块数
			int Ka;
			int Kb;
			int Ca;
			int Cb;
			int rvidx;
			ivec rSubTC;
			ivec Ncb;
			//int rSubTC;//记录Turbo码的行交织大小
			//int Ncb;//记录软缓冲大小
			Array<ivec> nanPosition;
			ivec turboOutputSize;
		}TB1;
		struct TB2
		{
			int sizeTB;
			int C;//码块分割时的码块数
			int Ka;
			int Kb;
			int Ca;
			int Cb;
			int rvidx;
			ivec rSubTC;
			ivec Ncb;
			//int rSubTC;//记录Turbo码的行交织大小
			//int Ncb;//记录软缓冲大小
			Array<ivec> nanPosition;
			ivec turboOutputSize;
		}TB2;
	}TB;
	int nRE;//读取UE用于PDSCH的RE数目
	int iMSC;//读取调制编码序号IMCS，取值为0~29
	int nRB;//分配给用户的物理资源数
	int transMod;//传输模式
		/* 读取传输模式，共7种 1: 'singleAntennaPort' 2: 'tranmitDiversity' 3: 'largeDelayCDD' 4: 'closedLoopSM'
                      5:'multiuserMIMO'      6: 'singleLayerCLSM'  7: 'singlePort5'
	*/

};

 struct  Ltea_param
 {
	 int iFrame;//系统当前帧号
	 int nUE;//系统用户数
	 int sysBand;//
	 int addNoise;//是否添加噪声
	 int cyclicPrefix;//循环前缀表示,1为normal cp； 0为扩展cp
	 int iSubFrame;//当前子帧号
	 int CFI;         //LTE中PDCCH在一个子帧内（注意，不是时系）占用的符号个数，是由PCFICH中定义的CFI所确定的。
	 int nItera_times;//Turbo译码迭代次数
	 int DecodeAlgoritm;//译码算法："MAP", LOGMAP", "LOGMAX", "TABLE".
	 int nCarrBand;//一个子载波的带宽，通常为15KHz
	 int Channel_SCM;//共四种
	 BS_param m_BS_param;
#ifdef _SCHEDUL
	 Schedule_param m_Schedule_param;
#endif
	 UE_param m_UE_param;
 } ;

 struct iUE
 {
	 int nUE;
	 Ltea_param  nUE_param;
 } ;
#ifdef TEST
 Array<ivec> iUE_nanPosition;
 Array<ivec> CB_E;
 Array<ivec> CB_turboOutputSize;
 Array<ivec> CB_rSubTC;//记录Turbo码的行交织大小
 Array<ivec> CB_Ncb;//记录软缓冲大小
#endif
#endif
