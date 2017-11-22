//��׼ͷ�ļ���������
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//�Զ���ͷ�ļ���������
#include "dev_property.h"
#include "ini_config.h"
#include "printf_func.h"

//������������

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�ȫ�ֱ���

//�ֲ�����ԭ��

//���ʵ��

//�ڲ�����

//�ⲿ����

/*
* ��������	��	��ȡ�豸�汾��Ϣ
* ��Ա����	:		��
* �������	��	��
* �������	��	psDevVer    :�洢�汾��Ϣ
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevVersion(DEV_VERSION *psDevVer)
{
    memset(psDevVer, 0, sizeof(DEV_VERSION));
    cfg_rwlock_rdlock();
    strcpy(psDevVer->m_DevId, cfg_get_string(Config_Private, "dev_perporty", "dev_id", "201508040001"));
    strcpy(psDevVer->m_VBoardVer, cfg_get_string(Config_Private, "dev_perporty", "vboard_ver", "20150820"));
    strcpy(psDevVer->m_SBoardVer, cfg_get_string(Config_Private, "dev_perporty", "sboard_ver", "20150820"));
    strcpy(psDevVer->m_VSoftVer, cfg_get_string(Config_Private, "dev_perporty", "vsoft_ver", "20150925"));
    strcpy(psDevVer->m_SSoftVer, cfg_get_string(Config_Private, "dev_perporty", "ssoft_ver", "20150925"));
    cfg_rwlock_unlock();
}

/*
* ��������	��	��ȡ�豸ID
* ��Ա����	:		��
* �������	��	��
* �������	��	pcDevId     :�����洢�豸ID,�ÿռ䲻Ӧ����32���ֽ�
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevId(char* pcDevId)
{
    cfg_rwlock_rdlock();
    strcpy(pcDevId, cfg_get_string(Config_Private, "dev_perporty", "dev_id", "123456789"));
    cfg_rwlock_unlock();
}

/*
* ��������	��	�����豸ID
* ��Ա����	:		��
* �������	��	pcDevId     :�����洢�豸ID
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertySetDevId(char* pcDevId)
{
    cfg_rwlock_wrlock();
    cfg_set_string(Config_Private, "dev_perporty", "dev_id", pcDevId);
    cfg_save_ini(Config_Private);
    cfg_rwlock_unlock();
}

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
void netSetMac(char* pcMac)
{
	cfg_rwlock_wrlock();
	cfg_set_string(Config_Private, "dev_perporty", "mac", pcMac);
	cfg_save_ini(Config_Private);
	cfg_rwlock_unlock();
	return ;
}

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
void netGetMac(char* pcMac)
{
	cfg_rwlock_wrlock();
	strcpy(pcMac, cfg_get_string(Config_Private, "dev_perporty", "mac", "11:22:33:44:55:66"));
	cfg_rwlock_unlock();
	return ;
}



