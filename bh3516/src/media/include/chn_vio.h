#ifndef __CHN_VIO_H__
#define __CHN_VIO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_common.h"
#include "sample_comm.h"

/*
* 功能描述	：	启动视频采集设备
* 成员更新	:		无
* 输入参数	： ViDev 		视频采集设备编号
										enViDevType	视频采集所选用的视频输入时序
* 输出参数	： 无
* 返 回 值	：		0	 代表成功
										其他值代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_dev(VI_DEV ViDev, SAMPLE_VI_DEV_TYPE_E enViDevType);

/*
* 功能描述	：	停止视频采集设备
* 成员更新	:		无
* 输入参数	： 	ViDev 视频采集设备编号
* 输出参数	： 无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_dev(VI_DEV ViDev);

/*
* 功能描述	：	启动视频采集通道
* 成员更新	:		无
* 输入参数	： 	ViDev 		视频采集设备编号
										ViChn 		视频采集设备的通道的采集通道号
										enViDevType	视频采集设备类型
* 输出参数	： 无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_base_chn(VI_DEV ViDev, VI_CHN ViChn, SAMPLE_VI_DEV_TYPE_E enViDevType);

/*
* 功能描述	：	停止视频采集通道
* 成员更新	:		无
* 输入参数	： 	ViChn 		视频采集设备的通道的采集通道号
* 输出参数	： 	无
* 返 回 值	：		0	代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_base_chn(VI_CHN ViChn);

/*
* 功能描述	：	启动抓拍通道
* 成员更新	:		无
* 输入参数	： 	ViBaseChn 		视频采集base通道
										ViExtChn			视频采集扩展通道
* 输出参数	： 无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_snap_ext_chn(VI_CHN ViBaseChn, VI_CHN ViExtChn);

/*
* 功能描述	：	停止抓拍通道
* 成员更新	:		无
* 输入参数	： 	ViExtChn	视频采集扩展通道
* 输出参数	： 无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_snap_ext_chn(VI_CHN ViExtChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


