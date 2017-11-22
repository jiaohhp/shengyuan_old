#ifndef _CDM_PROTOCOL_H_
#define _CDM_PROTOCOL_H_
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define 	MAX_STRING_LEN							(64)		//协议中分隔符分割的单个字符串最大值
#define 	MAX_PARA_NUM								(64)		//一条命令体中最多运行的参数个数

#define	SEPARATOR 										('|')		//分割符
#define	CMD_END_FLAG								(0xEF)	//命令结束符
#define 	CMD_HEAD_LEN								(6)		//协议固定部分长度
#define 	MAX_CMD_DATA_LEN						((MAX_STRING_LEN + 1) * MAX_PARA_NUM + 1 + CMD_HEAD_LEN)	//最大的命令协议长度


#define	ERR_NO												0x0000		//成功
#define 	ERR_UNKNOW									0x0001		//未知错误

//////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_DEV_PROPERTY					0x01			//设备属性主ID

#define	SUB_REQ_DEV_VERSION				0x0001		//获取设备版本信息
#define	SUB_RSP_DEV_VERSION				0x0002		//回复设备版本信息

#define	SUB_SET_DEV_ID							0x0003		//设置设备ID
#define	SUB_SET_DEV_ID_RSP					0x0004		//回复设置设备ID

#define	SUB_GET_DEV_ID							0x0005		//获取设备ID
#define	SUB_GET_DEV_ID_RSP					0x0006		//回复获取设备ID
//////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_NET_PARA								0x02			//网络参数主ID

#define 	SUB_GET_TCP_IP_PARA				0x0001		//获取设备tcp/ip参数
#define	SUB_GET_TCP_IP_PARA_RSP		0x0002		//回复获取设备tcp/ip参数

#define 	SUB_SET_TCP_IP_PARA					0x0003		//设置设备tcp/ip参数
#define	SUB_SET_TCP_IP_PARA_RSP		0x0004		//回复设置设备tcp/ip参数

#define	SUB_GET_MAC									0x0005		//获取MAC
#define	SUB_GET_MAC_RSP							0x0006		//回复获取MAC

#define	SUB_SET_MAC									0x0007		//设置MAC
#define	SUB_SET_MAC_RSP							0x0008		//回复设置AC

#define	SUB_GET_DEV_GROUP_NO			0x0009		//获取设备分组编号
#define	SUB_GET_DEV_GROUP_NO_RSP	0x000A		//回复获取设备分组编号

#define	SUB_SET_DEV_GROUP_NO				0x000B		//设置设备分组编号
#define	SUB_SET_DEV_GROUP_NO_RSP	0x000C		//回复设置设备分组编号

#define	SUB_GET_PC_SEVER_CONFIG				0x000D		//获取PC服务器配置
#define	SUB_GET_PC_SERVER_CONFIG_RSP	0x000E		//回复获取PC服务器配置

#define	SUB_SET_PC_SERVER_CONFIG				0x000F		//设置PC服务器配置
#define	SUB_SET_PC_SERVER_CONFIG_RSP	0x0010		//回复设置PC服务器配置
/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_SAMPLE_BOARD_PARA				0x03		//采集板相关参数

#define	SUB_GET_SAMPLE_PARA					0x0001		//获取采集板工作参数
#define	SUB_GET_SAMPLE_PARA_RSP			0x0002		//回复获取采集板工作参数

#define	SUB_SET_SAMPLE_PARA					0x0003		//设置采集板工作参数
#define	SUB_SET_SAMPLE_PARA_RSP			0x0004		//回复设置采集板工作参数

#define	SUB_VERIFY_ACC								0x0005		//校准加速度计
#define 	SUB_VERIFY_ACC_RSP						0x0006		//回复校准加速度计

#define	SUB_GET_SEND_INTER_TIME				0x0007		//获取采集数据发送时间间隔
#define 	SUB_GET_SEND_INTER_TIME_RSP		0x0008		//回复获取采集数据发送时间间隔

