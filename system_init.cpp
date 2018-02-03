/*
这个部分主要初始化工作
1.完成原始数据的生成，完成LTEA参数的配置，并检测合理性
2.整个系统工作在单用户的8天线的天线端口5的模式下，采用波束赋形，2个码字，2各层
的模式，传输块有两层，系统带宽可配置，目前为20MHz，常规CP，接收端有两根天线

*/
#include <stdio.h>
#include "Constant.h"
#include "system_init.h"
using namespace itpp;
void param_init(iUE* m_iue)
{
	m_iue->nUE_param.addNoise=1;                                     //添加噪声
	m_iue->nUE_param.m_UE_param.nRx=1;                   //单天线接收
	m_iue->nUE_param.cyclicPrefix=1;                                ////1 normal cp ；0 extend cp
	m_iue->nUE_param.Channel_SCM=PedA;                  //信道模型
	m_iue->nUE_param.nCarrBand=15000;                       //子载波带宽
	m_iue->nUE_param.m_UE_param.startCarrPos=0;
	m_iue->nUE=1;                                                                    //UE个数
	m_iue->nUE_param.iFrame=4;                                       //第四个无线帧
	m_iue->nUE_param.CFI=2;                                            //指示PDCCH占用OFDM符号数
	m_iue->nUE_param.iSubFrame=4;                              //第四个子帧
	m_iue->nUE_param.m_BS_param.cellID=0;              //小区号
	m_iue->nUE_param.m_BS_param.nAnt=1;               //发端天线数
	m_iue->nUE_param.m_UE_param.nLayer=1;//        //层数
	m_iue->nUE_param.m_UE_param.nRB=16;            // 分配给用户的物理资源数
	m_iue->nUE_param.m_UE_param.nRE=2208;        //用于PDSCH的RE数目
	m_iue->nUE_param.m_UE_param.preCoding_m=SingleAnt;
	m_iue->nUE_param.m_UE_param.DetectAlgorithm=MMSE;
	m_iue->nUE_param.m_UE_param.RNIT=30;
	m_iue->nUE_param.m_UE_param.iCW=1;                 //一个码字
	m_iue->nUE_param.m_UE_param.transMod=1;
	m_iue->nUE_param.m_UE_param.TB.TB1.sizeTB=6200;

	m_iue->nUE_param.m_UE_param.Qm=4;                      //16QAM
	m_iue->nUE_param.m_UE_param.KMIMO=1;
	m_iue->nUE_param.m_UE_param.LTEA_category=4;
	m_iue->nUE_param.m_UE_param.TB.TB1.rvidx=3;
	m_iue->nUE_param.m_UE_param.TB.TB2.rvidx=2;

}

bvec bit_init(iUE m_iue)
{
	int size=m_iue.nUE_param.m_UE_param.TB.TB1.sizeTB;
	//int size1=m_iue.nUE_param.m_UE_param.TB.TB2.sizeTB;
	bvec TB;                                                           //生成一个传输块处理向量
	TB=randb(size);                                             //随机生成指定size的数据
	return TB;
}
