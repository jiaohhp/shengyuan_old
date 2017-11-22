#ifndef _CDM_PROTOCOL_H_
#define _CDM_PROTOCOL_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define 	MAX_STRING_LEN							(64)		//Э���зָ����ָ�ĵ����ַ������ֵ
#define 	MAX_PARA_NUM								(64)		//һ����������������еĲ�������

#define	SEPARATOR 										('|')		//�ָ��
#define	CMD_END_FLAG								(0xEF)	//���������
#define 	CMD_HEAD_LEN								(6)		//Э��̶����ֳ���
#define 	MAX_CMD_DATA_LEN						((MAX_STRING_LEN + 1) * MAX_PARA_NUM + 1 + CMD_HEAD_LEN)	//��������Э�鳤��


#define	ERR_NO												0x0000		//�ɹ�
#define 	ERR_UNKNOW									0x0001		//δ֪����

//////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_DEV_PROPERTY					0x01			//�豸������ID

#define	SUB_REQ_DEV_VERSION				0x0001		//��ȡ�豸�汾��Ϣ
#define	SUB_RSP_DEV_VERSION				0x0002		//�ظ��豸�汾��Ϣ

#define	SUB_SET_DEV_ID							0x0003		//�����豸ID
#define	SUB_SET_DEV_ID_RSP					0x0004		//�ظ������豸ID

#define	SUB_GET_DEV_ID							0x0005		//��ȡ�豸ID
#define	SUB_GET_DEV_ID_RSP					0x0006		//�ظ���ȡ�豸ID
//////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_NET_PARA								0x02			//���������ID

#define 	SUB_GET_TCP_IP_PARA				0x0001		//��ȡ�豸tcp/ip����
#define	SUB_GET_TCP_IP_PARA_RSP		0x0002		//�ظ���ȡ�豸tcp/ip����

#define 	SUB_SET_TCP_IP_PARA					0x0003		//�����豸tcp/ip����
#define	SUB_SET_TCP_IP_PARA_RSP		0x0004		//�ظ������豸tcp/ip����

#define	SUB_GET_MAC									0x0005		//��ȡMAC
#define	SUB_GET_MAC_RSP							0x0006		//�ظ���ȡMAC

#define	SUB_SET_MAC									0x0007		//����MAC
#define	SUB_SET_MAC_RSP							0x0008		//�ظ�����AC

#define	SUB_GET_DEV_GROUP_NO			0x0009		//��ȡ�豸������
#define	SUB_GET_DEV_GROUP_NO_RSP	0x000A		//�ظ���ȡ�豸������

#define	SUB_SET_DEV_GROUP_NO				0x000B		//�����豸������
#define	SUB_SET_DEV_GROUP_NO_RSP	0x000C		//�ظ������豸������

#define	SUB_GET_PC_SEVER_CONFIG				0x000D		//��ȡPC����������
#define	SUB_GET_PC_SERVER_CONFIG_RSP	0x000E		//�ظ���ȡPC����������

#define	SUB_SET_PC_SERVER_CONFIG				0x000F		//����PC����������
#define	SUB_SET_PC_SERVER_CONFIG_RSP	0x0010		//�ظ�����PC����������
/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_SAMPLE_BOARD_PARA				0x03		//�ɼ�����ز���

#define	SUB_GET_SAMPLE_PARA					0x0001		//��ȡ�ɼ��幤������
#define	SUB_GET_SAMPLE_PARA_RSP			0x0002		//�ظ���ȡ�ɼ��幤������

#define	SUB_SET_SAMPLE_PARA					0x0003		//���òɼ��幤������
#define	SUB_SET_SAMPLE_PARA_RSP			0x0004		//�ظ����òɼ��幤������

#define	SUB_VERIFY_ACC								0x0005		//У׼���ٶȼ�
#define 	SUB_VERIFY_ACC_RSP						0x0006		//�ظ�У׼���ٶȼ�

#define	SUB_GET_SEND_INTER_TIME				0x0007		//��ȡ�ɼ����ݷ���ʱ����
#define 	SUB_GET_SEND_INTER_TIME_RSP		0x0008		//�ظ���ȡ�ɼ����ݷ���ʱ����

