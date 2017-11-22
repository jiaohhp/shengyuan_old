//��׼ͷ�ļ���������

//�Զ���ͷ�ļ���������
#include "global_para.h"

//����������
//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���
CThread_pool_t* 	g_psThreadPool 		= NULL;
apr_allocator_t* 		g_psArpAllocator 	= NULL;
VENC_CTRL_INFO	g_sSensorVenc[2];
TDataFIFO*				g_cmdFifo 				= NULL;	//����fifo,������ſ��������

TDataFIFO*  			g_pTEMP_FIFO 		= NULL;	//�¶�����	fifo
TDataFIFO*  			g_pHUMI_FIFO 		= NULL;	//ʪ������	fifo
TDataFIFO*  			g_pXACC_FIFO 		= NULL;	//X����ٶ�fifo
TDataFIFO*  			g_pYACC_FIFO 		= NULL;	//Y����ٶ�fifo
TDataFIFO*  			g_pZACC_FIFO 		= NULL;	//Z����ٶ�fifo
TDataFIFO*  			g_pELEC_FIFO 		= NULL;	//��������	fifo
TDataFIFO*  			g_pHYD1_FIFO 		= NULL;	//Һѹ1����fifo
TDataFIFO*  			g_pHYD2_FIFO 		= NULL;	//Һѹ2����fifo
TDataFIFO*  			g_pLIQI_FIFO 			= NULL;	//Һλ����	fifo

//��̬�ֲ�����

//�ڲ�����

//�ⲿ����

/*
* ��������	��	ģ���ʼ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/
void global_para_init(void)
{
	g_psThreadPool 		= ThreadPoolConstructDefault();
	g_psArpAllocator 	= AllocatorPoolCreateDefault();
	
	g_sSensorVenc[0].m_ViDev		= 0;
	g_sSensorVenc[0].m_Sensor		= OV_9712_DC_1280_800P;
	g_sSensorVenc[0].m_ViChn		= 1;
	g_sSensorVenc[0].m_VencGrp	= 0;
	g_sSensorVenc[0].m_VencChn	= 0;
	g_sSensorVenc[0].m_DaceCenter	=  DataCenterCreate();	
	g_sSensorVenc[0].m_SnapExtViChn		=  3;	
	g_sSensorVenc[0].m_SnapExtVencGrp	=  3;	
	g_sSensorVenc[0].m_SnapExtVencChn	=  3;	

	g_sSensorVenc[1].m_ViDev		= 1;
	g_sSensorVenc[1].m_Sensor		= OV_2640_DC_800_600P;
	g_sSensorVenc[1].m_ViChn		= 2;
	g_sSensorVenc[1].m_VencGrp	= 1;
	g_sSensorVenc[1].m_VencChn	= 1;
	g_sSensorVenc[1].m_DaceCenter	=  DataCenterCreate();
	g_sSensorVenc[1].m_SnapExtViChn		=  4;	
	g_sSensorVenc[1].m_SnapExtVencGrp	=  4;	
	g_sSensorVenc[1].m_SnapExtVencChn	=  4;	


	g_cmdFifo = DataFifoConstruct(FIFO_PRI_CYC, 50);

//	g_pTEMP_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 50);
//	g_pHUMI_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pTEMP_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 1);
	g_pHUMI_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 1);

	g_pXACC_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pYACC_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pZACC_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pELEC_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pHYD1_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pHYD2_FIFO 	= DataFifoConstruct(FIFO_PRI_CYC, 50);
	//g_pLIQI_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 50);
	g_pLIQI_FIFO		= DataFifoConstruct(FIFO_PRI_CYC, 1);

	return ; 
}




