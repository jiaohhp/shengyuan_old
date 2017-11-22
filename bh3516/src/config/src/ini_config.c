//标准头文件包含区域
#include <pthread.h>

//自定义头文件包含区域
#include "ini_config.h"

#include "dictionary.h"
#include "iniparser.h"
#include "printf_func.h"

//常量定义区
char*	pastrIniFile[] 		=  {(char*)"/ipc/private.ini",(char*)"/ipc/public.ini",(char*)"/ipc/avchn_config.ini"};
char*	pastrIniFilebak[] 	=  {(char*)"/ipc/private.ini.bak",(char*)"/ipc/public.ini.bak",(char*)"/ipc/avchn_config.ini.bak"};

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部变量
static 	dictionary	**	s_ptDicIni 		= NULL;
static	dictionary	**	s_ptDicIniBak 	= NULL;
static 	pthread_rwlock_t s_CfgMutex 	= PTHREAD_RWLOCK_INITIALIZER;		//配置文件互斥锁

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
int cfg_set_string(ConfigType type,char* sec,char* key,char* val)
{
	int ret = iniparser_set_string(s_ptDicIni[type], sec, key, val);
	return ret;
}

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
char* cfg_get_string(ConfigType type, char* sec, char* key, char* val)
{
	char* ret = iniparser_get_string(s_ptDicIni[type], sec, key, val);
	return ret;
}

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
const char* cfg_get_string_bak(ConfigType type,char* sec,char* key,char* val)
{
	char* ret = iniparser_get_string(s_ptDicIniBak[type], sec, key, val);
	return ret;
}

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
int cfg_set_int(ConfigType type,char* sec,char* key,int val)
{
	int ret = iniparser_set_int(s_ptDicIni[type], sec, key, val);
	return ret;
}


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
int cfg_get_int(ConfigType type,char* sec,char* key,int val)
{
	int ret =  iniparser_get_int(s_ptDicIni[type], sec, key, val);
	return ret;
}

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
int cfg_get_int_bak(ConfigType type,char* sec,char* key,int val)
{
	int ret =  iniparser_get_int(s_ptDicIniBak[type], sec, key, val);
	return ret;
}

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
int cfg_save_ini(ConfigType type)
{
    FILE *fIni = fopen(pastrIniFile[type], "w");
    if(fIni == NULL)
    {
        ptf_err("Error: fopen< %s > is failed !!!\n",pastrIniFile[type]);
        return -1;
    }
	ptf_dbg("pastrIniFile[%d] %s",type,pastrIniFile[type]);

    iniparser_dump_ini(s_ptDicIni[type], fIni);
	ptf_dbg("pastrIniFile[%d] %s",type,pastrIniFile[type]);
    fclose(fIni);

    return 0;
}

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
void cfg_init(void)
{
	int	i 			= 0;
	int num 		= sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);
    s_ptDicIni 		= (dictionary **)malloc(num * sizeof(dictionary *));
    s_ptDicIniBak 	= (dictionary **)malloc(num * sizeof(dictionary *));

	for (i = 0; i < num; i++)
	{
		s_ptDicIni[i] 		= iniparser_load(pastrIniFile[i]);
		s_ptDicIniBak[i] 	= iniparser_load(pastrIniFilebak[i]);
	}
	
    return ;
}

/*
* 功能描述	：	配置文件模块反初始化
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void cfg_uninit(void)
{
	int	i 			= 0;
	int num 		= sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);

	for (i = 0; i < num; i++)
	{
		iniparser_freedict(s_ptDicIni[i]);
		s_ptDicIni[i] = NULL;
		iniparser_freedict(s_ptDicIniBak[i]);
		s_ptDicIniBak[i] = NULL;
	}

	free(s_ptDicIni);
	s_ptDicIni = NULL;
	free(s_ptDicIniBak);
	s_ptDicIniBak = NULL;
	
    return ;
}

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
void cfg_restore_default(void)
{	
	int cnt = 0;
	char string[256];
	int num = sizeof(pastrIniFile) / sizeof(pastrIniFile[0]);
	for (cnt = 1; cnt < num; cnt++)
	{
		memset(string, 0, sizeof(string));
		sprintf(string, "cp -a %s %s",pastrIniFilebak[cnt],pastrIniFile[cnt]);
		system(string);
	}
}

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
void cfg_rwlock_rdlock(void)
{
    pthread_rwlock_rdlock(&(s_CfgMutex));
	return ;
}

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
void cfg_rwlock_wrlock(void)
{
    pthread_rwlock_wrlock(&(s_CfgMutex));
	return ;
}

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
void cfg_rwlock_unlock(void)
{
    pthread_rwlock_unlock(&(s_CfgMutex));
	return ;
}