#define	SUB_SET_SEND_INTER_TIME				0x0009		//设置采集数据发送时间间隔
#define 	SUB_SET_SEND_INTER_TIME_RSP		0x000A		//回复设置采集数据发送时间间隔

#define SUB_RETURN_FINISH_SIMBOL            0x000B//发送数据结束的标志

/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_FILE_UPDATE							0x04		//文件升级

#define	SUB_FILE_UPDATE								0x0001		//文件升级
#define	SUB_FILE_UPDATE_RSP						0x0002		//回复文件升级
/////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_MEDIA_CONTROL						0x05		//媒体流控制

#define	SUB_PC_TO_DEV_STREAM_START			0x0001		//开始发送视频流
#define	SUB_PC_TO_DEV_STREAM_START_RSP	0x0002		//回复开始发送视频流

#define	SUB_PC_TO_DEV_STREAM_STOP				0x0003		//停止发送视频流
#define	SUB_PC_TO_DEV_STREAM_STOP_RSP	0x0004		//回复停止发送视频流

#define	SUB_DEV_TO_PC_STREAM_START			0x0005		//开始发送视频流
#define	SUB_DEV_TO_PC_STREAM_START_RSP	0x0006		//回复开始发送视频流

#define	SUB_DEV_TO_PC_STREAM_STOP				0x0007		//停止发送视频流
#define	SUB_DEV_TO_PC_STREAM_STOP_RSP	0x0008		//回复停止发送视频流

#define	SUB_MANUAL_GET_SNAP_PIC					0x0009		//手动获取抓拍图片
#define	SUB_MANUAL_GET_SNAP_PIC_RSP			0x000A		//回复手动获取抓拍图片
#define SUB_RED_STRAP_START		0x0B		//red-light strap started
#define SUB_RED_STRAP_FINISH		0x0C		//red-light strap finished, may not a pair to red-light started.
////////////////////////////////////////////////////////////////////////////////////////////////
#define	MAIN_MC												0x06			//广播命令

#define	SUB_SEARCH_DEV								0x0001		//搜索在线设备
#define	SUB_SEARCH_DEV_RSP						0x0002		//回复搜索在线设备

#define	SUB_LINK_GROUP_RECD_START		0x0003		//联动同组设备开始录像
#define	SUB_LINK_GROUP_RECD_STOP		0x0004		//联动同组设备停止录像
///////////////////////////////////////////////////////////////////////////////////////////////
#define	SUB_DEV_MANAGE								0x07			//设备管理

#define	SUB_DEV_REGISTER							0x0001		//设备注册
#define	SUB_DEV_REGISTER_RSP					0x0002		//回复设备注册

#define	SUB_DEV_REBOOT								0x0003		//重启设备
#define	SUB_DEV_REBOOT_RSP						0x0004		//回复重启设备

#define	SUB_GET_CAMERA_POS						0x0005		//获取摄像头定反位
#define	SUB_GET_CAMERA_POS_RSP			0x0006		//回复获取摄像头定反位

#define	SUB_SET_CAMERA_POS						0x0007		//设置摄像头定反位
#define	SUB_SET_CAMERA_POS_RSP			0x0008		//回复设置摄像头定反位

#define SUB_SET_CAMERA_STATE					0x0009		//设置视频板定反位状态
#define SUB_SET_CAMERA_STATE_RSP		0x000a		//回复设置视频板定反位状态

#define SUB_GET_CAMERA_STATE					0x000b		//设置视频板定反位状态
#define SUB_GET_CAMERA_STATE_RSP		0x000c		//回复设置视频板定反位状态

///////////////////////////////////////////////////////////////////////////////////////////////
//modeifed by tqk
#define SUB_DEV_FAILURE 						    0x08
#define SUB_DEV_FAILURE_TYPE						0x0001
#define CAMERA_FAILURE	1
#define ELEC_FAILURE    2
#define ACC_FAILURE     3
#define PRESSURE_FAILURE 4
#define LIQUID_FAILURE  5
#define ACC_FINISH_SIMBOL 1
#define ELEC_FINISH_SIMBOL 2



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

