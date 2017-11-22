#ifndef __UDP_BC_H__
#define __UDP_BC_H__

//编译选项

//标准头文件

//非标准头文件

//常量定义

//全局宏

//全局数据类型

//类定义

//模版

//全局函数原型

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
int udpMCCmdSend(unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum);

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
void udpMBInit(void);

#endif

