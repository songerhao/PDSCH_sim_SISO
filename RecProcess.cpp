#include "RecProcess.h"
using namespace itpp;
bvec Reciever(cvec Rec_Code,double SNRdB,iUE*m_iUE)
{
	bvec Rec_Info_Bit;
	bvec err_output;//记录传输块出错的向量
	int nRx=m_iUE->nUE_param.m_UE_param.nRx;
	int nOfdm=14;
	int Qm=m_iUE->nUE_param.m_UE_param.Qm;
	int nTx=m_iUE->nUE_param.m_BS_param.nAnt;
	int nTB=1;
	int RNIT=m_iUE->nUE_param.m_UE_param.RNIT;
	int sizeTB1=m_iUE->nUE_param.m_UE_param.TB.TB1.sizeTB;
	int CellID=m_iUE->nUE_param.m_BS_param.cellID;
	int iSubFrame=m_iUE->nUE_param.iSubFrame;
	int M_0_SYMB=m_iUE->nUE_param.m_UE_param.M_0_SYMB;
	ivec nCB=m_iUE->nUE_param.m_UE_param.nCB;
	ivec E=m_iUE->nUE_param.m_UE_param.E;
    cmat RecOfdmed_Code;
    RecOfdmed_Code=de_LTE_Ofdm(Rec_Code,m_iUE);                     //OFDM解调
		//std::cout<<"size of mat after de_ofdm: "<<"columns: "<<RecOfdmed_Code.cols()<<"   "<<"Rows:  "<<RecOfdmed_Code.rows()<<std::endl;    2048*14
    cmat de_SubCarrMapped_Code;
    de_SubCarrMapped_Code=de_SubCarrier_Mapping(RecOfdmed_Code,m_iUE);
//
//
//
//		//std::cout<<"de_SubCarrMapped_Code:"<<de_SubCarrMapped_Code(0)<<std::endl;
//		/*
//		 *	信道估计
//		 */
//		Array<cmat> H_Channel_Est;
//		H_Channel_Est=Channel_Est_Real(H_Channel,m_iUE);
//         //std::cout<<H_Channel_Est<<std::endl;
//		/*
//		 *	去预编码
//		 */
//		Array<cvec>de_Precoded_Code;
//		de_Precoded_Code=de_Precoding(de_SubCarrMapped_Code,m_iUE,SNRdB,H_Channel_Est);
//		//std::cout<<de_Precoded_Code<<std::endl;
//		/*
//		 *	去层映射
//		 */
//		Array<cvec>de_LayerMapped_Code;
//		de_LayerMapped_Code=de_LayerMapping(de_Precoded_Code,m_iUE);
//		assert(de_LayerMapped_Code.length()==nTB);
//
//		for (int iTB=0;iTB<nTB;iTB++)
//		{
//			/*
//			 *	开始解调制
//			 */
//			cvec Modulated_Code;
//			Modulated_Code=de_LayerMapped_Code(iTB);
//			bvec de_Modulated_Code=de_Modulate(Modulated_Code,m_iUE);
//			/*
//			 *	开始解扰码
//			 */
//			bvec Scrambled_Code;
//			Scrambled_Code=de_LTE_Scramble(de_Modulated_Code,m_iUE,iTB);
//			/*
//			 *	开始记录出错信息
//			 */
//			err_output=zeros_b(nCB(iTB));
//			/*
//			 *	获取当前用户当前iCB的码块数,并将所有码块合并在一起送给解速率匹配段
//			 */
//			Array<bvec>RateMatched_Code;
//			RateMatched_Code.set_size(nCB(iTB));
//			for (int iCB=0;iCB<nCB(iTB);iCB++)
//			{
//				int start=iCB*E(iTB);
//				int end=start+E(iTB);
//				assert(end>Scrambled_Code.length());
//				RateMatched_Code(iCB)=Scrambled_Code.get(start,end);
//			}
//
//				Array<Array<bvec>>de_RateMatched_Code;
//				de_RateMatched_Code=de_RateMatching(RateMatched_Code,m_iUE,iTB);
//				/*
//				 *	接下来完成解Turbo的过程
//				 */
//				Array<bvec>de_Turboed_Code;
//				de_Turboed_Code=de_Turbo_Code(de_RateMatched_Code,SNRdB,m_iUE);
//				/*
//				 *	接下来开始得到原始消息
//				 */
//
//
//				Rec_Info_Bit.zeros();
//				if (nCB(iTB)>1)//不止一个码块
//				{
//					for (int iCB=0;iCB<nCB(iTB);iCB++)
//					{
//						bvec InfoBit=CRC24b_Check(de_Turboed_Code(iCB));
//						printf("the length of crcbit is:%d\n",InfoBit.length());
//						Rec_Info_Bit=concat(Rec_Info_Bit,InfoBit);
//					}
//
//				}
//				else
//					{
//						assert(de_Turboed_Code.length()==1);//只有一个码块
//						Rec_Info_Bit=CRC24a_Check(de_Turboed_Code(0));
//
//					}
//				if (nCB(iTB)>0)
//				{
//
//					Rec_Info_Bit=CRC24a_Check(Rec_Info_Bit);
//				}
//			}


	return Rec_Info_Bit;
}
