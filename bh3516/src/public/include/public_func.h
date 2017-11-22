#ifndef __PUBLIC_FUNC_H__
#define __PUBLIC_FUNC_H__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <arpa/inet.h>
#include <stdio.h>

#define		MAX_EVENTS_EPOLL	32

typedef enum
{
	EPOOL_READ = 0,
	EPOOL_WRITE,	
	EPOOL_WR,
}EPOOL_RW_OPER;

typedef enum
{
	EPOOL_ET = 0,
	EPOOL_LT,	
}EPOOL_TRIGGER_WAY;

//分区信息
typedef struct
{
	unsigned char				m_ucPart;										//分区编号
	unsigned long long		m_ulTotal;									//总分区大小
	unsigned long long		m_ulFree;										//剩余空间大小
	unsigned long long		m_ulUsed;										//已使用空间大小
}TPartInfo;
//___________________by tqk
//获取绝对值
unsigned int getABS(int a);

/*
* 功能描述	：	测试字节序的大小端
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		0	代表小端
										1	代表大端
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int EndianTest(void);

/*
* 功能描述	：	字节拷贝
* 成员更新	:		无
* 输入参数	：	pstrsource		:输入内存区域地址
										size					:要拷贝的字节数
* 输出参数	：	pstrdest			:输出内存区域地址
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
void sendChange(char* pstrsource,char* pstrdest,unsigned int size);

/*
* 功能描述	：	字节拷贝
* 成员更新	:		无
* 输入参数	：	pstrsource		:输入内存区域地址
										size			:要拷贝的字节数
* 输出参数	：	pstrdest		:输出内存区域地址
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
void recvChange(char* pstrsource,char* pstrdest,unsigned int size);

/*
* 功能描述	：	将十进制整形数转换为字符串
* 成员更新	:		无
* 输入参数	：	value		:要转化的十进制数
										radix		:进制，目前只支持十进制
* 输出参数	：	string		:存放输出字符串的地址
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
void itoa(unsigned int value, char *string, int radix);

/*
* 功能描述	：	从网络接收定长数据
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										iLen		:接收数据的缓冲器长度
* 输出参数	：	Buf		:存放接收到的数据
* 返 回 值	：		-1		:参数错误
										-2		:套接字错误
										其他值:接收到的数据长度
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int MyRecv(int fd, char* Buf, int iLen);

/*
* 功能描述	：	从网络接收定长数据
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										iLen		:接收数据的缓冲器长度
* 输出参数	：	Buf		:存放接收到的数据
* 返 回 值	：		0			:接收正确
										-1		:参数错误
										-2		:套接字错误
										-3		:套接字正常关闭
* 其它说明	：	无
* 修改日期	:	2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int RecvFixedLenData(int  fd, char* RecvDataBuf, int  Needlen);

/*
* 功能描述	：	向fd写入数据
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										buf 		:存放接收到的数据
										len		:接收数据的缓冲器长度
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int MyWritev(int fd, char* buf, int len);

/*
* 功能描述	：	向空文件写入定长数据
* 成员更新	:		无
* 输入参数	：	FileName		:文件名
										DataBuf 		:要写入数据的地址
										DataLen		:要写入数据的长度
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int WriteDataToFile(char* FileName,char* DataBuf,int DataLen);

/*
* 功能描述	：	把一个文件的内容追加到另一个文件中
* 成员更新	:		无
* 输入参数	：	destfile	:目的文件
										srcfile 	:源文件
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int FileAppend(char *destfile, char *srcfile);

/*
* 功能描述	： 将套接字加入到epool中
* 成员更新	:		无
* 输入参数	：	epfd		:epool句柄
										sfd	 	:套接字
										flag1	:读写方式
										flag2	:触发方式
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int AddSocketToEp(int epfd, int sfd, EPOOL_RW_OPER flag1, EPOOL_TRIGGER_WAY flag2);

/*
* 功能描述	： 	将套接字删除到epool中
* 成员更新	:		无
* 输入参数	：	epfd		:epool句柄
										sfd	 	:套接字
										flag1	:读写方式
										flag2	:触发方式
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int DelSocketFromEp(int epfd, int sfd, EPOOL_RW_OPER flag1, EPOOL_TRIGGER_WAY flag2);

/*
* 功能描述	：  base64加密
* 成员更新	:		无
* 输入参数	：	dest		:存放加密后内容的地址
										src	 	:要加密内容的地址
										size		:要加密内容的大小
* 输出参数	：	无
* 返 回 值	：		-1				:失败
										其他值:加密后内容的长度
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int EncryptB64(char* dest,const char* src,size_t size);

/*
* 功能描述	： 将32位整形IP地址转换为点分十进制字符串
* 成员更新	:		无
* 输入参数	：	_u32IPAddress	:32位的整形IP地址
* 输出参数	：	cIpAddress			:存储转换后的点分十进制IP字符串
* 返 回 值	：		0							:成功
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int IPAddressU32ToChar(unsigned int _u32IPAddress,char* cIpAddress);

/*
* 功能描述	： 通过域名连接网络服务器
* 成员更新	:		无
* 输入参数	：	sock		:套接字
										host		:服务器域名
										port		:服务器端口
* 输出参数	：	无
* 返 回 值	：		0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int ConnectHost(int sock, const char *host, int port);

/*
* 功能描述	： 获取设备的IP地址
* 成员更新	:		无
* 输入参数	：	interface_name	:网卡名称
										len						:存放输出IP地址的长度
* 输出参数	：	ip							:存放输出IP的地址
* 返 回 值	：		0							:成功
										-1						:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int GetIpaddr(char* interface_name, char* ip, int len);

/*
* 功能描述	： 获取设备的mac地址
* 成员更新	:		无
* 输入参数	：	interface_name	:网卡名称
										len						:存放输出mac地址的长度
* 输出参数	：	mac						:存放输出mac的地址
* 返 回 值	：		0							:成功
										-1						:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int GetMac(char* interface_name,char* mac,int len);

/*
* 功能描述	： 	TCP连接服务器
* 成员更新	:		无
* 输入参数	：	IPAddress	:十进制IP地址
										Port				:端口号
* 输出参数	：	无
* 返 回 值	：		-1				:失败
										其他值	:"连接生产的套接字
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int TcpConnect(int IPAddress, int Port);

/*
* 功能描述	： 	将udp的接收地址绑定到套接字
* 成员更新	:		无
* 输入参数	：	addr		:udp地址
										Port		:端口号
* 输出参数	：	无
* 返 回 值	：		-1				:失败
										其他值	:连接生产的套接字
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int UdpBind(struct sockaddr_in *addr, int Port);

/*
* 功能描述	： 	设定套接字读取的超时时间
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										sec		:秒
										usec		:微妙
* 输出参数	：	无
* 返 回 值	：		-1		:出错
										0			:超时
										1			:有时间产生，套接字可读
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int SelectTimeOutRead(int fd,int sec,int usec);

/*
* 功能描述	： 	设定套接字的写入超时时间
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										sec		:秒
										usec		:微妙
* 输出参数	：	无
* 返 回 值	：		-1		:出错
										0			:超时
										1			:有时间产生，套接字可写
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int SelectTimeOutWrite(int fd,int sec,int usec);

/*
* 功能描述	： 	将套接字设置为非阻塞
* 成员更新	:		无
* 输入参数	：	iSocket		:套接字
* 输出参数	：	无
* 返 回 值	：		-1			:出错
										0				:成功
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int SetSockNonBlock(int iSocket);

/*
* 功能描述	： 	在字符串里面查找指定的分隔符地址
* 成员更新	:		无
* 输入参数	：	strSource		:字符串
										Separator	:分隔符
										num				:分隔符序号
* 输出参数	：	无
* 返 回 值	：		NULL			:出错
										其他值	:成功
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/

char* FindSeparatByNum(char* strSource,char Separator,int num);

/*
* 功能描述	： 	调用fread读取定长数据
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
										buffer	:地址
										length	:要读取数据的长度
* 输出参数	：	无
* 返 回 值	：		成功返回要读取的长度
										在文件读取结束时返回实际读取到的数据长度
										-1	读取文件失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int FreadFixData(char* buffer, unsigned int length, FILE*fp);

/*
* 功能描述	：  文件读锁
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/

void FileReadLockSet(int fd);
/*
* 功能描述	： 	文件写锁
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/

void FileWriteLockSet(int fd);
/*
* 功能描述	： 	文件解锁
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/

void FileUnLockSet(int fd);

/*
* 功能描述	：  查看读文件是否被锁
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
* 输出参数	：	无
* 返 回 值	：		0代表文件未被加锁
										-1代表文件被加读锁
										-2代表文件被加写锁
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/

int FileReadLockCheck(int fd);

/*
* 功能描述	：  查看写文件是否被锁
* 成员更新	:		无
* 输入参数	：	fp		:文件指针
* 输出参数	：	无
* 返 回 值	：		0代表文件未被加锁
										-1代表文件被加读锁
										-2代表文件被加写锁
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/
int FileWriteLockCheck(int fd);

/*
* 功能描述	：  设置延迟时间
* 成员更新	:		无
* 输入参数	：	fd		:句柄
										sec		:秒
										usec		:微妙
* 输出参数	：	无
* 返 回 值	：		0		代表超时返回
										小于0 代表出错
* 其它说明	：	无
* 修改日期	:		2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/
int SelectTimeOut(int fd,int sec,int usec);

/*
* 功能描述	： 	设定套接字读取的超时时间纳秒
* 成员更新	:		无
* 输入参数	：	fd			:套接字
										sec		:秒
										usec		:微妙
* 输出参数	：	无
* 返 回 值	：		-1		:出错
										0			:超时
										1			:有时间产生，套接字可读
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int SelectTimeOutReadNSec(int fd,int sec,int nsec);
int base64_decode_binary_to_string(unsigned char*dstBuffer, int srcBufferSize, unsigned char *srcBuffer);
char* base64_encode(unsigned char* pSrc, int s32SrcLen, char* pDest, int s32DestLen);
unsigned long long  get_time_value();
unsigned int gen_session_id();
unsigned long long  hl64ton(unsigned long long   host);
unsigned long long  ntohl64(unsigned long long   net);
/*
* 函数名称	：	disk_get_part_space
* 功能描述	:  		根据磁盘挂载后的目录计算分区大小
* 输入参数	：	pucPath为磁盘的分区路径
* 输出参数	：	TPartInfo* _stInfo ，记录分区号，总大小，剩余大小，已用大小
* 返 回 值	：		0代表成功，-1代表失败
* 其它说明	：
* 修改日期
* -----------------------------------------------
* 2014/01/23	     V1.0	   steven
*/
int get_dir_space(unsigned char *pucPath, TPartInfo* stInfo);

