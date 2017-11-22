#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "datapool.h"

static void AllocatorMaxFreeSet(apr_allocator_t *allocator,int in_size)
{
    int  max_free_index;
    int  size = in_size;

    pthread_mutex_lock(&(allocator->m_tLock));

    max_free_index 					= APR_ALIGN(size, allocator->m_BoundarySize) >> allocator->m_BoundaryIndex;
    allocator->current_free_index 	+= max_free_index;
    allocator->current_free_index 	-= allocator->max_free_index;
    allocator->max_free_index 		= max_free_index;
	
    if (allocator->current_free_index > max_free_index)
    {
     	allocator->current_free_index = max_free_index;
    }
	
    pthread_mutex_unlock(&(allocator->m_tLock));
}

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
void AllocatorDestroy(apr_allocator_t * pthis)
{
	apr_allocator_t *allocator = pthis;
	pthread_mutex_lock(&(allocator->m_tLock));

	int i;
	apr_memnode_t *Next = NULL,*Cur = NULL;

	//逐个释放每个链表的资源
	for (i = 0; i < allocator->max_index; i++)
	{
		Next = allocator->free[i];

		while ((NULL != Next->next) && (Next != NULL))
		{
			//不止一个节点处理
			Cur = Next;
			Next = Next->next;
			free(Cur);
		}
		
		if (NULL != Next)
		{
			free(Next);
		}
		
	}
	
	free(allocator->free);
	
	pthread_mutex_unlock(&(allocator->m_tLock));
	pthread_mutex_destroy(&(allocator->m_tLock));
	
	free(allocator);

	return ;

}

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
void *AllocatorAlloc(apr_allocator_t *pthis, int  _size)
{
	apr_allocator_t *allocator = pthis;
    apr_memnode_t *node, **ref;
    int max_index;
    int i, index;
    int size;

    size = APR_ALIGN(_size + APR_MEMNODE_T_SIZE, allocator->m_BoundarySize);
    if (size < allocator->m_MinAlloc)
    {
    	size = allocator->m_MinAlloc;
    }


    index = (size >> allocator->m_BoundaryIndex) - 1;

    if (index > allocator->m_ArpUint32Max)
    {
        return NULL;
    }

    if (index <= allocator->max_index)
    {
        pthread_mutex_lock(&(allocator->m_tLock));

        max_index = allocator->max_index;
        ref = &allocator->free[index];
        i = index;
		
        while (*ref == NULL && i < max_index)
        {
            ref++;
            i++;
        }

        if ((node = *ref) != NULL)
        {
            if ((*ref = node->next) == NULL && i >= max_index)
            {
                do
                {
                    ref--;
                    max_index--;
                }
                while (*ref == NULL && max_index > 0);

                allocator->max_index = max_index;
            }

            allocator->current_free_index += node->index;
            if (allocator->current_free_index > allocator->max_free_index)
            {
             	allocator->current_free_index = allocator->max_free_index;
            }
            pthread_mutex_unlock(&(allocator->m_tLock));

            node->next 			= NULL;
            node->m_bData 		= (char *)node + APR_MEMNODE_T_SIZE;
			
#ifdef PRINTF
			node->m_iFreeFlg	= 0;
#endif
            return node->m_bData;
        }
        pthread_mutex_unlock(&(allocator->m_tLock));
    }
    else if (allocator->free[0])
    {
        pthread_mutex_lock(&(allocator->m_tLock));
        ref = &allocator->free[0];
        while ((node = *ref) != NULL && index > node->index)
        {
        	ref = &node->next;
        }

        if (node)
        {
            *ref = node->next;
            allocator->current_free_index += node->index;
            if (allocator->current_free_index > allocator->max_free_index)
            {
             	allocator->current_free_index = allocator->max_free_index;
            }

            pthread_mutex_unlock(&(allocator->m_tLock));
            node->next 			= NULL;
            node->m_bData 		= (char *)node + APR_MEMNODE_T_SIZE;
			
#ifdef PRINTF
			node->m_iFreeFlg	= 0;
#endif
            return node->m_bData;
        }

        pthread_mutex_unlock(&(allocator->m_tLock));
    }

    if ((node = (apr_memnode_t*)malloc(size)) == NULL)
    {
    	return NULL;
    }

    node->next 			= NULL;
    node->index 		= index;
    node->m_bData 		= (char *)node + APR_MEMNODE_T_SIZE;
    node->m_pool 		= allocator;
	
#ifdef PRINTF
	node->m_iFreeFlg	= 0;
#endif

    pthread_mutex_init(&(node->m_tLock), NULL);
	
    return node->m_bData;
}

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
void AllocatorFree(void* _node)
{
	apr_memnode_t *node = (apr_memnode_t *)((char *)_node - APR_MEMNODE_T_SIZE);
	
#ifdef PRINTF
	if (1 == node->m_iFreeFlg)
	{
		ptf_err("refree node");
		return ;
	}
#endif

	if (NULL == node)
	{
		ptf_err("null node");
		return ;
	}
	
    apr_memnode_t *next = NULL, *freelist = NULL;
    int index, max_index;
    int max_free_index, current_free_index;
    apr_allocator_t *allocator = node->m_pool;

    pthread_mutex_lock(&(allocator->m_tLock));

    max_index = allocator->max_index;
    max_free_index = allocator->max_free_index;
    current_free_index = allocator->current_free_index;

    do
    {
        next = node->next;
        index = node->index;

        if (max_free_index != APR_ALLOCATOR_MAX_FREE_UNLIMITED && index > current_free_index)
        {
            node->next = freelist;
            freelist = node;
        }
        else if (index < allocator->m_MaxIndex)
        {
            if ((node->next = allocator->free[index]) == NULL && index > max_index)
            {
                max_index = index;
            }
			
#ifdef PRINTF
			node->m_iFreeFlg		= 1;
#endif
            allocator->free[index] 	= node;
			
            if (current_free_index >= index)
            {
            	current_free_index -= index;
            }
            else
            {
            	current_free_index = 0;
            }
        }
        else
        {
        
#ifdef PRINTF
			node->m_iFreeFlg	= 1;
#endif
            node->next 			= allocator->free[0];
			
            allocator->free[0] = node;
            if (current_free_index >= index)
            {
             	current_free_index -= index;
            }
            else
            {
            	current_free_index = 0;
            }

        }
    }while ((node = next) != NULL);

    allocator->max_index = max_index;
    allocator->current_free_index = current_free_index;


    pthread_mutex_unlock(&(allocator->m_tLock));

    while (freelist != NULL)
    {
        node = freelist;
        freelist = node->next;
		pthread_mutex_destroy(&(node->m_tLock));
        free(node);
		node = NULL;
    }

}


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
void AllocatorNodeLock(void* _node)
{
	apr_memnode_t *node = (apr_memnode_t *)((char *)_node - APR_MEMNODE_T_SIZE);
    pthread_mutex_lock(&(node->m_tLock));
}

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
void AllocatorNodeUnlock(void* _node)
{
	apr_memnode_t *node = (apr_memnode_t *)((char *)_node - APR_MEMNODE_T_SIZE);
    pthread_mutex_unlock(&(node->m_tLock));
}

