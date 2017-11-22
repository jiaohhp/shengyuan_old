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
 *�̳߳����������к͵ȴ���������һ��CThread_worker
 *�������������������������һ������ṹ
*/
struct worker_t
{
	/*�ص���������������ʱ����ô˺�����ע��Ҳ��������������ʽ*/
    void *(*process) (void *arg);
    int   paratype;	//�ص���������
    void *arg;/*�ص������Ĳ���*/
    struct worker_t *next;
};

/*�̳߳ؽṹ*/
struct CThread_pool_t
{
	pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    /*����ṹ���̳߳������еȴ�����*/
    worker_t *queue_head;
    /*�Ƿ������̳߳�*/
    int shutdown;
    pthread_t *threadid;
    /*�̳߳�������Ļ�߳���Ŀ*/
    int max_thread_num;
	/*��ǰ�̳߳��ڴ��ڵ��߳���Ŀ*/
	int current_pthread_num;
	//��ǰ����ִ��������ѷ���������߳���Ŀ��
	int	 current_pthread_task_num;
    /*��ǰ�ȴ����е�������Ŀ*/
    int cur_queue_size;
	/*�̳߳���������ڵĿ����߳���Ŀ*/
	int	free_pthread_num;
};

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
int ThreadPoolAddWorkUnlimit(void* pthis,void *(*process) (void *arg), void *arg);

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
int ThreadPoolAddWorkLimit(void* pthis,void *(*process) (void *arg), void *arg);

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
int ThreadPoolGetThreadMaxNum(void* pthis);

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
int ThreadPoolGetCurrentThreadNum(void* pthis);

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
int ThreadPoolGetCurrentTaskThreadNum(void* pthis);

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
int ThreadPoolDestroy (void *pthis);

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
int ThreadPoolGetCurrentTaskNum(void* pthis) ;

/*
* ��������	��	�̳߳ع��캯��
* ��Ա����	:	��
* �������	��	max_num			�̳߳��������ɵ�����߳�����
				free_num		�̳߳��������ɵ��������߳�����
* �������	��	��
* �� �� ֵ	��	ָ���̳߳ص�ָ��
* ����˵��	��	��
* �޸�����	:	2013.08.28
* -----------------------------------------------
* 2013/08/15	 V1.0		XXXX		  XXXX
*/
CThread_pool_t* ThreadPoolConstruct(int max_num,int free_num);

/*
* ��������	��	�̳߳ع��캯��������Ĭ�ϲ���
* ��Ա����	:	��
* �������	��	��
* �������	��	��
* �� �� ֵ	��	ָ���̳߳ص�ָ��
* ����˵��	��	��
* �޸�����	:	2013.08.28
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

