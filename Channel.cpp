#include "Channel.h"
#include "MatOperate.h"
#include "Constant.h"
#include "error_log.h"
using namespace itpp;
using std::cout;
using std::endl;
 cvec channel(cvec Send_Code,double SNRdB)
{
    // 设置采样率
    double Ts = 1.0/(30.72e6);
  // 设置信道模型      Average power [dB]"0 -9.7 -19.2 -22.8"         delay["0 110 190 410"]ns  4抽头TDL信道
  Channel_Specification channel_spec(ITU_Pedestrian_A);
  // initialize with the predefined channel profile
   TDL_Channel tdl_channel(channel_spec, Ts);
   Array<cvec> coeff;
   tdl_channel.generate(Send_Code.size(),coeff);                                 //生成信道系数
   cvec after_tdl=elem_mult(Send_Code,coeff(0));                               //过TDL信道
   AWGN_Channel awgn_channel;
   double EbN0=inv_dB(SNRdB);
   double Ec=1.0;                                             //The transmitted energy per symbol
   double Eb=Ec/4.0;                                      //The transmitted energy per bit
   double N0 =Eb*pow(EbN0,-1.0);
   awgn_channel.set_noise(N0);
   cvec after_awgn =awgn_channel(after_tdl);                                       //过AWGN信道
   return after_awgn;
}


