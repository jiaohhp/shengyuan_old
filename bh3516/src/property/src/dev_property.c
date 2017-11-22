//标准头文件包含区域
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//自定义头文件包含区域
#include "dev_property.h"
#include "ini_config.h"
#include "printf_func.h"

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
* 功能描述	：	获取设备版本信息
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	psDevVer    :存储版本信息
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.04
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
* 功能描述	：	获取设备ID
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	pcDevId     :用来存储设备ID,该空间不应少于32个字节
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.04
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
* 功能描述	：	设置设备ID
* 成员更新	:		无
* 输入参数	：	pcDevId     :用来存储设备ID
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.04
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
* 功能描述	：	设置mac
* 成员更新	:		无
* 输入参数	：	pcMac mac地址
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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
* 功能描述	：	设置mac
* 成员更新	:		无
* 输入参数	：	pcMac mac地址
* 输出参数	：	无
* 返 回 值		：	无
* 其它说明	：	无
* 修改日期	:		2015.08.05
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



