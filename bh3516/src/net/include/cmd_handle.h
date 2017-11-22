#ifndef __CMD_HANDLE_H__
#define __CMD_HANDLE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



#include "project_func.h"

typedef struct CMD_HANDLE_T
{
	int	            		m_Socket;									//套接字
	unsigned short	m_CheckCode;							//验证码
	unsigned char 	m_MainId;									//主ID
	unsigned char		m_SubId;									//子ID
	unsigned char		m_BodyArrNum;						//参数个数

	PT_HEAD				m_ptHead;								//数据头
	unsigned char		m_Buf[PT_HEAD_LEN + MAX_CMD_DATA_LEN];	//接收和发送数据缓冲区
   	char 					m_ArrBuf[MAX_PARA_NUM][MAX_STRING_LEN];	//解析协议体buf
}CMD_HANDLE;


/*
* 功能描述	：	模块析构函数
* 成员更新	:		无
* 输入参数	：	socketFd	套接字
* 输出参数	：	无
* 返 回 值	：		参数错误返回	-1
										成功返回		0
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlDistruct(CMD_HANDLE* pthis);

/*
* 功能描述	：	接收统一信息头
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			套接字出错
										-2			协议头错误
										0				成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvPtHead(CMD_HANDLE* pthis);

/*
* 功能描述	：	利用内部的存储空间接收附载
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			数据太大
										-2			套接字出错
										0			成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyInBuf(CMD_HANDLE* pthis);

/*
* 功能描述	：	利用外部的存储空间接收附载
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			数据太大
										-2			套接字出错
										0			成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyOutBuf(CMD_HANDLE* pthis,char* buf,int buflen);

/*
* 功能描述	：	格式化命令参数到数组中
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlCmdFormat(CMD_HANDLE* pthis);

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdDevProperty(CMD_HANDLE* pthis);

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/
int tcpCmdNetPara(CMD_HANDLE* pthis);

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.03
* -----------------------------------------------
* 2015/08/03 V1.0		XXXX		  XXXX
*/
int tcpCmdHandleMainId(CMD_HANDLE* pthis);

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/
int tcpCmdFileUpdate(CMD_HANDLE* pthis);

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										MainId	主ID
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandleSendFailure(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum);

int tcpCmdHandlSendCmd1(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum);

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.15
* -----------------------------------------------
* 2015/08/15 V1.0		XXXX		  XXXX
*/
int tcpCmdRegister(CMD_HANDLE* pthis);

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendCmd2(CMD_HANDLE* pthis,unsigned char SubId,char* Body,unsigned char BodyParaNum);

/*
* 功能描述	：	发送图片
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										psVPara	视频描述信息
										picBuf	存放图片的内存地址
										picLen	图片大小
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendPicture(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen);
int tcpCmdHandlSendPicture_Elec(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen);


/*
* 功能描述	：	发送视频数据
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										picBuf	存放图片的内存地址
										picLen	图片大小
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoStream(CMD_HANDLE* pthis, char* streamBuf, int streamLen);

/*
* 功能描述	：	发送视频参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										psVPara	视频参数结构体
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoPara(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara);

/*
* 功能描述	：	发送心跳
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendHeartBeat(CMD_HANDLE* pthis);

/*
* 功能描述	：	发送采集参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										sampleData		采样数据
										sampleLen		采用数据大小
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendSampleData(CMD_HANDLE* pthis, char* sampleData, int sampleLen);

/*
* 功能描述	：	发送采集参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										samplePara		采样参数
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendSamplePara(CMD_HANDLE* pthis, CAPTURE_PARAM *samplePara);

/*
* 功能描述	：	模块构造函数
* 成员更新	:		无
* 输入参数	：	socketFd	套接字
* 输出参数	：	无
* 返 回 值	：		失败返回NULL
										成功返回地址
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
CMD_HANDLE* tcpCmdHandlConstruct(int socketFd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif




