//��׼ͷ�ļ���������
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>

//�Զ���ͷ�ļ���������
#include "cmd_handle.h"
#include "cmd_protocol.h"
#include "dev_property.h"
#include "tcp_client.h"
#include "printf_func.h"
#include "datapool.h"
#include "global_para.h"
#include "public_func.h"
#include "net_para.h"
#include "thread_pool.h"
#include "global_para.h"
#include "net_para.h"
#include "datafifo.h"
#include "project_func.h"
#include "tcp_client.h"
#include "ini_config.h"
#include "gpio_cfg.h"
#include "sample_board.h"


//������������

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������
typedef struct CMD_CLIENT_S
{
	CMD_HANDLE* 		m_pcCmdHandle;							//����ṹ��

	CMD_PARA				m_pushCmdPara;								//���ڼ�¼��������Ͳ���
	int							m_pushStreamFlg;							//����ʵʱ���ر�ʶ1����0������
	int							m_pushChn;										//����ʵʱ����ͨ��
	unsigned long long 	m_lastPicSenTime;							//�ϴη�����Ƭ��ʱ�䣬��λΪ����
	unsigned long long 	m_lastSampleDataSenTime;		//�ϴη��Ͳɼ����ݵ�ʱ�䣬��λΪ����
	unsigned long long 	m_lastHeartSenTime;		//�ϴη��Ͳɼ����ݵ�ʱ�䣬��λΪ����
	unsigned long long				m_interPicTimes;							//ͼƬ����ʱ����,��λΪ����
	unsigned long long				m_interSampleDataTimes;			//ͼƬ����ʱ����,��λΪ����
	TDataFIFO*				m_vChn0Fifo;									//ͨ��0 fifo
	TDataFIFO*				m_vChn1Fifo;									//ͨ��1 fifo
	TDataFIFO*				m_vSndFifo;										//����ʵʱ����fifo
	unsigned long long      m_lastLagDataSenTime;
}CMD_CLIENT;

typedef enum REC_ACTION_E
{
	ACTION_ELEC = 0,
	ACTION_ACC,
	ACTION_LINK,
}REC_ACTION;


//ȫ�ֱ���

//��̬�ֲ�ȫ�ֱ���
static DFW_E sDfw	;				//��ǰ�����Ķ���λ
CMD_CLIENT sCmdClient;
static int s_tcpClientState = 0;
static int manualChn;

//�ֲ�����ԭ��

//���ʵ��

//�ڲ�����

