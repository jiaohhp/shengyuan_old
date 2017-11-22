#ifndef __INI_CONFIG_H__
#define __INI_CONFIG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "project_func.h"

typedef enum
{
	Config_Private	= 0,	//公共配置文件
	Config_Public,			//私有设备文件
	Config_AVChn			//音视频配置文件
}ConfigType;

/*
* 功能描述	：	修改字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	0		代表成功
				-1		失败
* 修改日期	:	2013.09.16
* -----------------------------------------------
* 2013/09/16	 V1.0		XXXX		  XXXX
*/
int cfg_set_string(ConfigType type,char* sec,char* key,char* val);

/*
* 功能描述	：	获取字典哈希表中字符串类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的字符串
				失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
char* cfg_get_string(ConfigType type, char* sec, char* key, char* val);

/*
* 功能描述	：	获取字典哈希表中字符串类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的字符串
				失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
const char* cfg_get_string_bak(ConfigType type,char* sec,char* key,char* val);

/*
* 功能描述	：	修改字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	0		代表成功
				-1		失败
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_set_int(ConfigType type,char* sec,char* key,int val);


/*
* 功能描述	：	获取字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的整型值
				失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_get_int(ConfigType type,char* sec,char* key,int val);

/*
* 功能描述	：	获取字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
				sec		要修改的section
				key		要修改的key
				val		要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的整型值
				失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_get_int_bak(ConfigType type,char* sec,char* key,int val);

/*
* 功能描述	：	保存哈希表到配置文件
* 成员更新	:	无
* 输入参数	：	type	ConfigType类型
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回	0
				失败返回 	-1
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int cfg_save_ini(ConfigType type);

/*
* 功能描述	：	配置文件模块初始化
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void cfg_init(void);

/*
* 功能描述	：	配置文件模块反初始化
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void cfg_uninit(void);

/*
* 功能描述	：	配置文件恢复出厂设置
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_restore_default(void);

/*
* 功能描述	：	加读锁
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_rdlock(void);

/*
* 功能描述	：	加写锁
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_wrlock(void);

/*
* 功能描述	：	解锁
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.17
* -----------------------------------------------
* 2013/09/17 V1.0		XXXX		  XXXX
*/
void cfg_rwlock_unlock(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


