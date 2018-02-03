#ifndef _CHANNEL
#define _CHANNEL
#include "common.h"
#include "LTEA_param.h"
/*
*	发端数据经过信道，到达接收端，并添加噪声
*信道模型为
*ITU Pedestrian A+AWGN
*/

//信道处理函数
cvec channel(cvec Send_Code,double SNRdB);
/*
 *	理想信道估计--
 */
Array<cmat> Channel_Est_Real(Array<Array<cmat>> H_Channel,iUE*m_iUE);

#endif
