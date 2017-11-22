//��׼ͷ�ļ���������
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//�Զ���ͷ�ļ���������
#include "sample_board.h"
#include "project_func.h"
#include "uart.h"
#include "printf_func.h"
#include "thread_pool.h"
#include "global_para.h"
#include "ini_config.h"
#include "datafifo.h"
#include "net_para.h"
#include "tcp_client.h"
#include "udp_broadcast.h"
#include "public_func.h"

//����������

//�ļ��ڲ�ʹ�õĺ�
#define		MIN_CMD_LEN			(6)

// ��������������ķ���ʱ��
#define TEMP_TIME_NUMBER     900
#define LIQUID_TIME_NUMBER   1800


//�����������������ȷ��Χ
#define ELEC_DATA_MIN  0
#define ELEC_DATA_MAX  5  //ԭֵ3
#define ACC_DATA_MIN   0
#define ACC_DATA_MAX   3    //ԭֵ0.5
#define PRESURE_DATA_MIN 0
#define PRESURE_DATA_MAX 20
#define LIQUID_DATA_MIN 0
#define LIQUID_DATA_MAX 20
//�����������������Ĵ�����Χ
#define ELEC_DATA_FAILURE_TIME 10
#define ACC_DATA_FAILURE_TIME 10
#define PRESURE_DATA_FAILURE_TIME 10
#define LIQUID_DATA_FAILURE_TIME 10

static int times_failure_elec=0;
static int times_failure_acc=0;
static int times_failure_pressure=0;
static int times_failure_liquid=0;

//����������������ִ����ĸ����ʹ���
#define CHUFA_ELEC_TIMES 10
#define CHUFA_ACC_TIMES  10
#define CHUFA_AFTER_ELEC_TIMES 10
#define CHUFA_AFTER_ACC_TIMES  10
//������ٶ�����С������ֵ���¼��ʱ��
#define AFTER_ACC_RECORD_TIME  30    //������¼��ʱ��




//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����
static UartOper* s_pSampleUart = NULL;
static int times_ACC_Number = 0;
static int times_Current_Number = 0;
static int times_After_ACC_Number = 0;
static int times_After_Current_Number=0;
static int Current_ACC_flag = 0;//�ж��Ƿ��ǵ�������Ķ���¼��ֹͣ,����ָ����ظ�����
static char* const pFile = "/ipc/sample_para";
CAPTURE_PARAM s_samplePara;
SAMPLE_CTRL s_SampleCtrl;

void SetCurrentAccFlag(int flag) //���ǣ������2017_08_25
{
	Current_ACC_flag=flag;
}
//�ڲ�����

int sample_board_send_para(CAPTURE_PARAM * psCapPara);


