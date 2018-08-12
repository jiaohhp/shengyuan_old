#ifndef __PROJECT_FUNC_H__
#define __PROJECT_FUNC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdint.h>


#include "cmd_protocol.h"

typedef struct PoolDataVideo_t
{
	int							m_VFrameType;			//帧类型
	unsigned long long	m_PTS;							//时间戳	
	int 							m_VCount;					//帧计数
}PoolDataVideo;

typedef enum MY_PIC_SIZE_E
{
    MY_PIC_QCIF = 0,
    MY_PIC_CIF,    
    MY_PIC_2CIF,    
    MY_PIC_HD1, 
    MY_PIC_D1,
    
    MY_PIC_QVGA,    	/* 320 * 240 */
    MY_PIC_VGA,     		/* 640 * 480 */    
    MY_PIC_XGA,     		/* 1024 * 768 */   
    MY_PIC_SXGA,    	/* 1400 * 1050 */    
    MY_PIC_UXGA,    	/* 1600 * 1200 */    
    MY_PIC_QXGA,    	/* 2048 * 1536 */

    MY_PIC_WVGA,    	/* 854 * 480 */
    MY_PIC_WSXGA,   	/* 1680 * 1050 */      
    MY_PIC_WUXGA,   	/* 1920 * 1200 */
    MY_PIC_WQXGA,   	/* 2560 * 1600 */
    
    MY_PIC_HD720,   	/* 1280 * 720 */
    MY_PIC_HD1080,  	/* 1920 * 1080 */
    MY_PIC_1280_800, 	/* 1280 * 800 */
    MY_PIC_800_600,  	/* 800 * 600 */
    MY_PIC_960H,
    MY_PIC_480H,
    
    MY_PIC_BUTT
}MY_PIC_SIZE;


typedef struct W_H_S
{
    int u32Width;
    int u32Height;
} W_H;
typedef struct POINT_S
{
    int s32X;
    int s32Y;
} POINT;

typedef struct RECT_S
{
    int s32X;
    int s32Y;
    int u32Width;
    int u32Height;
} RECT;

typedef enum PT_LOAD_TYPE_E
{    
    CMD_DATA 		= 0,       	//信令数据
    VIDEO_PARA 		= 1,		//视频参数
    VIDEO_DATA		= 2,		//视频数据
    HEART_BEAT		= 3,		//心跳数据
    VIDEO_PICTURE	= 4,		//图片数据
    SAMPLE_DATA		= 5,		//采集数据
    MANUAL_SNAP_PICTURE = 6,    //xhjiao
    LAST,
}PT_LOAD_TYPE_E;

#define PT_HEAD_TAG				"BNX"		    	//百纳信标识头
#define DATA_HEAD_TAG	    	"STDH"
#define INFO_HEAD_TAG	    	"STFH"
#define FILE_UPDATE_TAG	   	"BNXD"


#define					PT_HEAD_LEN			(8)

typedef struct PT_HEAD_T
{
	unsigned char 	m_headTag[3];		//头部标识
	unsigned char 	m_ptType;				//附载类型
	unsigned int			m_ptLen;					//附载长度
} PT_HEAD;


#define					VIDEO_PARAM_LEN		(6)

typedef struct VIDEO_PARAM_T
{
	unsigned short 	m_width;			//图像宽度
	unsigned short	m_height;			//图像高度
	unsigned char		m_chn;				//视频通道
	unsigned char		m_positon;		//定反位标识
} VIDEO_PARAM;

#define CAP_DATA_INFO_LEN 		18

typedef struct CAPTURE_DATA_INFO_T
{
	unsigned short		m_TempNum;		//温度数据个数
	unsigned short		m_HumiNum;		//湿度数据个数
	unsigned short		m_XAccNum;		//X轴加速度个数
	unsigned short		m_YAccNum;		//Y轴加速度个数
	unsigned short		m_ZAccNum;		//Z轴加速度个数
	unsigned short		m_ElecNum;			//电流数据个数
	unsigned short		m_Hydp1Num;		//液压1数据个数
	unsigned short		m_Hydp2Num;		//液压2数据个数
	unsigned short		m_LiqlNum;			//液位数据个数
} CAPTURE_DATA_INFO;


#define CAPTURE_PARA_LEN		9

typedef struct CAPTURE_PARAM_T
{
	unsigned char 	m_eaLimit_I;					//电流门限值整数位
	unsigned char 	m_eaLimit_D;					//电流门限值小数位
	unsigned char		m_accLimit_I;					//加速度门限值整数位
	unsigned char		m_accLimit_D;					//加速度门限值小数位

	unsigned char 	m_acRecDelayTime_I;	//电流结束延时录像时间整数位
	unsigned char 	m_acRecDelayTime_D;	//电流结束延时录像时间小数位

} CAPTURE_PARAM;

#define					UPDATE_FILE_LEN		(20)


typedef struct UPDATE_FILE_T
{
	unsigned char		m_company[4];		//固定为"BNXD"
	unsigned int			m_filesize;				//升级文件大小不包括文件信息头
	unsigned char		m_productID;	    	//产品id 目前为0
	unsigned char		m_filetype;				//升级文件类型1 代表内核执行程序
	unsigned short	m_version;				//硬件版本号 目前为 0
	int64_t  				m_checksum;			//校验和（不包括信息头）
} UPDATE_FILE;


/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCaptureData2Buf(CAPTURE_DATA_INFO *psHead, unsigned char* outBuf);


/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutPtHead2Buf(PT_HEAD *psHead, unsigned char* outBuf);

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2PtHead(PT_HEAD *psHead, unsigned char* inBuf);

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutVideoPara2Buf(VIDEO_PARAM *psHead, unsigned char* outBuf);

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2VideoPara(VIDEO_PARAM *psHead, unsigned char* inBuf);

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutCapParam2Buf(CAPTURE_PARAM *psHead, unsigned char* outBuf);

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2CapParam(CAPTURE_PARAM *psHead, unsigned char* inBuf);

/*
* 功能描述	：	将结构体转内容按照网络字节序放置到buf中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutUpdatFile2Buf(UPDATE_FILE *psHead, unsigned char* outBuf);

/*
* 功能描述	：	将buf中网络字节序的数据转化为主机字节序放置到结构体中
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		转换的字节数
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int PutBuf2UpdatFile(UPDATE_FILE *psHead, unsigned char* inBuf);

/*
* 功能描述	：	将字符串按照分隔符格式化到字符串数组
* 成员更新	:		无
* 输入参数	：	strSource	要格式化的字符串
										Separator	分隔符
										arrayNum	数组的一维大小
* 输出参数	：	stringArray	存放格式化后的字符串
* 返 回 值	：		大于 	0 	成功返回字符串成功格式化的个数
										小于	0 	返回失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02	 V1.0		XXXX		  XXXX
*/
int  DivideStringBySeparator(char* strSource, char(*stringArray)[MAX_STRING_LEN],char Separator,int arrayNum);

/*
* 功能描述	：	根据分辨率获取视频的宽和高
* 成员更新	:		无
* 输入参数	：	enPicSize		分辨率枚举类型
* 输出参数	：	pstSize		视频的宽和高
* 返 回 值	：		0 	成功
										-1	失败
* 其它说明	：	无
* 修改日期	:		2013.09.18
* -----------------------------------------------
* 2013/09/18	 V1.0		XXXX		  XXXX
*/
int GetPicSize(MY_PIC_SIZE enPicSize, W_H *pstSize);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

