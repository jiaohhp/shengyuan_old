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
	int					m_fd;		//��������򿪵�uart�豸�ļ����
	pthread_mutex_t 	m_lock;		//������

}UartOper;

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
int UartSetSpeed(UartOper* pthis, int speed);

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
int UartSetParity(UartOper* pthis, int databits, int stopbits, int parity);

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
int UartRead(UartOper* pthis,int size,char *pbuf);

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
int UartWrite(UartOper* pthis,int _iSize, char *pbuf);

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
int UartClose(UartOper* pthis);

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
UartOper* UartOpen(const char *Dev);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

