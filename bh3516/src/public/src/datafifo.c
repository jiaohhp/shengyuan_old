#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "datafifo.h"
#include "datapakg.h"

/*
* 功能描述	：	将数据包放入fifo
* 成员更新	:	无
* 输入参数	：	ptPkg	调用DataPkgCreate产生的数据包
					pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoPush(DataPakg *ptPkg, TDataFIFO *ptFifo)
{
    if (ptPkg != NULL && ptFifo != NULL)
    {
        if (ptPkg->m_iDataSize > ptPkg->m_iCreateSize)
        {
            ptf_err("m_iDataSize %d m_iCreateSize %d ptPkg 0x%x ptFifo 0x%x\n",ptPkg->m_iDataSize,ptPkg->m_iCreateSize,ptPkg,ptFifo);
            return ;
        }
        int iCnt;
		
		pthread_mutex_lock(&(ptFifo->m_tLock));

        iCnt = ptFifo->m_iTop - ptFifo->m_iBase;
        if (iCnt < ptFifo->m_iMaxPkgCnt)
        {
            int iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
            ptFifo->m_ptBuf[iIndx] = ptPkg;
			DataPkgRefer((void*)ptPkg);
        }
        else if (FIFO_PRI_CYC == ptFifo->m_iPri)
        {
           	//ptf_errs("m_iDataSize %d m_iCreateSize %d ptPkg 0x%x ptFifo 0x%x\n",ptPkg->m_iDataSize,ptPkg->m_iCreateSize,ptPkg,ptFifo);
			//丢弃旧包
            int iIndx = (ptFifo->m_iBase++) % ptFifo->m_iMaxPkgCnt;
            DataPakg *ptOld = ptFifo->m_ptBuf[iIndx];
			DataPkgRelease((void*)ptOld);

			//更新新包
            iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
			ptFifo->m_ptBuf[iIndx] = ptPkg;
			DataPkgRefer((void*)ptPkg);
        }
        else if (FIFO_PRI_SEQ == ptFifo->m_iPri)
        {
            int iIndx;

            do
            {
				pthread_mutex_unlock(&(ptFifo->m_tLock));
                usleep(10);
				pthread_mutex_lock(&(ptFifo->m_tLock));
                iCnt = ptFifo->m_iTop - ptFifo->m_iBase;
            }
            while (iCnt >= ptFifo->m_iMaxPkgCnt);

            iIndx = (ptFifo->m_iTop++) % ptFifo->m_iMaxPkgCnt;
            ptFifo->m_ptBuf[iIndx] = ptPkg;
			DataPkgRefer((void*)ptPkg);
        }

		pthread_mutex_unlock(&(ptFifo->m_tLock));
		
    }
}

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
int DataFifoGetPkgsNum(TDataFIFO *ptFifo)
{	
    int PacketsNum = 0;
	
    if (ptFifo != NULL)
    {
		pthread_mutex_lock(&(ptFifo->m_tLock));
        PacketsNum = ptFifo->m_iTop - ptFifo->m_iBase;
		pthread_mutex_unlock(&(ptFifo->m_tLock));
    }
	
    return PacketsNum;
}

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
void DataFifoMove(TDataFIFO *ptFifo)
{
    DataPakg *ptPkg = NULL;
    if (ptFifo != NULL)
    {
		pthread_mutex_lock(&(ptFifo->m_tLock));

        if (ptFifo->m_iTop > ptFifo->m_iBase)
        {
            int iIndx = (ptFifo->m_iBase++) % ptFifo->m_iMaxPkgCnt;
            ptPkg = ptFifo->m_ptBuf[iIndx];			
			DataPkgRelease((void*)ptPkg);
            if (ptFifo->m_iBase == ptFifo->m_iTop)
            {
                ptFifo->m_iBase = 0;
                ptFifo->m_iTop 	= 0;
            }
        }

		pthread_mutex_unlock(&(ptFifo->m_tLock));
    }

}


/*
* 功能描述	：	从fifo中复制出一个数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	成功     返回指向数据的指针
				失败	 返回NULL
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoCopy(TDataFIFO *ptFifo)
{	
    DataPakg *ptPkg = NULL;

    if (ptFifo != NULL)
    {
		pthread_mutex_lock(&(ptFifo->m_tLock));

        if (ptFifo->m_iTop > ptFifo->m_iBase)
        {
            int iIndx = (ptFifo->m_iBase) % ptFifo->m_iMaxPkgCnt;
            ptPkg = ptFifo->m_ptBuf[iIndx];
        }

		pthread_mutex_unlock(&(ptFifo->m_tLock));
    }

    return ptPkg;
}


/*
* 功能描述	：	从fifo中获取一个数据包
* 成员更新	:	无
* 输入参数	：	pthis		指向fifo的this 指针
* 输出参数	：	无
* 返 回 值	：	成功     返回指向数据的指针
				失败	 返回NULL
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoGet(TDataFIFO *ptFifo)
{
    DataPakg *ptPkg = NULL;

    if (ptFifo != NULL)
    {
		pthread_mutex_lock(&(ptFifo->m_tLock));

        if (ptFifo->m_iTop > ptFifo->m_iBase)
        {
            int iIndx = (ptFifo->m_iBase++) % ptFifo->m_iMaxPkgCnt;

            ptPkg = ptFifo->m_ptBuf[iIndx];
            if (ptFifo->m_iBase == ptFifo->m_iTop)
            {
                ptFifo->m_iBase = 0;
                ptFifo->m_iTop 	= 0;
            }
        }

		pthread_mutex_unlock(&(ptFifo->m_tLock));
    }

    return ptPkg;
}


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
void DataFifoClear(TDataFIFO *ptFifo)
{
    if (ptFifo != NULL)
    {
        int i;
		pthread_mutex_lock(&(ptFifo->m_tLock));

        for (i = ptFifo->m_iBase; i < ptFifo->m_iTop; i++)
        {
            int iIndx = i % ptFifo->m_iMaxPkgCnt;
            DataPakg *ptPkg = ptFifo->m_ptBuf[iIndx];			
			DataPkgRelease((void*)ptPkg);
        }

        ptFifo->m_iBase 	= 0;
        ptFifo->m_iTop 		= 0;

		pthread_mutex_unlock(&(ptFifo->m_tLock));
    }
}

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
void DataFifoDestruct(TDataFIFO *ptFifo)
{
    if (ptFifo != NULL)
    {
        DataFifoClear(ptFifo);
        free(ptFifo);
    }
}

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
void DataFifoLock(TDataFIFO *ptFifo)
{
    if (ptFifo != NULL)
    {
		pthread_mutex_lock(&(ptFifo->m_tLock));

    }
}

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
void DataFifoUnlock(TDataFIFO *ptFifo)
{
    if (ptFifo != NULL)
    {
		pthread_mutex_unlock(&(ptFifo->m_tLock));

    }
}

/*
* 功能描述	：	数据fifo构造函数
* 成员更新	:	无
* 输入参数	：	pool		要申请数据包的内存池指针
					size		要申请存放数据的数据包大小
* 输出参数	：	无
* 返 回 值	：	非空指针	指向数据包的指针
					NULL			申请失败
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
TDataFIFO *DataFifoConstruct(Pri _iPri, int _iSize)
{
	if (_iSize <= 0)
	{
		//printf("%s %s %d _iSize %d\n",__FILE__,__func__,__LINE__,_iSize);
		return NULL;
	}
	
    TDataFIFO *ptFifo = (TDataFIFO *)malloc(sizeof(TDataFIFO) + _iSize * sizeof(DataPakg *));

    if (ptFifo != NULL)
    {
        memset(ptFifo, 0, sizeof(TDataFIFO) + _iSize * sizeof(DataPakg *));
        ptFifo->m_iMaxPkgCnt 	= _iSize;
        ptFifo->m_iPri 			= _iPri;
		pthread_mutex_init(&(ptFifo->m_tLock), NULL);
    }

    return ptFifo;
}

