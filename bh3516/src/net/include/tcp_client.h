#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

//����ѡ��

//��׼ͷ�ļ�

//�Ǳ�׼ͷ�ļ�
#include "net_para.h"

//��������

//ȫ�ֺ�

//ȫ����������
typedef enum CMD_TYPE_E
{
    CMD_MANUAL_START_PUSH,					//��ʼ����ʵʱ��
    CMD_MANUAL_STOP_PUSH,						//ֹͣ����ʵʱ��
    CMD_ELEC_STOP_SNAP,							//������������ֵʱץ��ͼƬ
    CMD_ELEC_ALARM_START_PUSH,			//������ʼ����ʵʱ��
    CMD_ELEC_ALARM_STOP_PUSH,				//����ֹͣ����ʵʱ��
    CMD_CROSS_ALARM_START_PUSH,		//������ʼ����ʵʱ��
    CMD_CROSS_ALARM_STOP_PUSH,			//����ֹͣ����ʵʱ��
    CMD_ELEC_LINK_START_PUSH,			//������ʼ����ʵʱ��
    CMD_ELEC_LINK_STOP_PUSH,				//����ֹͣ����ʵʱ��
    CMD_FAILURE_CAMERA,//����ͷ����
    CMD_FAILURE_ELEC,//��������
    CMD_FAILURE_ACC,//���ٶȹ���
    CMD_FAILURE_PRESSURE,//ѹ������
    CMD_FAILURE_LIQUID,//Һλ����
    CMD_FINISH_ELEC,// �����������ݷ���
    CMD_FINISH_ACC,//���ٶȽ������ݷ���
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
	int 						m_enable;		//0	ʧЧ1ʹ��
	unsigned short 	m_port;
	char						m_ipaddr[32];
}PC_SERVER_PARA;


//�ඨ��

//ģ��

//ȫ�ֺ���ԭ��

/*
* ��������	��	�趨��ǰץ��¼��Ķ���λ
* ��Ա����	:		��
* �������	��	dfw	����λ
* �������	��	��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientSetDFWRecSnap(DFW_E dfw);
int tcpClientGetDFWRecSnap(void);


/*
* ��������	��	�趨��ʱ������ʱ����
* ��Ա����	:		��
* �������	��	psSendTime	����ʱ����Ϣ�ṹ��
* �������	��	��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientSetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime);

/*
* ��������	��	��ȡ��ʱ������ʱ����
* ��Ա����	:		��
* �������	��	��
* �������	��	psSendTime	����ʱ����Ϣ�ṹ��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientGetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime);

/*
* ��������	��	������fifo��Ͷ������
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientHandlePushCmd(CMD_PARA* cmdPara);

/*
* ��������	��	����tcp�ͻ��˷��񣬿�ʼ����PC������
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0		�����������ӳɹ�
										1		����֮ǰ�Ѿ���������״̬
* ����˵��	��	��
* �޸�����	:		2015.08.12
* -----------------------------------------------
* 2015/08/12	 V1.0		XXXX		  XXXX
*/
int tcpClientStart(void);

/*
* ��������	��	��ȡpc����������
* ��Ա����	:		��
* �������	��	��
* �������	��	psPara 	���������ȡ���Ĳ���
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netGetPcServerPara(PC_SERVER_PARA *psPara);

/*
* ��������	��	����pc����������
* ��Ա����	:		��
* �������	��	psPara 	���������ȡ���Ĳ���
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetPcServerPara(PC_SERVER_PARA *psPara);

#endif


