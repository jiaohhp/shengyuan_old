#ifndef __UART_H__
#define __UART_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <pthread.h>


typedef struct UartOper_t
{
	int					m_fd;		//用来保存打开的uart设备文件句柄
	pthread_mutex_t 	m_lock;		//互斥锁

}UartOper;

/*
* 功能描述	：	设置串口比特率
* 成员更新	:	无
* 输入参数	：	pthis	  	指向Uart信息的this指针，具体为调用UartOpen返回的指针
				speed 		要设置的串口比特率
* 输出参数	：	无
* 返 回 值	：	-1			代表失败
				0			代表成功
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartSetSpeed(UartOper* pthis, int speed);

/*
* 功能描述	：	设置串口奇偶校验、数据位、停止位
* 成员更新	:	无
* 输入参数	：	pthis	  	指向Uart信息的this指针，具体为调用UartOpen返回的指针
				databits	数据位
				stopbits	停止位
				parity		奇偶校验
* 输出参数	：	无
* 返 回 值	：	-1			代表失败
				0			代表成功
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartSetParity(UartOper* pthis, int databits, int stopbits, int parity);

/*
* 功能描述	：	读取串口数据
* 成员更新	:	无
* 输入参数	：	pthis	  	指向Uart信息的this指针，具体为调用UartOpen返回的指针
				size		要读取的数据大小
				pbuf		存放数据的内存空间
* 输出参数	：	无
* 返 回 值	：	-1	代表失败
					其他值代表读取到的字节数
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartRead(UartOper* pthis,int size,char *pbuf);

/*
* 功能描述	：	向串口写入数据
* 成员更新	:	无
* 输入参数	：	pthis	  	指向Uart信息的this指针，具体为调用UartOpen返回的指针
				size		要写入的数据大小
				pbuf		存放数据的内存空间
* 输出参数	：	无
* 返 回 值	：	-1	代表失败
					其他值代表写入的字节数
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartWrite(UartOper* pthis,int _iSize, char *pbuf);

/*
* 功能描述	：	关闭串口，释放资源
* 成员更新	:	无
* 输入参数	：	pthis	  指向Uart信息的this指针，具体为调用UartOpen返回的指针
* 输出参数	：	无
* 返 回 值	：	-1	代表失败
					0	代表成功
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartClose(UartOper* pthis);

/*
* 功能描述	： 打开Uart设备文件句柄并初始化
* 成员更新	:	无
* 输入参数	：	Dev	 设备文件名称
* 输出参数	：	无
* 返 回 值	：	成功返回指向Uart信息的this指针
				失败返回NULL
* 其它说明	：	无
* 修改日期	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
UartOper* UartOpen(const char *Dev);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