/*
* ��������	��	���մ������ݴ����߳�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		0	�ɹ�
										����ֵʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
static int sample_board_read_fixlen_data(unsigned char* buf, int len)
{
	int offset = 0;
	int ret = 0;
	int needlen = len;
	
	while (1)
	{
		ret = UartRead(s_pSampleUart,needlen,(char*)buf + offset);
		if (ret == 0)
		{
			return 0;
		}
		if (ret == needlen)
		{
			break;
		}
		offset += ret;
		needlen -= ret;
	}

	return 0;
}

/*
* ��������	��	�����������
* ��Ա����	:		��
* �������	��	cmdid	:����ID
										body	:��Ϣ��
										bodylen	:��Ϣ�峤��
* �������	��	buf		:�洢����õ�������Ϣ
* �� �� ֵ	��		��Ϣ���ܳ���
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
static int pack_cmd(unsigned char cmdid,unsigned char* buf, unsigned char* body, unsigned int bodylen)
{
	buf[0] = 0x24;
	buf[1] = 0x4D;
	buf[2] = 0x3C;
	buf[3] = bodylen;
	buf[4] = cmdid;
	if (NULL != body)
	memcpy(buf + 5, body, bodylen);
	buf[5 + bodylen] = 0xEF;
	return (MIN_CMD_LEN + bodylen);
}



/*
* ��������	��	�ȴ��ɼ�����������������ʱ��ɼ����������ɹ�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
static void* sample_board_query_proc(void* args)
{
	unsigned char sendBuf[64] = {0};
	unsigned char recvBuf[64] = {0};
	int len = 0;

	while (1)
	{
		//���Ͳ�ѯָ��
		pack_cmd(S_CMD_ID_QUERY_RUN,sendBuf, NULL, 0);
		UartWrite(s_pSampleUart,MIN_CMD_LEN,(char*)sendBuf);
		len = sample_board_read_fixlen_data(recvBuf,MIN_CMD_LEN);
		if (len != 0)
			continue;
		else
		{
			if (S_CMD_ID_REPLAY_QUERY == recvBuf[4])
			{
				ptf_suc("sample board is running");
				break;
			}
		}
	}

	return NULL;
}

/*
* ��������	��	��ʱ���Ͷ���¼���������
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int ele_stop_snap_dfw=0;
void* pthread_delay_rec_stop(void* args)
{
	//������ʱ¼��ʱ��
	unsigned int delayUSec 	= s_samplePara.m_acRecDelayTime_I * 1000000 + s_samplePara.m_acRecDelayTime_D * 100000;
	usleep(delayUSec);
	sleep(1);
	ptf_log("stop snap vedio");
	CMD_PARA cmdPara;
	//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
	cmdPara.m_eType = CMD_ELEC_STOP_SNAP;
	cmdPara.m_para1 	= ele_stop_snap_dfw;
	//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
	//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
	//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
	tcpClientHandlePushCmd(&cmdPara);
	//char body[64] = {0};
	//sprintf(body, "%d|",netGetGroup());
	//udpMCCmdSend(MAIN_MC,SUB_LINK_GROUP_RECD_STOP,body,1);
	s_SampleCtrl.m_iEleRecFlg = 0;
	//tcpClientHandlePushCmd(&cmdPara);
	ptf_dbg("stop Current vedio");
	return NULL;
}
//�������ӳ�¼��
static void* pthread_delay_ACC_stop(void* args)
{
	sleep(AFTER_ACC_RECORD_TIME);
	if(s_SampleCtrl.m_iAccRecFlg == 1 && Current_ACC_flag == 0)
	{
		CMD_PARA cmdPara;
		cmdPara.m_eType=CMD_CROSS_ALARM_STOP_PUSH;
		
		cmdPara.m_para1=tcpClientGetDFWRecSnap();
		
		tcpClientHandlePushCmd(&cmdPara);
	}
	s_SampleCtrl.m_iAccRecFlg = 0;
	ptf_dbg("stop ACC vedio");
	return NULL;
}

/*
* ��������	��	���մ������ݴ����߳�
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
static void* pthread_sample_recv(void* args)
{
	unsigned char recvBuf[32];
    DataPakg *ptPkg = NULL;
	s_SampleCtrl.m_iEleRecFlg 	= 0;
	s_SampleCtrl.m_iAccRecFlg 	= 0;
	s_SampleCtrl.m_eDfw=tcpClientGetDFWRecSnap();//��ȡ��ǰ�Ķ���λ

	unsigned int number_temp=0;
	unsigned int number_humi=0;
	unsigned int number_hytrau=0;
	unsigned int number_acc=0;
	unsigned int number_current=0;
    
	while (1)
	{
		sample_board_read_fixlen_data(recvBuf,5);
		switch (recvBuf[4])
		{
			case S_CMD_ID_TEMP://�¶�
			{
				//ֱ�ӱ��漴��
				sample_board_read_fixlen_data(recvBuf + 5, 3);
			//	if(number_temp==2)
			//	{
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
					ptPkg->m_bData[0] = recvBuf[5]; 	//����λ
					ptPkg->m_bData[1] = recvBuf[6];	//С��λ
					DataFifoPush(ptPkg, g_pTEMP_FIFO);
					DataPkgRelease(ptPkg);
			//	}
				number_temp++;
				if(number_temp>=TEMP_TIME_NUMBER)
				{
					number_temp=0;
				}
			}break;
			
			case S_CMD_ID_HUMI:
			{
				//ֱ�ӱ��漴��
				sample_board_read_fixlen_data(recvBuf + 5, 3);
			//	if(number_humi==2)
			//	{
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
					ptPkg->m_bData[0] = recvBuf[5]; 	//����λ
					ptPkg->m_bData[1] = recvBuf[6];	//С��λ
					DataFifoPush(ptPkg, g_pHUMI_FIFO);
					DataPkgRelease(ptPkg);
			//	}
				number_humi++;
				if(number_humi>=TEMP_TIME_NUMBER)
				{
					number_humi=0;
				}

			}break;
			
			case S_CMD_ID_ELEC:
			{
				//�洢��ͬʱ��Ҫ��������
				sample_board_read_fixlen_data(recvBuf + 5, 3);
				unsigned int elecLimit 	= s_samplePara.m_eaLimit_I * 10 + s_samplePara.m_eaLimit_D;
				unsigned int elecNow 	= recvBuf[5] * 10 + recvBuf[6];
				ptf_dbg("The elecNow1 is %d",elecNow);
				
				ptf_dbg("The shi data is %d",recvBuf[5]);
				ptf_dbg("The ge data is %d",recvBuf[6]);
				//ptf_dbg("The jiaoyan1 data is %d",(recvBuf[7]>>4)&0x0f);
				//ptf_dbg("The jiaoyan2 data is %d",recvBuf[7]&0x0f);
				//ptf_dbg("The elecLimit is %d",elecLimit);
				if(elecNow>=elecLimit)
				{
					number_current=1;
				}else
				{
					number_current=0;
				}
				if(elecNow>ELEC_DATA_MAX*10 || elecNow<ELEC_DATA_MIN*10)
				{
					//2017_06_13
					number_current=0;

					times_failure_elec++;
					if(times_failure_elec>=ELEC_DATA_FAILURE_TIME)
					{
						times_failure_elec=0;
						//���ϻ㱨����������������
						CMD_PARA cmdPara;
						cmdPara.m_eType = CMD_FAILURE_ELEC;
						//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
						//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
					}
				}
				else
				{
					times_failure_elec=0;
				}
				if(number_current>0)//2017_06_13
				{
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
				//	printf("elecNew = %d\n",elecNow);
					ptPkg->m_bData[0] = elecNow/10;     //recvBuf[5]; 	//����λ
					ptPkg->m_bData[1] = elecNow%10;    //recvBuf[6];	//С��λ
					DataFifoPush(ptPkg, g_pELEC_FIFO);
					DataPkgRelease(ptPkg);
				}
				//��ȷ��С�����1λ����������10�Ŵ�10����С���������Ѿ��Ŵ���10��
				//���������Ϊ��ֵ����Ŵ�10������ԭֵΪ15.1 ������λΪ15��С��λΪ1
				//���������Ϊ15 * 10 + 1 = 151���Ŵ���10��
				//15.9  ��Ӧ15 * 10 + 9 = 159
				//���������������ֵ������tcpclient�ӿڷ���¼������,ͬʱ���öಥ�ӿڽ�����������
				if (elecNow >= elecLimit)
				{
					ptf_dbg("The elecNow2 is %d",elecNow);
					times_Current_Number++;
					if(times_Current_Number>=CHUFA_ELEC_TIMES)
					{
						times_Current_Number=CHUFA_ELEC_TIMES;
					}
					if (0 ==s_SampleCtrl.m_iEleRecFlg && times_Current_Number>=CHUFA_ELEC_TIMES && 1 == s_SampleCtrl.m_iAccRecFlg)
					{
						ptf_dbg("ACC is on and must stop it");
						CMD_PARA cmdPara;
						cmdPara.m_eType=CMD_CROSS_ALARM_STOP_PUSH;
						
						cmdPara.m_para1=tcpClientGetDFWRecSnap();
						
						tcpClientHandlePushCmd(&cmdPara);
						Current_ACC_flag = 1;
						s_SampleCtrl.m_iAccRecFlg = 0;
						s_SampleCtrl.m_iAccRecCurrentFlg=0;
						//��ʼ��������¼��
						ptf_dbg("ACC is off and start current vedio");
						s_SampleCtrl.m_eDfw=tcpClientGetDFWRecSnap();
						if (DFW_DW == s_SampleCtrl.m_eDfw)
							s_SampleCtrl.m_eDfw = DFW_FW;
						else
							s_SampleCtrl.m_eDfw = DFW_DW;
					
						tcpClientSetDFWRecSnap(s_SampleCtrl.m_eDfw);
						cmdPara.m_eType 	= CMD_ELEC_ALARM_START_PUSH;
						cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
						ptf_dbg("start Current vedio");
						s_SampleCtrl.m_iEleRecCurrentFlg=1;
						s_SampleCtrl.m_iEleRecFlg = 1;
						char body[64] = {0};
						sprintf(body, "%d|%d|",s_SampleCtrl.m_eDfw,netGetGroup());
						udpMCCmdSend(MAIN_MC,SUB_LINK_GROUP_RECD_START,body,2);		
					}
					else if (0 == s_SampleCtrl.m_iEleRecFlg && times_Current_Number>=CHUFA_ELEC_TIMES && 0 == s_SampleCtrl.m_iAccRecFlg)
					{
						//ptf_dbg("elecLimit %d elecNow %d",elecLimit,elecNow);
						s_SampleCtrl.m_eDfw=tcpClientGetDFWRecSnap();
						if (DFW_DW == s_SampleCtrl.m_eDfw)
							s_SampleCtrl.m_eDfw = DFW_FW;
						else
							s_SampleCtrl.m_eDfw = DFW_DW;
					
						tcpClientSetDFWRecSnap(s_SampleCtrl.m_eDfw);

						CMD_PARA cmdPara;
						cmdPara.m_eType 	= CMD_ELEC_ALARM_START_PUSH;
						cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
						ptf_dbg("start Current vedio");
						s_SampleCtrl.m_iEleRecFlg = 1;
						s_SampleCtrl.m_iEleRecCurrentFlg=1;
						char body[64] = {0};
						sprintf(body, "%d|%d|",s_SampleCtrl.m_eDfw,netGetGroup());
						udpMCCmdSend(MAIN_MC,SUB_LINK_GROUP_RECD_START,body,2);
					}
				}
				else if (elecNow < elecLimit) //�������С������ֵ����ʼ��ʱ����ʱ�趨ʱ��
				{
					if(1 == s_SampleCtrl.m_iEleRecFlg)
					{
						times_After_Current_Number++;
					}
					if(times_After_Current_Number>=CHUFA_AFTER_ELEC_TIMES)
					{
						times_Current_Number=0;
						times_After_Current_Number=0;
						if (1 == s_SampleCtrl.m_iEleRecFlg && 1== s_SampleCtrl.m_iEleRecCurrentFlg)
						{
							s_SampleCtrl.m_iEleRecCurrentFlg=0;
							//ptf_dbg("elecLimit %d elecNow %d",elecLimit,elecNow);
							//����ץ��һ��ͼƬȡ��
							//ֹͣ¼��
							usleep(500000);
							ptf_log("current video stopped");
							CMD_PARA cmdPara;
							
							Current_ACC_flag=0; 
							
							//�ϴ�����Ƶ�޸ģ�����Ƶ����ֹͣ���͡�
							cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
							cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
							ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
							tcpClientHandlePushCmd(&cmdPara);
							char body[64] = {0};
							sprintf(body, "%d|",netGetGroup());
							udpMCCmdSend(MAIN_MC,SUB_LINK_GROUP_RECD_STOP,body,1);
							ThreadPoolAddWorkLimit(g_psThreadPool,pthread_delay_rec_stop, NULL);
							//
							CMD_PARA cmdPara1;
							cmdPara1.m_eType = CMD_FINISH_ELEC;
							//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
							//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
							//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
							tcpClientHandlePushCmd(&cmdPara1);
						}
					}
				}
				//ptf_dbg("S_CMD_ID_ELEC");
			}break;
			
			case S_CMD_ID_LIQI:
			{
				sample_board_read_fixlen_data(recvBuf + 5, 3);
				unsigned int liquidNow 	= recvBuf[5] * 10 + recvBuf[6];
				ptf_dbg("The liquidNow is %d",liquidNow);
				if(liquidNow>LIQUID_DATA_MAX*10 || liquidNow<LIQUID_DATA_MIN*10)
				{
					times_failure_liquid++;
					if(times_failure_liquid>=LIQUID_DATA_FAILURE_TIME)
					{
						times_failure_liquid=0;
						//���ϻ㱨����������������
						CMD_PARA cmdPara;
						cmdPara.m_eType = CMD_FAILURE_LIQUID;
						//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
						//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
					}
				}else
				{
					times_failure_liquid=0;
				}
				
			//	if(number_hytrau==2)
			//	{
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
					ptPkg->m_bData[0] = recvBuf[5]; 	//����λ
					ptPkg->m_bData[1] = recvBuf[6];	//С��λ
					DataFifoPush(ptPkg, g_pLIQI_FIFO);
					DataPkgRelease(ptPkg);
			//	}
				number_hytrau++;
				if(number_hytrau>=LIQUID_TIME_NUMBER)
				{
					number_hytrau=0;
				}
			}break;

			case S_CMD_ID_AACC:
			{
				//�洢��ͬʱ��Ҫ���й���¼����ж�
				sample_board_read_fixlen_data(recvBuf + 5, 7);
				unsigned int accLimit 	= s_samplePara.m_accLimit_I * 100 + s_samplePara.m_accLimit_D*10;
				unsigned int accI;
				unsigned int accD;

				accI 	= recvBuf[5] & 0x7F;
				accD 	= recvBuf[6];
				unsigned int accXNow 	= accI * 10 + accD;
				accI 	= recvBuf[7] & 0x7F;
				accD 	= recvBuf[8];
				unsigned int accYNow 	= accI * 10 + accD;
				accI 	= recvBuf[9] & 0x7F;
				accD 	= recvBuf[10];
				unsigned int accZNow 	= accI * 10 + accD;

				if(accXNow>=accLimit||accYNow>=accLimit||accZNow>=accLimit)
				{
					number_acc=1;
				}else
				{
					number_acc=0;
				}
				
				if(accXNow>ACC_DATA_MAX*10 || accXNow<ACC_DATA_MIN*10 || accYNow>ACC_DATA_MAX*10 || accYNow<ACC_DATA_MIN*10 ||accZNow>ACC_DATA_MAX*10 || accZNow<ACC_DATA_MIN*10)
				{
					times_failure_acc++;
					if(times_failure_acc>=ACC_DATA_FAILURE_TIME)
					{
						times_failure_acc=0;
						//���ϻ㱨����������������
						CMD_PARA cmdPara;
						cmdPara.m_eType = CMD_FAILURE_ACC;
						//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
						//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
					}
				}else
				{
					times_failure_acc=0;
				}

				if(number_acc!=0)
				{
					///////////////////////////////////////////////////////////////////xjc 2017_06_18
						ptPkg = DataPkgCreate(g_psArpAllocator,2);
						//code by Xiangjie
						if (NULL == ptPkg)
						{
							ptf_err("DataPackageCreate err len by Xiangjie\n");
							break;
						}
						ptPkg->m_bData[0] = recvBuf[5]; 	//����λ
						ptPkg->m_bData[1] = recvBuf[6];	//С��λ
						DataFifoPush(ptPkg, g_pXACC_FIFO);
						DataPkgRelease(ptPkg);
						ptPkg = DataPkgCreate(g_psArpAllocator,2);
						//code by Xiangjie
						if (NULL == ptPkg)
						{
							ptf_err("DataPackageCreate err len by Xiangjie\n");
							break;
						}
						ptPkg->m_bData[0] = recvBuf[7]; 	//����λ
						ptPkg->m_bData[1] = recvBuf[8];	//С��λ
						DataFifoPush(ptPkg, g_pYACC_FIFO);
						DataPkgRelease(ptPkg);
						ptPkg = DataPkgCreate(g_psArpAllocator,2);
						//code by Xiangjie
						if (NULL == ptPkg)
						{
							ptf_err("DataPackageCreate err len by Xiangjie\n");
							break;
						}
						ptPkg->m_bData[0] = recvBuf[9]; 	//����λ
						ptPkg->m_bData[1] = recvBuf[10];	//С��λ
						DataFifoPush(ptPkg, g_pZACC_FIFO);
						DataPkgRelease(ptPkg);
						/////////////////////////////////////////////////////////////////////
				}
				#if 1
				//�������һ·aac��������ֵ������tcpclient�ӿڷ���¼������
				if (accXNow >= accLimit || accYNow >= accLimit ||accZNow >= accLimit)
				{
					//��Ҫ�ж��������ٴη��������������Ƿ񶼴�����ֵ
					times_ACC_Number++;
					if(times_ACC_Number>=CHUFA_ACC_TIMES)
					{
						times_ACC_Number=CHUFA_ACC_TIMES;
					}
					if (0 == s_SampleCtrl.m_iEleRecFlg && 0 == s_SampleCtrl.m_iAccRecFlg && times_ACC_Number>=CHUFA_ACC_TIMES)
					{
						//�жϵ�ǰ�ĵ���ֵ�Ƿ������ֵ
						ptf_dbg("start ACC vedio");
						s_SampleCtrl.m_eDfw = tcpClientGetDFWRecSnap();
						//���Ϳ�ʼ����
						CMD_PARA cmdPara;
						cmdPara.m_eType 	= CMD_CROSS_ALARM_START_PUSH;
						cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;

						tcpClientSetDFWRecSnap(s_SampleCtrl.m_eDfw);
						
						tcpClientHandlePushCmd(&cmdPara);
						s_SampleCtrl.m_iAccRecFlg = 1;
						s_SampleCtrl.m_iAccRecCurrentFlg=1;
						ThreadPoolAddWorkLimit(g_psThreadPool,pthread_delay_ACC_stop, NULL);
					}
				}
				else  if (accXNow < accLimit && accYNow < accLimit && accZNow < accLimit )
				{
					if(1 == s_SampleCtrl.m_iAccRecFlg)
					{
						times_After_ACC_Number++;
					}
					if(times_After_ACC_Number>=CHUFA_AFTER_ACC_TIMES)
					{
						times_ACC_Number=0;
						
						times_After_ACC_Number=0;

						//���ȫ����������ֵ�������tcpclient�ӿڷ���¼��ֹͣ����
						if (1 == s_SampleCtrl.m_iAccRecFlg && 1 == s_SampleCtrl.m_iAccRecCurrentFlg)
						{
							s_SampleCtrl.m_iAccRecCurrentFlg=0;
							//CMD_PARA cmdPara;
							//cmdPara.m_eType = CMD_CROSS_ALARM_STOP_PUSH;
							//tcpClientHandlePushCmd(&cmdPara);
							//ThreadPoolAddWorkLimit(g_psThreadPool,pthread_delay_ACC_stop, NULL);
							//s_SampleCtrl.m_iAccRecFlg = 0;
							CMD_PARA cmdPara;
							cmdPara.m_eType = CMD_FINISH_ACC;
							//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
							//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
							//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
							tcpClientHandlePushCmd(&cmdPara);
						}
					}
				}
				#endif
				//ptf_dbg("S_CMD_ID_AACC");
			}break;

			case S_CMD_ID_AHYD:
			{
				//�洢��ͬʱ��Ҫ�жϺ�������λ¼���ץͼ
				sample_board_read_fixlen_data(recvBuf + 5, 5);
				unsigned int presureNow1 	= recvBuf[5] * 10 + recvBuf[6];
				unsigned int presureNow2    = recvBuf[7] * 10 + recvBuf[8];
				ptf_dbg("The presureNow1 is %d The presureNow2 is %d \n",presureNow1,presureNow2);
				if(presureNow1>PRESURE_DATA_MAX*10 || presureNow1<PRESURE_DATA_MIN*10 || presureNow2>PRESURE_DATA_MAX*10 || presureNow2<PRESURE_DATA_MIN*10)
				{
					times_failure_pressure++;
					if(times_failure_pressure>=PRESURE_DATA_FAILURE_TIME)
					{
						times_failure_pressure=0;
						//���ϻ㱨����������������
						CMD_PARA cmdPara;
						cmdPara.m_eType = CMD_FAILURE_PRESSURE;
						//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
						//cmdPara.m_para1 	= s_SampleCtrl.m_eDfw;
						//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
						tcpClientHandlePushCmd(&cmdPara);
					}
				}else
				{
					times_failure_pressure=0;
				}
				
				if(number_current!=0)
				{
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
					ptPkg->m_bData[0] = recvBuf[5]; 	//����λ
					ptPkg->m_bData[1] = recvBuf[6];	//С��λ
					DataFifoPush(ptPkg, g_pHYD1_FIFO);
					DataPkgRelease(ptPkg);
					ptPkg = DataPkgCreate(g_psArpAllocator,2);
					//code by Xiangjie
					if (NULL == ptPkg)
					{
						ptf_err("DataPackageCreate err len by Xiangjie\n");
						break;
					}
					ptPkg->m_bData[0] = recvBuf[7]; 	//����λ
					ptPkg->m_bData[1] = recvBuf[8];	//С��λ
					DataFifoPush(ptPkg, g_pHYD2_FIFO);
					DataPkgRelease(ptPkg);
				}	
				//ptf_dbg("S_CMD_ID_AHYD");
			}break;
			
			default:
			{
				//ptf_err("--------------recvBuf[4] 0x%x",recvBuf[4]);
			}break;
		}

		usleep(10);
	}

	return NULL;
}

//�ⲿ����

/*
* ��������	��	��ȡ�ɼ�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_get_sample_para(CAPTURE_PARAM *samplePara)
{
	char section[32] = {0};
	sprintf(section, "sample_para");
	cfg_rwlock_rdlock();
	
   	samplePara->m_eaLimit_I 					= cfg_get_int(Config_Public, section, "ealimit_i", 5);
   	samplePara->m_eaLimit_D 					= cfg_get_int(Config_Public, section, "ealimit_d", 0);
   	samplePara->m_accLimit_I 					= cfg_get_int(Config_Public, section, "acclimit_i", 5);
   	samplePara->m_accLimit_D 					= cfg_get_int(Config_Public, section, "acclimit_d", 0);
   	samplePara->m_acRecDelayTime_I 		= cfg_get_int(Config_Public, section, "delaytime_i", 5);
   	samplePara->m_acRecDelayTime_D 	= cfg_get_int(Config_Public, section, "delaytime_d", 0);
	s_samplePara = *samplePara;
	cfg_rwlock_unlock();
}

/*
* ��������	��	��ȡ�ɼ�����
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.22
* -----------------------------------------------
* 2015/08/22 V1.0		XXXX		  XXXX
*/
void sample_set_sample_para(CAPTURE_PARAM *samplePara)
{
	//������
	char section[32] = {0};
	sprintf(section, "sample_para");

	cfg_rwlock_wrlock();
	
   	cfg_set_int(Config_Public, section, "ealimit_i", samplePara->m_eaLimit_I );
   	cfg_set_int(Config_Public, section, "ealimit_d", samplePara->m_eaLimit_D);
   	cfg_set_int(Config_Public, section, "acclimit_i", samplePara->m_accLimit_I);
   	cfg_set_int(Config_Public, section, "acclimit_d", 	samplePara->m_accLimit_D);
   	cfg_set_int(Config_Public, section, "delaytime_i", samplePara->m_acRecDelayTime_I);
   	cfg_set_int(Config_Public, section, "delaytime_d", samplePara->m_acRecDelayTime_D);

	s_samplePara = *samplePara;
	cfg_save_ini(Config_Public);
	cfg_rwlock_unlock();
}
/*
* ��������	��	У׼���ٶȼ�
* ��Ա����	:		��
* �������	��	psCapPara	:��Ŷ�ȡ���Ĳɼ��幤������
* �������	��	��
* �� �� ֵ	��		0	�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int sample_board_verify_aac(void)
{
	unsigned char sendBuf[64];
	int len = 0;
	len = pack_cmd(S_CMD_ID_VERIFY_ACC,sendBuf, NULL, 0);
	UartWrite(s_pSampleUart,len, (char*)sendBuf);

	return 0;
}

/*
* ��������	��	���Ͳɼ��幤���������ɼ���
* ��Ա����	:		��
* �������	��	psCapPara	:��Ŷ�ȡ���Ĳɼ��幤������
* �������	��	��
* �� �� ֵ	��		0	�ɹ�
										-1	ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
int sample_board_send_para(CAPTURE_PARAM * psCapPara)
{
	unsigned char sendBuf[64];
	unsigned char body[8];
	int len = 0;
	
	body[0] = psCapPara->m_eaLimit_I;
	body[1] = psCapPara->m_eaLimit_D;
	len = pack_cmd(S_CMD_ID_SET_ELEC_LIMIT,sendBuf, body, 2);
	UartWrite(s_pSampleUart,len, (char*)sendBuf);
	//ptf_dbg("----------------------------------------------------");
	usleep(1000 * 5);
	body[0] = psCapPara->m_accLimit_I;
	body[1] = psCapPara->m_accLimit_D;
	len = pack_cmd(S_CMD_ID_SET_ACC_LIMIT,sendBuf, body, 2);
	UartWrite(s_pSampleUart,len, (char*)sendBuf);
	//ptf_dbg("----------------------------------------------------");
	usleep(1000 * 5);
	
	body[0] = psCapPara->m_acRecDelayTime_I;
	body[1] = psCapPara->m_acRecDelayTime_D;
	len = pack_cmd(S_CMD_ID_SET_ELEC_DELAY_RECD_TIME,sendBuf, body, 2);
	UartWrite(s_pSampleUart,len, (char*)sendBuf);
	//ptf_dbg("----------------------------------------------------");
	usleep(1000 * 5);

	return 0;
}

/*
* ��������	��	��ʼ���ɼ�ģ��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
* -----------------------------------------------
* 2015/08/01 V1.0		XXXX		  XXXX
*/
void sample_board_init(void)
{
	s_pSampleUart = UartOpen("/dev/ttyAMA3");
	if (NULL == s_pSampleUart)
	{
		ptf_err("UartOpen err");
		return ;
	}
	UartSetSpeed(s_pSampleUart, 115200);
	UartSetParity(s_pSampleUart, 8, 1, 'n');
	sample_get_sample_para(&s_samplePara);
	s_SampleCtrl.m_eDfw = DFW_DW;
	tcpClientSetDFWRecSnap(s_SampleCtrl.m_eDfw);
	ThreadPoolAddWorkLimit(g_psThreadPool,sample_board_query_proc, NULL);
	//�������ղɼ��崮�������߳�
	ThreadPoolAddWorkLimit(g_psThreadPool,pthread_sample_recv, NULL);
	return ;
}
