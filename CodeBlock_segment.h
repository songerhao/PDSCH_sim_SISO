/*
完成Turbo编码前的传输块分段操作
并将分段信息记录在全局变量iUE中
*/
#ifndef _SEGMENT
#define _SEGMENT
#include "common.h"
#include "LTEA_param.h"
#include <itpp/base/itfile.h>
/*
CRC_24a_Bit完成crc24a校验的原始数据
m_iUE 全局变量用来记录码块分段后的信息
*/
Array<bvec> code_segment(iUE* m_iUE,bvec CRC_24a_Bit,int iTB);
/*leaver_path :扰码序列的码本文件路径
将转换成it文件格式，便于储存和读取
*/
bool load_LeaverPar(float turbo_param[][3],const char* filepath);

#endif