/*
* ��������	��	����fifo�е�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/

/*
		DataPakg* picture;
		int logicChn = GetDFWChn(sDfw);
		ptf_dbg("current curnel = %d",sDfw);
		gpio_led_ctrl(logicChn, led_on,led_snap);
		usleep(500000);
		picture = venc_get_snap_picture(logicChn);
		if (NULL != picture)
		{
			VIDEO_PARAM sVPara;
			sVPara.m_width 		= BH_DEST_WIDTH;
			sVPara.m_height 	= BH_DEST_HEIGHT;
			sVPara.m_chn 		= logicChn;
			sVPara.m_positon 	= sDfw;
			iRet = tcpCmdHandlSendPicture(psCmdClient->m_pcCmdHandle, &sVPara,picture->m_bData, picture->m_iDataSize);
			//ptf_dbg("iRet %dpicture->m_iDataSize %d",iRet,picture->m_iDataSize);
			DataPkgRelease(picture);
		}
		usleep(500000);
		gpio_led_ctrl(logicChn, led_off,led_snap);

*/
static int tcpClientHandleFIfoCmd(CMD_CLIENT* psCmdClient)
{
	int iRet = 0;
	char body[32] = {0};
	DataPakg * cmdPkg = DataFifoGet(g_cmdFifo);
	if (NULL != cmdPkg)
	{
		CMD_PARA* psPara = (CMD_PARA*)(cmdPkg->m_datainfo);
		if (CMD_ELEC_STOP_SNAP == psPara->m_eType)
		{	
				DataPakg* picture;
				//int logicChn = GetDFWChn(psPara->m_para1);
				int logicChn = GetDFWChn(sDfw);
				ptf_dbg("current curnel_stopCurrent=%d",logicChn);
				gpio_led_ctrl(logicChn, led_on,led_snap);
				usleep(1100000);
				picture = venc_get_snap_picture(logicChn);
				if (NULL != picture)
				{
					VIDEO_PARAM sVPara;
					sVPara.m_width 		= BH_DEST_WIDTH;
					sVPara.m_height 	= BH_DEST_HEIGHT;
					sVPara.m_chn 		= logicChn;
					sVPara.m_positon 	= sDfw;
					sVPara.m_positon    = (sDfw|0x10);
					iRet = tcpCmdHandlSendPicture(psCmdClient->m_pcCmdHandle, &sVPara,picture->m_bData, picture->m_iDataSize);
					DataPkgRelease(picture);
				}
				usleep(500000);
				gpio_led_ctrl(logicChn, led_off,led_snap);
		}
		else if (CMD_MANUAL_START_PUSH == psPara->m_eType)
		{	
			//int logicChn = GetDFWChn(psPara->m_para1);
			if (1 == psCmdClient->m_pushStreamFlg)
			{	
				//�������������Ƶ��
				if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					DataPkgRelease(cmdPkg);
					return 0;
				}
				else if (CMD_ELEC_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					sprintf(body, "%d|",ACTION_ELEC);
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,1);
				}
				else if (CMD_CROSS_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					sprintf(body, "%d|",ACTION_ACC);
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,1);
				}
				else if (CMD_ELEC_LINK_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					sprintf(body, "%d|",ACTION_LINK);
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,1);
				}
			}

			int dfw = psPara->m_para1;
			int chn = GetDFWChn(dfw);
			manualChn=chn;
			if (chn == 0)
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn0Fifo;
			}
			else
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn1Fifo;
			}
			DataFifoClear(psCmdClient->m_vSndFifo);
			psCmdClient->m_pushStreamFlg = 1;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(chn, led_on,led_recd);
			////printf("tqk________________________tqk_led_on is %d\n", chn);
		}
		else if (CMD_ELEC_ALARM_START_PUSH == psPara->m_eType)
		{	
			if (1 == psCmdClient->m_pushStreamFlg)
			{
				//�������������Ƶ��
				if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					DataPkgRelease(cmdPkg);
					return 0;
				}
				else if (CMD_ELEC_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_CROSS_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_ELEC_LINK_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
			}

			//���Ϳ�ʼ����
			sprintf(body, "%d|%d|%d|%d|%d|",ACTION_ELEC,GetDFWChn(sDfw),sDfw,BH_DEST_WIDTH,BH_DEST_HEIGHT);
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_START,body,5);
			int dfw = psPara->m_para1;
			int chn = GetDFWChn(dfw);
			if (chn == 0)
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn0Fifo;
			}
			else
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn1Fifo;
			}
			DataFifoClear(psCmdClient->m_vSndFifo);
			psCmdClient->m_pushStreamFlg = 1;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(chn, led_on,led_recd);
		}
		else if ( (CMD_CROSS_ALARM_START_PUSH == psPara->m_eType) || (CMD_RED_STRAP_START_PUSH == psPara->m_eType) )
		{	
			if (1 == psCmdClient->m_pushStreamFlg)
			{
				//�������������Ƶ��
				if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					DataPkgRelease(cmdPkg);
					return 0;
				}
				else if (CMD_ELEC_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_CROSS_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_ELEC_LINK_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
			}
			
			//���Ϳ�ʼ����
			sprintf(body, "%d|%d|%d|%d|%d|",ACTION_ACC,GetDFWChn(sDfw),sDfw,BH_DEST_WIDTH,BH_DEST_HEIGHT);
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_START,body,5);

			int dfw = psPara->m_para1;
			int chn = GetDFWChn(dfw);
			if (chn == 0)
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn0Fifo;
			}
			else
			{
				psCmdClient->m_vSndFifo = 	psCmdClient->m_vChn1Fifo;
			}
			DataFifoClear(psCmdClient->m_vSndFifo);
			psCmdClient->m_pushStreamFlg = 1;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(chn, led_on,led_recd);
		}
		else if (CMD_ELEC_LINK_START_PUSH == psPara->m_eType)
		{	
			if (1 == psCmdClient->m_pushStreamFlg)
			{
				//�������������Ƶ��
				if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					DataPkgRelease(cmdPkg);
					return 0;
				}
				else if (CMD_ELEC_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_CROSS_ALARM_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
				else if (CMD_ELEC_LINK_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
				{
					//����ֹͣ¼������ֶ��㲥���ȼ��������������
					tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
				}
			}
			int dfw = tcpClientGetDFWRecSnap();
			if(dfw == DFW_DW){
				dfw = DFW_FW;
			}else{
				dfw = DFW_DW;
			}
			tcpClientSetDFWRecSnap(dfw);
			
			//���Ϳ�ʼ����
			sprintf(body, "%d|%d|%d|%d|%d|",ACTION_LINK,GetDFWChn(sDfw),sDfw,BH_DEST_WIDTH,BH_DEST_HEIGHT);
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_START,body,5);
			

			int chn = GetDFWChn(dfw);
			if(chn == 0){
				psCmdClient->m_vSndFifo = psCmdClient->m_vChn0Fifo;
			}
			else{
				psCmdClient->m_vSndFifo = psCmdClient->m_vChn1Fifo;
			}
			DataFifoClear(psCmdClient->m_vSndFifo);
			psCmdClient->m_pushStreamFlg = 1;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(chn, led_on, led_recd);
			
		}
		else if (CMD_MANUAL_STOP_PUSH == psPara->m_eType)
		{	
			int logicChn = manualChn;//GetDFWChn(sDfw);
			gpio_led_ctrl(logicChn, led_off,led_recd);
			psCmdClient->m_pushStreamFlg = 0;
			psCmdClient->m_vSndFifo = NULL;
			psCmdClient->m_pushCmdPara = *psPara;
		}
		else if (CMD_ELEC_ALARM_STOP_PUSH == psPara->m_eType)
		{	
			if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
			{
				DataPkgRelease(cmdPkg);
				return 0;
			}
			int logicChn = GetDFWChn(sDfw);
			psCmdClient->m_pushStreamFlg = 0;
			psCmdClient->m_vSndFifo = NULL;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(logicChn, led_off,led_recd);
			//����ֹͣ����
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
			//printf("xj________________________xj ELEC PUSH STOP\n");
		}
		else if ( ((CMD_CROSS_ALARM_STOP_PUSH == psPara->m_eType)||(CMD_RED_STRAP_STOP_PUSH == psPara->m_eType)) && psCmdClient->m_pushStreamFlg == 1)
		{	
			if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
			{
				DataPkgRelease(cmdPkg);
				return 0;
			}
			int logicChn = GetDFWChn(sDfw);
			
			
			psCmdClient->m_pushStreamFlg = 0;
			psCmdClient->m_vSndFifo = NULL;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(logicChn, led_off,led_recd);
			//����ֹͣ����
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
		}
		else if (CMD_ELEC_LINK_STOP_PUSH == psPara->m_eType)
		{	
			if (CMD_MANUAL_START_PUSH == psCmdClient->m_pushCmdPara.m_eType)
			{
				DataPkgRelease(cmdPkg);
				return 0;
			}
			SetCurrentAccFlag(0);
			int logicChn = GetDFWChn(sDfw);
			psCmdClient->m_pushStreamFlg = 0;
			psCmdClient->m_vSndFifo = NULL;
			psCmdClient->m_pushCmdPara = *psPara;
			gpio_led_ctrl(logicChn, led_off,led_recd);
			//����ֹͣ����
			tcpCmdHandlSendCmd1(psCmdClient->m_pcCmdHandle,MAIN_MEDIA_CONTROL,SUB_DEV_TO_PC_STREAM_STOP,body,0);
		}
		else if(CMD_FAILURE_CAMERA == psPara->m_eType)
		{
			sprintf(body, "%d|%d|",CAMERA_FAILURE,psPara->m_para1);
			
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, SUB_DEV_FAILURE,SUB_DEV_FAILURE_TYPE,body,2);
			ptf_dbg("camera failure");
		}
		else if(CMD_FAILURE_ACC ==psPara->m_eType)
		{
			sprintf(body, "%d|%d|",ACC_FAILURE,0);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, SUB_DEV_FAILURE,SUB_DEV_FAILURE_TYPE,body,2);
		}
		else if(CMD_FAILURE_ELEC ==psPara->m_eType)
		{
			sprintf(body, "%d|%d|",ELEC_FAILURE,0);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, SUB_DEV_FAILURE,SUB_DEV_FAILURE_TYPE,body,2);
		}
		else if(CMD_FAILURE_PRESSURE ==psPara->m_eType)
		{
			sprintf(body, "%d|%d|",PRESSURE_FAILURE,0);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, SUB_DEV_FAILURE,SUB_DEV_FAILURE_TYPE,body,2);
		}
		else if(CMD_FAILURE_LIQUID ==psPara->m_eType)
		{
			sprintf(body, "%d|%d|",LIQUID_FAILURE,0);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, SUB_DEV_FAILURE,SUB_DEV_FAILURE_TYPE,body,2);
		}
		else if(CMD_FINISH_ELEC == psPara->m_eType)
		{
			sprintf(body, "%d|",ELEC_FINISH_SIMBOL);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, MAIN_SAMPLE_BOARD_PARA,SUB_RETURN_FINISH_SIMBOL,body,1);
		}
		else if(CMD_FINISH_ACC == psPara->m_eType)
		{
			sprintf(body, "%d|",ACC_FINISH_SIMBOL);
			tcpCmdHandleSendFailure(psCmdClient->m_pcCmdHandle, MAIN_SAMPLE_BOARD_PARA,SUB_RETURN_FINISH_SIMBOL,body,1);
		}
		else
		{
			ptf_dbg("iRet %d",iRet);
		}

		DataPkgRelease(cmdPkg);
	}

	return iRet;
}

