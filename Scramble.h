#ifndef _SCRAMBLE
#define _SCRAMBLE
#include "common.h"
#include "LTEA_param.h"
/*比特级的系统用户扰码
*/
bvec LTEA_scramble(bvec BitCat_code,iUE* m_iUE,int iTB);

#endif
