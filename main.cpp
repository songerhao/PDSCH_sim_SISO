/*
By  SEH of BUPT
*/
#include "system_init.h"
#include "CRC.h"
#include "CodeBlock_segment.h"
#include "Turbo.h"
#include "run_time.h"
//#include "Scramble.h"
//#include "modulation.h"
//#include "layerMapping.h"
//#include "precoding.h"
//#include "SubCarrier_Mapping.h"
#include "SendProcess.h"
#include "Channel.h"
#include "OFDM.h"
#include "RecProcess.h"
#include "de_OFDM.h"
//include "de_SubCarrMapping.h"
//#include "de_Precoding.h"
//#include "de_LayerMapping.h"
//#include "de_Modulate.h"
//#include "de_RateMatch.h"
//#include "de_Turbo.h"
//#include "de_Scramble.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <itpp/base/itfile.h>
using namespace std;
using namespace itpp;
iUE  m_iUE;//全局变量
int main(int arg,char**args)
{
    for(double SNRdB=0;SNRdB<10;SNRdB++)
	{
		cvec Send_Code=Send(&m_iUE);                                                             //发送
        cvec Channelled_Code=channel(Send_Code,SNRdB);                       //数据过信道
        bvec Rec_Info_Bit=Reciever(Channelled_Code,SNRdB,&m_iUE);               //接收
	}
}