apr_allocator_t * AllocatorPoolCreate(int in_size,int MaxIndex,int MinAlloc,int ArpUint32Max,int BoundaryIndex)
{
    apr_allocator_t *new_allocator;

    if ((new_allocator = (apr_allocator_t*)malloc(SIZEOF_ALLOCATOR_T)) == NULL)
    {
     	return NULL;
    }

    memset(new_allocator, 0, SIZEOF_ALLOCATOR_T);
    new_allocator->max_free_index = APR_ALLOCATOR_MAX_FREE_UNLIMITED;

	if ((new_allocator->free = (apr_memnode_t**)malloc(APR_MEMNODE_T_SIZE * MaxIndex)) == NULL)
    {
    	free(new_allocator);
     	return NULL;
    }
	
    memset(new_allocator->free, 0, APR_MEMNODE_T_SIZE * MaxIndex);
	
	new_allocator->m_MaxIndex 		= MaxIndex;
	new_allocator->m_MinAlloc 		= MinAlloc;
	new_allocator->m_ArpUint32Max 	= ArpUint32Max;
	new_allocator->m_BoundaryIndex 	= BoundaryIndex;
	new_allocator->m_BoundarySize 	= (1 << BoundaryIndex);

    pthread_mutex_init(&(new_allocator->m_tLock), NULL);

	if(APR_ALLOCATOR_MAX_FREE_UNLIMITED != new_allocator->max_free_index)
	{
		AllocatorMaxFreeSet(new_allocator,in_size);
	}

    return new_allocator;
}


apr_allocator_t * AllocatorPoolCreateDefault(void)
{
    apr_allocator_t *new_allocator;

    if ((new_allocator = (apr_allocator_t*)malloc(SIZEOF_ALLOCATOR_T)) == NULL)
    {
     	return NULL;
    }

    memset(new_allocator, 0, SIZEOF_ALLOCATOR_T);
    new_allocator->max_free_index = APR_ALLOCATOR_MAX_FREE_UNLIMITED;

	if ((new_allocator->free = (apr_memnode_t**)malloc(APR_MEMNODE_T_SIZE * DEFAULT_MAX_INDEX)) == NULL)
    {
    	free(new_allocator);
     	return NULL;
    }
	
    memset(new_allocator->free, 0, APR_MEMNODE_T_SIZE * DEFAULT_MAX_INDEX);
	
	new_allocator->m_MaxIndex 		= DEFAULT_MAX_INDEX;
	new_allocator->m_MinAlloc 		= DEFAULT_MIN_ALLOC;
	new_allocator->m_ArpUint32Max 	= DEFAULT_ARPUINT32_MAX;
	new_allocator->m_BoundaryIndex 	= DEFAULT_BOUNDARY_INDEX;
	new_allocator->m_BoundarySize 	= (1 << DEFAULT_BOUNDARY_INDEX);
    pthread_mutex_init(&(new_allocator->m_tLock), NULL);

    return new_allocator;
}

