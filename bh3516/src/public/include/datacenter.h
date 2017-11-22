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
void DataCenterDestroy(void* pthis);

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
void DataCenterBindFIFO(TDataFIFO *_ptFifo, void *pthis);

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
void DataCenterUnBindFIFO(TDataFIFO *_ptFifo, void *pthis);

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
void PushData(DataPakg *_ptPkg, void *pthis);

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
void DataCenterClear(void *pthis);

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
TDataCenter *DataCenterCreate(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