#define	SUB_SET_SEND_INTER_TIME				0x0009		//���òɼ����ݷ���ʱ����
#define 	SUB_SET_SEND_INTER_TIME_RSP		0x000A		//�ظ����òɼ����ݷ���ʱ����

#define SUB_RETURN_FINISH_SIMBOL            0x000B//�������ݽ����ı�־

/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_FILE_UPDATE							0x04		//�ļ�����

#define	SUB_FILE_UPDATE								0x0001		//�ļ�����
#define	SUB_FILE_UPDATE_RSP						0x0002		//�ظ��ļ�����
/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_MEDIA_CONTROL						0x05		//ý��������

#define	SUB_PC_TO_DEV_STREAM_START			0x0001		//��ʼ������Ƶ��
#define	SUB_PC_TO_DEV_STREAM_START_RSP	0x0002		//�ظ���ʼ������Ƶ��

#define	SUB_PC_TO_DEV_STREAM_STOP				0x0003		//ֹͣ������Ƶ��
#define	SUB_PC_TO_DEV_STREAM_STOP_RSP	0x0004		//�ظ�ֹͣ������Ƶ��

#define	SUB_DEV_TO_PC_STREAM_START			0x0005		//��ʼ������Ƶ��
#define	SUB_DEV_TO_PC_STREAM_START_RSP	0x0006		//�ظ���ʼ������Ƶ��

#define	SUB_DEV_TO_PC_STREAM_STOP				0x0007		//ֹͣ������Ƶ��
#define	SUB_DEV_TO_PC_STREAM_STOP_RSP	0x0008		//�ظ�ֹͣ������Ƶ��

#define	SUB_MANUAL_GET_SNAP_PIC					0x0009		//�ֶ���ȡץ��ͼƬ
#define	SUB_MANUAL_GET_SNAP_PIC_RSP			0x000A		//�ظ��ֶ���ȡץ��ͼƬ
#define SUB_RED_STRAP_START		0x0B		//red-light strap started
#define SUB_RED_STRAP_FINISH		0x0C		//red-light strap finished, may not a pair to red-light started.
////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_MC												0x06			//�㲥����

#define	SUB_SEARCH_DEV								0x0001		//���������豸
#define	SUB_SEARCH_DEV_RSP						0x0002		//�ظ����������豸

#define	SUB_LINK_GROUP_RECD_START		0x0003		//����ͬ���豸��ʼ¼��
#define	SUB_LINK_GROUP_RECD_STOP		0x0004		//����ͬ���豸ֹͣ¼��
///////////////////////////////////////////////////////////////////////////////////////////////
#define	SUB_DEV_MANAGE								0x07			//�豸����

#define	SUB_DEV_REGISTER							0x0001		//�豸ע��
#define	SUB_DEV_REGISTER_RSP					0x0002		//�ظ��豸ע��

#define	SUB_DEV_REBOOT								0x0003		//�����豸
#define	SUB_DEV_REBOOT_RSP						0x0004		//�ظ������豸

#define	SUB_GET_CAMERA_POS						0x0005		//��ȡ����ͷ����λ
#define	SUB_GET_CAMERA_POS_RSP			0x0006		//�ظ���ȡ����ͷ����λ

#define	SUB_SET_CAMERA_POS						0x0007		//��������ͷ����λ
#define	SUB_SET_CAMERA_POS_RSP			0x0008		//�ظ���������ͷ����λ

#define SUB_SET_CAMERA_STATE					0x0009		//������Ƶ�嶨��λ״̬
#define SUB_SET_CAMERA_STATE_RSP		0x000a		//�ظ�������Ƶ�嶨��λ״̬

#define SUB_GET_CAMERA_STATE					0x000b		//������Ƶ�嶨��λ״̬
#define SUB_GET_CAMERA_STATE_RSP		0x000c		//�ظ�������Ƶ�嶨��λ״̬

///////////////////////////////////////////////////////////////////////////////////////////////
//modeifed by tqk
#define SUB_DEV_FAILURE 						    0x08
#define SUB_DEV_FAILURE_TYPE						0x0001
#define CAMERA_FAILURE	1
#define ELEC_FAILURE    2
#define ACC_FAILURE     3
#define PRESSURE_FAILURE 4
#define LIQUID_FAILURE  5
#define ACC_FINISH_SIMBOL 1
#define ELEC_FINISH_SIMBOL 2



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

