//��׼ͷ�ļ���������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>	
#include <fcntl.h>

//�Զ���ͷ�ļ���������
#include "watch_dog.h"


#include "printf_func.h"

//������������

//�ļ��ڲ�ʹ�õĺ�
#define 	WATCH_DOG_DEV			"/dev/watchdog"
#define 	WATCH_DOG_TIMEOUT     	30 //���Ź���ʱʱ��

#define    	WATCHDOG_IOCTL_BASE   	'W'

#define    	WDIOC_KEEPALIVE      	_IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define    	WDIOC_SETTIMEOUT     	_IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define    	WDIOC_GETTIMEOUT     	_IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define    	WDIOC_SETOPTIONS     	_IOR(WATCHDOG_IOCTL_BASE, 4, int)


#define    	WDIOS_ENABLECARD     	0x0002    /* Turn on the watchdog timer */

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����
static int s_WatchDogHandle = -1;

//�ֲ�����ԭ��

//���ʵ��

//�ڲ�����

static int watch_dog_set_timeout(int *ptimeout)
{
  	int ret;
  	if( s_WatchDogHandle <= 0 )
  	{
	  	ptf_err("");
  	}
 
  	ret = ioctl(s_WatchDogHandle, WDIOC_SETTIMEOUT, ptimeout);
  	if (ret)
  	{
		ptf_err("");
    	return -1;
  	}
  
  	return 0;
}

static int watch_dog_get_timeout(int *ptimeout)
{
  	int ret;
  	ret = ioctl(s_WatchDogHandle, WDIOC_GETTIMEOUT, ptimeout);
  	if (ret)
  	{
    	perror("WDIOC_GETTIMEOUT");
		ptf_err("");
    	return -1;
  	}
  	return 0;
}

//ȫ�ֺ���

int watch_dog_open(void)
{
  	s_WatchDogHandle = open(WATCH_DOG_DEV, O_RDWR);
  	if(s_WatchDogHandle <= 0)
  	{
  		perror("Open_WatchDog");
		ptf_err("");
  	}
  #if 0
  	int ret;
  	ret = ioctl(s_WatchDogHandle, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);
  	if (ret)
  	{		
		perror("ioctl");
		ptf_err("s_WatchDogHandle %d",s_WatchDogHandle);
    	return -1;
  	}
#endif
	int timeout = WATCH_DOG_TIMEOUT;
  	watch_dog_set_timeout(&timeout);
	timeout = 0;
	watch_dog_get_timeout(&timeout);
	ptf_dbg("timeout %d",timeout);
  	return 0;
}

void watch_dog_keeplive(void)
{
	int ret;
  	if( WATCH_DOG_TIMEOUT != 0 )
  	{
	  	ret = ioctl(s_WatchDogHandle, WDIOC_KEEPALIVE, NULL);
	  	if (ret)
	  	{
			ptf_err("");
	    	return ;
	  	}
  	}

  	return ;
}


