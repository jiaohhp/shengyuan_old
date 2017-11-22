#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "datacenter.h"


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
void DataCenterDestroy(void* pthis)
{
	TDataCenter *ptDc = (TDataCenter *)pthis;
    if (ptDc != NULL)
    {
		pthread_mutex_lock(&(ptDc->m_tLock));
		
        TDataFIFO *ptFifo;
        for (ptFifo = ptDc->m_ptFifo; ptFifo != NULL; ptFifo = ptFifo->m_ptNext)
        {
			DataFifoClear(ptFifo);
        }
		
		pthread_mutex_unlock(&(ptDc->m_tLock));

		pthread_mutex_destroy(&(ptDc->m_tLock));
        free(ptDc);
		ptDc = NULL;
    }
}

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
void DataCenterBindFIFO(TDataFIFO *_ptFifo, void *pthis)
{
	TDataCenter *_ptDc = (TDataCenter *)pthis;
    if (_ptFifo != NULL && _ptDc != NULL)
    {
        TDataFIFO *ptFifo;
		pthread_mutex_lock(&(_ptDc->m_tLock));
        DataFifoLock(_ptFifo);
        ptFifo = _ptDc->m_ptFifo;
        if (ptFifo != NULL)
        {
            _ptFifo->m_ptNext 	= ptFifo;
            _ptDc->m_ptFifo 	= _ptFifo;
        }
        else
        {
            _ptDc->m_ptFifo 	= _ptFifo;
            _ptFifo->m_ptNext 	= NULL;
        }

        DataFifoUnlock(_ptFifo);
		pthread_mutex_unlock(&(_ptDc->m_tLock));
    }
}

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
void DataCenterUnBindFIFO(TDataFIFO *_ptFifo, void *pthis)
{
	TDataCenter *_ptDc = (TDataCenter *)pthis;
    if (_ptFifo != NULL && _ptDc != NULL)
    {
        TDataFIFO *ptPre 		= NULL;
        TDataFIFO *ptFifo 	= NULL;
        TDataFIFO *ptCur 		= NULL;

		pthread_mutex_lock(&(_ptDc->m_tLock));
        DataFifoLock(_ptFifo);

        for (ptCur = _ptDc->m_ptFifo; ptCur != NULL; ptCur = ptCur->m_ptNext)
        {
            if (ptCur == _ptFifo)
            {
                ptFifo = ptCur;
                break;
            }

            ptPre = ptCur;
        }

        if (ptFifo != NULL)
        {
            if (ptPre != NULL)
            {
                ptPre->m_ptNext = ptFifo->m_ptNext;
            }
            else
            {
                _ptDc->m_ptFifo = ptFifo->m_ptNext;
            }
        }

        DataFifoUnlock(_ptFifo);
		pthread_mutex_unlock(&(_ptDc->m_tLock));

    }
}

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
void PushData(DataPakg *_ptPkg, void *pthis)
{
	TDataCenter *_ptDc = (TDataCenter *)pthis;
    if (_ptPkg != NULL && _ptDc != NULL)
    {
        if (_ptPkg->m_iDataSize > _ptPkg->m_iCreateSize)
        {
            //printf(" %s %s %d m_iDataSize %d m_iCreateSize %d",__FILE__,__func__,__LINE__,_ptPkg->m_iDataSize,_ptPkg->m_iCreateSize);
            return ;
        }
		
        TDataFIFO *ptFifo;
		
		pthread_mutex_lock(&(_ptDc->m_tLock));

		//锁包
		
		DataPkgRefer((void*)_ptPkg);
		
        for (ptFifo = _ptDc->m_ptFifo; ptFifo != NULL; ptFifo = ptFifo->m_ptNext)
        {
			DataFifoPush(_ptPkg,ptFifo);
        }
		
		DataPkgRelease((void*)_ptPkg);
		
		//解包锁

		pthread_mutex_unlock(&(_ptDc->m_tLock));
    }
}

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
void DataCenterClear(void *pthis)
{
	TDataCenter *ptDc = (TDataCenter *)pthis;
    if (ptDc != NULL)
    {
		pthread_mutex_lock(&(ptDc->m_tLock));
		
        TDataFIFO *ptFifo;
        for (ptFifo = ptDc->m_ptFifo; ptFifo != NULL; ptFifo = ptFifo->m_ptNext)
        {
			DataFifoClear(ptFifo);
        }
		
		pthread_mutex_unlock(&(ptDc->m_tLock));
    }
}

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
TDataCenter *DataCenterCreate(void)
{
    TDataCenter *ptDc = (TDataCenter *)malloc(sizeof(TDataCenter));

    if (ptDc != NULL)
    {
        memset(ptDc, 0, sizeof(*ptDc));
		pthread_mutex_init(&(ptDc->m_tLock), NULL);
    }

    return ptDc;
}

