#ifndef _DATA_CENTER_H_
#define _DATA_CENTER_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>

#include "datafifo.h"

typedef struct
{
	
	pthread_mutex_t 	m_tLock;
	TDataFIFO 			*m_ptFifo;	
} TDataCenter;


/*
* 功能描述	：	数据中心销毁函数
* 成员更新	:	无
* 输入参数	：	pthis	 指向数据中心的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataCenterDestroy(void* pthis);

/*
* 功能描述	：	将fifo绑定到数据中心
* 成员更新	:	无
* 输入参数	：	_ptFifo 	指向fifo的指针
				pthis	 	指向数据中心的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataCenterBindFIFO(TDataFIFO *_ptFifo, void *pthis);

/*
* 功能描述	：	将fifo从数据中心解除绑定
* 成员更新	:	无
* 输入参数	：	_ptFifo 	指向fifo的指针
				pthis	 	指向数据中心的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataCenterUnBindFIFO(TDataFIFO *_ptFifo, void *pthis);

/*
* 功能描述	：	将数据包放入数据中心
* 成员更新	:	无
* 输入参数	：	_ptPkg 数据包指针
				pthis	 指向数据中心的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void PushData(DataPakg *_ptPkg, void *pthis);

/*
* 功能描述	：	情况数据中心中的数据
* 成员更新	:	无
* 输入参数	： 	pthis	 指向数据中心的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataCenterClear(void *pthis);

/*
* 功能描述	：	情况数据中心中的数据
* 成员更新	:	无
* 输入参数	： 	无
* 输出参数	：	无
* 返 回 值	：	成功返回指向数据中心的指针
				失败返回NULL
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
TDataCenter *DataCenterCreate(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

