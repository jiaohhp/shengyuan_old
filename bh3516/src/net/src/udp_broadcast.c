//标准头文件包含区域
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>

//自定义头文件包含区域
#include "udp_broadcast.h"
#include "project_func.h"
#include "printf_func.h"
#include "net_para.h"
#include "dev_property.h"
#include "thread_pool.h"
#include "global_para.h"
#include "tcp_client.h"
#include "sample_board.h"
//常量定义区域

//文件内部使用的宏
#define SEARCH_PORT 1688
#define SEARCH_IP 		"234.56.78.9"

//文件内部使用的数据类型

extern int ele_stop_snap_dfw;

typedef struct UDP_MC_S
{

	int	            		m_Socket;									//套接字
	unsigned short	m_CheckCode;							//验证码
	unsigned char 	m_MainId;									//主ID
	unsigned char		m_SubId;									//子ID
	unsigned char		m_BodyArrNum;						//参数个数

	PT_HEAD				m_ptHead;								//数据头
	unsigned char		m_Buf[PT_HEAD_LEN + MAX_CMD_DATA_LEN];	//接收和发送数据缓冲区
   	char 					m_ArrBuf[MAX_PARA_NUM][MAX_STRING_LEN];	//解析协议体buf

    struct sockaddr_in m_client;								//存放客户端信息
    struct sockaddr_in m_muc;								//存放多播地址信息
} UDP_MC;

//全局变量

//静态局部全局变量
UDP_MC s_sUdpMc;
//局部函数原型

//类的实现

//内部函数

