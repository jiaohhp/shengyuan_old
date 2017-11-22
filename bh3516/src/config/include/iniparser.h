#ifndef _INIPARSER_H_
#define _INIPARSER_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define ASCIILINESZ         1024
#define INI_INVALID_KEY     ((char*)-1)
#define KVCSIZE             512


/*
* 功能描述	：	通过配置文件构造字典哈希表
* 成员更新	:	无
* 输入参数	：	ininame	文件名称
* 输出参数	：	无
* 返 回 值	：	指向哈希表的指针
* 其它说明	：	无
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
dictionary *iniparser_load(char * ininame);

/*
* 功能描述	：	将哈希表内容写入文件
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
					f	文件句柄
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void 	iniparser_dump_ini(dictionary * d, FILE * f);

/*
* 功能描述	：	销毁字典哈希表
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
void 	iniparser_freedict(dictionary * d);
/*
* 功能描述	：	修改字典哈希表中字符串类型section的值
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
					sec	要修改的section
					key	要修改的key
					val	要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	0	代表成功
					-1	失败
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_set_string(dictionary *d, char* sec, char* key, char* val);

/*
* 功能描述	：	修改字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
					sec	要修改的section
					key	要修改的key
					val	要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	0	代表成功
					-1	失败
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_set_int(dictionary * d, char* sec, char* key, int  val);

/*
* 功能描述	：	获取字典哈希表中字符串类型section的值
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
					sec	要修改的section
					key	要修改的key
					val	要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的字符串
					失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
char* 	iniparser_get_string(dictionary * d, char* sec, char* key, char* val);

/*
* 功能描述	：	获取字典哈希表中整形类型section的值
* 成员更新	:	无
* 输入参数	：	d	指向字典哈希表的指针
					sec	要修改的section
					key	要修改的key
					val	要给key设置的值
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	成功返回查询到的整型值
					失败返回 val
* 修改日期	:	2013.09.06
* -----------------------------------------------
* 2013/09/06	 V1.0		XXXX		  XXXX
*/
int 	iniparser_get_int(dictionary * d, char* sec, char* key, int val);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
