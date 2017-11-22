#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <signal.h>

#include "public_func.h"
#include "printf_func.h"
#include <limits.h>

#define	SMALL_ENDIAN

unsigned int getABS(int a)
{
	return a>=0?a:(-a);
}

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
int EndianTest(void)
{
    unsigned int test = 0x12345678;
    unsigned char p = (unsigned char)test;
    if (p == 0x78)
    {
        //小端数值低位在内存低位为小端
        return 0;	//小端
    }
    else if (p == 0x12)
    {
        //大端数值高位在内存低位为大端
        return 1;	//大端
    }

    return 0;
}

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
void sendChange(char* pstrsource,char* pstrdest,unsigned int size)
{
    int cnt = 0;

	#ifdef SMALL_ENDIAN
	
	for (cnt = 0; cnt < size; cnt++)
    {
      	*(pstrdest + cnt) = *(pstrsource  + (size - 1 - cnt));
   	}
	
	#else
	
	for (cnt = 0; cnt < size; cnt++)
    {
       	*(pstrdest + cnt) = *(pstrsource + cnt);
   	}
	
	#endif
}

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
void recvChange(char* pstrsource,char* pstrdest,unsigned int size)
{
    int cnt = 0;

	#ifdef SMALL_ENDIAN
	
	for (cnt = 0; cnt < size; cnt++)
    {
      	*(pstrdest + (size - 1 - cnt)) = *(pstrsource  + cnt);
   	}
	
	#else
	
	for (cnt = 0; cnt < size; cnt++)
    {
       	*(pstrdest + cnt) = *(pstrsource + cnt);
   	}
	
	#endif
}

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
void itoa(unsigned int value, char *string, int radix)
{
    if (NULL == string)
    {
        return ;
    }

    unsigned int    m = value;
    unsigned int    n;
    unsigned int    k = 0;
    int i;
    char    cTmp[16];

    memset(cTmp, 0, 16);

    if(value < 10 )
    {
        string[0] = value + 48;
        string[1] = 0;
        return;
    }

    while(m / 10 > 0)
    {
        n = m - m / 10 * 10;
        cTmp[k] = n + 48;
        m /= 10;
        if(m < 10)
        {
            cTmp[k + 1] = m + 48;
        }
        k++;
    }

    for(i = k; i >= 0; i--)
    {
        string[i] = cTmp[k - i];
    }
    string[k + 1] = 0;

}


