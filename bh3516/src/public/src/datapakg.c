#include <string.h>
#include <stdio.h>
#include "datapakg.h"
#include "datapool.h"
#include "printf_func.h"

/*
* 功能描述	：	数据包引用计数增加
* 成员更新	:	无
* 输入参数	：	ptPkg	调用DataPkgCreate生成的数据包
* 输出参数	：	无
* 返 回 值	：	0	成功
				-1	失败
* 其它说明	：	无
* 修改日期	:	2013.08.29
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
* 功能描述	：	释放数据包
* 成员更新	:	无
* 输入参数	：	ptPkg	调用DataPkgCreate生成的数据包
* 输出参数	：	无
* 返 回 值	：	0	成功
				-1	失败

* 其它说明	：	该方法与DataPkgCreate配合使用即调用DataPkgCreate产生的空间由Release方法释放
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
int DataPkgRelease(void* pthis) //数据包解引用
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

