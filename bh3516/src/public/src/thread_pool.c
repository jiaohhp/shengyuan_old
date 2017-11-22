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
* 功能描述	：	向线程池内投递任务，并且在线程池内线程数量达到最大值
				时可以自动扩大线程池最大容量
* 成员更新	:	无
* 输入参数	：	pthis		指向线程池的this指针
				process 	任务处理函数指针
				arg 		任务处理函数所需参数
* 输出参数	：	无
* 返 回 值	：	成功 		返回0
				失败		返回 -1
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	向线程池内投递任务，在线程池内线程数量达到最大值时
				将延迟处理任务，在线程池有空余线程后才会执行任务
* 成员更新	:	无
* 输入参数	：	pthis		指向线程池的this指针
				process 	任务处理函数指针
				arg 		任务处理函数所需参数
* 输出参数	：	无
* 返 回 值	：	0	成功
				-1	失败	
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	获取线程池能容纳的最大限制
* 成员更新	:	无
* 输入参数	：	pthis		指向线程池的this指针
* 输出参数	：	无
* 返 回 值	：	获取线程池能容纳的最大限制
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	获取当前线程池内线程数量
* 成员更新	:	无
* 输入参数	：	pthis	指向线程池的this指针
* 输出参数	：	无
* 返 回 值	：	当前线程池内线程数量
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	获取当前线程池正在执行任务的线程数量
* 成员更新	:	无
* 输入参数	：	pthis	指向线程池的this指针
* 输出参数	：	无
* 返 回 值	：	当前线程池正在执行任务的线程数量
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	获取当前线程池任务数量
* 成员更新	:	无
* 输入参数	：	pthis	指向线程池的this指针
* 输出参数	：	无
* 返 回 值	：	当前线程池任务数量
* 其它说明	：	无
* 修改日期	:	2013.08.28
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
* 功能描述	：	线程池析构，如调用此接口时线程池尚有任务执行，
				该接口返回，线程池会异步销毁
* 成员更新	:	无
* 输入参数	：	pthis		指向线程池的this指针
* 输出参数	：	无
* 返 回 值	：	0			成功
				-1	 		该线程池已经设置过销毁
* 其它说明	：	无
* 修改日期	:	2013.08.28
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