/*
* 功能描述	：	从网络接收定长数据
* 成员更新	:		无
* 输入参数	：	fd		:套接字
										iLen	:接收数据的缓冲器长度
* 输出参数	：	Buf		:存放接收到的数据
* 返 回 值	：		-1		:参数错误
										-2		:套接字错误
										其他值	:接收到的数据长度
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int MyRecv(int fd, char* Buf, int iLen)
{
    if (fd < 0 || NULL == Buf)
    {
        return -1;
    }
    int iLRet = -1;
read:
	iLRet = recv(fd, Buf, iLen, 0);
    if(iLRet == -1)
    {
        //对端正常close返回0，异常退出返回-1
		if ( errno == EINTR )
		{
			goto read;   //return 0; //Interrupted system call
		}
        return -2;
    }
    return iLRet;
}

/*
* 功能描述	：	从网络接收定长数据
* 成员更新	:		无
* 输入参数	：	fd		:套接字
										iLen	:接收数据的缓冲器长度
* 输出参数	：	Buf	:存放接收到的数据
* 返 回 值	：		0		:接收正确
										-1	:参数错误
										-2	:套接字错误
										-3	:套接字正常关闭
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int RecvFixedLenData(int  fd, char* RecvDataBuf, int  Needlen)
{
    int iRecBufMovePointer 	= 0;
    int iRecNeedLen 			= Needlen;
    int iActualReadLen 		= -1;

    while (1)
    {
        iActualReadLen = MyRecv(fd, RecvDataBuf + iRecBufMovePointer, iRecNeedLen);
        if (iRecNeedLen == iActualReadLen)
        {
            return 0;
        }
        else if (-1 == iActualReadLen)
        {
            ptf_dbg("MyRecv: return -1 -- para error");
            return -1;
        }
        else if (-2 == iActualReadLen)
        {
            ptf_err("MyRecv: return -2 -- socket not regular close");
            return -2;
        }
    	else if(0 == iActualReadLen)
    	{
    		ptf_dbg("MyRecv: return 0 -- socket regular close");
    		return -3;
    	}
        else
        {
            iRecNeedLen -= iActualReadLen;
            iRecBufMovePointer += iActualReadLen;
            continue;
        }
    }

    return -1;
}


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
int MyWritev(int fd, char* buf, int len)
{
    if (fd <= 0 || NULL == buf)
    {
        ptf_err("s_fd :%d", fd);
        return -1;
    }

    struct iovec iov[1];
    iov[0].iov_base = (void*)buf;
    iov[0].iov_len  = len;
    int iRet = writev(fd, iov, 1);
    if (-1 == iRet)
    {
        perror("writev");
        ptf_err("send data errno %d", errno);
        return -1;
    }

    return 0;

}

/*
* 功能描述	：	向空文件写入定长数据
* 成员更新	:		无
* 输入参数	：	FileName		:文件名
										DataBuf 		:要写入数据的地址
										DataLen		:要写入数据的长度
* 输出参数	：	无
* 返 回 值	：		0					:成功
										-1				:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int WriteDataToFile(char* FileName,char* DataBuf,int DataLen)
{
	FILE* fp = fopen(FileName,"w+");
	if (fp != NULL)
	{
		int ret = fwrite(DataBuf, 1,DataLen,fp);
		if (ret != DataLen)
		{
			fclose(fp);
			remove(FileName);
			return -1;
		}
		//写入成功
		fclose(fp);
		return 0;
	}
	remove(FileName);
	return -1;
}

/*
* 功能描述	：	把一个文件的内容追加到另一个文件中
* 成员更新	:		无
* 输入参数	：	destfile	:目的文件
										srcfile 	:源文件
* 输出参数	：	无
* 返 回 值		：	0			:成功
										-1		:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int FileAppend(char *destfile, char *srcfile)
{
	FILE *file_src, *file_dest;
	int  readlen;
	char readbuf[1024];
	file_dest = fopen(destfile, "a+");
	int ret = access(srcfile, F_OK);
	if (0 == ret)
	{
		//文件存在
		file_src = fopen(srcfile, "r");
		while((readlen = fread(readbuf, 1, 1024, file_src)) != 0)
		{
			fwrite(readbuf, 1, readlen, file_dest);
		}
		fclose(file_src);
		fflush(file_dest);
		fclose(file_dest);
		return 0;
	}	
	fclose(file_dest);	
	return -1;
}


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
int AddSocketToEp(int epfd, int sfd, EPOOL_RW_OPER flag1, EPOOL_TRIGGER_WAY flag2)
{
    int iRet;

    struct epoll_event ev;
    bzero(&ev , sizeof(ev));
    ev.data.fd = sfd;

    switch (flag1)
    {
    case EPOOL_READ:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLIN;
        }
    }
    break;
    case EPOOL_WRITE:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLOUT | EPOLLET;
        }
        else
        {
            ev.events = EPOLLOUT;
        }
    }
    break;
    case EPOOL_WR:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLOUT | EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLOUT | EPOLLIN;
        }
    }
    break;
    default :
        break;
    }

    iRet = epoll_ctl(epfd , EPOLL_CTL_ADD , sfd, &ev);

    return iRet;
}

/*
* 功能描述	： 将套接字删除到epool中
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
int DelSocketFromEp(int epfd, int sfd, EPOOL_RW_OPER flag1, EPOOL_TRIGGER_WAY flag2)
{
    struct epoll_event ev;

    bzero(&ev , sizeof(ev));
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sfd;

    switch (flag1)
    {
    case EPOOL_READ:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLIN;
        }
    }
    break;
    case EPOOL_WRITE:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLOUT | EPOLLET;
        }
        else
        {
            ev.events = EPOLLOUT;
        }
    }
    break;
    case EPOOL_WR:
    {
        if (flag2 == EPOOL_ET)
        {
            ev.events = EPOLLOUT | EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLOUT | EPOLLIN;
        }
    }
    break;
    default :
        break;
    }

    int iRet = epoll_ctl(epfd , EPOLL_CTL_DEL , sfd , &ev);

    return iRet;
}


/*
* 功能描述	：  base64加密
* 成员更新	:		无
* 输入参数	：	dest		:存放加密后内容的地址
										src	 	:要加密内容的地址
										size		:要加密内容的大小
* 输出参数	：	无
* 返 回 值	：		-1				:失败
										其他值	:加密后内容的长度
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int EncryptB64(char* dest,const char* src,size_t size)
{
    int i, tmp = 0, b64_tmp = 0;

    if ( dest == NULL || src == NULL )
    {
        return -1;
    }

    while ((size - b64_tmp) >= 3)
    {
        // every 3 bytes of source change to 4 bytes of destination, 4*6 = 3*8
        dest[tmp] 	= 0x3F & (src[b64_tmp] >> 2);
        dest[tmp+1] = ((src[b64_tmp]<<4) & 0x30) | ((src[b64_tmp+1]>>4) & 0x0F);
        dest[tmp+2] = ((src[b64_tmp+1]<<2) & 0x3C) | ((src[b64_tmp+2]>>6) & 0x03);
        dest[tmp+3] = 0x3F & src[b64_tmp+2];
        for (i = 0; i <= 3; i++)
        {
            if ( (dest[tmp+i] <= 25) )
            {
                dest[tmp+i] += 'A';
            }
            else
            {
                if (dest[tmp+i] <= 51)
                {
                    dest[tmp+i] += 'a' - 26;
                }
                else
                {
                    if (dest[tmp+i] <= 61)
                    {
                        dest[tmp+i] += '0' - 52;
                    }
                }
            }
            if (dest[tmp+i] == 62)
            {
                dest[tmp+i] = '+';
            }
            if (dest[tmp+i] == 63)
            {
                dest[tmp+i] = '/';
            }
        }

        tmp += 4;
        b64_tmp += 3;
    } //end while
    if (b64_tmp == size)
    {
        dest[tmp] = '\0';
        return tmp;
    }
    if ((size - b64_tmp) == 1)
    {
        //one
        dest[tmp] = 0x3F & (src[b64_tmp] >> 2);
        dest[tmp + 1] = (src[b64_tmp] << 4) & 0x30;
        dest[tmp + 2] = '=';
    }
    else
    {
        //two
        dest[tmp] = 0x3F & (src[b64_tmp] >>2 );
        dest[tmp + 1] = ((src[b64_tmp] << 4) & 0x30) | ((src[b64_tmp + 1] >>4 ) & 0x0F);
        dest[tmp + 2] = (src[b64_tmp + 1] << 2) & 0x3F;
    }

    for (i=0; i<=(size - b64_tmp); i++)
    {
        if  (dest[tmp + i] <= 25)
        {
            dest[tmp + i] += 'A';
        }
        else if (dest[tmp + i] <= 51)
        {
            dest[tmp+i] += 'a' - 26;
        }
        else if (dest[tmp + i] <= 61)
        {
            dest[tmp + i] += '0' - 52;	   //end if
        }
        if (dest[tmp + i] == 62)
        {
            dest[tmp + i] = '+';
        }
        if (dest[tmp + i] == 63)
        {
            dest[tmp + i] = '/';
        }
    }

    dest[tmp + 3] = '=';
    dest[tmp + 4] = '\0';

    return tmp + 4;
}

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
int IPAddressU32ToChar(unsigned int _u32IPAddress,char* cIpAddress)
{
    char    cTempBuf[4];
    int     iTemp;
    int     i;
    unsigned int  u32Temp;

    memset(cIpAddress,'\0',16);
    for(i = 3; i >= 0; i--)
    {
        u32Temp = _u32IPAddress;
        memset(cTempBuf,'\0',4);
        iTemp = (u32Temp << i * 8) >> (3 * 8);
        itoa(iTemp,cTempBuf,10);
        strncat(cIpAddress,cTempBuf,strlen(cTempBuf));
        if(i>0)
        {
            strncat(cIpAddress,".",1);
        }
    }

    return 0;
}

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
int ConnectHost(int sock, const char *host, int port)
{
    struct sockaddr_in remote;
    int rt;
    struct hostent *ph;
    if(host == NULL)
    {
        return -1;
    }

    if ((ph = gethostbyname(host)) == NULL)
    {
        ptf_err("error : %s\n",strerror(errno));
        return -1;
    }

    memset(&remote, 0, sizeof(struct sockaddr));
    remote.sin_family = AF_INET;
    remote.sin_port   = htons(port);
    remote.sin_addr.s_addr = *((unsigned int *)*(ph->h_addr_list));

    rt = connect(sock, (struct sockaddr *)&remote, sizeof(struct sockaddr));
    if(rt < 0)
    {
        ptf_err("connect err error : %s\n",strerror(errno));
        return -1;
    }

    return 0;
}

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
int GetIpaddr(char* interface_name, char* ip, int len)
{
    int s;
    struct ifreq ifr;
    struct sockaddr_in *ptr;

    if (len < 16)
    {
        ptf_err("The ip need 16 byte !\n");
        return  - 1;
    }

    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        ptf_err("errno %d\n",errno);
        return  - 1;
    }

    strcpy(ifr.ifr_name, interface_name);

    if (ioctl(s, SIOCGIFADDR, &ifr) < 0)
    {
        close(s);
        return  - 1;
    }

    ptr = (struct sockaddr_in*) &ifr.ifr_ifru.ifru_addr;
    snprintf(ip, len, "%s", inet_ntoa(ptr->sin_addr));

    close(s);
    return 0;
}

/*
* 功能描述	： 获取设备的mac地址
* 成员更新	:		无
* 输入参数	：	interface_name	:网卡名称
										len				:存放输出mac地址的长度
* 输出参数	：	mac				:存放输出mac的地址
* 返 回 值	：		0					:成功
										-1				:失败
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int GetMac(char* interface_name, char* mac, int len)
{
    struct ifreq tmp;
    int sock_mac;
    char mac_addr[64] = {0};

    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if( sock_mac == -1)
    {
        perror("create socket fail\n");
        return -1;
    }

    memset(&tmp,0,sizeof(tmp));

    strncpy(tmp.ifr_name,"eth0",sizeof(tmp.ifr_name) - 1);

    if( (ioctl( sock_mac, SIOCGIFHWADDR, &tmp)) < 0 )
    {
        //printf("mac ioctl error\n");
        return -1;
    }

    sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)tmp.ifr_hwaddr.sa_data[0],
            (unsigned char)tmp.ifr_hwaddr.sa_data[1],
            (unsigned char)tmp.ifr_hwaddr.sa_data[2],
            (unsigned char)tmp.ifr_hwaddr.sa_data[3],
            (unsigned char)tmp.ifr_hwaddr.sa_data[4],
            (unsigned char)tmp.ifr_hwaddr.sa_data[5]
           );

    close(sock_mac);

    memcpy(mac,mac_addr,strlen(mac_addr));

    return 0;
}

/*
* 功能描述	： 	TCP连接服务器
* 成员更新	:		无
* 输入参数	：	IPAddress	:十进制IP地址
										Port				:端口号
* 输出参数	：	无
* 返 回 值	：		-1				:失败
										其他值	:连接生产的套接字
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int TcpConnect(int IPAddress, int Port)
{
    int iRet = -1;
    int s_fd;
    struct sockaddr_in sin;

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == s_fd)
    {
        perror("socket fail");
        return -1;
    }

    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family 		= AF_INET;
    sin.sin_port   		= htons(Port);
    sin.sin_addr.s_addr = IPAddress;

    struct timeval timeo = {60, 0};
    socklen_t len = sizeof(timeo);
    setsockopt(s_fd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);

    if((iRet = connect(s_fd,(struct sockaddr*)&sin,sizeof(sin))) < 0)
    {
        perror("connect");
        char IP[16];
        memset(IP, 0, sizeof(IP));
        IPAddressU32ToChar(IPAddress, IP);
        ptf_err("errno no is :%d ip %s port %d",errno,IP,Port);
        close(s_fd);
        return -1;
    }

    return s_fd;
}

/*
* 功能描述	： 	将udp的接收地址绑定到套接字
* 成员更新	:		无
* 输入参数	：	addr		:udp地址
										Port		:端口号
* 输出参数	：	无
* 返 回 值	：		-1					:失败
										其他值		:连接生产的套接字
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int UdpBind(struct sockaddr_in *addr, int Port)
{
    int s_fd;

    s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == s_fd)
    {
        perror("socket fail");
        return -1;
    }

    addr->sin_family 		= AF_INET;
    addr->sin_port   		= htons(Port);

    int iRet;
    if((iRet = connect(s_fd,(struct sockaddr*)addr,sizeof(struct sockaddr_in))) < 0)
    {
        return -1;
    }
    return s_fd;
}

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
int SelectTimeOutRead(int fd,int sec,int usec)
{
    fd_set rset;
    struct timeval tv;

    FD_ZERO(&rset);
    FD_SET(fd,&rset);

    tv.tv_sec 	= sec;
    tv.tv_usec	= usec;

    int num 	= select(fd + 1,&rset,NULL,NULL,&tv);
	if (num < 0)
	{
		return -1;
	}
	else if (0 == num)
	{
		//超时返回
		return 0;
	}
	else
	{
		return 1;
	}
}

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
int SelectTimeOutWrite(int fd,int sec,int usec)
{
    fd_set rset;
    struct timeval tv;

    FD_ZERO(&rset);
    FD_SET(fd,&rset);

    tv.tv_sec 	= sec;
    tv.tv_usec	= usec;

    int num = select(fd + 1,NULL,&rset,NULL,&tv);
    if (num < 0)
	{
		return -1;
	}
	else if (0 == num)
	{
		//超时返回
		return 0;
	}
	else
	{
		return 1;
	}

}

/*
* 功能描述	： 	将套接字设置为非阻塞
* 成员更新	:		无
* 输入参数	：	iSocket		:套接字
* 输出参数	：	无
* 返 回 值	：		-1			:出错
											0			:成功
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int SetSockNonBlock(int iSocket)
{
    if (iSocket < 0)
    {
        return -1;
    }

    int opts = fcntl(iSocket , F_GETFL);
    if(-1 == opts)
    {
        //ptf_err("%s\n" , "fcntl F_GETFL is faild");
        return -1;
    }

    opts = opts | O_NONBLOCK;
    if(fcntl(iSocket , F_SETFL , opts) < 0)
    {
        //ptf_err("%s\n" , "fcntl F_SETFL is faild");
        return -1;
    }

    return 0;
}

/*
* 功能描述	： 	在字符串里面查找指定的分隔符地址
* 成员更新	:		无
* 输入参数	：	strSource		:字符串
										Separator	:分隔符
											num			:分隔符序号
* 输出参数	：	无
* 返 回 值	：		NULL			:出错
										其他值	:成功
* 其它说明	：	无
* 修改日期	:		2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/

char* FindSeparatByNum(char* strSource,char Separator,int num)
{
    char* start 	= strSource;
    char* find 	= NULL;
    int cnt 		= 0;


    while (NULL != (find = strchr(start,Separator)))
    {
        cnt++;

        if (cnt == num)
        {
            return find;
        }
        start = find + 1;
    }
    return NULL;
}

/*
* 功能描述	： 	调用fread读取定长数据
* 成员更新	:		无
* 输入参数	：	fp			:文件指针
										buffer	:地址
										length	:要读取数据的长度
* 输出参数	：	无
* 返 回 值	：		成功返回要读取的长度
										在文件读取结束时返回实际读取到的数据长度
										-1	读取文件失败
* 其它说明	：	无
* 修改日期	:	2013.09.11
* -----------------------------------------------
* 2013/09/11	 V1.0	XXXX		  XXXX
*/
int FreadFixData(char* buffer, unsigned int length, FILE*fp)
{
    int iRecBufMovePointer = 0;
    int iRecNeedLen = length;
    int iActualReadLen = -1;
    while (1)
    {		
		iActualReadLen = fread(buffer + iRecBufMovePointer, 1,iRecNeedLen, fp);
        if (iRecNeedLen == iActualReadLen)
        {
        	//完整读取返回实际长度
            return length;
        }
		else if (iActualReadLen < 0)
		{
			return -1;
		}
        else
        {   
            iRecNeedLen -= iActualReadLen;
            iRecBufMovePointer += iActualReadLen;
			
			if (0 != feof(fp))
        	{
				//文件读取结束返回收到的数据长度
				return (length - iRecNeedLen);
        	}
			
            continue;
        }
    }
	
    return -1;
}


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

