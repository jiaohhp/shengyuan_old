#ifndef __DATA_POOL_H__
#define __DATA_POOL_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>
#include "printf_func.h"

#define DEFAULT_MAX_INDEX		(256)
#define DEFAULT_MIN_ALLOC		(4096)
#define DEFAULT_ARPUINT32_MAX	(2048)
#define DEFAULT_BOUNDARY_INDEX	(12)


#define APR_ALLOCATOR_MAX_FREE_UNLIMITED 	0
#define APR_ALIGN(size, boundary) 			(((size) + ((boundary) - 1)) & ~((boundary) - 1))
#define APR_ALIGN_DEFAULT(size) 			APR_ALIGN(size, 8)
#define SIZEOF_ALLOCATOR_T  				APR_ALIGN_DEFAULT(sizeof(apr_allocator_t))
#define APR_MEMNODE_T_SIZE 					APR_ALIGN_DEFAULT(sizeof(apr_memnode_t))

typedef struct apr_allocator_t apr_allocator_t;
typedef struct apr_memnode_t apr_memnode_t;

struct apr_memnode_t 
{
    pthread_mutex_t 	m_tLock;
	apr_memnode_t 		*next;           		/**< next memnode */
	apr_memnode_t 		**ref;            		/**< reference to self */
	int   				index;           		/**< size */
	apr_allocator_t 	*m_pool;				//指向内存池的指针
	char 				*m_bData;				//返给用户的内存地址

	#ifdef PRINTF
	int					m_iFreeFlg;				//重复释放标识
	#endif
};

struct apr_allocator_t 
{
    int        			max_index;
    int					max_free_index;
    int       			current_free_index;
    pthread_mutex_t 	m_tLock;
    int         		*owner;
    apr_memnode_t     **free;

	int					m_MinAlloc;				//最小规则内存块大小
	int					m_MaxIndex;				//最大规则内存块索引
	int					m_ArpUint32Max;			//内存池内能存放的最大内存块索引
	int					m_BoundaryIndex;		//规则内存块的递增大小以2为底的指数
	int					m_BoundarySize;			//规则内存块的递增大小
};

/*
* 功能描述	：	内存池构析构函数
* 成员更新	:	无
* 输入参数	：	pthis	内存池指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorDestroy(apr_allocator_t *pthis);

/*
* 功能描述	：	从内存池申请内存
* 成员更新	:	无
* 输入参数	：	pthis	内存池指针
					_size	申请的内存大小
* 输出参数	：	无
* 返 回 值	：	指向申请的内存的指针
* 其它说明	：	无
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void *AllocatorAlloc(apr_allocator_t *pthis, int  _size);

/*
* 功能描述	：	释放内存到内存池
* 成员更新	:	无
* 输入参数	：	_node 从内存池申请的节点
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	内存不允许重复释放，重复一次导致内存覆盖的可能，重复两次函数打印出提示信息
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorFree(void* _node);

/*
* 功能描述	：	给节点加锁
* 成员更新	:	无
* 输入参数	：	_node 从内存池申请的节点
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorNodeLock(void* _node);

/*
* 功能描述	：	给节点解锁
* 成员更新	:	无
* 输入参数	：	_node 从内存池申请的节点
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorNodeUnlock(void* _node);

/*
* 功能描述	：	内存池构造函数
* 成员更新	:	无
* 输入参数	：	in_size			内存池能容纳的最大内存块数量，APR_ALLOCATOR_MAX_FREE_UNLIMITED代表数量不受限制
					MaxIndex		内存池中规则内存块的最大索引号
					MinAlloc			内存池规最小规则内存块的大小
					ArpUint32Max		内存池中能容纳的最大内存块的索引值
					BoundaryIndex	规则内存块的递增大小以2为底的指数
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.08.015
* -----------------------------------------------
* 2013/08/02		 V1.0		XXXX		  XXXX
*/
apr_allocator_t * AllocatorPoolCreate(int in_size,int MaxIndex,int MinAlloc,int ArpUint32Max,int BoundaryIndex);

/*
* 功能描述	：	内存池构造函数，采用默认参数
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	指向内存池的指针
* 其它说明	：	无
* 修改日期	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
apr_allocator_t * AllocatorPoolCreateDefault(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

