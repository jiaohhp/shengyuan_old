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
	int	            		m_Socket;									//�׽���
	unsigned short	m_CheckCode;							//��֤��
	unsigned char 	m_MainId;									//��ID
	unsigned char		m_SubId;									//��ID
	unsigned char		m_BodyArrNum;						//��������

	PT_HEAD				m_ptHead;								//����ͷ
	unsigned char		m_Buf[PT_HEAD_LEN + MAX_CMD_DATA_LEN];	//���պͷ������ݻ�����
   	char 					m_ArrBuf[MAX_PARA_NUM][MAX_STRING_LEN];	//����Э����buf
}CMD_HANDLE;


/*
* ��������	��	ģ����������
* ��Ա����	:		��
* �������	��	socketFd	�׽���
* �������	��	��
* �� �� ֵ	��		�������󷵻�	-1
										�ɹ�����		0
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlDistruct(CMD_HANDLE* pthis);

/*
* ��������	��	����ͳһ��Ϣͷ
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		-1			�׽��ֳ���
										-2			Э��ͷ����
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvPtHead(CMD_HANDLE* pthis);

/*
* ��������	��	�����ڲ��Ĵ洢�ռ���ո���
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		-1			����̫��
										-2			�׽��ֳ���
										0			�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyInBuf(CMD_HANDLE* pthis);

/*
* ��������	��	�����ⲿ�Ĵ洢�ռ���ո���
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		-1			����̫��
										-2			�׽��ֳ���
										0			�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyOutBuf(CMD_HANDLE* pthis,char* buf,int buflen);

/*
* ��������	��	��ʽ�����������������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlCmdFormat(CMD_HANDLE* pthis);

/*
* ��������	��	������ID���������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ		��	0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdDevProperty(CMD_HANDLE* pthis);

/*
* ��������	��	������ID���������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ		��	0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/
int tcpCmdNetPara(CMD_HANDLE* pthis);

/*
* ��������	��	������ID���������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.03
* -----------------------------------------------
* 2015/08/03 V1.0		XXXX		  XXXX
*/
int tcpCmdHandleMainId(CMD_HANDLE* pthis);

/*
* ��������	��	������ID���������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ		��	0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/
int tcpCmdFileUpdate(CMD_HANDLE* pthis);

/*
* ��������	��	��������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										MainId	��ID
										SubId	��ID
										Body	��Ϣ��
										BodyParaNum	��Ϣ��������
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandleSendFailure(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum);

int tcpCmdHandlSendCmd1(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum);

/*
* ��������	��	��������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15 V1.0		XXXX		  XXXX
*/
int tcpCmdRegister(CMD_HANDLE* pthis);

/*
* ��������	��	��������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										SubId	��ID
										Body	��Ϣ��
										BodyParaNum	��Ϣ��������
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendCmd2(CMD_HANDLE* pthis,unsigned char SubId,char* Body,unsigned char BodyParaNum);

/*
* ��������	��	����ͼƬ
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										psVPara	��Ƶ������Ϣ
										picBuf	���ͼƬ���ڴ��ַ
										picLen	ͼƬ��С
* �������	��	��
* �� �� ֵ	��		0			�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendPicture(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen);
int tcpCmdHandlSendPicture_Elec(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen);


/*
* ��������	��	������Ƶ����
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										picBuf	���ͼƬ���ڴ��ַ
										picLen	ͼƬ��С
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoStream(CMD_HANDLE* pthis, char* streamBuf, int streamLen);

/*
* ��������	��	������Ƶ����
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										psVPara	��Ƶ�����ṹ��
* �������	��	��
* �� �� ֵ	��		0		�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoPara(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara);

/*
* ��������	��	��������
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
* �������	��	��
* �� �� ֵ	��		0			�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendHeartBeat(CMD_HANDLE* pthis);

/*
* ��������	��	���Ͳɼ�����
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										sampleData		��������
										sampleLen		�������ݴ�С
* �������	��	��
* �� �� ֵ	��		0			�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendSampleData(CMD_HANDLE* pthis, char* sampleData, int sampleLen);

/*
* ��������	��	���Ͳɼ�����
* ��Ա����	:		��
* �������	��	pthis	tcpCmdHandlConstruct����ֵ
										samplePara		��������
* �������	��	��
* �� �� ֵ	��		0			�ɹ�
										-1		ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendSamplePara(CMD_HANDLE* pthis, CAPTURE_PARAM *samplePara);

/*
* ��������	��	ģ�鹹�캯��
* ��Ա����	:		��
* �������	��	socketFd	�׽���
* �������	��	��
* �� �� ֵ	��		ʧ�ܷ���NULL
										�ɹ����ص�ַ
* ����˵��	��	��
* �޸�����	:		2015.08.02
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




