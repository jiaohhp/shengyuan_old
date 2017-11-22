#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<fcntl.h>
#include <stdlib.h>

#include"uart.h"

static int UartInit(int fd)
{
    int ret = 0;
    struct termios options;

    tcgetattr(fd, &options);

    tcflush(fd, TCIOFLUSH);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSTOPB;
    options.c_oflag  &= ~OPOST;

    tcflush(fd, TCIFLUSH);
    options.c_cc[VTIME] = 128;
    options.c_cc[VMIN] = 1;

    cfmakeraw(&options);

    ret = tcsetattr(fd, TCSANOW, &options);
    if (ret < 0)
    {
        perror("InitCom");
    }
    return ret;
}


/*
* ��������	��	���ô��ڱ�����
* ��Ա����	:	��
* �������	��	pthis	  	ָ��Uart��Ϣ��thisָ�룬����Ϊ����UartOpen���ص�ָ��
				speed 		Ҫ���õĴ��ڱ�����
* �������	��	��
* �� �� ֵ	��	-1			����ʧ��
				0			����ɹ�
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartSetSpeed(UartOper* pthis, int speed)
{
	UartOper* oper = pthis;
	
    pthread_mutex_lock(&(oper->m_lock)); 
	int fd = oper->m_fd;
    int loo;
    int status;
    struct termios opt;
    unsigned int name_arr[15] = { 115200,  38400,  19200,  9600,  4800,  2400,  1200,  300,  38400,  19200,  9600,  4800,  2400,  1200,  300};
    unsigned int sped_arr[15] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300};

    tcgetattr(fd, &opt);

    for(loo = 0; loo < (sizeof(sped_arr) / sizeof(*sped_arr)); loo++)
    {
        if(speed == name_arr[loo])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&opt, sped_arr[loo]);
            cfsetospeed(&opt, sped_arr[loo]);
            status = tcsetattr(fd, TCSANOW, &opt);
            if(status != 0)
            {
                perror("tcsetattr fd");
            }
			pthread_mutex_unlock(&(oper->m_lock)); 

            return 0;
        }

        opt.c_lflag             = 0;
        opt.c_oflag             = 0;
        opt.c_iflag             = 0;

        opt.c_cc[VINTR]         = 0;
        opt.c_cc[VQUIT]         = 0;
        opt.c_cc[VERASE]        = 0;
        opt.c_cc[VKILL]         = 0;
        opt.c_cc[VEOF]          = 0;
        opt.c_cc[VTIME]         = 1;
        opt.c_cc[VMIN]          = 0;
        opt.c_cc[VSWTC]         = 0;
        opt.c_cc[VSTART]        = 0;
        opt.c_cc[VSTOP]         = 0;
        opt.c_cc[VSUSP]         = 0;
        opt.c_cc[VEOL]          = 0;
        opt.c_cc[VREPRINT]      = 0;
        opt.c_cc[VDISCARD]      = 0;
        opt.c_cc[VWERASE]       = 0;
        opt.c_cc[VLNEXT]        = 0;
        opt.c_cc[VEOL2]         = 0;

        tcflush(fd, TCIOFLUSH);
    }
	
    pthread_mutex_unlock(&(oper->m_lock)); 
	
	return -1;
}

/*
* ��������	��	���ô�����żУ�顢����λ��ֹͣλ
* ��Ա����	:	��
* �������	��	pthis	  	ָ��Uart��Ϣ��thisָ�룬����Ϊ����UartOpen���ص�ָ��
				databits	����λ
				stopbits	ֹͣλ
				parity		��żУ��
* �������	��	��
* �� �� ֵ	��	-1			����ʧ��
				0			����ɹ�
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartSetParity(UartOper* pthis, int databits, int stopbits, int parity)
{	
	UartOper* oper = (UartOper*)pthis;
	
    pthread_mutex_lock(&(oper->m_lock)); 
	int fd = oper->m_fd;
    struct termios options;

    if(tcgetattr(fd, &options) != 0)
    {
        perror("SetupSerial 1");
		pthread_mutex_unlock(&(oper->m_lock)); 
        return(-1);
    }

    options.c_cflag &= ~CSIZE;
    switch(databits)
    {
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unsupported data size\n");
		pthread_mutex_unlock(&(oper->m_lock)); 
        return(-1);
    }

    switch(parity)
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;          	/* Clear parity enable */
        options.c_iflag &= ~INPCK;           	/* Enable parity checking */
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;           	/* Disnable parity checking */
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;          	/* Enable parity */
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;           	/* Disnable parity checking */
        break;
    case 'S':
    case 's':                                 	/*as no parity*/
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr, "Unsupported parity\n");
		pthread_mutex_unlock(&(oper->m_lock)); 
        return -1;
    }

    switch(stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr, "Unsupported stop bits\n");
		pthread_mutex_unlock(&(oper->m_lock)); 
        return(-1);
    }

    /* Set input parity option */
    if (parity != 'n')
    {
        options.c_iflag |= INPCK;
    }


    options.c_cc[VTIME] = 150;
    options.c_cc[VMIN]  = 0;

    /* Update the options and do it NOW */
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        perror("SetupSerial 3");
		pthread_mutex_unlock(&(oper->m_lock)); 
        return(-1);
    }
	
    pthread_mutex_unlock(&(oper->m_lock)); 

    return(0);
}

