//标准头文件包含区域
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

//自定义头文件包含区域
#include "public_func.h"
#include "ini_config.h"

//常量定义区

//文件内部使用的宏
#define UPDATE_FILE_NAME		"/ipc/update_file"
#define EXEC_FILE_NAME		"/ipc/IpcServer"

//文件内部使用的数据类型

//全局变量

//静态局部变量

//内部函数

//外部函数

/*
* 功能描述	：	写入升级文件
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_write_update_file(char* buf, int len)
{
	WriteDataToFile(UPDATE_FILE_NAME,buf,len);
}

/*
* 功能描述	：	检测升级文件是否存在
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_update_check(void)
{
	int ret = access(UPDATE_FILE_NAME, F_OK);
	if (0 == ret)
	{
		//升级文件存在
		char buf[64] = {0};
		sprintf(buf, "mv %s %s;chmod 777 %s",UPDATE_FILE_NAME,EXEC_FILE_NAME,EXEC_FILE_NAME);
		system(buf);
		sleep(1);
		system("reboot");
	}

}

/*
* 功能描述	：	系统重启
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sys_mag_reboot(void)
{
	cfg_rwlock_wrlock();
	system("reboot");
}

