#include "Scramble.h"
#include <math.h>
bvec LTEA_scramble(bvec BitCat_code,iUE* m_iUE,int iTB)
{
	bvec output_code;
	int slot=2*m_iUE->nUE_param.iSubFrame;
	const int Nc=1600;
	int c_init;
	int RNIT=m_iUE->nUE_param.m_UE_param.RNIT;
	int CellID=m_iUE->nUE_param.m_BS_param.cellID;
	int iCW=iTB;

	int Bit_len=BitCat_code.length();//用户某个特定码字的比特数
	int nPN=Bit_len;//用来加扰的PN序列，其长度等于信息马长度

	/*生成公式为c(n) = (x1(n+Nc)+x2(n+Nc))mod 2，先求x1和x2*/

	 bvec x1,x2;//用来生成Golden序列的中间变量
	 x1=zeros_b(nPN+Nc);
	 x2=zeros_b(nPN+Nc);
	 //对于第一个序列，第一个元素为1 从1-30号元素为0
	 x1(0)=1;
	 x1.set_subvector(1,30,(bin)0);

	 //PDSCH 加扰初始化
	 if (slot%2==0)
	 {
		 c_init=(int)(RNIT*std::pow(2.0,14)+(iCW-1)*std::pow(2.0,13)+floor_i(slot/2)*std::pow(2.0,9)+CellID);
		//assert(c_init<=65536);
		bvec temp;
		temp.set_length(31);temp.zeros();
		bvec b=dec2bin(c_init);
		temp.set_subvector(0,b.length()-1,b);
#ifdef _LTEA_DEBUG
		printf("the length of temp is:%d\n",temp.length());
#endif
		 x2.set_subvector(0,30,temp.get(0,30));

	 }
	 for (int n=0;n<nPN+Nc-31;n++)
	 {
		 x1(n+31)=mod((x1(n+3)+x1(n)),2);
		 x2(n+31) = mod((x2(n+3)+x2(n+2)+x2(n+1)+x2(n)),2);

	 }
	bvec c = x1(Nc,x1.length()-1)+x2(Nc,x2.length()-1);
	output_code=concat(output_code,(BitCat_code+c));
	return output_code;
}
