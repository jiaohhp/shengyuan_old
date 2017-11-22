#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "datacenter.h"


/*
* ��������	��	�����������ٺ���
* ��Ա����	:	��
* �������	��	pthis	 ָ���������ĵ�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	��fifo�󶨵���������
* ��Ա����	:	��
* �������	��	_ptFifo 	ָ��fifo��ָ��
				pthis	 	ָ���������ĵ�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	��fifo���������Ľ����
* ��Ա����	:	��
* �������	��	_ptFifo 	ָ��fifo��ָ��
				pthis	 	ָ���������ĵ�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	�����ݰ�������������
* ��Ա����	:	��
* �������	��	_ptPkg ���ݰ�ָ��
				pthis	 ָ���������ĵ�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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

		//����
		
		DataPkgRefer((void*)_ptPkg);
		
        for (ptFifo = _ptDc->m_ptFifo; ptFifo != NULL; ptFifo = ptFifo->m_ptNext)
        {
			DataFifoPush(_ptPkg,ptFifo);
        }
		
		DataPkgRelease((void*)_ptPkg);
		
		//�����

		pthread_mutex_unlock(&(_ptDc->m_tLock));
    }
}

/*
* ��������	��	������������е�����
* ��Ա����	:	��
* �������	�� 	pthis	 ָ���������ĵ�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
* ��������	��	������������е�����
* ��Ա����	:	��
* �������	�� 	��
* �������	��	��
* �� �� ֵ	��	�ɹ�����ָ���������ĵ�ָ��
				ʧ�ܷ���NULL
* ����˵��	��	��
* �޸�����	:	2013.08.29
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

