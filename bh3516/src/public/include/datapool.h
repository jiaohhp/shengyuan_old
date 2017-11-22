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
	apr_allocator_t 	*m_pool;				//ָ���ڴ�ص�ָ��
	char 				*m_bData;				//�����û����ڴ��ַ

	#ifdef PRINTF
	int					m_iFreeFlg;				//�ظ��ͷű�ʶ
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

	int					m_MinAlloc;				//��С�����ڴ���С
	int					m_MaxIndex;				//�������ڴ������
	int					m_ArpUint32Max;			//�ڴ�����ܴ�ŵ�����ڴ������
	int					m_BoundaryIndex;		//�����ڴ��ĵ�����С��2Ϊ�׵�ָ��
	int					m_BoundarySize;			//�����ڴ��ĵ�����С
};

/*
* ��������	��	�ڴ�ع���������
* ��Ա����	:	��
* �������	��	pthis	�ڴ��ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorDestroy(apr_allocator_t *pthis);

/*
* ��������	��	���ڴ�������ڴ�
* ��Ա����	:	��
* �������	��	pthis	�ڴ��ָ��
					_size	������ڴ��С
* �������	��	��
* �� �� ֵ	��	ָ��������ڴ��ָ��
* ����˵��	��	��
* �޸�����	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void *AllocatorAlloc(apr_allocator_t *pthis, int  _size);

/*
* ��������	��	�ͷ��ڴ浽�ڴ��
* ��Ա����	:	��
* �������	��	_node ���ڴ������Ľڵ�
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	�ڴ治�����ظ��ͷţ��ظ�һ�ε����ڴ渲�ǵĿ��ܣ��ظ����κ�����ӡ����ʾ��Ϣ
* �޸�����	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorFree(void* _node);

/*
* ��������	��	���ڵ����
* ��Ա����	:	��
* �������	��	_node ���ڴ������Ľڵ�
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorNodeLock(void* _node);

/*
* ��������	��	���ڵ����
* ��Ա����	:	��
* �������	��	_node ���ڴ������Ľڵ�
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.15
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
void AllocatorNodeUnlock(void* _node);

/*
* ��������	��	�ڴ�ع��캯��
* ��Ա����	:	��
* �������	��	in_size			�ڴ�������ɵ�����ڴ��������APR_ALLOCATOR_MAX_FREE_UNLIMITED����������������
					MaxIndex		�ڴ���й����ڴ������������
					MinAlloc			�ڴ�ع���С�����ڴ��Ĵ�С
					ArpUint32Max		�ڴ���������ɵ�����ڴ�������ֵ
					BoundaryIndex	�����ڴ��ĵ�����С��2Ϊ�׵�ָ��
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2013.08.015
* -----------------------------------------------
* 2013/08/02		 V1.0		XXXX		  XXXX
*/
apr_allocator_t * AllocatorPoolCreate(int in_size,int MaxIndex,int MinAlloc,int ArpUint32Max,int BoundaryIndex);

/*
* ��������	��	�ڴ�ع��캯��������Ĭ�ϲ���
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	ָ���ڴ�ص�ָ��
* ����˵��	��	��
* �޸�����	:	2013.08.15
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

