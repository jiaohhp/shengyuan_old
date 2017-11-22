#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "datafifo.h"
#include "datapakg.h"

/*
* ��������	��	�����ݰ�����fifo
* ��Ա����	:	��
* �������	��	ptPkg	����DataPkgCreate���������ݰ�
					pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
			//�����ɰ�
            int iIndx = (ptFifo->m_iBase++) % ptFifo->m_iMaxPkgCnt;
            DataPakg *ptOld = ptFifo->m_ptBuf[iIndx];
			DataPkgRelease((void*)ptOld);

			//�����°�
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
* ��������	��	��ȡfifo�����ݰ�������
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	��fifo���Ƴ�һ�����ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	��fifo�и��Ƴ�һ�����ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	�ɹ�     ����ָ�����ݵ�ָ��
				ʧ��	 ����NULL
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	��fifo�л�ȡһ�����ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	�ɹ�     ����ָ�����ݵ�ָ��
				ʧ��	 ����NULL
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	���fifo��������ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	����fifo
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	Fifo����
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	Fifo����
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	����fifo���캯��
* ��Ա����	:	��
* �������	��	pool		Ҫ�������ݰ����ڴ��ָ��
					size		Ҫ���������ݵ����ݰ���С
* �������	��	��
* �� �� ֵ	��	�ǿ�ָ��	ָ�����ݰ���ָ��
					NULL			����ʧ��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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

