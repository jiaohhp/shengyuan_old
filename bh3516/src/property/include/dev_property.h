#ifndef _DEV_PROPERTY_H_
#define _DEV_PROPERTY_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct DEV_VERSION_T
{
    char    m_DevId[32];                  //�豸ID
    char    m_VBoardVer[32];           //��Ƶ��Ӳ���汾��
    char    m_SBoardVer[32];           //�ɼ���Ӳ���汾��
    char    m_VSoftVer[32];              //��Ƶ������汾��
    char    m_SSoftVer[32];              //�ɼ�������汾��
}DEV_VERSION;

/*
* ��������	��	��ȡ�豸�汾��Ϣ
* ��Ա����	:	��
* �������	��	��
* �������	��	psDevVer    :�洢�汾��Ϣ
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevVersion(DEV_VERSION *psDevVer);

/*
* ��������	��	��ȡ�豸ID
* ��Ա����	:	��
* �������	��	��
* �������	��	pcDevId     :�����洢�豸ID,�ÿռ䲻Ӧ����32���ֽ�
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevId(char* pcDevId);

/*
* ��������	��	�����豸ID
* ��Ա����	:	��
* �������	��	pcDevId     :�����洢�豸ID
* �������	��	��
* �� �� ֵ	��	��
* ����˵��	��	��
* �޸�����	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertySetDevId(char* pcDevId);

/*
* ��������	��	����mac
* ��Ա����	:		��
* �������	��	pcMac mac��ַ
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetMac(char* pcMac);

/*
* ��������	��	����mac
* ��Ա����	:		��
* �������	��	pcMac mac��ַ
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netGetMac(char* pcMac);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

