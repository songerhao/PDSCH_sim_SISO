/*
这个是主要的CRC校验的模块
包括CRC-24a、CRC-24b、CRC-16
By :
	yusaint TONGJI University 2010/11/30
*/
#ifndef _CRC
#define _CRC
#include "common.h"
bvec CRC24a(bvec RawBit);
bvec CRC24b(bvec RawBit);
bvec CRC24a_Check(bvec CRC24a_Code);
bvec CRC24b_Check(bvec CRC24b_Code);


#endif
