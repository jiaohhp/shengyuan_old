#ifndef __PTHREAD_POOL_H__
#define __PTHREAD_POOL_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>

#define DEFAULT_MAX_THREAD_NUM		100
#define DEFAULT_FREE_THREAD_NUM		10	

typedef struct worker_t worker_t;
typedef struct CThread_pool_t CThread_pool_t;

/*
 *线程池里所有运行和等待的任务都是一个CThread_worker
 *由于所有任务都在链表里，所以是一个链表结构
*/
struct worker_t
{
	/*回调函数，任务运行时会调用此函数，注意也可声明成其它形式*/
    void *(*process) (void *arg);
    int   paratype;	//回调参数类型
    void *arg;/*回调函数的参数*/
    struct worker_t *next;
};

/*线程池结构*/
struct CThread_pool_t
{
	pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    /*链表结构，线程池中所有等待任务*/
    worker_t *queue_head;
    /*是否销毁线程池*/
    int shutdown;
    pthread_t *threadid;
    /*线程池中允许的活动线程数目*/
    int max_thread_num;
	/*当前线程池内存在的线程数目*/
	int current_pthread_num;
	//当前正在执行任务和已分配任务的线程数目和
	int	 current_pthread_task_num;
    /*当前等待队列的任务数目*/
    int cur_queue_size;
	/*线程池内允许存在的空闲线程数目*/
	int	free_pthread_num;
};

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
int ThreadPoolAddWorkUnlimit(void* pthis,void *(*process) (void *arg), void *arg);

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
int ThreadPoolAddWorkLimit(void* pthis,void *(*process) (void *arg), void *arg);

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
int ThreadPoolGetThreadMaxNum(void* pthis);

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
int ThreadPoolGetCurrentThreadNum(void* pthis);

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
int ThreadPoolGetCurrentTaskThreadNum(void* pthis);

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
int ThreadPoolDestroy (void *pthis);

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
int ThreadPoolGetCurrentTaskNum(void* pthis) ;

/*
* 功能描述	：	线程池构造函数
* 成员更新	:	无
* 输入参数	：	max_num			线程池内能容纳的最大线程数量
				free_num		线程池内能容纳的最大空闲线程数量
* 输出参数	：	无
* 返 回 值	：	指向线程池的指针
* 其它说明	：	无
* 修改日期	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
CThread_pool_t* ThreadPoolConstruct(int max_num,int free_num);

/*
* 功能描述	：	线程池构造函数，采用默认参数
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	指向线程池的指针
* 其它说明	：	无
* 修改日期	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
CThread_pool_t* ThreadPoolConstructDefault(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

