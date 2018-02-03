#include "SubCarrier_Mapping.h"
#include "Constant.h"
using namespace itpp;
cmat SubCarrierMapping(cvec modulate_code,iUE* m_iUE)
{
	cmat Mapped_Code;
	int nTx=m_iUE->nUE_param.m_BS_param.nAnt;                                       //nAnt=1
	int CFI=m_iUE->nUE_param.CFI;                                                                   //CFI=2
	int scPerRB=12;//每个资源快包含12个子载波
	int nOfdmSymb=14;
	int nRB=m_iUE->nUE_param.m_UE_param.nRB;//该用户的可用物理资源快     //nRB=16
	int cellID=m_iUE->nUE_param.m_BS_param.cellID;                              //cellID=0
	int iSubFrame=m_iUE->nUE_param.iSubFrame;
	int startCarr_Pos=m_iUE->nUE_param.m_UE_param.startCarrPos;//起始子载波位置
	int ns=iSubFrame*2;
	int nCarrUsed=nRB*scPerRB;//该用户将占用的子载波数目,16*12
	int DL_RB;//下行总的可用资源快，带宽小于20MHz
	if (FFT_SIZE==1024)
	{
		DL_RB=50;
	}
	else if (FFT_SIZE==2048)
	{
		DL_RB=100;//占满20MHz的带宽
	}
	ivec AvailableSubCarrPos;
	AvailableSubCarrPos.set_size(nCarrUsed);          //可用子载波数量
	int index=0;
	for (int i=0;i<nCarrUsed/2;i++)
	{
		AvailableSubCarrPos(index)=i+FFT_SIZE-nCarrUsed/2;
		index++;
	}
	for(int j=0;j<nCarrUsed/2;j++)
	{
		AvailableSubCarrPos(index)=j+1;
		index++;
	}                                                                                                       //bug free  2018/1/9

	imat PilotLocation=find_pilot(m_iUE);                                  //192行*14列
	//std::cout<<"导频位置: "<<PilotLocation<<std::endl;
	int tag=0;
	for (int i=0;i<PilotLocation.rows();i++)
	{
		for (int j=0;j<PilotLocation.cols();j++)
		{
			if (PilotLocation(i,j)==1)
			{
				tag++;
			}
		}
	}
	double RS_cost=(double)tag/(PilotLocation.rows()*PilotLocation.cols());
	printf("the the Reference Signal  are:%d\nthe total Signal in a slot are:%d\n",tag,PilotLocation.rows()*PilotLocation.cols());
	printf("the system Reference Signal cost is:%f%%\n",RS_cost*100);

		Mapped_Code.set_size(FFT_SIZE,nOfdmSymb);//整个子帧的资源网格映射图
		Mapped_Code.zeros();
		int iSymbol=0;
		/*
		 *	物理资源映射在这里完成
		 */
		int iOfdm=0,iCarr=0;
		for (iOfdm=CFI;iOfdm<nOfdmSymb;iOfdm++)//时域上从非控制区域即PDSCH区域开始映射资源   nOfdmSymb=14
		{
			for (iCarr=0;iCarr<nCarrUsed;iCarr++)//频域上的资源映射      nCarrUsed=16*12
			{
				if (PilotLocation(iCarr,iOfdm)==0)//在非导频符号上
				{

					assert(iSymbol<modulate_code.length());
					assert((iCarr)<AvailableSubCarrPos.length());
					assert(AvailableSubCarrPos(iCarr)<Mapped_Code.rows());
					assert(iOfdm<Mapped_Code.cols());
					assert(iSymbol<modulate_code.length());


					Mapped_Code(AvailableSubCarrPos(iCarr),iOfdm)=modulate_code(iSymbol);   //插入数据
					iSymbol++;

				}
				else {
                        assert(iSymbol<modulate_code.length());
                        assert((iCarr+startCarr_Pos)<AvailableSubCarrPos.length());
					    assert(AvailableSubCarrPos(iCarr+startCarr_Pos)<Mapped_Code.rows());
					    assert(iOfdm<Mapped_Code.cols());
				     	assert(iSymbol<modulate_code.length());
                        Mapped_Code(AvailableSubCarrPos(iCarr),iOfdm)=100;     //插入导频

				}
			}
		}




	return Mapped_Code;



}
 imat find_pilot(iUE* m_iUE)
{
	int nOfdmSymbol=14;//每个子帧内为14个ofdm符号
	int cellID=m_iUE->nUE_param.m_BS_param.cellID;
	int nTx=m_iUE->nUE_param.m_BS_param.nAnt;
	int nRB=m_iUE->nUE_param.m_UE_param.nRB;
	int iSubFrame=m_iUE->nUE_param.iSubFrame;
	int scPerRB=12;//每个资源快包含12个子载波
	imat pilotPos=zeros_i(scPerRB*nRB,nOfdmSymbol);                 //192(12*16)*14
	int ns=iSubFrame*2;
	int N_symb_DL=7;
	int v;int vShift;int k;ivec L;
	for (int m=0;m<2*nRB;m++)
	{
		for (int p=0;p<nTx;p++)
		{
			if (p==0&&m==0)
			{
					 v=0;
					 vShift=mod(cellID,6);
					 k=6*m+mod(v+vShift,6);
					 L="0 4";//L=[0 N_symb_DL-3]
			}
			else if (p==0&&m!=0)
			{
				v=3;
				vShift=mod(cellID,6);
				k=6*m+mod(v+vShift,6);
				L="0 4";//L=[0 N_symb_DL-3]
			}
			else if (p==1&&m==0)
			{
					v=3;
					vShift=6*m+mod(v+vShift,6);
					L="0 4";
			}
			else if (p==1&&m!=0)
			{
				v=0;
				vShift=6*m+mod(v+vShift,6);
				L="0 4";
			}
			else if (p==2)
			{
				v=3*mod(ns,2);
				vShift = mod(cellID,6);
			    k =6*m+mod(v+vShift,6);
				L="1";//p<{2,3} L=1

			}
			else if (p==3)
			{
				v=3+3*mod(ns,2);
				vShift = mod(cellID,6);
				k =6*m+mod(v+vShift,6);
				L="1";//p<{2,3} L=1
			}
			if (L.size()==1)
			{
					pilotPos(k,L(0))=1;//第一个时隙
					pilotPos(k,L(0)+N_symb_DL)=1;//第二个时隙
			}
			else
			{
				pilotPos(k,L(0))=1;//第一个时隙
				pilotPos(k,L(0)+N_symb_DL)=1;//第二个时隙
				pilotPos(k,L(1))=1;//第一个时隙
				pilotPos(k,L(1)+N_symb_DL)=1;//第二个时隙

			}




		}
	}

	return pilotPos;

}
