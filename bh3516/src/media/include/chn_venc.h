#ifndef __CHN_VENC_H__
#define __CHN_VENC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_common.h"
#include "sample_comm.h"
#include "datacenter.h"

//全局数据类型
typedef struct VENC_CTRL_INFO_S
{
	VI_DEV						m_ViDev;						//视频采集设备号
	int 								m_Sensor;					//穿管器类型
  	VI_CHN						m_ViChn;						//ViChn
	VENC_GRP					m_VencGrp;					//编码通道组
  	VENC_CHN					m_VencChn;					//VENC通道号
   	TDataCenter*				m_DaceCenter;			//绑定的数据中心

	VI_CHN						m_SnapExtViChn;		//抓拍用的扩展采集通道
	VENC_GRP					m_SnapExtVencGrp;	//抓拍用的编码通道组
	VENC_CHN					m_SnapExtVencChn;	//抓拍用的编码通道

} VENC_CTRL_INFO;

/*
* 功能描述	：	获取抓拍图片
* 输入参数	：	Venc	编码逻辑通道
* 输出参数	：
* 返 回 值	：	失败返回空指针
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
DataPakg* venc_get_snap_picture(int  logicChn);

/*
* 功能描述	：	强制关键帧
* 输入参数	：	逻辑通道
* 输出参数	：
* 返 回 值	：		0代表成功
										-1代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2015/08/16     V1.0	    
*/
int venc_request_idr(int iLogicChn);

/*
* 功能描述	：	编码模块初始化
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		0		代表成
										-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
HI_S32 venc_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