void FileReadLockSet(int fd)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len =0;
	lock.l_type = F_RDLCK;
	fcntl(fd, F_SETLKW, &lock);
	ptf_dbg("read lock set by %d\n",getpid());
	return;
}
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

void FileWriteLockSet(int fd)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len =0;
	lock.l_type = F_WRLCK;
	fcntl(fd, F_SETLKW, &lock);
	ptf_dbg("write lock set by %d\n",getpid());
	return;
}
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

void FileUnLockSet(int fd)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len =0;
	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &lock);
	ptf_dbg("release lock by %d\n",getpid());
	return;
}

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

int FileReadLockCheck(int fd)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len =0;
	lock.l_type = F_RDLCK;
	fcntl(fd, F_GETLK,&lock);
	if(lock.l_type != F_UNLCK)
	{
		if( lock.l_type == F_RDLCK )
		{
			//printf("read lock already set by %d\n",lock.l_pid);
			return -1;
		}
		else if( lock.l_type == F_WRLCK )
		{
			//printf("write lock already set by %d\n",lock.l_pid);
			return -2;
		}
	}
	return 0;
}

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

int FileWriteLockCheck(int fd)
{
	struct flock lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len =0;
	lock.l_type = F_WRLCK;
	fcntl(fd, F_GETLK,&lock);
	if(lock.l_type != F_UNLCK)
	{
		if( F_RDLCK == lock.l_type )
		{
			//printf("read lock already set by %d\n",lock.l_pid);
			return -1;
		}
		else if( F_WRLCK == lock.l_type )
		{
			//printf("write lock already set by %d\n",lock.l_pid);
			return -2;
		}
	}
	return 0;
}