//外部函数

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis		UDP_MC结构体指针
										MainId	主ID
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/
static int udpMCCmdSendInner(UDP_MC* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum)
{
	int offset = 0;
	pthis->m_ptHead.m_headTag[0] = 'B';
	pthis->m_ptHead.m_headTag[1] = 'N';
	pthis->m_ptHead.m_headTag[2] = 'X';
	pthis->m_ptHead.m_ptType 	= CMD_DATA;
	pthis->m_ptHead.m_ptLen  		= CMD_HEAD_LEN + strlen(Body);
	unsigned char* pBuf = pthis->m_Buf + PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);

	pBuf[offset] = (unsigned char)(pthis->m_CheckCode >> 8);
	offset += 1;
	pBuf[offset] = (unsigned char)(pthis->m_CheckCode);
	offset += 1;

	pBuf[offset] = MainId;
	offset += 1;
	pBuf[offset] = SubId;
	offset += 1;

	pBuf[offset] = BodyParaNum;
	offset += 1;

	memcpy(pBuf + offset,Body,strlen(Body));
	offset += strlen(Body);
	pBuf[offset] = CMD_END_FLAG;
	offset += 1;
	int iRet = sendto(pthis->m_Socket, pthis->m_Buf, PT_HEAD_LEN + offset, 0, (struct sockaddr*)&(pthis->m_muc), sizeof(struct sockaddr));
	if (iRet < 0)
	{
		perror("sendto");
		ptf_err("send Body %s",Body);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	处理udp命令
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值		：	0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
static int udpMCCmdHandle(UDP_MC* pthis)
{
    int paraNum = 0;
	TCP_IP_PARA sPara;
	CMD_PARA cmdPara;
    char body[256] = {0};
    unsigned char replySubId = 0;
	if (pthis->m_MainId != MAIN_MC)
	{
		ptf_err("pthis->m_MainId 0x%x",pthis->m_MainId);
		return -1;
	}
	
    switch (pthis->m_SubId)
    {
        case SUB_SEARCH_DEV:
        {
			TCP_IP_PARA sPara;
			netGetTcpPara(&sPara);
			char cMac[32];
			netGetMac(cMac);
			int group = netGetGroup();
			sprintf(body, "%d|%s|%s|%s|%s|%d|", 0, sPara.m_cIPAddr, sPara.m_cGateWay, sPara.m_cNetMask,cMac,group);
			replySubId = SUB_SEARCH_DEV_RSP;
			paraNum = 6;
        }break;
        case SUB_LINK_GROUP_RECD_START:
        {
			char* clientip = inet_ntoa(s_sUdpMc.m_client.sin_addr);
			netGetTcpPara(&sPara);
			if (0 == strcmp(clientip, sPara.m_cIPAddr))
			{
				ptf_dbg("clientip %s",clientip);
				return 0;
			}
			
			ptf_dbg("clientip %s sPara.m_cIPAddr %s",clientip,sPara.m_cIPAddr);

			DFW_E eDfw = atoi(pthis->m_ArrBuf[0]);
			int linkGroup = atoi(pthis->m_ArrBuf[1]);
			int group = netGetGroup();
			if (linkGroup == group)
			{
				cmdPara.m_eType		= CMD_CROSS_ALARM_STOP_PUSH; //stop it whenever. decided by itself.
				tcpClientHandlePushCmd(&cmdPara);

				cmdPara.m_eType 	= CMD_ELEC_LINK_START_PUSH;
				ptf_dbg("CMD_ELEC_LINK_START_PUSH");
				cmdPara.m_para1 	= eDfw;
				tcpClientHandlePushCmd(&cmdPara);
			}
			return 0;
        }break;
        case SUB_LINK_GROUP_RECD_STOP:
        {
			char* clientip = inet_ntoa(s_sUdpMc.m_client.sin_addr);
			netGetTcpPara(&sPara);
			if (0 == strcmp(clientip, sPara.m_cIPAddr))
			{
				ptf_dbg("clientip %s",clientip);
				return 0;
			}
			
			ptf_dbg("clientip %s sPara.m_cIPAddr %s",clientip,sPara.m_cIPAddr);

			int linkGroup = atoi(pthis->m_ArrBuf[0]);
			int group = netGetGroup();
			if (linkGroup == group)
			{
				cmdPara.m_eType 	= CMD_ELEC_LINK_STOP_PUSH;
				ptf_dbg("CMD_ELEC_LINK_STOP_PUSH");
				tcpClientHandlePushCmd(&cmdPara);
				// 多机牵引新增 2017_08_25
				ele_stop_snap_dfw = tcpClientGetDFWRecSnap();
				ThreadPoolAddWorkLimit((void*)g_psThreadPool,pthread_delay_rec_stop,(void*)NULL);
			}
			return 0;
        }break;
        default:
        {
            ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
            return -1;
        }break;
    }
    
    return udpMCCmdSendInner(pthis,pthis->m_MainId,replySubId,body,paraNum);
} 


/*
* 功能描述	：	格式化命令参数到数组中
* 成员更新	:		无
* 输入参数	：	pthis		多播结构体指针
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/
static int udpMCCmdFormat(UDP_MC* pthis)
{
	int ret;
	pthis->m_CheckCode 	= (pthis->m_Buf[0 + PT_HEAD_LEN] << 8) + pthis->m_Buf[1 + PT_HEAD_LEN];
	pthis->m_MainId			= pthis->m_Buf[2 + PT_HEAD_LEN];
	pthis->m_SubId 			= pthis->m_Buf[3 + PT_HEAD_LEN];
	pthis->m_BodyArrNum	= pthis->m_Buf[4 + PT_HEAD_LEN];	

	memset(pthis->m_ArrBuf, 0, sizeof(pthis->m_ArrBuf));
	ret = DivideStringBySeparator((char*)pthis->m_Buf + PT_HEAD_LEN + CMD_HEAD_LEN - 1, pthis->m_ArrBuf,SEPARATOR,pthis->m_BodyArrNum);
	if (ret != pthis->m_BodyArrNum)
	{
		ptf_err("ret is %d pthis->m_BodyArrNum %d",ret,pthis->m_BodyArrNum);
		return -1;
	}

	return 0;
}


/*
* 功能描述	：	udp多播处理线程
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08	 V1.0		XXXX		  XXXX
*/
static void* udpMCHandleProc(void* args)
{
	int fd, size, ret;
	struct sockaddr_in srv;
	struct ip_mreq mreq;

	memset( &srv, 0, sizeof(struct sockaddr_in) );
	memset( &mreq, 0, sizeof(struct ip_mreq) );

	srv.sin_family	= AF_INET;
	srv.sin_port 	= htons(SEARCH_PORT);

#if 1
	ret = inet_aton(SEARCH_IP, &srv.sin_addr);
	if(ret < 0 )
	{
		perror("inet_aton");
		return (void*)NULL;
	}
#else
	srv.sin_addr.s_addr = INADDR_ANY;
#endif

	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(fd < 0)
	{
		perror("socket");
		return (void*)NULL;
	}

	ret = bind(fd, (struct sockaddr *)&srv, sizeof(srv));
	if(ret < 0)
	{
		perror("bind");
		return (void*)NULL;
	}

	ret = inet_aton(SEARCH_IP, &mreq.imr_multiaddr);
	if (ret < 0)
	{
		perror("inet_aton");
		return (void*)NULL;
	}

	mreq.imr_multiaddr.s_addr 	= inet_addr(SEARCH_IP);
	mreq.imr_interface.s_addr 	= htonl(INADDR_ANY);

	ret = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,sizeof(mreq));
	if(ret < 0)
	{
		perror("setsockopt");
		return (void*)NULL;
	}

	size 	= sizeof(s_sUdpMc.m_client);
	s_sUdpMc.m_muc.sin_family	= AF_INET;
	s_sUdpMc.m_muc.sin_port 	= htons(SEARCH_PORT);
	inet_aton(SEARCH_IP, &s_sUdpMc.m_muc.sin_addr);

	int nfds;
	int epoolfd = epoll_create(1);
	struct epoll_event ev;
	ev.data.fd 		= fd;
	//ev.events 		= EPOLLIN | EPOLLET;
	ev.events 		= EPOLLIN;
	epoll_ctl(epoolfd, EPOLL_CTL_ADD, fd, &ev);
	s_sUdpMc.m_Socket  = fd;
	int bodysize;
	while(1)
	{
		//nfds = epoll_wait(epoolfd, &ev, 1, -1);		
		nfds = epoll_wait(epoolfd, &ev, 1, 1000 * 20);
		if (nfds > 0)
		{
			s_sUdpMc.m_Socket = ev.data.fd;
			memset(s_sUdpMc.m_Buf, 0, sizeof(s_sUdpMc.m_Buf));
			ret = recvfrom(s_sUdpMc.m_Socket, s_sUdpMc.m_Buf, sizeof(s_sUdpMc.m_Buf), 0, (struct sockaddr *)&s_sUdpMc.m_client, (socklen_t*)&size);
			if(ret < 0)
			{
				perror("recvfrom");
				continue;
			}
			
			//解析数据头
			PutBuf2PtHead(&(s_sUdpMc.m_ptHead), s_sUdpMc.m_Buf);
			if(memcmp(s_sUdpMc.m_ptHead.m_headTag,PT_HEAD_TAG,3) != 0)
			{
				ptf_err("datahead err client ip %s:\n", inet_ntoa(s_sUdpMc.m_client.sin_addr));
				continue;
			}

			bodysize = s_sUdpMc.m_ptHead.m_ptLen;
			//验证接收数据是否完整
			if ((bodysize + PT_HEAD_LEN) != ret)
			{
				ptf_err("datahead err client ip %s:bodysize %d", inet_ntoa(s_sUdpMc.m_client.sin_addr),bodysize);
				continue;
			}

			ret = udpMCCmdFormat(&s_sUdpMc);
			if(ret < 0)
			{
				ptf_dbg("ret %d",ret);
				continue;
			}

			udpMCCmdHandle(&s_sUdpMc);
		}
		else
		{
			close(fd);
			usleep(1000 * 20);
			ThreadPoolAddWorkUnlimit((void*)g_psThreadPool,udpMCHandleProc,(void*)NULL);
			break;
		}
	}

	//close(fd);
	close(epoolfd);
	return (void*)NULL;
}


/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	MainId	主ID
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/
int udpMCCmdSend(unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum)
{
    ptf_dbg("send Body %s",Body);
	return udpMCCmdSendInner(&s_sUdpMc, MainId, SubId, Body, BodyParaNum);
}

/*
* 功能描述	：	UDP多播初始化
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08	 V1.0		XXXX		  XXXX
*/
void udpMBInit(void)
{
	ThreadPoolAddWorkUnlimit((void*)g_psThreadPool,udpMCHandleProc,(void*)NULL);	
    return ;
}

