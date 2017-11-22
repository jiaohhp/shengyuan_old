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
extern TDataFIFO*  			g_pTEMP_FIFO;	//�¶�����	fifo
extern TDataFIFO*  			g_pHUMI_FIFO;	//ʪ������	fifo
extern TDataFIFO*  			g_pXACC_FIFO;	//X����ٶ�fifo
extern TDataFIFO*  			g_pYACC_FIFO;	//Y����ٶ�fifo
extern TDataFIFO*  			g_pZACC_FIFO;	//Z����ٶ�fifo
extern TDataFIFO*  			g_pELEC_FIFO;	//��������	fifo
extern TDataFIFO*  			g_pHYD1_FIFO;	//Һѹ1����fifo
extern TDataFIFO*  			g_pHYD2_FIFO;	//Һѹ2����fifo
extern TDataFIFO*  			g_pLIQI_FIFO;		//Һλ����	fifo
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
void global_para_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

