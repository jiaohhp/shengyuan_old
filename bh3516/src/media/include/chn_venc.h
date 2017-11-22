#ifndef __CHN_VENC_H__
#define __CHN_VENC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_common.h"
#include "sample_comm.h"
#include "datacenter.h"

//ȫ����������
typedef struct VENC_CTRL_INFO_S
{
	VI_DEV						m_ViDev;						//��Ƶ�ɼ��豸��
	int 								m_Sensor;					//����������
  	VI_CHN						m_ViChn;						//ViChn
	VENC_GRP					m_VencGrp;					//����ͨ����
  	VENC_CHN					m_VencChn;					//VENCͨ����
   	TDataCenter*				m_DaceCenter;			//�󶨵���������

	VI_CHN						m_SnapExtViChn;		//ץ���õ���չ�ɼ�ͨ��
	VENC_GRP					m_SnapExtVencGrp;	//ץ���õı���ͨ����
	VENC_CHN					m_SnapExtVencChn;	//ץ���õı���ͨ��

} VENC_CTRL_INFO;

/*
* ��������	��	��ȡץ��ͼƬ
* �������	��	Venc	�����߼�ͨ��
* �������	��
* �� �� ֵ	��	ʧ�ܷ��ؿ�ָ��
* ����˵��	��
* �޸����� 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
DataPakg* venc_get_snap_picture(int  logicChn);

/*
* ��������	��	ǿ�ƹؼ�֡
* �������	��	�߼�ͨ��
* �������	��
* �� �� ֵ	��		0����ɹ�
										-1����ʧ��
* ����˵��	��
* �޸����� 
* -----------------------------------------------
* 2015/08/16     V1.0	    
*/
int venc_request_idr(int iLogicChn);

/*
* ��������	��	����ģ���ʼ��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0		�����
										-1	����ʧ��
* ����˵��	��
* �޸����� 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
HI_S32 venc_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


