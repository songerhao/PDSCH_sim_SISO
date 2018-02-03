#ifndef _DE_OFDM
#define _DE_OFDM
/*
 *	这个是属于接收端的模块了，开始解OFDM数据~~
 */
#include "common.h"
#include "Constant.h"
#include "LTEA_param.h"
cmat de_LTE_Ofdm(cvec OFDM_Code,iUE*m_iUE);
#endif
