#ifndef __SYS_MAG_H__
#define __SYS_MAG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


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
void sys_mag_write_update_file(char* buf, int len);

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
void sys_mag_update_check(void);

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
void sys_mag_reboot(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


