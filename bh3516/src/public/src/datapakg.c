#include <string.h>
#include <stdio.h>
#include "datapakg.h"
#include "datapool.h"
#include "printf_func.h"

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
int DataPkgRefer(void* pthis)
{	
	DataPakg *ptPkg = (DataPakg *)pthis;
    if (ptPkg != NULL)
    {
		AllocatorNodeLock((void *)ptPkg);
        ptPkg->m_iRefCnt++;
		AllocatorNodeUnlock((void *)ptPkg);
    }
	else
	{
		return -1;
	}

	return 0;
}

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
int DataPkgRelease(void* pthis) //���ݰ�������
{
	DataPakg *ptPkg = (DataPakg *)pthis;
    if (ptPkg != NULL)
    {
		AllocatorNodeLock((void *)ptPkg);
        if (ptPkg->m_iRefCnt < 0)
        {
        	ptf_err("DataPkgRelease again");
			AllocatorNodeUnlock((void *)ptPkg);
        }
		else if (0 == ptPkg->m_iRefCnt)
		{
			AllocatorNodeUnlock((void *)ptPkg);
			AllocatorFree((void *)ptPkg);
		}
        else
        {
            ptPkg->m_iRefCnt--;
			AllocatorNodeUnlock((void *)ptPkg);
        }
    }
	else
	{
		return -1;	
	}

	return 0;
}

DataPakg* DataPkgCreate(apr_allocator_t *pool,int size)
{
	DataPakg* pkg = AllocatorAlloc((void*)pool,size + sizeof(DataPakg));
	if (NULL == pkg)
	{
		return NULL;
	}
	else
	{
		memset(pkg, 0, sizeof(DataPakg));
		pkg->m_datatype 	= INVALID_DATA_TYPE;
		pkg->m_iCreateSize 	= size;
		pkg->m_ower 		= pool;
		pkg->m_bData 		= (char*)pkg + sizeof(DataPakg);
		pkg->m_iRefCnt 		= 0;
	}

	return pkg;
}