/*�������� : ��ʱ������������
*/
int tcpClientSendHeartBeat(CMD_CLIENT* psCmdClient)
{
	int iRet=0;
	struct timeval sCurTime;
	gettimeofday(&sCurTime, NULL);
	unsigned long long nowTime  = sCurTime.tv_usec  + sCurTime.tv_sec * 1000000;//stStream.u64TimeStamp;    //֡ģʽ
	if ((nowTime - psCmdClient->m_lastHeartSenTime) >= 1000000)//1000000
	{
		psCmdClient->m_lastHeartSenTime = nowTime;
		iRet=tcpCmdHandlSendHeartBeat(psCmdClient->m_pcCmdHandle);
	}
	return iRet;
}

/*
* ��������	��	��ʱ���Ͳɼ�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
//static 
int tcpClientSendSampleDataTimer(CMD_CLIENT* psCmdClient)
{
	int iRet = 0;
	struct timeval sCurTime;

 	gettimeofday(&sCurTime, NULL);
 	unsigned long long nowTime  = sCurTime.tv_usec  + sCurTime.tv_sec * 1000000;//stStream.u64TimeStamp;    //֡ģʽ
 	if ((nowTime - psCmdClient->m_lastSampleDataSenTime) >= psCmdClient->m_interSampleDataTimes)
 	{
 		psCmdClient->m_lastSampleDataSenTime = nowTime;
		CAPTURE_DATA_INFO sHead;
		memset(&sHead, 0, sizeof(sHead));
		int pkglen = CAP_DATA_INFO_LEN;
		//�¶�ʪ��Һλ�ȶ�ʱ����
		sHead.m_TempNum 	= DataFifoGetPkgsNum(g_pTEMP_FIFO);
		pkglen += sHead.m_TempNum * 2;
		sHead.m_HumiNum 	= DataFifoGetPkgsNum(g_pHUMI_FIFO);
		pkglen += sHead.m_HumiNum * 2;
		sHead.m_LiqlNum 		= DataFifoGetPkgsNum(g_pLIQI_FIFO);	
		pkglen += sHead.m_LiqlNum * 2;
		
		//ʵʱ����
	//	if(sHead.m_TempNum||sHead.m_HumiNum||sHead.m_LiqlNum)
			{
			//
			}
	//	else
	//		{
		if(sHead.m_TempNum&&sHead.m_HumiNum&&sHead.m_LiqlNum&&((
			nowTime-psCmdClient->m_lastLagDataSenTime)>=1800000000))
			{
				psCmdClient->m_lastLagDataSenTime=nowTime;
			}
		else{
			memset(&sHead,0,sizeof(sHead));
			pkglen=CAP_DATA_INFO_LEN;
			sHead.m_XAccNum 	= DataFifoGetPkgsNum(g_pXACC_FIFO);
			pkglen += sHead.m_XAccNum * 2;
			sHead.m_YAccNum 	= DataFifoGetPkgsNum(g_pYACC_FIFO);
			pkglen += sHead.m_YAccNum * 2;
			sHead.m_ZAccNum 	= DataFifoGetPkgsNum(g_pZACC_FIFO);
			pkglen += sHead.m_ZAccNum * 2;
			sHead.m_ElecNum 		= DataFifoGetPkgsNum(g_pELEC_FIFO);
			pkglen += sHead.m_ElecNum * 2;
			sHead.m_Hydp1Num	= DataFifoGetPkgsNum(g_pHYD1_FIFO);
			pkglen += sHead.m_Hydp1Num * 2;
			sHead.m_Hydp2Num	= DataFifoGetPkgsNum(g_pHYD2_FIFO);
			pkglen += sHead.m_Hydp2Num * 2;
	//		}
			}



		////printf("xj_______________________xj SampleDataTime is %d\n",psCmdClient->m_interSampleDataTimes);

		int offset = 0;
		DataPakg* tmpPkg = NULL;
		
		DataPakg* totalPkg = NULL;
		if(pkglen > CAP_DATA_INFO_LEN)
			totalPkg = DataPkgCreate(g_psArpAllocator,pkglen);
		//code by Xiangjie
		if (NULL == totalPkg)
		{
			//ptf_err("DataPackageCreate err len by Xiangjie\n");
			return -1;
		}
		PutCaptureData2Buf(&sHead, (unsigned char*)totalPkg->m_bData);
		offset += CAP_DATA_INFO_LEN;
		while (sHead.m_TempNum--)
		{
			tmpPkg = DataFifoGet(g_pTEMP_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_HumiNum--)
		{
			tmpPkg = DataFifoGet(g_pHUMI_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_XAccNum--)
		{
			tmpPkg = DataFifoGet(g_pXACC_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_YAccNum--)
		{
			tmpPkg = DataFifoGet(g_pYACC_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_ZAccNum--)
		{
			tmpPkg = DataFifoGet(g_pZACC_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_ElecNum--)
		{
			tmpPkg = DataFifoGet(g_pELEC_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_Hydp1Num--)
		{
			tmpPkg = DataFifoGet(g_pHYD1_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_Hydp2Num--)
		{
			tmpPkg = DataFifoGet(g_pHYD2_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}

		while (sHead.m_LiqlNum--)
		{
			tmpPkg = DataFifoGet(g_pLIQI_FIFO);
			if (tmpPkg != NULL)
			{
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[0];
				totalPkg->m_bData[offset++] = tmpPkg->m_bData[1];
				DataPkgRelease(tmpPkg);
			}
		}
		iRet =  tcpCmdHandlSendSampleData(psCmdClient->m_pcCmdHandle, totalPkg->m_bData, pkglen);
		DataPkgRelease(totalPkg);
		//ptf_dbg("pkglen %d offset %d",pkglen,offset);
 	}
	return iRet;
}

/*
* ��������	��	����fifo�е�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX


*/
//static 
int tcpClientSendPictureTimer(CMD_CLIENT* psCmdClient)
{
	int iRet = 0;
	struct timeval sCurTime;
 	gettimeofday(&sCurTime, NULL);
 	//unsigned long long nowTime  = sCurTime.tv_usec  + sCurTime.tv_sec * 1000000;//stStream.u64TimeStamp;    //֡ģʽ
 	unsigned long long nowTime  = sCurTime.tv_sec;
 	//if ((nowTime - psCmdClient->m_lastPicSenTime) >= psCmdClient->m_interPicTimes)
 	if ((nowTime - psCmdClient->m_lastPicSenTime) >= psCmdClient->m_interPicTimes/1000000)
 	{
 		DataPakg* picture;
		int logicChn = GetDFWChn(sDfw);
		ptf_dbg("current curnel = %d",sDfw);
		gpio_led_ctrl(logicChn, led_on,led_snap);
		usleep(500000);
		picture = venc_get_snap_picture(logicChn);
		if (NULL != picture)
		{
			VIDEO_PARAM sVPara;
			sVPara.m_width 		= BH_DEST_WIDTH;
			sVPara.m_height 	= BH_DEST_HEIGHT;
			sVPara.m_chn 		= logicChn;
			sVPara.m_positon 	= sDfw;
			iRet = tcpCmdHandlSendPicture(psCmdClient->m_pcCmdHandle, &sVPara,picture->m_bData, picture->m_iDataSize);
			//ptf_dbg("iRet %dpicture->m_iDataSize %d",iRet,picture->m_iDataSize);
			DataPkgRelease(picture);
		}
		usleep(500000);
		gpio_led_ctrl(logicChn, led_off,led_snap);
		psCmdClient->m_lastPicSenTime = nowTime;
 	}
	return iRet;
}

