//标准头文件包含区域

//自定义头文件包含区域
#include "global_para.h"

//常量定义区
//文件内部使用的宏

//文件内部使用的数据类型

//全局变量
CThread_pool_t* 	g_psThreadPool 		= NULL;
apr_allocator_t* 		g_psArpAllocator 	= NULL;
VENC_CTRL_INFO	g_sSensorVenc[2];
TDataFIFO*				g_cmdFifo 				= NULL;	//命令fifo,用来存放控制命令包

TDataFIFO*  			g_pTEMP_FIFO 		= NULL;	//温度数据	fifo
TDataFIFO*  			g_pHUMI_FIFO 		= NULL;	//湿度数据	fifo
TDataFIFO*  			g_pXACC_FIFO 		= NULL;	//X轴加速度fifo
TDataFIFO*  			g_pYACC_FIFO 		= NULL;	//Y轴加速度fifo
TDataFIFO*  			g_pZACC_FIFO 		= NULL;	//Z轴加速度fifo
TDataFIFO*  			g_pELEC_FIFO 		= NULL;	//电流数据	fifo
TDataFIFO*  			g_pHYD1_FIFO 		= NULL;	//液压1数据fifo
TDataFIFO*  			g_pHYD2_FIFO 		= NULL;	//液压2数据fifo
TDataFIFO*  			g_pLIQI_FIFO 			= NULL;	//液位数据	fifo

//静态局部变量

//内部函数

//外部函数

/*
* 功能描述	：	模块初始化
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.08
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