/*
* 功能描述	：  设置延迟时间
* 成员更新	:		无
* 输入参数	：	fd			:句柄
										sec		:秒
										usec		:微妙
* 输出参数	：	无
* 返 回 值	：		0		代表超时返回
										小于0 	代表出错
* 其它说明	：	无
* 修改日期	:	2013.11.19
* -----------------------------------------------
* 2013/11/19	 V1.0	XXXX		  XXXX
*/

int SelectTimeOut(int fd,int sec,int usec)
{
    struct timeval tv;
    tv.tv_sec 	= sec;
    tv.tv_usec	= usec;

    int num = select(fd + 1,NULL,NULL,NULL,&tv);
    return num;

}

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
int SelectTimeOutReadNSec(int fd,int sec,int nsec)
{
    fd_set rset;
	struct timespec restrict1;
	
    FD_ZERO(&rset);
    FD_SET(fd,&rset);

    restrict1.tv_nsec=nsec;
	restrict1.tv_sec=sec;
	int num 	= pselect(fd + 1,&rset,0,0,&restrict1,0);
	if (num < 0)
	{
		return -1;
	}
	else if (0 == num)
	{
		//超时返回
		return 0;
	}
	else
	{
		return 1;
	}

}


int base64_decode_binary_to_string(unsigned char*dstBuffer, int srcBufferSize, unsigned char *srcBuffer)
{
    static const int b64[256] =
    {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 00-0F    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 10-1F        */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,    /* 20-2F    */
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,    /* 30-3F    */
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,    /* 40-4F    */
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,    /* 50-5F    */
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,    /* 60-6F    */
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,    /* 70-7F    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 80-8F    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* 90-9F    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* A0-AF    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* B0-BF    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* C0-CF    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* D0-DF    */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,    /* E0-EF    */

    };                                                              /* Base64 Buffer        	*/
    unsigned char *startPtr = dstBuffer;                                  	/* Start Pointer            	*/
    unsigned char *tempPtr = srcBuffer;                          	/* Temporary Pointer    */

    int level;                                               	/* Level of buffer         	*/
    int last;                                                	/* Last in buffer           	*/

    for( level = 0, last = 0;(int)( dstBuffer - startPtr ) < srcBufferSize && *tempPtr != '\0';tempPtr++ )
    {
        const int c = b64[(int)*tempPtr];
        if( c == -1 )
        {
        	continue;
        }

        switch( level )
        {
            case 0:
                level++;
                break;
            case 1:
                *dstBuffer++ = ( last << 2 ) | ( ( c >> 4)&0x03 );
                level++;
                break;
            case 2:
                *dstBuffer++ = ( ( last << 4 )&0xf0 ) | ( ( c >> 2 )&0x0f );
                level++;
                break;
            case 3:
                *dstBuffer++ = ( ( last &0x03 ) << 6 ) | c;
                level = 0;
        }
        last = c;
    }
    return dstBuffer - startPtr; /* returning the size of decoded buffer      */
}