/*
* ��������	��	����ʵʱ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
//static 
int tcpClientSendStream(CMD_CLIENT* psCmdClient)
{
	int iRet = 0;
	if(1 == psCmdClient->m_pushStreamFlg)
	{
		if (NULL == psCmdClient->m_vSndFifo)
		{
			ptf_err("m_vSndFifo null");
			return -1;
		}
		
		//��ȡ��Ƶ����
		DataPakg* pkg = DataFifoGet(psCmdClient->m_vSndFifo);
		if (NULL != pkg)
		{
			iRet = tcpCmdHandlSendVideoStream(psCmdClient->m_pcCmdHandle, pkg->m_bData, pkg->m_iDataSize);
			DataPkgRelease(pkg);
		}
	}
	return iRet;
}

/*
* ��������	��	tcp�ͻ��˷����߳�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.12
* -----------------------------------------------
* 2015/08/12	 V1.0		XXXX		  XXXX
*/
static void* tcpClientHandleProc(void* arg)
{
	int socketFd = -1;
	int nfds,iRet = 0;
	int isDataNew=-1;//�ж��Ƿ���������
	
	int epoolfd = epoll_create(1);
	memset(&sCmdClient, 0, sizeof(CMD_CLIENT));
	
	sCmdClient.m_vChn0Fifo =  DataFifoConstruct(FIFO_PRI_CYC, 30 * 10);
	DataCenterBindFIFO(sCmdClient.m_vChn0Fifo , g_sSensorVenc[0].m_DaceCenter);
	sCmdClient.m_vChn1Fifo =  DataFifoConstruct(FIFO_PRI_CYC, 30 * 10);
	DataCenterBindFIFO(sCmdClient.m_vChn1Fifo , g_sSensorVenc[1].m_DaceCenter); //2017_07_30
	//DataCenterBindFIFO(sCmdClient.m_vChn1Fifo , g_sSensorVenc[0].m_DaceCenter);  //2017_07_30   ˫�ĵ����
	SAMPLE_SEND_TIME_S sSendTime;
	tcpClientGetTimerSendInter(&sSendTime);
	sCmdClient.m_interSampleDataTimes = sSendTime.sample_i * 1000000 + sSendTime.sample_d * 100000;
	//�ı䷢��ͼƬ��ʱ����ԭ�������Ϊ����
	sCmdClient.m_interPicTimes = sSendTime.sanp_i * 1000000*60 + sSendTime.sanp_d * 100000*60;
	struct epoll_event ev;
	isDataNew=0;
	while (1)
	{
		if (socketFd < 0)
		{
			struct sockaddr_in srv;
			PC_SERVER_PARA sPara;
			netGetPcServerPara(&sPara);
			ptf_dbg("sPara.m_ipaddr %s sPara.m_port %d",sPara.m_ipaddr,sPara.m_port);
			inet_aton(sPara.m_ipaddr, &srv.sin_addr);
			if(isDataNew == -1)
			{
				tcpClientGetTimerSendInter(&sSendTime);
				sample_get_sample_para(&s_samplePara);
				sCmdClient.m_interSampleDataTimes = sSendTime.sample_i * 1000000 + sSendTime.sample_d * 100000;
				//�ı䷢��ͼƬ��ʱ����ԭ�������Ϊ����
				sCmdClient.m_interPicTimes = sSendTime.sanp_i * 1000000*60 + sSendTime.sanp_d * 100000*60;
				isDataNew = 0;
			}
			//DataCenterUnBindFIFO(sCmdClient.m_vChn0Fifo , g_sSensorVenc[0].m_DaceCenter);
			//DataCenterUnBindFIFO(sCmdClient.m_vChn1Fifo , g_sSensorVenc[1].m_DaceCenter);
			socketFd = TcpConnect(srv.sin_addr.s_addr, sPara.m_port);
			if (socketFd < 0)
			{
				sleep(1);
				//sleep(1500);
				continue;
			}
			//DataCenterBindFIFO(sCmdClient.m_vChn0Fifo , g_sSensorVenc[0].m_DaceCenter);
			//DataCenterBindFIFO(sCmdClient.m_vChn1Fifo , g_sSensorVenc[1].m_DaceCenter);
			//�������
			AddSocketToEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
			if (NULL != sCmdClient.m_pcCmdHandle)
			{
				tcpCmdHandlDistruct(sCmdClient.m_pcCmdHandle);
			}
			sCmdClient.m_pcCmdHandle =  tcpCmdHandlConstruct(socketFd);
			iRet = tcpCmdRegister(sCmdClient.m_pcCmdHandle);
			if (iRet < 0)
			{
				DelSocketFromEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
				close(socketFd);
				socketFd = -1;
				continue;
			}
		}

		nfds = epoll_wait(epoolfd, &ev, 1, 1);
		if (nfds == 0)
		{
			//�������
			//��ⷢ�Ͳɼ�����
			tcpClientSendSampleDataTimer(&sCmdClient);
			//��ⷢ��ͼƬʱ��
			tcpClientSendPictureTimer(&sCmdClient);
			//��ⷢ����Ƶ
			tcpClientSendStream(&sCmdClient);
			//��ⷢ��������
			tcpClientSendHeartBeat(&sCmdClient);
			//�������
			tcpClientHandleFIfoCmd(&sCmdClient);
		}
		else if (nfds > 0)
		{
			iRet =tcpCmdHandlRecvPtHead(sCmdClient.m_pcCmdHandle);
			if (0 != iRet)
			{
				ptf_err("iRet %d",iRet);
				DelSocketFromEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
				close(socketFd);
				socketFd = -1;
				continue;
			}
			
			iRet =tcpCmdHandlRecvBodyInBuf(sCmdClient.m_pcCmdHandle);
			if (0 != iRet)
			{
				ptf_err("iRet %d",iRet);
				DelSocketFromEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
				close(socketFd);
				socketFd = -1;
				continue;
			}		

			iRet =tcpCmdHandlCmdFormat(sCmdClient.m_pcCmdHandle);
			if (0 != iRet)
			{
				ptf_err("iRet %d",iRet);
				DelSocketFromEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
				close(socketFd);
				socketFd = -1;
				continue;
			}		

			//��������Э��
			iRet = tcpCmdHandleMainId(sCmdClient.m_pcCmdHandle);
			if (iRet != 0)
			{
				ptf_err("iRet %d",iRet);
				DelSocketFromEp(epoolfd, socketFd, EPOOL_READ, EPOOL_LT);
				close(socketFd);
				socketFd = -1;
				isDataNew = -1;
				//tcpClientGetTimerSendInter(&sSendTime);
				//sCmdClient.m_interSampleDataTimes = sSendTime.sample_i * 1000000 + sSendTime.sample_d * 100000;
				//�ı䷢��ͼƬ��ʱ����ԭ�������Ϊ����
				//sCmdClient.m_interPicTimes = sSendTime.sanp_i * 1000000*60 + sSendTime.sanp_d * 100000*60;
				continue;
			}
		}
	}

	return NULL;
}

