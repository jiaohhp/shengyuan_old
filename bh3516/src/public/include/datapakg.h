#ifndef __DATA_PAKG_H__
#define __DATA_PAKG_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>

#include "datapool.h"

#define INVALID_DATA_TYPE (-1)

typedef struct DataPakg_t
{
	apr_allocator_t *	m_ower;				//���������ڴ��
	int 				m_iRefCnt;					//���ݰ����ü�����
	int 				m_iDataSize;				//��ʵ�����ݳ���
	int 				m_iCreateSize;			//�������ݰ���С
	int				m_datatype;					//��ʶ��������
	char				m_datainfo[64];			//���������Ϣ
	char 			*m_bData;					//���ݰ���ŵ�ַ
}DataPakg;

/*
* ��������	��	���ݰ����ü�������
* ��Ա����	:	��
* �������	��	ptPkg	����DataPkgCreate���ɵ����ݰ�
* �������	��	��
* �� �� ֵ	��	0	�ɹ�
					-1	ʧ��
* ����˵��	��	��
* �޸�����	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
int DataPkgRefer(void* pthis);

/*
* ��������	��	�ͷ����ݰ�
* ��Ա����	:	��
* �������	��	ptPkg	����DataPkgCreate���ɵ����ݰ�
* �������	��	��
* �� �� ֵ	��	0	�ɹ�
					-1	ʧ��

* ����˵��	��	�÷�����DataPkgCreate���ʹ�ü�����DataPkgCreate�����Ŀռ���Release�����ͷ�
* �޸�����	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
int DataPkgRelease(void* pthis);


/*
* ��������	��	�������ݰ�
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
DataPakg* DataPkgCreate(apr_allocator_t *pool,int size);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