char* base64_encode(unsigned char* pSrc, int s32SrcLen, char* pDest, int s32DestLen)
{
    static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *ret, *dst;
    unsigned i_bits = 0;
    int i_shift = 0;
    int bytes_remaining = s32SrcLen;
    if (s32SrcLen >= UINT_MAX / 4 || s32DestLen < s32SrcLen * 4 / 3 + 12)
    {
        return NULL;
    }
    ret = dst = pDest;
    if (s32SrcLen)
    {
        // edge case
        while (bytes_remaining)
        {
            i_bits = (i_bits << 8) + *pSrc++;
            bytes_remaining--;
            i_shift += 8;
            do
            {
                *dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
                i_shift -= 6;
            }
            while (i_shift > 6 || (bytes_remaining == 0 && i_shift > 0));
        }
        while ((dst - ret) & 3)
        {
            *dst++ = '=';
        }
    }
    *dst = '\0';
    return ret;
}

unsigned long long get_time_value()
{
    struct  timeval tvDate;
    gettimeofday( &tvDate, NULL );
    return((unsigned long long) tvDate.tv_sec * 1000 + tvDate.tv_usec/1000);
}

unsigned int gen_session_id()
{
	srand((unsigned)get_time_value);
	return rand();
}

unsigned long long  hl64ton(unsigned long long   host)   
{   
	unsigned long long   ret = 0;   
	unsigned int   high,low;

	low   =   host & 0xFFFFFFFF;
	high  =  (host >> 32) & 0xFFFFFFFF;
	low   =   htonl(low);   
	high  =   htonl(high);   

	//ret   =   low;
	//ret   <<= 32;   
	//ret   |=   high;   

	ret   =   high;
	ret   <<= 32;   
	ret   |=   low;   

	return   ret;   
}

 
unsigned long long  ntohl64(unsigned long long   host)   
{   
	unsigned long long   ret = 0;   
	unsigned int   high,low;

	low   =   host & 0xFFFFFFFF;
	high  =  (host >> 32) & 0xFFFFFFFF;
	low   =   ntohl(low);   
	high  =   ntohl(high);   

	//ret   =   low;
	//ret   <<= 32;   
	//ret   |=   high;   

	ret   =   high;
	ret   <<= 32;   
	ret   |=   low;  

	return   ret;   
}

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
int get_dir_space(unsigned char *pucPath, TPartInfo* stInfo)
{
	struct statfs stStatFs;
	int iRet = 0;
	if (pucPath == NULL  || pucPath[0] == '\0' || stInfo == NULL)
	{
		return -1;
	}
	memset(stInfo, 0, sizeof(TPartInfo));
	iRet = statfs((char *)pucPath, &stStatFs);
	if (iRet == 0)
	{
		stInfo->m_ulTotal = (unsigned long long)stStatFs.f_blocks * (unsigned long long)stStatFs.f_bsize;
		stInfo->m_ulFree = (unsigned long long)stStatFs.f_bavail * (unsigned long long)stStatFs.f_bsize;
		if((stInfo->m_ulTotal < 0) || (stInfo->m_ulFree < 0))
		{
			stInfo->m_ulTotal = stInfo->m_ulFree = 0;
		}
		stInfo->m_ulUsed = stInfo->m_ulTotal - stInfo->m_ulFree;
	}
	return iRet;

}

