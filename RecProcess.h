#ifndef _RECPROCESS
#define _RECPROCESS
#include "common.h"
#include "LTEA_param.h"
#include "Channel.h"
#include "CRC.h"
#include "de_OFDM.h"
//#include "de_SubCarrMapping.h"
//#include "de_Modulate.h"
//#include "de_RateMatch.h"
//#include "de_Turbo.h"
//#include "de_Scramble.h"
/*
 *	接收到信道处理后的数据，处理后最终返回TB错误指示位向量
 */
bvec Reciever(cvec Rec_Code,double SNRdB,iUE*m_iUE);
#endif
