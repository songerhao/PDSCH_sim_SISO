#ifndef _INIT
#define _INIT
#include "common.h"
#include "LTEA_param.h"
#ifndef _CRC
#define _CRC
#include "common.h"
bvec CRC24a(bvec RawBit);
bvec CRC24b(bvec RawBit);
bvec CRC24a_Check(bvec CRC24a_Code);
bvec CRC24b_Check(bvec CRC24b_Code);


#endif
void param_init(iUE* m_iue);
bvec  bit_init(iUE m_iue);
#endif