/*
* 功能描述	：	创建目录
* 成员更新	:		无
* 输入参数	： 	_pcDirName 目录的路径
* 输出参数	： 	无
* 返 回 值	：		0代表成功，-1失败
* 其它说明	：	无
* 修改日期	:		2015.01.08
* -----------------------------------------------
* 2015/01/08	 V1.0		XXXX		  XXXX
*/
int make_dir(unsigned char* pcDirName);

/*
* 功能描述	：	卸载挂载目录
* 成员更新	:		无
* 输入参数	： psDir要卸载的挂载目录
* 输出参数	：	无
* 返 回 值	：		0 代表成功 -1代表失败
* 其它说明	：	无
* 修改日期	:		2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
int umount_dir(const char* const psDir);

/*
* 功能描述	：	获取文件大小
* 成员更新	:	无
* 输入参数	： pcFileName文件名
* 输出参数	：	无
* 返 回 值	：	-1代表失败
					其他值代表文件大小
* 其它说明	：	无
* 修改日期	:	2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
unsigned long get_file_size(char * const pcFileName);

/*
* 功能描述	：	注册新号处理函数
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值		：	0 	代表成功
								-1	代表失败
* 其它说明	：	无
* 修改日期	:		2013.09.23
* -----------------------------------------------
* 2013/09/23	 V1.0		XXXX		  XXXX
*/
int ParaSignal(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

