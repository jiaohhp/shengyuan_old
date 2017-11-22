#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

//编译选项

//标准头文件

//非标准头文件
#include "net_para.h"

//常量定义

//全局宏

//全局数据类型
typedef enum CMD_TYPE_E
{
    CMD_MANUAL_START_PUSH,					//开始推送实时流
    CMD_MANUAL_STOP_PUSH,						//停止推送实时流
    CMD_ELEC_STOP_SNAP,							//电流低于门限值时抓拍图片
    CMD_ELEC_ALARM_START_PUSH,			//电流开始推送实时流
    CMD_ELEC_ALARM_STOP_PUSH,				//电流停止推送实时流
    CMD_CROSS_ALARM_START_PUSH,		//过车开始推送实时流
    CMD_CROSS_ALARM_STOP_PUSH,			//过车停止推送实时流
    CMD_ELEC_LINK_START_PUSH,			//电流开始推送实时流
    CMD_ELEC_LINK_STOP_PUSH,				//电流停止推送实时流
    CMD_FAILURE_CAMERA,//摄像头故障
    CMD_FAILURE_ELEC,//电流故障
    CMD_FAILURE_ACC,//加速度故障
    CMD_FAILURE_PRESSURE,//压力故障
    CMD_FAILURE_LIQUID,//液位故障
    CMD_FINISH_ELEC,// 电流结束数据发送
    CMD_FINISH_ACC,//加速度结束数据发送
    CMD_RED_STRAP_START_PUSH,
    CMD_RED_STRAP_STOP_PUSH,
    CMD_BUTT,
}CMD_TYPE;

typedef struct CMD_PARA_S
{
	CMD_TYPE	m_eType;
	int m_para1;
	int m_para2;
	int m_para3;
	int m_para4;
}CMD_PARA;

typedef struct SAMPLE_SEND_TIME_S
{
	unsigned int 	sample_i;
	unsigned int		sample_d;
	unsigned int		sanp_i;
	unsigned int		sanp_d;
}SAMPLE_SEND_TIME_S;

typedef struct PC_SERVER_PARA_S
{
	int 						m_enable;		//0	失效1使能
	unsigned short 	m_port;
	char						m_ipaddr[32];
}PC_SERVER_PARA;


//类定义

//模版

//全局函数原型

/*
* 功能描述	：	设定当前抓拍录像的定反位
* 成员更新	:		无
* 输入参数	：	dfw	定反位
* 输出参数	：	无
* 返 回 值	：		-1			失败
										0				成功
* 其它说明	：	无
* 修改日期	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientSetDFWRecSnap(DFW_E dfw);
int tcpClientGetDFWRecSnap(void);


/*
* 功能描述	：	设定定时器发送时间间隔
* 成员更新	:		无
* 输入参数	：	psSendTime	发送时间信息结构体
* 输出参数	：	无
* 返 回 值	：		-1			失败
										0				成功
* 其它说明	：	无
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientSetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime);

/*
* 功能描述	：	获取定时器发送时间间隔
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	psSendTime	发送时间信息结构体
* 返 回 值	：		-1			失败
										0				成功
* 其它说明	：	无
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientGetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime);

/*
* 功能描述	：	向命令fifo中投递命令
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		0				成功
										-1			失败
* 其它说明	：	无
* 修改日期	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientHandlePushCmd(CMD_PARA* cmdPara);

/*
* 功能描述	：	启动tcp客户端服务，开始连接PC服务器
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		0		代表启动连接成功
										1		代表之前已经处于启动状态
* 其它说明	：	无
* 修改日期	:		2015.08.12
* -----------------------------------------------
* 2015/08/12	 V1.0		XXXX		  XXXX
*/
int tcpClientStart(void);

/*
* 功能描述	：	获取pc服务器配置
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	psPara 	用来保存获取到的参数
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netGetPcServerPara(PC_SERVER_PARA *psPara);

/*
* 功能描述	：	设置pc服务器配置
* 成员更新	:		无
* 输入参数	：	psPara 	用来保存获取到的参数
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetPcServerPara(PC_SERVER_PARA *psPara);

#endif


