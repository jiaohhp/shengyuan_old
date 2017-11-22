#ifndef _DEV_PROPERTY_H_
#define _DEV_PROPERTY_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct DEV_VERSION_T
{
    char    m_DevId[32];                  //设备ID
    char    m_VBoardVer[32];           //视频板硬件版本号
    char    m_SBoardVer[32];           //采集板硬件版本号
    char    m_VSoftVer[32];              //视频板软件版本号
    char    m_SSoftVer[32];              //采集板软件版本号
}DEV_VERSION;

/*
* 功能描述	：	获取设备版本信息
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	psDevVer    :存储版本信息
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevVersion(DEV_VERSION *psDevVer);

/*
* 功能描述	：	获取设备ID
* 成员更新	:	无
* 输入参数	：	无
* 输出参数	：	pcDevId     :用来存储设备ID,该空间不应少于32个字节
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertyGetDevId(char* pcDevId);

/*
* 功能描述	：	设置设备ID
* 成员更新	:	无
* 输入参数	：	pcDevId     :用来存储设备ID
* 输出参数	：	无
* 返 回 值	：	无
* 其它说明	：	无
* 修改日期	:	2015.08.04
* -----------------------------------------------
* 2015/08/04	 V1.0		XXXX		  XXXX
*/
void devPropertySetDevId(char* pcDevId);

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
void netSetMac(char* pcMac);

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
void netGetMac(char* pcMac);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

