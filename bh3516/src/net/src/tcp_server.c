//��׼ͷ�ļ���������
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
//�Զ���ͷ�ļ���������
#include "cmd_handle.h"
#include "cmd_protocol.h"
#include "dev_property.h"
#include "tcp_server.h"
#include "printf_func.h"
#include "datapool.h"
#include "global_para.h"
#include "public_func.h"
#include "net_para.h"
#include "thread_pool.h"
#include "global_para.h"

//������������

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�ȫ�ֱ���

//�ֲ�����ԭ��

//���ʵ��

//�ڲ�����

/*
* ��������	��	������������߳�
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	:      	��
* ����˵��	��	��
* �޸�����	:		2015.08.03
* -----------------------------------------------
* 2015/08/03 V1.0		XXXX		  XXXX
*/
static void* tcpServerHandleProc(void* args)
{
	int	ret;
	int	socketfd = (int)args;
	
	int keepalive		= 1;
	int keepidle		= 15;
	int keepinterval	= 5;
	int keepcount	= 2;
	setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE , (void *)&keepalive, sizeof(keepalive));
	setsockopt(socketfd, SOL_TCP   , TCP_KEEPIDLE , (void *)&keepidle, sizeof(keepidle));
	setsockopt(socketfd, SOL_TCP   , TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval));
	setsockopt(socketfd, SOL_TCP   , TCP_KEEPCNT  , (void *)&keepcount, sizeof(keepcount));
	struct linger so_linger;
	so_linger.l_onoff	= 1;
	so_linger.l_linger	= 0;
	setsockopt(socketfd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)); 
	// ���ö˿�����
	int nREUSEADDR = 1;
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&nREUSEADDR, sizeof(int));
	fd_set read_fds;
	struct timeval sValue;
	int  maxfd = socketfd;

	CMD_HANDLE* psCmdHandle = tcpCmdHandlConstruct(socketfd);
	if (NULL == psCmdHandle)
	{
		ptf_err("tcpCmdHandlConstruct");
		close(socketfd);
		return NULL;
	}

	while (1)
	{       
		FD_ZERO(&read_fds);
		FD_SET(socketfd, &read_fds);

		sValue.tv_sec 	  = 2;
		sValue.tv_usec = 0;

		ret = select(maxfd + 1, &read_fds, NULL, NULL, &sValue);
		if (ret < 0)
		{
			ptf_err("select is failed !!!\n");
			break;
		}
		else if (ret == 0)
		{
			continue;
		}
		
		ret = tcpCmdHandlRecvPtHead(psCmdHandle);
		if (ret != 0)
		{
			ptf_err("ret %d",ret);
			break;
		}

		ret = tcpCmdHandlRecvBodyInBuf(psCmdHandle);
		if (ret != 0)
		{
			ptf_err("ret %d",ret);
			break;
		}

		ret = tcpCmdHandlCmdFormat(psCmdHandle);
		if (ret != 0)
		{
			ptf_err("ret %d",ret);
			break;
		}

		ret = tcpCmdHandleMainId(psCmdHandle);
		if (ret != 0)
		{
			ptf_err("ret %d",ret);
			break;
		}
	}
	
	tcpCmdHandlDistruct(psCmdHandle);
	
	close(socketfd);
	ptf_dbg("************server_cmd_handle: close fd = %d***************", socketfd);
	return (void*)NULL;
}


/*
* ��������	��	�����߳�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01	 V1.0		XXXX		  XXXX
*/
static void* tcpServerListenProc(void* arg)
{
	int listenfd, connfd;
	socklen_t clilen = sizeof(struct sockaddr_in);

	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
	{
		ptf_err("Error: fail to socket !!!\n");
		return NULL;
	}

	int port = 8000;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr	= INADDR_ANY;
	serveraddr.sin_port 	= htons(port);

	if(bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0)
	{
		ptf_err("Error: fail to bind !!!\n");
		return NULL;
	}

	if(listen(listenfd, MAX_EVENTS_EPOLL) < 0)
	{
		ptf_err("Error: fail to listen !!!\n");
		return NULL;
	}

	while (1)
	{		
		connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clilen);
		if(connfd < 0)
		{
			ptf_err("Error: fail to accept !!!\n");
			continue;
		}
		ptf_dbg("connfd %d^^^^^^^^^^^^tcpcmd: accept connect from: %s - %d",connfd, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		ThreadPoolAddWorkLimit((void*)g_psThreadPool, tcpServerHandleProc, (void *)connfd);
	}

	return NULL;
}

//�ⲿ����

/*
* ��������	��	�޸��ֵ��ϣ������������section��ֵ
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01	 V1.0		XXXX		  XXXX
*/
void tcpServerInit(void)
{
	ThreadPoolAddWorkLimit((void*)g_psThreadPool, tcpServerListenProc, (void *)NULL);
	return ;
}


