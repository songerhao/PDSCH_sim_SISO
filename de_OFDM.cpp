#include "de_OFDM.h"
#include "error_log.h"
cmat de_LTE_Ofdm(cvec OFDM_Code,iUE*m_iUE)
{
	 ivec CP;
	 int cyclicPrefix=m_iUE->nUE_param.cyclicPrefix;                 //循环前缀的指示比特
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
	    int nOfdmSybol=CP.length();
	    cmat de_ofdm_Code;
		 de_ofdm_Code.set_size(FFT_SIZE,nOfdmSybol);
		 int start=0;int end;
		 for (int iOfdm=0;iOfdm<nOfdmSybol;iOfdm++)                                 //解调
		 {
			 end=start+FFT_SIZE+CP(iOfdm);
			 OFDM ofdm_decoder(FFT_SIZE,CP(iOfdm));
			 assert(end<=OFDM_Code.size());
			 cvec temp=ofdm_decoder.demodulate(OFDM_Code(start,end-1));
			 de_ofdm_Code.set_col(iOfdm,temp);
			 start=end;
		 }

	 return de_ofdm_Code;
}