/*
* ��������	��	��ȡ��������
* ��Ա����	:	��
* �������	��	pthis	  	ָ��Uart��Ϣ��thisָ�룬����Ϊ����UartOpen���ص�ָ��
				size		Ҫ��ȡ�����ݴ�С
				pbuf		������ݵ��ڴ�ռ�
* �������	��	��
* �� �� ֵ	��	-1	����ʧ��
					����ֵ�����ȡ�����ֽ���
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartRead(UartOper* pthis,int size,char *pbuf)
{	
	UartOper* oper = (UartOper*)pthis;
	
    pthread_mutex_lock(&(oper->m_lock)); 
	int comfd = oper->m_fd;
	
	int nBytes = 0;
	int iRet = 0;
	fd_set fs_read;
	struct timeval timeout;
	if(NULL == pbuf)
	{
		pthread_mutex_unlock(&(oper->m_lock)); 
		return -1;
	}

	FD_ZERO(&fs_read);
	FD_SET(comfd, &fs_read);
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	iRet = select(comfd + 1, &fs_read, NULL, NULL, &timeout);
	if(iRet > 0 && FD_ISSET(comfd, &fs_read))
	{
		nBytes = read(comfd, pbuf, size);
	}
	
    pthread_mutex_unlock(&(oper->m_lock)); 
	return nBytes;

}

/*
* ��������	��	�򴮿�д������
* ��Ա����	:	��
* �������	��	pthis	  	ָ��Uart��Ϣ��thisָ�룬����Ϊ����UartOpen���ص�ָ��
				size		Ҫд������ݴ�С
				pbuf		������ݵ��ڴ�ռ�
* �������	��	��
* �� �� ֵ	��	-1	����ʧ��
					����ֵ����д����ֽ���
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartWrite(UartOper* pthis,int _iSize, char *pbuf)
{
	UartOper* oper = (UartOper*)pthis;
	
    pthread_mutex_lock(&(oper->m_lock)); 
	int comfd = oper->m_fd;

	int nBytes = 0;
	if(NULL == pbuf || _iSize <= 0 || comfd < 0)
	{
		pthread_mutex_unlock(&(oper->m_lock)); 
		return -1;
	}
	nBytes = write(comfd, pbuf, _iSize);
	
    pthread_mutex_unlock(&(oper->m_lock)); 
	return nBytes;
}

/*
* ��������	��	�رմ��ڣ��ͷ���Դ
* ��Ա����	:	��
* �������	��	pthis	  ָ��Uart��Ϣ��thisָ�룬����Ϊ����UartOpen���ص�ָ��
* �������	��	��
* �� �� ֵ	��	-1	����ʧ��
					0	����ɹ�
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
int UartClose(UartOper* pthis)
{
	UartOper* oper = (UartOper*)pthis;
	
    pthread_mutex_lock(&(oper->m_lock)); 
	int ret = close(oper->m_fd);
    pthread_mutex_unlock(&(oper->m_lock)); 
    pthread_mutex_destroy(&(oper->m_lock)); 
	free(pthis);
	
    return ret;//0 �ɹ�-1ʧ��
}

/*
* ��������	�� ��Uart�豸�ļ��������ʼ��
* ��Ա����	:	��
* �������	��	Dev	 �豸�ļ�����
* �������	��	��
* �� �� ֵ	��	�ɹ�����ָ��Uart��Ϣ��thisָ��
				ʧ�ܷ���NULL
* ����˵��	��	��
* �޸�����	:	2013.09.13
* -----------------------------------------------
* 2013/09/13	 V1.0		XXXX		  XXXX
*/
UartOper* UartOpen(const char *Dev)
{
	UartOper* oper = (UartOper*)malloc(sizeof(UartOper));
	if (NULL == oper)
	{
		return NULL;
	}
	
	
    int  fd = open( Dev, O_RDWR); //| O_NOCTTY | O_NDELAY
    if (-1 == fd)
    {
        perror("Can't Open Serial Port");
		free(oper);
        return NULL;
    }
	
    int ret = UartInit(fd);
	if (ret < 0)
	{
		close(fd);
		free(oper);
		return NULL;
	}
	
    pthread_mutex_init(&(oper->m_lock), NULL); 
	oper->m_fd 		= fd;
	return oper;
}



