#ifndef _MODULATION
#define _MODULATION
#include "LTEA_param.h"
#include "common.h"
/*
根据初始化信息中的Qm指示比特来对加扰后的数据进行调制
Qm：2 QPSK调制 4：16QAM   6:64QAM
*/
cvec modulate(bvec scrambeld_code,iUE* m_iUE);
/*
这个函数的功能是将所有传输块O(∩_∩)O~这里最多为2个的调制后的数据放入一个容器中
供层映射来使用
*/
//Array<cvec> combineTB(cvec modulated_code,iUE* m_iUE);

#endif
