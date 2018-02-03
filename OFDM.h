#ifndef _OFDM
#define _OFDM
#include "common.h"
#include "LTEA_param.h"
#include "Constant.h"
/*
*	 OFDM符号生成，附加循环前缀，并完成天线端口数据并串变换
*  输入：mapSymbCell：来自子载波映射后的符号
*    FFT_SIZE：FFT变换点数
*    nTx：发端天线数
*   CP：循环前缀
*	输出：ofdmSymb：生成的ofdm符号
*/
cvec LTE_Ofdm(cmat SubCarrierMapped_Code,iUE* m_iUE);

#endif
