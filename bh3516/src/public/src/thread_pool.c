#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <pthread.h> 
#include <assert.h> 
#include <string.h>


#include "thread_pool.h"

static void * ThreadPoolRoutine (void *arg); 
/*
* ��������	��	���̳߳���Ͷ�����񣬲������̳߳����߳������ﵽ���ֵ
				ʱ�����Զ������̳߳��������
* ��Ա����	:	��
* �������	��	pthis		ָ���̳߳ص�thisָ��
				process 	��������ָ��
				arg 		���������������
* �������	��	��
* �� �� ֵ	��	�ɹ� 		����0
				ʧ��		���� -1
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolAddWorkUnlimit(void* pthis,void *(*process) (void *arg), void *arg) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
    worker_t *newworker = (worker_t *) malloc (sizeof (worker_t)); 
	if (NULL == newworker)
	{
		return -1;
	}
    newworker->process 	= process; 
    newworker->arg 		= arg; 
    newworker->next 	= NULL;

    pthread_mutex_lock(&(pool->queue_lock)); 
	
    worker_t *member = pool->queue_head; 
    if (member != NULL) 
    { 
        while (member->next != NULL) 
        {
        	member = member->next; 
        }
        member->next = newworker; 
    } 
    else 
    { 
        pool->queue_head = newworker; 
    } 
	
    assert (pool->queue_head != NULL); 
    pool->cur_queue_size++; 

	int FreeThreadNum = pool->current_pthread_num - pool->current_pthread_task_num;
    if(0 == FreeThreadNum)
    {
    	int current_pthread_num = pool->current_pthread_num;
    	pool->threadid = (pthread_t *) realloc(pool->threadid,(current_pthread_num + 1) * sizeof (pthread_t)); 
		pthread_create (&(pool->threadid[current_pthread_num]), NULL, ThreadPoolRoutine,  (void*)pool);
		pool->current_pthread_num++;
		if (pool->current_pthread_num > pool->max_thread_num)
		{
			pool->max_thread_num = pool->current_pthread_num;
		}
		
		pool->current_pthread_task_num++;
		pthread_mutex_unlock (&(pool->queue_lock)); 
    	pthread_cond_signal (&(pool->queue_ready)); 
		return 0;
    }
	
	pool->current_pthread_task_num++;
    pthread_mutex_unlock(&(pool->queue_lock)); 
    pthread_cond_signal(&(pool->queue_ready)); 
	
    return 0; 
} 

/*
* ��������	��	���̳߳���Ͷ���������̳߳����߳������ﵽ���ֵʱ
				���ӳٴ����������̳߳��п����̺߳�Ż�ִ������
* ��Ա����	:	��
* �������	��	pthis		ָ���̳߳ص�thisָ��
				process 	��������ָ��
				arg 		���������������
* �������	��	��
* �� �� ֵ	��	0	�ɹ�
				-1	ʧ��	
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolAddWorkLimit(void* pthis,void *(*process) (void *arg), void *arg) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
    worker_t *newworker = (worker_t *) malloc (sizeof (worker_t)); 
	if (NULL == newworker)
	{
		return -1;
	}
    newworker->process 	= process; 
    newworker->arg 		= arg; 
    newworker->next 	= NULL;

    pthread_mutex_lock(&(pool->queue_lock)); 
	
    worker_t *member = pool->queue_head; 
    if (member != NULL) 
    { 
        while (member->next != NULL) 
        {
        	member = member->next; 
        }
        member->next = newworker; 
    } 
    else 
    { 
        pool->queue_head = newworker; 
    } 
	
    assert (pool->queue_head != NULL); 
    pool->cur_queue_size++; 

	int FreeThreadNum = pool->current_pthread_num - pool->current_pthread_task_num;
    if((0 == FreeThreadNum) && (pool->current_pthread_num < pool->max_thread_num))
    {
    	int current_pthread_num = pool->current_pthread_num;
    	pool->threadid = (pthread_t *) realloc(pool->threadid,(current_pthread_num + 1) * sizeof (pthread_t)); 
		pthread_create (&(pool->threadid[current_pthread_num]), NULL, ThreadPoolRoutine,  (void*)pool);
		pool->current_pthread_num++;
		
		pool->current_pthread_task_num++;
		pthread_mutex_unlock (&(pool->queue_lock)); 
    	pthread_cond_signal (&(pool->queue_ready)); 
		return 0;
    }

	pool->current_pthread_task_num++;
    pthread_mutex_unlock(&(pool->queue_lock)); 
    pthread_cond_signal(&(pool->queue_ready)); 
    return 0; 
} 

/*
* ��������	��	��ȡ�̳߳������ɵ��������
* ��Ա����	:	��
* �������	��	pthis		ָ���̳߳ص�thisָ��
* �������	��	��
* �� �� ֵ	��	��ȡ�̳߳������ɵ��������
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolGetThreadMaxNum(void* pthis) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
	
    pthread_mutex_lock(&(pool->queue_lock)); 
	int num = pool->max_thread_num;
    pthread_mutex_unlock(&(pool->queue_lock)); 
	
    return num; 
} 

/*
* ��������	��	��ȡ��ǰ�̳߳����߳�����
* ��Ա����	:	��
* �������	��	pthis	ָ���̳߳ص�thisָ��
* �������	��	��
* �� �� ֵ	��	��ǰ�̳߳����߳�����
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolGetCurrentThreadNum(void* pthis) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
	
    pthread_mutex_lock(&(pool->queue_lock)); 
	int num = pool->current_pthread_num;
    pthread_mutex_unlock(&(pool->queue_lock)); 
	
    return num; 
} 

/*
* ��������	��	��ȡ��ǰ�̳߳�����ִ��������߳�����
* ��Ա����	:	��
* �������	��	pthis	ָ���̳߳ص�thisָ��
* �������	��	��
* �� �� ֵ	��	��ǰ�̳߳�����ִ��������߳�����
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolGetCurrentTaskThreadNum(void* pthis) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
	
    pthread_mutex_lock(&(pool->queue_lock)); 
	int num = pool->current_pthread_task_num;
    pthread_mutex_unlock(&(pool->queue_lock)); 
	
    return num; 
} 

/*
* ��������	��	��ȡ��ǰ�̳߳���������
* ��Ա����	:	��
* �������	��	pthis	ָ���̳߳ص�thisָ��
* �������	��	��
* �� �� ֵ	��	��ǰ�̳߳���������
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolGetCurrentTaskNum(void* pthis) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
	
    pthread_mutex_lock(&(pool->queue_lock)); 
	int num = pool->cur_queue_size;
    pthread_mutex_unlock(&(pool->queue_lock)); 
	
    return num; 
} 

/*
* ��������	��	�̳߳�����������ô˽ӿ�ʱ�̳߳���������ִ�У�
				�ýӿڷ��أ��̳߳ػ��첽����
* ��Ա����	:	��
* �������	��	pthis		ָ���̳߳ص�thisָ��
* �������	��	��
* �� �� ֵ	��	0			�ɹ�
				-1	 		���̳߳��Ѿ����ù�����
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
int ThreadPoolDestroy (void *pthis) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)pthis;
    if (pool->shutdown) 
    {
    	return -1;
    }
    pool->shutdown = 1; 
	
    pthread_cond_broadcast (&(pool->queue_ready)); 
    int i; 
    for (i = 0; i < pool->current_pthread_num; i++) 
    {
    	pthread_join (pool->threadid[i], NULL); 
    }
	
    free (pool->threadid); 
    worker_t *head = NULL; 
	
    while (pool->queue_head != NULL) 
    { 
        head = pool->queue_head; 
        pool->queue_head = pool->queue_head->next; 
        free (head); 
    } 
	
    pthread_mutex_destroy(&(pool->queue_lock)); 
    pthread_cond_destroy(&(pool->queue_ready)); 
     
    free (pool); 
    pool=NULL; 
	
    return 0; 
} 



static void * ThreadPoolRoutine (void *arg) 
{ 
	CThread_pool_t *pool = (CThread_pool_t *)arg;
    while (1) 
    { 
        pthread_mutex_lock (&(pool->queue_lock)); 

        while ((pool->cur_queue_size == 0) && (!pool->shutdown)) 
        { 
            pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock)); 
        } 

        if (pool->shutdown) 
        { 
            pthread_mutex_unlock (&(pool->queue_lock)); 
            pthread_exit (NULL); 
        } 

        assert (pool->cur_queue_size != 0); 
        assert (pool->queue_head != NULL); 
         
        pool->cur_queue_size--; 
        worker_t *worker 	= pool->queue_head; 
        pool->queue_head 	= worker->next; 
		
        pthread_mutex_unlock (&(pool->queue_lock)); 
        (*(worker->process)) (worker->arg); 
        pthread_mutex_lock (&(pool->queue_lock)); 
		
		pool->current_pthread_task_num--;
        free (worker); 
        worker = NULL; 

		if ((pool->current_pthread_num - pool->current_pthread_task_num) > pool->free_pthread_num)
		{
			pool->current_pthread_num--;
			pthread_mutex_unlock (&(pool->queue_lock)); 
			break;
		}
        pthread_mutex_unlock (&(pool->queue_lock)); 
		
    } 
	pthread_detach(pthread_self());
    pthread_exit (NULL); 
    return (void*)NULL;
} 

CThread_pool_t* ThreadPoolConstruct(int max_num,int free_num) 
{ 
    CThread_pool_t *pool = (CThread_pool_t *) malloc (sizeof (CThread_pool_t)); 
	if (NULL == pool)
	{
		return NULL;
	}
	memset(pool, 0, sizeof(CThread_pool_t));
	
    pthread_mutex_init (&(pool->queue_lock), NULL); 
    pthread_cond_init (&(pool->queue_ready), NULL); 

    pool->queue_head 				= NULL; 
    pool->max_thread_num 			= max_num;
    pool->cur_queue_size 			= 0; 
	pool->current_pthread_task_num 	= 0;
    pool->shutdown 					= 0; 
	pool->current_pthread_num 		= 0;
	pool->free_pthread_num 			= free_num;
	pool->threadid					= NULL;
    pool->threadid 					= (pthread_t *) malloc (max_num * sizeof (pthread_t)); 
    int i = 0; 
    for (i = 0; i < max_num; i++) 
    {  
		pool->current_pthread_num++;
        pthread_create (&(pool->threadid[i]), NULL, ThreadPoolRoutine,  (void*)pool);
		usleep(1000);
    } 

	return pool;
} 


CThread_pool_t* ThreadPoolConstructDefault(void) 
{ 
    CThread_pool_t *pool = (CThread_pool_t *) malloc (sizeof (CThread_pool_t)); 
	if (NULL == pool)
	{
		return NULL;
	}
	memset(pool, 0, sizeof(CThread_pool_t));
	
    pthread_mutex_init(&(pool->queue_lock), NULL); 
    pthread_cond_init(&(pool->queue_ready), NULL); 

    pool->queue_head 				= NULL; 
    pool->max_thread_num 			= DEFAULT_MAX_THREAD_NUM;
    pool->cur_queue_size 			= 0; 
	pool->current_pthread_task_num 	= 0;
    pool->shutdown 					= 0; 
	pool->current_pthread_num 		= 0;
	pool->free_pthread_num 			= DEFAULT_FREE_THREAD_NUM;
	pool->threadid					= NULL;

	return pool;
}

