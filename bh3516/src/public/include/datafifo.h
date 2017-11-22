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
	FIFO_PRI_CYC = 1,	//���滷���������ʱѭ�����Ǿ�����
	FIFO_PRI_COM = 2,	//��ͨFIFO���������ʱ����������
	FIFO_PRI_SEQ = 3,	//���л�FIFO�������ݶ�ʧ���������ʱ������������
}Pri;

typedef struct TDataFIFO_t TDataFIFO;
struct TDataFIFO_t
{
	TDataFIFO 			*m_ptNext;
	pthread_mutex_t 	m_tLock;
	int 				m_iPri;			// ���ݴ������ȼ�
	int 				m_iBase;		// fifo baseָ��
	int 				m_iTop;			// fifo topָ��
	int 				m_iMaxPkgCnt;	//���ɻ������
	DataPakg 			*m_ptBuf[0];	//�ó�Ա���������󣬷�����ڴ渲��
} ;

/*
* ��������	��	�����ݰ�����fifo
* ��Ա����	:	��
* �������	��	ptPkg		����DataPkgCreate���������ݰ�
				pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
void DataFifoPush(DataPakg *ptPkg, TDataFIFO *pthis);

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
int DataFifoGetPkgsNum(TDataFIFO* pthis);

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
void DataFifoMove(TDataFIFO* pthis);

/*
* ��������	��	��fifo�и��Ƴ�һ�����ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	�ɹ�     	����ָ�����ݵ�ָ��
				ʧ��	 	����NULL
* ����˵��	��	��
* �޸�����	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoCopy(TDataFIFO* pthis);

/*
* ��������	��	��fifo�л�ȡһ�����ݰ�
* ��Ա����	:	��
* �������	��	pthis		ָ��fifo��this ָ��
* �������	��	��
* �� �� ֵ	��	�ɹ�     	����ָ�����ݵ�ָ��
				ʧ��	 	����NULL
* ����˵��	��	��
* �޸�����	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
DataPakg *DataFifoGet(TDataFIFO* pthis);

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
void DataFifoClear(TDataFIFO* pthis);

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
void DataFifoDestruct(TDataFIFO* pthis);

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
void DataFifoLock(TDataFIFO* pthis);

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
void DataFifoUnlock(TDataFIFO* pthis);

/*
* ��������	��	����fifo���캯��
* ��Ա����	:	��
* �������	��	pool		Ҫ�������ݰ����ڴ��ָ��
				size		Ҫ���������ݵ����ݰ���С
* �������	��	��
* �� �� ֵ	��	�ǿ�ָ��	ָ�����ݰ���ָ��
				NULL		����ʧ��
* ����˵��	��	��
* �޸�����	:	2013.08.29
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
