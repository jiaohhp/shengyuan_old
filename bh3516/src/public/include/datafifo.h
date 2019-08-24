#ifndef __DATA_FIFO_H__
#define __DATA_FIFO_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>

#include "datapakg.h"
typedef enum
{
	FIFO_PRI_CYC = 1,	//缓存环，缓存溢出时循环覆盖旧数据
	FIFO_PRI_COM = 2,	//普通FIFO，缓存溢出时丢弃新数据
	FIFO_PRI_SEQ = 3,	//序列化FIFO，无数据丢失，缓存溢出时阻塞其它操作
}Pri;

typedef struct TDataFIFO_t TDataFIFO;
struct TDataFIFO_t
{
	TDataFIFO 			*m_ptNext;
	pthread_mutex_t 	m_tLock;
	int 				m_iPri;			// 数据处理优先级
	int 				m_iBase;		// fifo base指针
	int 				m_iTop;			// fifo top指针
	int 				m_iMaxPkgCnt;	//最大可缓存包数
	DataPakg 			*m_ptBuf[0];	//该成员必须放在最后，否则会内存覆盖
} ;

/*
* 功能描述	：	将数据包放入fifo
* 成员更新	:	无
* 输入参数	：	ptPkg		调用DataPkgCreate产生的数据包
				pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoPush(DataPakg *ptPkg, TDataFIFO *pthis);

/*
* 功能描述	：	获取fifo中数据包的数量
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
int DataFifoGetPkgsNum(TDataFIFO* pthis);

/*
* 功能描述	：	从fifo中移除一个数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoMove(TDataFIFO* pthis);

/*
* 功能描述	：	从fifo中复制出一个数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	成功     	返回指向数据的指针
				失败	 	返回NULL
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoCopy(TDataFIFO* pthis);

/*
* 功能描述	：	从fifo中获取一个数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	成功     	返回指向数据的指针
				失败	 	返回NULL
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoGet(TDataFIFO* pthis);

/*
* 功能描述	：	清空fifo里面的数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoClear(TDataFIFO* pthis);

/*
* 功能描述	：	销毁fifo
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoDestruct(TDataFIFO* pthis);

/*
* 功能描述	：	Fifo加锁
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoLock(TDataFIFO* pthis);

/*
* 功能描述	：	Fifo解锁
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoUnlock(TDataFIFO* pthis);

/*
* 功能描述	：	数据fifo构造函数
* 成员更新	:	无
* 输入参数	：	pool		要申请数据包的内存池指针
				size		要申请存放数据的数据包大小
* 输出参数	：	无
* 返 回 值	：	非空指针	指向数据包的指针
				NULL		申请失败
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
TDataFIFO *DataFifoConstruct(Pri _iPri, int _iSize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