//�ⲿ����

/*
* ��������	��	�趨��ǰץ��¼��Ķ���λ
* ��Ա����	:		��
* �������	��	dfw	����λ
* �������	��	��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientSetDFWRecSnap(DFW_E dfw)
{
	if (dfw < DFW_DW || dfw > DFW_FW )
	{
		ptf_err("dfw %d");
		return -1;
	}

	sDfw = dfw;
	return 0;
}

int tcpClientGetDFWRecSnap()
{
	return sDfw;
}



/*
* ��������	��	�趨��ʱ������ʱ����
* ��Ա����	:		��
* �������	��	psSendTime	����ʱ����Ϣ�ṹ��
* �������	��	��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientSetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime)
{
	char section[32] = {0};
	sprintf(section, "sample_para");

	cfg_rwlock_wrlock();
	
   	cfg_set_int(Config_Public, section, "sample_send_i", psSendTime->sample_i);
   	cfg_set_int(Config_Public, section, "sample_send_d", psSendTime->sample_d);
   	cfg_set_int(Config_Public, section, "snap_send_i", psSendTime->sanp_i);
   	cfg_set_int(Config_Public, section, "snap_send_d", psSendTime->sanp_d);

	cfg_save_ini(Config_Public);
	sCmdClient.m_interSampleDataTimes = psSendTime->sample_i * 1000000 + psSendTime->sample_d * 100000;
	sCmdClient.m_interPicTimes = psSendTime->sanp_i * 1000000*60 + psSendTime->sanp_d * 100000*60;
	cfg_rwlock_unlock();
	return 0;
}

/*
* ��������	��	��ȡ��ʱ������ʱ����
* ��Ա����	:		��
* �������	��	��
* �������	��	psSendTime	����ʱ����Ϣ�ṹ��
* �� �� ֵ	��		-1			ʧ��
										0				�ɹ�
* ����˵��	��	��
* �޸�����	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
int tcpClientGetTimerSendInter(SAMPLE_SEND_TIME_S *psSendTime)
{
	char section[32] = {0};
	sprintf(section, "sample_para");

	cfg_rwlock_rdlock();
	
   	psSendTime->sample_i 	= cfg_get_int(Config_Public, section, "sample_send_i", 5);
   	psSendTime->sample_d = cfg_get_int(Config_Public, section, "sample_send_d", 0);
   	psSendTime->sanp_i 		= cfg_get_int(Config_Public, section, "snap_send_i", 2);
   	psSendTime->sanp_d 	= cfg_get_int(Config_Public, section, "snap_send_d", 0);
	cfg_rwlock_unlock();
	return 0;
}

/*
* ��������	��	������fifo��Ͷ������
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0				�ɹ�
										-1			ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.15
* -----------------------------------------------
* 2015/08/15	 V1.0		XXXX		  XXXX
*/
int tcpClientHandlePushCmd(CMD_PARA* cmdPara)
{
	int iRet = 0;
	DataPakg * cmdPkg = 	 DataPkgCreate(g_psArpAllocator,0);
	//code by Xiangjie
	if (NULL == cmdPkg)
	{
		ptf_err("DataPackageCreate err len by Xiangjie\n");
		return -1;
	}
	CMD_PARA* tmpCmdPara = (CMD_PARA*)cmdPkg->m_datainfo;
	memcpy(tmpCmdPara, cmdPara, sizeof(CMD_PARA));
   	DataFifoPush(cmdPkg, g_cmdFifo);
	DataPkgRelease(cmdPkg);
	return iRet;
}

