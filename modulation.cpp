#include "modulation.h"
#include <itpp/comm/modulator.h>
#include "error_log.h"
using namespace itpp;
cvec modulate(bvec scrambeld_code,iUE* m_iUE)
{
	int Qm=m_iUE->nUE_param.m_UE_param.Qm;
	assert(Qm==2||Qm==4||Qm==6);
	cvec modulate_code,symbols;
	PSK LTEA_qpsk;
	QAM LTEA_16qam;
	QAM LTEA_64qam;
	switch(Qm)
	{
	case 2://QPSK
		LTEA_qpsk.set_M(4);
		modulate_code=LTEA_qpsk.modulate_bits(scrambeld_code);
		break;
	case 4:
		LTEA_16qam.set_M(16);
		modulate_code=LTEA_16qam.modulate_bits(scrambeld_code);
		break;
	case 6:
		LTEA_64qam.set_M(64);
	  // symbols=LTEA_64qam.get_symbols();
		//std::cout<<symbols<<std::endl;
		modulate_code=LTEA_64qam.modulate_bits(scrambeld_code);
		break;
	default:
		printf("the Qm may not be set correctly...!\n");
		char* err_msg="the Qm was not set in the limited number range from 2 to 6!";
		loger(err_msg);
		modulate_code=0;
		break;
	}
	//std::cout<<modulate_code<<std::endl;
	return modulate_code;
}

