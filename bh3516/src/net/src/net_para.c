//标准头文件包含区域
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//自定义头文件包含区域
#include "net_para.h"
#include "ini_config.h"
#include "dev_property.h"
#include "printf_func.h"
#include "project_func.h"

//常量定义区域

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部全局变量

//局部函数原型

//类的实现

//内部函数

//外部函数


/*
* 功能描述	：	获取tcp/ip参数
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	psPara 	用来保存获取到的网络参数
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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
* 功能描述	：	设置tcp/ip参数
* 成员更新	:		无
* 输入参数	：	psPara 	用来保存获取到的网络参数
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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
* 功能描述	：	设置设备分组
* 成员更新	:		无
* 输入参数	：	group 分组号
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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
* 功能描述	：	获取组编号
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值		：	组编号
* 其它说明	：	无
* 修改日期	:		2015.08.05
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
* 功能描述	：	获取定反位通道
* 成员更新	:		无
* 输入参数	： 	视频逻辑通道
* 输出参数	： 	无
* 返 回 值	：		视频通道对应的定反位
										0				代表定位
										1				代表反位
										-1			参数错误
* 其它说明	：	无
* 修改日期	:		2015.08.16
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
	//默认9712 (vi0)为定位2640(vi1)为反位
	sprintf(key, "dfw%d",chn);
    int ret = cfg_get_int(Config_Public, "location", key, DFW_DW);
    cfg_rwlock_unlock();
	return ret;
}

/*
* 功能描述	：	设置定反位通道
* 成员更新	:		无
* 输入参数	： 	chn	视频逻辑通道
										dfw	定反位
* 输出参数	： 	无
* 返 回 值	：		0		成功
										-1	失败
* 其它说明	：	无
* 修改日期	:		2015.08.16
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
* 功能描述	：	获取定反位视频通道
* 成员更新	:		无
* 输入参数	：	dfw	定反位0
* 输出参数	：	无
* 返 回 值	：		-1			失败
										其他值为对应的通道号
* 其它说明	：	无
* 修改日期	:		2015.08.15
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
			
			return chn;  //双摄像头
			//return 0;  //双摄像头改单摄像头
	}

	return -1;
}

/*
* 功能描述	：	网络参数初始化
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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

	//设置eth0网关
	memset(Para, 0, sizeof(Para));
	sprintf(Para,"route add default gw %s",sTcpIpPara.m_cGateWay);
	s32Ret 	 = system(Para);
	//ptf_dbg("s32Ret=%d Para=%s",s32Ret, Para);

	return ;
}


