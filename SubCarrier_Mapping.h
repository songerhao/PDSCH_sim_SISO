#ifndef _SUBCARRIER_MAPPING
#define _SUBCARRIER_MAPPING
#include "common.h"
#include "LTEA_param.h"
/*
 *	子载波映射~~~
 对预编码的复值符号进行资源粒子映射
 从某个起始载波开始，将连续的可用载波资源分配给第iUE个用户
 输出：子载波映射后的符号
 */
cmat SubCarrierMapping(cvec modulate_code,iUE* m_iUE);
 imat find_pilot(iUE* m_iUE);
#endif
