#ifndef __SAMPLE_BOARD_H__
#define __SAMPLE_BOARD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "project_func.h"
#include "net_para.h"

#define		S_CMD_H_0										0x24
#define		S_CMD_H_1										0x4D
#define 		S_CMD_H_2										0x3C

#define 		S_CMD_EF											0xEF

#define		S_CMD_ID_TEMP								0x01		//传输温度数据
#define		S_CMD_ID_HUMI								0x02		//传输湿度数据
//#define		S_CMD_ID_XACC								0x03		//传输X轴加速度
//#define		S_CMD_ID_YACC								0x04		//传输Y轴加速度
//#define		S_CMD_ID_ZACC								0x05		//传输Z轴加速度
#define		S_CMD_ID_ELEC								0x06		//传输电流数据
//#define		S_CMD_ID_HYD1								0x07		//传输液压1数据
//#define		S_CMD_ID_HYD2								0x08		//传输液压2数据
#define		S_CMD_ID_LIQI								0x09		//传输液位数据

#define		S_CMD_ID_AACC								0x13		//传输所有加速度
#define		S_CMD_ID_AHYD								0x14		//传输所有液压数据

//#define		S_CMD_ID_ELEC_RECD_START				0x21		//电流动作录像开始
//#define		S_CMD_ID_ELEC_RECD_STOP				0x22		//电流动作录像停止

//#define		S_CMD_ID_SPEED_RECD_START			0x23		//过车速度录像开始
//#define		S_CMD_ID_SPEED_RECD_STOP			0x24		//过车速度录像停止

#define		S_CMD_ID_VERIFY_ACC							0x25		//校准mpu6050加速度计

#define		S_CMD_ID_REPLAY_QUERY					0x26		//回应视频板的查询命令

#define		S_CMD_ID_SET_ELEC_LIMIT				0x55		//设定电流门限值
#define		S_CMD_ID_SET_ACC_LIMIT					0x56		//设定加速度门限值

#define		S_CMD_ID_SET_ELEC_DELAY_RECD_TIME	0x57		//设定电流延时录像时间

//协议废除
//#define		S_CMD_ID_SET_SEND_TIME_INTER				0x58		//设定传输时间间隔，单位为秒

#define		S_CMD_ID_QUERY_RUN										0x59		//查询采集板是否启动

//modified by tqk

typedef struct SAMPLE_CTRL_S
{
	int 			m_iEleRecFlg;	//用于控制发送录像开始停止的变量0代表可以发送开始1代表可以发送录像停止
	int 			m_iAccRecFlg;	//用于控制发送录像开始停止的变量0代表可以发送开始1代表可以发送录像停止

	int 			m_iEleRecCurrentFlg;//用于控制当前接受到的指令
	int 			m_iAccRecCurrentFlg;//用于控制当前接收到的指令

	DFW_E		m_eDfw;
}SAMPLE_CTRL;


extern SAMPLE_CTRL s_SampleCtrl;
extern CAPTURE_PARAM s_samplePara;
//多机牵引新增 2017_08_25
void* pthread_delay_rec_stop(void* args);
void SetCurrentAccFlag(int flag);



/*
* 功能描述	：	获取采集参数
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_get_sample_para(CAPTURE_PARAM *samplePara);

/*
* 功能描述	：	获取采集参数
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_set_sample_para(CAPTURE_PARAM *samplePara);

/*
* 功能描述	：	校准加速度计
* 成员更新	:		无
* 输入参数	：	psCapPara	:存放读取到的采集板工作参数
* 输出参数	：	无
* 返 回 值	：		0	成功
										-1	失败
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int sample_board_verify_aac(void);

/*
* 功能描述	：	初始化采集模块
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sample_board_init(void);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

