#include "CRC.h"
#include <iostream>
using namespace itpp;
bvec  CRC24a(bvec RawBit)
{
	CRC_Code CRC24a_encoder;
	bvec G="1 1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1";//校验多项式
	CRC24a_encoder.set_generator(G);
	bvec CRC24a_Coded_Code=CRC24a_encoder.encode(RawBit);
	return CRC24a_Coded_Code;

}

bvec CRC24b(bvec RawBit)
{

	CRC_Code CRC24b_encoder;
	bvec G="1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1";//校验多项式
	CRC24b_encoder.set_generator(G);
	bvec CRC24b_Coded_Code=CRC24b_encoder.encode(RawBit);
	return CRC24b_Coded_Code;

}

bvec CRC24a_Check(bvec CRC24a_Code)
{
	CRC_Code CRC24a_decoder;
	bvec G="1 1 0 0 0 0 1 1 0 0 1 0 0 1 1 0 0 1 1 1 1 1 0 1 1";//校验多项式
	CRC24a_decoder.set_generator(G);
	bvec RawBit;
	CRC24a_decoder.decode(CRC24a_Code,RawBit);
	return RawBit;

}
bvec CRC24b_Check(bvec CRC24b_Code)
{
	CRC_Code CRC24b_decoder;
	bvec G="1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 0 0 0 1 1";//校验多项式
	CRC24b_decoder.set_generator(G);
	bvec RawBit;
	CRC24b_decoder.decode(CRC24b_Code,RawBit);
	return RawBit;

}