/*
* ��������	��	����tcp�ͻ��˷��񣬿�ʼ����PC������
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0		�����������ӳɹ�
										1		����֮ǰ�Ѿ���������״̬
* ����˵��	��	��
* �޸�����	:		2015.08.12
* -----------------------------------------------
* 2015/08/12	 V1.0		XXXX		  XXXX
*/
int tcpClientStart(void)
{
	if (0 != s_tcpClientState)
		return -1;
	s_tcpClientState = 1;
	sDfw = DFW_DW;//Ĭ�϶�ʱץ�ķ���λ����������Һѹ���㷢�Ͷ�λ���Ƿ�λ
	//��������PC�������߳�
	ThreadPoolAddWorkLimit((void*)g_psThreadPool, tcpClientHandleProc, (void *)NULL);
	return 0;
}


/*
* ��������	��	��ȡpc����������
* ��Ա����	:		��
* �������	��	��
* �������	��	psPara 	���������ȡ���Ĳ���
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netGetPcServerPara(PC_SERVER_PARA *psPara)
{
	memset(psPara, 0, sizeof(PC_SERVER_PARA));
	cfg_rwlock_rdlock();
	psPara->m_enable = cfg_get_int(Config_Public, "pc_server", "enable", 0);
	strcpy(psPara->m_ipaddr, cfg_get_string(Config_Public, "pc_server", "ipaddr", "192.168.1.123"));
	psPara->m_port = cfg_get_int(Config_Public, "pc_server", "port", 6666);
	cfg_rwlock_unlock();
	return ;
}

/*
* ��������	��	����pc����������
* ��Ա����	:		��
* �������	��	psPara 	���������ȡ���Ĳ���
* �������	��	��
* �� �� ֵ		��	��
* ����˵��	��	��
* �޸�����	:		2015.08.05
* -----------------------------------------------
* 2015/08/05	 V1.0		XXXX		  XXXX
*/
void netSetPcServerPara(PC_SERVER_PARA *psPara)
{
	cfg_rwlock_wrlock();
	cfg_set_string(Config_Public, "pc_server", "ipaddr", psPara->m_ipaddr);
	cfg_set_int(Config_Public, "pc_server", "port", psPara->m_port);
	cfg_set_int(Config_Public, "pc_server", "enable", psPara->m_enable);
	cfg_save_ini(Config_Public);
	cfg_rwlock_unlock();
	return ;
}

