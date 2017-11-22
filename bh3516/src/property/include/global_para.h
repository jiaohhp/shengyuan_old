#ifndef __GLOBAL_PARA_H__
#define __GLOBAL_PARA_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "datapool.h"
#include "thread_pool.h"
#include "chn_venc.h"

extern CThread_pool_t* 	g_psThreadPool;
extern apr_allocator_t* 		g_psArpAllocator;
extern VENC_CTRL_INFO	g_sSensorVenc[2];
extern TDataFIFO*				g_cmdFifo;
extern TDataFIFO*  			g_pTEMP_FIFO;	//温度数据	fifo
extern TDataFIFO*  			g_pHUMI_FIFO;	//湿度数据	fifo
extern TDataFIFO*  			g_pXACC_FIFO;	//X轴加速度fifo
extern TDataFIFO*  			g_pYACC_FIFO;	//Y轴加速度fifo
extern TDataFIFO*  			g_pZACC_FIFO;	//Z轴加速度fifo
extern TDataFIFO*  			g_pELEC_FIFO;	//电流数据	fifo
extern TDataFIFO*  			g_pHYD1_FIFO;	//液压1数据fifo
extern TDataFIFO*  			g_pHYD2_FIFO;	//液压2数据fifo
extern TDataFIFO*  			g_pLIQI_FIFO;		//液位数据	fifo
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
void global_para_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

