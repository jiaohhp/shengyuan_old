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
	apr_allocator_t *	m_ower;				//所归属的内存池
	int 				m_iRefCnt;					//数据包引用计数器
	int 				m_iDataSize;				//包实际数据长度
	int 				m_iCreateSize;			//申请数据包大小
	int				m_datatype;					//标识数据类型
	char				m_datainfo[64];			//存放类型信息
	char 			*m_bData;					//数据包存放地址
}DataPakg;

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
int DataPkgRefer(void* pthis);

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
int DataPkgRelease(void* pthis);


/*
* 功能描述	：	创建数据包
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
DataPakg* DataPkgCreate(apr_allocator_t *pool,int size);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


