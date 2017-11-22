#ifndef __SAMPLE_BOARD_H__
#define __SAMPLE_BOARD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "project_func.h"
#include "net_para.h"

#define		S_CMD_H_0										0x24
#define		S_CMD_H_1										0x4D
#define 		S_CMD_H_2										0x3C

#define 		S_CMD_EF											0xEF

#define		S_CMD_ID_TEMP								0x01		//�����¶�����
#define		S_CMD_ID_HUMI								0x02		//����ʪ������
//#define		S_CMD_ID_XACC								0x03		//����X����ٶ�
//#define		S_CMD_ID_YACC								0x04		//����Y����ٶ�
//#define		S_CMD_ID_ZACC								0x05		//����Z����ٶ�
#define		S_CMD_ID_ELEC								0x06		//�����������
//#define		S_CMD_ID_HYD1								0x07		//����Һѹ1����
//#define		S_CMD_ID_HYD2								0x08		//����Һѹ2����
#define		S_CMD_ID_LIQI								0x09		//����Һλ����

#define		S_CMD_ID_AACC								0x13		//�������м��ٶ�
#define		S_CMD_ID_AHYD								0x14		//��������Һѹ����

//#define		S_CMD_ID_ELEC_RECD_START				0x21		//��������¼��ʼ
//#define		S_CMD_ID_ELEC_RECD_STOP				0x22		//��������¼��ֹͣ

//#define		S_CMD_ID_SPEED_RECD_START			0x23		//�����ٶ�¼��ʼ
//#define		S_CMD_ID_SPEED_RECD_STOP			0x24		//�����ٶ�¼��ֹͣ

#define		S_CMD_ID_VERIFY_ACC							0x25		//У׼mpu6050���ٶȼ�

#define		S_CMD_ID_REPLAY_QUERY					0x26		//��Ӧ��Ƶ��Ĳ�ѯ����

#define		S_CMD_ID_SET_ELEC_LIMIT				0x55		//�趨��������ֵ
#define		S_CMD_ID_SET_ACC_LIMIT					0x56		//�趨���ٶ�����ֵ

#define		S_CMD_ID_SET_ELEC_DELAY_RECD_TIME	0x57		//�趨������ʱ¼��ʱ��

//Э��ϳ�
//#define		S_CMD_ID_SET_SEND_TIME_INTER				0x58		//�趨����ʱ��������λΪ��

#define		S_CMD_ID_QUERY_RUN										0x59		//��ѯ�ɼ����Ƿ�����

//modified by tqk

typedef struct SAMPLE_CTRL_S
{
	int 			m_iEleRecFlg;	//���ڿ��Ʒ���¼��ʼֹͣ�ı���0������Է��Ϳ�ʼ1������Է���¼��ֹͣ
	int 			m_iAccRecFlg;	//���ڿ��Ʒ���¼��ʼֹͣ�ı���0������Է��Ϳ�ʼ1������Է���¼��ֹͣ

	int 			m_iEleRecCurrentFlg;//���ڿ��Ƶ�ǰ���ܵ���ָ��
	int 			m_iAccRecCurrentFlg;//���ڿ��Ƶ�ǰ���յ���ָ��

	DFW_E		m_eDfw;
}SAMPLE_CTRL;


extern SAMPLE_CTRL s_SampleCtrl;
extern CAPTURE_PARAM s_samplePara;
//���ǣ������ 2017_08_25
void* pthread_delay_rec_stop(void* args);
void SetCurrentAccFlag(int flag);



/*
* ��������	��	��ȡ�ɼ�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_get_sample_para(CAPTURE_PARAM *samplePara);

/*
* ��������	��	��ȡ�ɼ�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_set_sample_para(CAPTURE_PARAM *samplePara);

/*
* ��������	��	У׼���ٶȼ�
* ��Ա����	:		��
* �������	��	psCapPara	:��Ŷ�ȡ���Ĳɼ��幤������
* �������	��	��
* �� �� ֵ	��		0	�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int sample_board_verify_aac(void);

/*
* ��������	��	��ʼ���ɼ�ģ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sample_board_init(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