/*
* 功能描述	：	创建目录
* 成员更新	:		无
* 输入参数	： 	pcDirName 目录的路径
* 输出参数	： 	无
* 返 回 值	：		0代表成功，-1失败
* 其它说明	：	无
* 修改日期	:		2015.01.08
* -----------------------------------------------
* 2015/01/08	 V1.0		XXXX		  XXXX
*/
int make_dir(unsigned char* pcDirName)
{
	struct stat stStat;
	if (pcDirName == NULL || pcDirName[0] == '\0')
	{
		return -1;
	}
	if(stat((char *)pcDirName, &stStat) == 0)//目录存在
    {
        if(!S_ISDIR(stStat.st_mode))//如果不是文件夹
        {	
        	remove((char *)pcDirName);
            if(mkdir((char *)pcDirName, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) == 0)
            {
				return 0;
            }
            else
            {
				return -1;
            }
        }
		else//是文件修改权限
		{
			chmod((char *)pcDirName, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
 			return 0;
		}
    }
    else //目录不存在
    {
        if(mkdir((char *)pcDirName, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) == 0)
        {	
 			return 0;
        }
        else
        {
			return -1;
        }
    }
}

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
int umount_dir(const char* const psDir)
{
	if (umount2(psDir, MNT_DETACH) != 0)
	{
		return -1;//卸载失败
	}
	return 0;  
}

/*
* 功能描述	：	获取文件大小
* 成员更新	:		无
* 输入参数	： pcFileName文件名
* 输出参数	：	无
* 返 回 值	：		-1代表失败
										其他值代表文件大小
* 其它说明	：	无
* 修改日期	:		2013.08.29
* -----------------------------------------------
* 2013/08/29 V1.0		XXXX		  XXXX
*/
unsigned long get_file_size(char * const pcFileName)    
{    
    unsigned long filesize = -1;        
    struct stat statbuff;    
    if(stat(pcFileName, &statbuff) < 0)
	{    
        return filesize;    
    }
	else
    {    
        filesize = statbuff.st_size;    
    }    
    return filesize;    
}  

/**********************************************************************
* 函数名称	： SignalHandle
* 功能描述	： 信号响应函数
* 访问的表	：
* 修改的表	：
* 输入参数	：
* 输出参数	：
* 返 回 值	：
* 其它说明	：
* 修改日期
* -----------------------------------------------
* 2013/02/21     V1.0	    XXXX	      XXXX
***********************************************************************/
void SignalHandle(int signal)
{
	//static int flag_SignalHandle_tqk=0;
	switch(signal)
	{
		case SIGUSR1:	//用来接收配置文件修改命令
			ptf_suc("receive sigusr1\n");
			break;
		case SIGUSR2:	//用来发接收udev umount SD卡的命令
			ptf_suc("receive sigusr2\n");
			break;
		case SIGCONT:	//用来接收udev mount SD卡的命令
			ptf_suc("receive SIGCONT\n");
			break;
		case SIGPIPE:	//用来接收udev mount SD卡的命令
			ptf_suc("receive SIGPIPE\n");
			break;
		//case SIGINT:   //用来处理ctrl+c
			//ptf_suc("receive SIGINT\n");
			//break;
		//case SIGTERM:  //用来处理ctrl+z
			//ptf_suc("receive SIGTERM\n");
			//hi_mpp_vb_uninit();
			//flag_SignalHandle_tqk=1;
			//break;
		default:
			ptf_suc("receive sigusr other\n");
			//hi_mpp_vb_uninit();
			//flag_SignalHandle_tqk=1;
			break;
	}
	/*if(flag_SignalHandle_tqk==1)
	{
		exit(-1);
	}*/
	return ;
}
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
int ParaSignal(void)
{
	signal(SIGUSR1, SignalHandle);
	signal(SIGUSR2, SignalHandle);
	signal(SIGCONT, SignalHandle);
	signal(SIGPIPE, SignalHandle);
	//signal(SIGINT,  SignalHandle);
	//signal(SIGTERM, SignalHandle);
	return 0;
}
