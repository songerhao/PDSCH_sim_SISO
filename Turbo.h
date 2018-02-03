#ifndef _TURBO
#define _TURBO
#include "common.h"
#include "LTEA_param.h"

 /*
 这个函数的作用是将turbocode的输出分离出三路信号，
 */
Array<bvec> dispatch_turboCode(bvec turbo_code);
/*根据码块分割后的比特来进行turbo码编码
	所有Turbo码输出信息
	turbo_encode(iCode)(0)对应信息比特
	turbo_encode(iCode)(1)对应校验比特1
	turbo_encode(iCode)(2)对应校验比特2
*/
Array<Array<bvec>> turbo_encode(Array<bvec>segment_code);

/*
速率匹配的输入为turbo_encode后的编码,是一个嵌套结构的array,每个array中包含三个元素
分别为信息比特，校验比特1和2；
输出为码块iCode对应的速率匹配的输出，
*/
Array<bvec>rate_match(Array<Array<bvec>> turbo_code,iUE* m_iUE,int iTB);//这里把UE参数弄进来是为了给今后的系统级仿真留接口
//产生扰码序列的函数
ivec gen_leaver_par(int code_length);
//比特级联
 bvec BitCat(Array<bvec>rate_match_code);

#endif
