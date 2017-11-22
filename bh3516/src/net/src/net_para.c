//��׼ͷ�ļ���������
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//�Զ���ͷ�ļ���������
#include "net_para.h"
#include "ini_config.h"
#include "dev_property.h"
#include "printf_func.h"
#include "project_func.h"

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
* ��������	��	��ȡtcp/ip����
* ��Ա����	:		��
* �������	��	��
* �������	��	psPara 	���������ȡ�����������
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netGetTcpPara(TCP_IP_PARA *psPara)
{
	cfg_rwlock_rdlock();
	strcpy(psPara->m_cIPAddr, cfg_get_string(Config_Public, "net_para", "ipaddr", "192.168.1.2"));
	strcpy(psPara->m_cNetMask, cfg_get_string(Config_Public, "net_para", "netmask", "255.255.255.0"));
	strcpy(psPara->m_cGateWay, cfg_get_string(Config_Public, "net_para", "gateway", "192.168.1.1"));
	cfg_rwlock_unlock();
	return ;
}

/*
* ��������	��	����tcp/ip����
* ��Ա����	:		��
* �������	��	psPara 	���������ȡ�����������
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetTcpPara(TCP_IP_PARA *psPara)
{
	cfg_rwlock_wrlock();
	cfg_set_string(Config_Public, "net_para", "ipaddr", psPara->m_cIPAddr);
	cfg_set_string(Config_Public, "net_para", "netmask", psPara->m_cNetMask);
	cfg_set_string(Config_Public, "net_para", "gateway", psPara->m_cGateWay);
	cfg_save_ini(Config_Public);
	cfg_rwlock_unlock();
	return ;
}

/*
* ��������	��	�����豸����
* ��Ա����	:		��
* �������	��	group �����
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetGroup(int group)
{
	cfg_rwlock_wrlock();
	cfg_set_int(Config_Public, "net_para", "group", group);
	cfg_save_ini(Config_Public);
	cfg_rwlock_unlock();
	return ;
}

/*
* ��������	��	��ȡ����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ		��	����
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
int netGetGroup(void)
{
	cfg_rwlock_rdlock();
	int group = cfg_get_int(Config_Public, "net_para", "group", 0);
	cfg_rwlock_unlock();
	return group;
}

/*
* ��������	��	��ȡ����λͨ��
* ��Ա����	:		��
* �������	�� 	��Ƶ�߼�ͨ��
* �������	�� 	��
* �� �� ֵ	��		��Ƶͨ����Ӧ�Ķ���λ
										0				����λ
										1				����λ
										-1			��������
* ����˵��	��	��
* �޸�����	:		2015.08.16
* -----------------------------------------------
* 2015/08/16		 V1.0		XXXX		  XXXX
*/
int dev_get_dfw(int chn)
{
	if (chn < 0 || chn > 1)
	{
		ptf_err("chn %d");
		return -1;
	}
	
    cfg_rwlock_rdlock();
	char key[16] = {0};
	//Ĭ��9712 (vi0)Ϊ��λ2640(vi1)Ϊ��λ
	sprintf(key, "dfw%d",chn);
    int ret = cfg_get_int(Config_Public, "location", key, DFW_DW);
    cfg_rwlock_unlock();
	return ret;
}

/*
* ��������	��	���ö���λͨ��
* ��Ա����	:		��
* �������	�� 	chn	��Ƶ�߼�ͨ��
										dfw	����λ
* �������	�� 	��
* �� �� ֵ	��		0		�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.16
* -----------------------------------------------
* 2015/08/16		 V1.0		XXXX		  XXXX
*/
int dev_set_dfw(int chn, DFW_E dfw)
{
	if (chn < 0 || chn > 1)
	{
		ptf_err("chn %d");
		return -1;
	}

	if (dfw < DFW_DW  || dfw > DFW_FW)
	{
		ptf_err("dfw %d");
		return -1;
	}
		
   	cfg_rwlock_wrlock();
	char key[16] = {0};
	sprintf(key, "dfw%d",chn);
   	cfg_set_int(Config_Public, "location", key, dfw);
	cfg_save_ini(Config_Public);
   	cfg_rwlock_unlock();
	return 0;
}

/*
* ��������	��	��ȡ����λ��Ƶͨ��
* ��Ա����	:		��
* �������	��	dfw	����λ0
* �������	��	��
* �� �� ֵ	��		-1			ʧ��
										����ֵΪ��Ӧ��ͨ����
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int GetDFWChn(DFW_E dfw)
{
	if (dfw < DFW_DW || dfw > DFW_FW )
	{
		ptf_err("dfw %d",dfw);
		return -1;
	}

	int chn;
	for (chn = 0; chn < 2; chn++)
	{
		if (dfw == dev_get_dfw(chn))
			
			return chn;  //˫����ͷ
			//return 0;  //˫����ͷ�ĵ�����ͷ
	}

	return -1;
}

/*
* ��������	��	���������ʼ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netParaInit(void)
{
	TCP_IP_PARA sTcpIpPara;
	memset(&sTcpIpPara, 0, sizeof(sTcpIpPara));
	netGetTcpPara(&sTcpIpPara);
	char mac[32] = {0};
	netGetMac(mac);

	char Para[128];
	memset(Para, 0, sizeof(Para));
	sprintf(Para,"ifconfig eth0 hw ether %s",mac);
	int s32Ret	= system(Para);
	//ptf_dbg("s32Ret=%d Para=%s",s32Ret, Para);

	memset(Para, 0, sizeof(Para));
	sprintf(Para,"ifconfig eth0 %s",sTcpIpPara.m_cIPAddr);
	sprintf(Para,"ifconfig eth0 %s netmask %s up",sTcpIpPara.m_cIPAddr,sTcpIpPara.m_cNetMask);
	s32Ret 	= system(Para);
	//ptf_dbg("s32Ret=%d Para=%s",s32Ret, Para);

	//����eth0����
	memset(Para, 0, sizeof(Para));
	sprintf(Para,"route add default gw %s",sTcpIpPara.m_cGateWay);
	s32Ret 	 = system(Para);
	//ptf_dbg("s32Ret=%d Para=%s",s32Ret, Para);

	return ;
}


