//标准头文件包含区域
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

//自定义头文件包含区域
#include "cmd_handle.h"
#include "printf_func.h"
#include "public_func.h"
#include "cmd_protocol.h"
#include "dev_property.h"
#include "net_para.h"
#include "datapool.h"
#include "global_para.h"
#include "tcp_client.h"
#include "sample_board.h"
#include "sys_mag.h"
#include "gpio_cfg.h"

int tcpCmdHandlSendPicture_ManualSnap(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen)
{
        pthis->m_ptHead.m_headTag[0] 	= 'B';
        pthis->m_ptHead.m_headTag[1] 	= 'N';
        pthis->m_ptHead.m_headTag[2] 	= 'X';
        pthis->m_ptHead.m_ptType 		= MANUAL_SNAP_PICTURE;
        pthis->m_ptHead.m_ptLen  			= picLen + VIDEO_PARAM_LEN;

        struct iovec iov[3];
        iov[0].iov_base 	= (void*)(pthis->m_Buf);
        iov[0].iov_len  	= PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);

        unsigned char* pBuf = pthis->m_Buf + PT_HEAD_LEN;
        PutVideoPara2Buf(psVPara, pBuf);

        iov[1].iov_base 	= (void*)(pBuf);
        iov[1].iov_len  	= VIDEO_PARAM_LEN;

        iov[2].iov_base 	= (void*)(picBuf);
        iov[2].iov_len  	= picLen;

        int iRet = writev(pthis->m_Socket, iov, 3);
        if (-1 == iRet)
        {
                perror("writev");
                ptf_err("send data errno %d", errno);
                return -1;
        }

        return 0;
}



//常量定义区

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部变量

/*
* 功能描述	：	模块析构函数
* 成员更新	:		无
* 输入参数	：	socketFd	套接字
* 输出参数	：	无
* 返 回 值	：		参数错误返回	-1
										成功返回		0
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlDistruct(CMD_HANDLE* pthis)
{
	if (pthis == NULL)
	{
		ptf_err("pthis is null");
		return -1;
	}

	free(pthis);
	return 0;
}

/*
* 功能描述	：	接收统一信息头
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			套接字出错
										-2			协议头错误
										0				成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvPtHead(CMD_HANDLE* pthis)
{
    	int ret = 0;
    	unsigned char headbuf[PT_HEAD_LEN];
    	ret = RecvFixedLenData(pthis->m_Socket, (char*)headbuf, PT_HEAD_LEN);
    	if (ret != 0)
    	{
			ptf_err("socket err ret %d", ret);
			ret = -1;
			goto leave;//socket 出错返回
    	}

    	PutBuf2PtHead(&(pthis->m_ptHead), headbuf);

    	if(memcmp(pthis->m_ptHead.m_headTag,PT_HEAD_TAG,3) != 0)
    	{
	        ptf_fix_len_string((char*)pthis->m_ptHead.m_headTag, 3);
			ret = -2;
			goto leave;//协议错误返回
    	}

leave:
	
    	return ret;
}

/*
* 功能描述	：	利用内部的存储空间接收附载
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			数据太大
										-2			套接字出错
										0			成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyInBuf(CMD_HANDLE* pthis)
{
	int ret = 0;

	int len = pthis->m_ptHead.m_ptLen;
	//ptf_dbg("len %d", len);
	if(len > sizeof(pthis->m_Buf))
	{
		ptf_err("buf is small bodysize %d ptInfo->m_RecvBufLen %d", len,sizeof(pthis->m_Buf));
		ret = -1;//数据太大
		goto leave;
	}	

	pthis->m_Buf[len] = '\0';
	ret = RecvFixedLenData(pthis->m_Socket,(char*)(pthis->m_Buf), len);
	if (ret != 0)
	{
		ptf_err("socket err ret %d", ret);
		ret = -2;//socket 出错返回
		goto leave;
	}
	//ptf_dbg("pthis->m_Buf %s", pthis->m_Buf);

	ret = 0;
	leave:

	return ret;
}

/*
* 功能描述	：	利用外部的存储空间接收附载
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		-1			数据太大
										-2			套接字出错
										0			成功
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlRecvBodyOutBuf(CMD_HANDLE* pthis,char* buf,int buflen)
{
	int ret = 0;

	int len = pthis->m_ptHead.m_ptLen;
	if(len > buflen)
	{
		ptf_err("buf is small bodysize %d buflen %d", len,buflen);
		ret = -1;//数据太大
		goto leave;
	}	

	buf[len] = '\0';
	ret = RecvFixedLenData(pthis->m_Socket,buf, len);
	if (ret != 0)
	{
		ptf_err("socket err ret %d", ret);
		ret = -2;//socket 出错返回
		goto leave;
	}

	ret = 0;
	leave:

	return ret;
}

/*
* 功能描述	：	格式化命令参数到数组中
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlCmdFormat(CMD_HANDLE* pthis)
{
	int ret;
	pthis->m_CheckCode 	= (pthis->m_Buf[0] << 8) + pthis->m_Buf[1];
	pthis->m_MainId			= pthis->m_Buf[2];
	pthis->m_SubId 			= pthis->m_Buf[3];
	pthis->m_BodyArrNum	= pthis->m_Buf[4];	

	memset(pthis->m_ArrBuf, 0, sizeof(pthis->m_ArrBuf));
	ret = DivideStringBySeparator((char*)pthis->m_Buf + (CMD_HEAD_LEN - 1), pthis->m_ArrBuf,SEPARATOR,pthis->m_BodyArrNum);
	if (ret != pthis->m_BodyArrNum)
	{
		ptf_err("ret is %d pthis->m_BodyArrNum %d",ret,pthis->m_BodyArrNum);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdDevProperty(CMD_HANDLE* pthis)
{
    unsigned char paraNum = 0;
    char body[256] = {0};
    unsigned char replySubId = 0;
    switch (pthis->m_SubId)
    {
        case SUB_REQ_DEV_VERSION:
        {
            DEV_VERSION sDevVer;
            devPropertyGetDevVersion(&sDevVer);
            sprintf(body, "%d|%s|%s|%s|%s|%s|", ERR_NO, sDevVer.m_DevId, sDevVer.m_VBoardVer, sDevVer.m_SBoardVer,\
                sDevVer.m_VSoftVer, sDevVer.m_SSoftVer);
            replySubId = SUB_RSP_DEV_VERSION;
            paraNum = 6;
        }break;
        case SUB_SET_DEV_ID:
        {
            devPropertySetDevId(pthis->m_ArrBuf[0]);
            sprintf(body, "%d|", ERR_NO);
            replySubId = SUB_SET_DEV_ID_RSP;
            paraNum = 1;
        }break;
        case SUB_GET_DEV_ID:
        {
            char DevId[32] = {0};
            devPropertyGetDevId(DevId);
            sprintf(body, "%d|%s|", ERR_NO, DevId);
            replySubId = SUB_GET_DEV_ID_RSP;
            paraNum = 2;
        }break;
        default:
        {
            ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
            return -1;
        }break;
    }
    
    return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/
int tcpCmdNetPara(CMD_HANDLE* pthis)
{
    int paraNum = 0;
    char body[256] = {0};
    unsigned char replySubId = 0;
    switch (pthis->m_SubId)
    {
        case SUB_GET_TCP_IP_PARA:
        {
          	TCP_IP_PARA sPara;
			netGetTcpPara(&sPara);
          	sprintf(body, "%d|%s|%s|%s|", ERR_NO, sPara.m_cIPAddr, sPara.m_cNetMask, sPara.m_cGateWay);
           replySubId 	= SUB_GET_TCP_IP_PARA_RSP;
           paraNum 	= 4;
        }break;
        case SUB_SET_TCP_IP_PARA:
        {
          	TCP_IP_PARA sPara;
			strcpy(sPara.m_cIPAddr,pthis->m_ArrBuf[0]);
			strcpy(sPara.m_cNetMask,pthis->m_ArrBuf[1]);
			strcpy(sPara.m_cGateWay,pthis->m_ArrBuf[2]);
			netSetTcpPara(&sPara);
          	sprintf(body, "%d|", ERR_NO);
          	replySubId 	= SUB_SET_TCP_IP_PARA_RSP;
          	paraNum 	= 1;
			tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
			usleep(1000 * 40);
			sys_mag_reboot();
			return 0;
        }break;
        case SUB_GET_MAC:
        {
			char mac[32] = {0};
			netGetMac(mac);
			sprintf(body, "%d|%s|", ERR_NO, mac);
			replySubId 	= SUB_GET_MAC_RSP;
			paraNum 	= 2;
        }break;
		 case SUB_SET_MAC:
        {
            netSetMac(pthis->m_ArrBuf[0]);
            sprintf(body, "%d|", ERR_NO);
            replySubId = SUB_SET_MAC_RSP;
            paraNum = 1;
        }break;
		 case SUB_GET_DEV_GROUP_NO:
        {
			sprintf(body, "%d|%d|", ERR_NO, netGetGroup());
			replySubId 	= SUB_GET_DEV_GROUP_NO_RSP;
			paraNum 	= 2;
        }break;
		 case SUB_SET_DEV_GROUP_NO:
        {
			netSetGroup(atoi(pthis->m_ArrBuf[0]));
			sprintf(body, "%d|", ERR_NO);
			replySubId 	= SUB_SET_DEV_GROUP_NO_RSP;
			paraNum 	= 1;
        }break;
		 case SUB_GET_PC_SEVER_CONFIG:
        {
			PC_SERVER_PARA sPara;
			netGetPcServerPara(&sPara);
			sprintf(body, "%d|%d|%s|%d|", ERR_NO, sPara.m_enable,sPara.m_ipaddr,sPara.m_port);
			replySubId 	= SUB_GET_PC_SERVER_CONFIG_RSP;
			paraNum 	= 4;
        }break;
		 case SUB_SET_PC_SERVER_CONFIG:
        {
			PC_SERVER_PARA sPara;
			sPara.m_enable = atoi(pthis->m_ArrBuf[0]);
			strcpy(sPara.m_ipaddr, pthis->m_ArrBuf[1]);
			sPara.m_port = atoi(pthis->m_ArrBuf[2]);
			netSetPcServerPara(&sPara);
			sprintf(body, "%d|", ERR_NO);
			replySubId 	= SUB_SET_PC_SERVER_CONFIG_RSP;
			paraNum 	= 1;
			tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
			usleep(1000 * 40);
			sys_mag_reboot();
			return 0;
        }break;
        default:
        {
            ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
            return -1;
        }break;
    }
    
    return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.05
* -----------------------------------------------
* 2015/08/05 V1.0		XXXX		  XXXX
*/

#if 0
int tcpCmdFileUpdate(CMD_HANDLE* pthis)
{
    int iret = 0;
    int paraNum = 0;
    char body[256] = {0};
    unsigned char replySubId = 0;
    switch (pthis->m_SubId)
    {
        case SUB_FILE_UPDATE:
        {
          	int fileType 	= atoi(pthis->m_ArrBuf[0]);
			int fileLen 		= atoi(pthis->m_ArrBuf[1]);
			if (fileType != 1)
			{
				ptf_err("fileType %d",fileType);
				return -1;
			}
			
			ptf_dbg("fileLen %d",fileLen);
			char* fileBuf = (char*)AllocatorAlloc(g_psArpAllocator, fileLen);
			iret = RecvFixedLenData(pthis->m_Socket,fileBuf, fileLen);
			if (iret != 0)
			{
				ptf_err("iret %d",iret);
				AllocatorFree(fileBuf);
				return -1;
			}
			
			UPDATE_FILE sHead;
			PutBuf2UpdatFile(&sHead, (unsigned char*)fileBuf);
			if (memcmp(sHead.m_company, FILE_UPDATE_TAG, strlen(FILE_UPDATE_TAG)) != 0)
			{
				PrintfFixString("sHead.m_company %s",4);
				AllocatorFree(fileBuf);
				return -1;
			}

			int64_t checkSum =0;
			int FilePtLen = sHead.m_filesize;
			while (FilePtLen--)
			{
				checkSum += (unsigned char)fileBuf[FilePtLen + UPDATE_FILE_LEN];
			}
			
			if (checkSum != sHead.m_checksum)
			{
				ptf_err("checkSum %lld sHead.m_checksum %lld", checkSum, sHead.m_checksum);
				AllocatorFree(fileBuf);
				return -1;
			}

			//写入升级文件，下次开机检测进行升级
			sys_mag_write_update_file(fileBuf + UPDATE_FILE_LEN,fileLen - UPDATE_FILE_LEN);
			AllocatorFree(fileBuf);
			//
          	sprintf(body, "%d|", ERR_NO);
			replySubId 	= SUB_FILE_UPDATE_RSP;
         	paraNum 	= 1;
        }break;
        default:
        {
            ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
            return -1;
        }break;
    }
    
    return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

#endif
int tcpCmdFileUpdate(CMD_HANDLE* pthis)
{
    int iret = 0;
    int paraNum = 0;
    char body[256] = {0};
    unsigned char replySubId = 0;
    switch (pthis->m_SubId)
    {
        case SUB_FILE_UPDATE:
        {
          	int fileType 	= atoi(pthis->m_ArrBuf[0]);
			int fileLen 		= atoi(pthis->m_ArrBuf[1]);
			if (fileType != 1)
			{
				ptf_err("fileType %d",fileType);
				return -1;
			}
			
			ptf_dbg("fileLen %d",fileLen);
			char* fileBuf = (char*)AllocatorAlloc(g_psArpAllocator, fileLen);
			iret = RecvFixedLenData(pthis->m_Socket,fileBuf, fileLen);
			if (iret != 0)
			{
				ptf_err("iret %d",iret);
				AllocatorFree(fileBuf);
				return -1;
			}
			
			//写入升级文件，下次开机检测进行升级
			sys_mag_write_update_file(fileBuf,fileLen);
			AllocatorFree(fileBuf);
			//
          	sprintf(body, "%d|", ERR_NO);
			replySubId 	= SUB_FILE_UPDATE_RSP;
         	paraNum 	= 1;
        }break;
        default:
        {
            ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
            return -1;
        }break;
    }
    
    return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdSamplePara(CMD_HANDLE* pthis)
{
	unsigned char paraNum = 0;
	char body[256] = {0};
	unsigned char replySubId = 0;
	switch (pthis->m_SubId)
	{
		case SUB_GET_SAMPLE_PARA:
		{
			CAPTURE_PARAM samplePara;
			sample_get_sample_para(&samplePara);
			sprintf(body, "%d|%d|%d|%d|%d|%d|%d|", ERR_NO, samplePara.m_eaLimit_I,samplePara.m_eaLimit_D,samplePara.m_accLimit_I,
				samplePara.m_accLimit_D,samplePara.m_acRecDelayTime_I,samplePara.m_acRecDelayTime_D);
			replySubId = SUB_GET_SAMPLE_PARA_RSP;
			paraNum = 7;
			if (tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum) < 0)
			{
				ptf_err("tcpCmdHandlSendCmd2 err");
				return -1;
			}
			////printf("tqk_______________________________tqk%s\n",body);
			return 0;
		}break;
		
		case SUB_SET_SAMPLE_PARA:
		{
			CAPTURE_PARAM samplePara;
			samplePara.m_eaLimit_I 					= atoi(pthis->m_ArrBuf[0]);
			samplePara.m_eaLimit_D 					= atoi(pthis->m_ArrBuf[1]);
			samplePara.m_accLimit_I 					= atoi(pthis->m_ArrBuf[2]);
			samplePara.m_accLimit_D 				= atoi(pthis->m_ArrBuf[3]);
			samplePara.m_acRecDelayTime_I 	= atoi(pthis->m_ArrBuf[4]);
			samplePara.m_acRecDelayTime_D 	= atoi(pthis->m_ArrBuf[5]);

			sample_set_sample_para(&samplePara);
			sprintf(body, "%d|", ERR_NO);
			////printf("samplePara.m_eaLimit_I=%d\n",samplePara.m_eaLimit_I);
			////printf("samplePara.m_eaLimit_D=%d\n",samplePara.m_eaLimit_D);
			////printf("samplePara.m_accLimit_I=%d\n",samplePara.m_accLimit_I);
			////printf("samplePara.m_accLimit_D=%d\n",samplePara.m_accLimit_D);
			////printf("samplePara.m_acRecDelayTime_I=%d\n",samplePara.m_acRecDelayTime_I);
			////printf("samplePara.m_acRecDelayTime_D=%d\n",samplePara.m_acRecDelayTime_D);
			replySubId = SUB_SET_SAMPLE_PARA_RSP;
			paraNum = 1;
		}break;
		case SUB_VERIFY_ACC:
		{
			sample_board_verify_aac();
			sprintf(body, "%d|", ERR_NO);
			replySubId = SUB_VERIFY_ACC_RSP;
			paraNum = 1;
		}break;
		
		case SUB_GET_SEND_INTER_TIME:
		{
			SAMPLE_SEND_TIME_S sSndTime;
			tcpClientGetTimerSendInter(&sSndTime);
			sprintf(body, "%d|%d|%d|%d|%d|", ERR_NO,sSndTime.sample_i,sSndTime.sample_d,sSndTime.sanp_i,sSndTime.sanp_d);
			replySubId = SUB_GET_SEND_INTER_TIME_RSP;
			paraNum = 5;
		}break;

		case SUB_SET_SEND_INTER_TIME:
		{
			SAMPLE_SEND_TIME_S sSndTime;
			sSndTime.sample_i 		= atoi(pthis->m_ArrBuf[0]);
			sSndTime.sample_d 		= atoi(pthis->m_ArrBuf[1]);
			sSndTime.sanp_i 			= atoi(pthis->m_ArrBuf[2]);
			sSndTime.sanp_d 			= atoi(pthis->m_ArrBuf[3]);
			tcpClientSetTimerSendInter(&sSndTime);
			sprintf(body, "%d|", ERR_NO);
			replySubId = SUB_SET_SEND_INTER_TIME_RSP;
			paraNum = 1;
		}break;
				
		default:
		{
			ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
			return -1;
		}break;
	}

	return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

static int redstrap_delay_id = 0;
void pthread_RedStrap_Interrupt()
{
    redstrap_delay_id++;
}
static void* pthread_delay_RedStrap_stop(void* args)
{
        int id = redstrap_delay_id;
	sleep(20);
        if(id != redstrap_delay_id) return NULL;

	CMD_PARA cmdPara;
	cmdPara.m_eType = CMD_RED_STRAP_STOP_PUSH;
	tcpClientHandlePushCmd(&cmdPara);
	

	return NULL;
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdMediaCtrl(CMD_HANDLE* pthis)
{
	unsigned char paraNum = 0;
	char body[256] = {0};
	unsigned char replySubId = 0;
	switch (pthis->m_SubId)
	{
		case SUB_PC_TO_DEV_STREAM_START:
		{
			//投递fifo
			CMD_PARA cmdPara;
			cmdPara.m_eType = CMD_MANUAL_START_PUSH;
			cmdPara.m_para1 = atoi(pthis->m_ArrBuf[0]);
			ptf_dbg("cmdPara->m_para1 %d",cmdPara.m_para1);
			////printf("tqk??????????tqk_start_to_get_stream_command\n");
			tcpClientHandlePushCmd(&cmdPara);
			////printf("tqk??????????tqk_finish_getting_stream_command\n");
			dev_get_dfw(cmdPara.m_para1);
			sprintf(body, "%d|%d|%d|%d|", ERR_NO,dev_get_dfw(cmdPara.m_para1),BH_DEST_WIDTH,BH_DEST_HEIGHT);
			replySubId = SUB_PC_TO_DEV_STREAM_START_RSP;
			paraNum = 4;
		}break;
		case SUB_PC_TO_DEV_STREAM_STOP:
		{
			//投递fifo
			CMD_PARA cmdPara;
			cmdPara.m_eType = CMD_MANUAL_STOP_PUSH;
			tcpClientHandlePushCmd(&cmdPara);
			sprintf(body, "%d|", ERR_NO);
			replySubId = SUB_PC_TO_DEV_STREAM_STOP_RSP;
			paraNum = 1;
		}break;
		case SUB_DEV_TO_PC_STREAM_START_RSP:
		{
				return 0;
		}break;
		case SUB_DEV_TO_PC_STREAM_STOP_RSP:
		{
				return 0;
		}break;
		case SUB_MANUAL_GET_SNAP_PIC:
		{
                        DataPakg* picture;
                        DFW_E eDfw = atoi(pthis->m_ArrBuf[0]);
                        int logicChn = GetDFWChn(eDfw);
                        ptf_dbg("current curnel = %d",eDfw);
                        gpio_led_ctrl(logicChn, led_on,led_snap);
                        usleep(500000);
                        picture = venc_get_snap_picture(logicChn);
                        if (NULL != picture)
                        {
                                VIDEO_PARAM sVPara;
                                sVPara.m_width 		= BH_DEST_WIDTH;
                                sVPara.m_height 	= BH_DEST_HEIGHT;
                                sVPara.m_chn 		= logicChn;
                                sVPara.m_positon 	= eDfw;
                                tcpCmdHandlSendPicture_ManualSnap(pthis, &sVPara,picture->m_bData, picture->m_iDataSize);
                                //ptf_dbg("iRet %dpicture->m_iDataSize %d",iRet,picture->m_iDataSize);
                                DataPkgRelease(picture);
                        }
                        usleep(500000);
                        gpio_led_ctrl(logicChn, led_off,led_snap);
		}break;
		case SUB_RED_STRAP_START:
		{
			CMD_PARA cmdPara;
			cmdPara.m_eType = CMD_RED_STRAP_START_PUSH;
			cmdPara.m_para1 = tcpClientGetDFWRecSnap();
			tcpClientHandlePushCmd(&cmdPara);
			ThreadPoolAddWorkLimit(g_psThreadPool, pthread_delay_RedStrap_stop, NULL);
			sprintf(body, "%d|%d|%d|%d|", ERR_NO,tcpClientGetDFWRecSnap(),BH_DEST_WIDTH,BH_DEST_HEIGHT);
			replySubId = SUB_RED_STRAP_START;
			
		}break;
		case SUB_RED_STRAP_FINISH:
		{
			CMD_PARA cmdPara;
			cmdPara.m_eType = CMD_RED_STRAP_STOP_PUSH;
			tcpClientHandlePushCmd(&cmdPara);
			sprintf(body, "%d|", ERR_NO);
			replySubId = SUB_RED_STRAP_FINISH;
			
		}break;

		default:
		{
			ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
			return -1;
		}break;
	}

	return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}

/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值		：	0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdDevMag(CMD_HANDLE* pthis)
{
	unsigned char paraNum = 0;
	char body[256] = {0};
	unsigned char replySubId = 0;
	ptf_dbg("pthis->m_SubId 0x%x",pthis->m_SubId);

	switch (pthis->m_SubId)
	{
		case SUB_DEV_REGISTER_RSP:
		{
			return 0;
		}break;
		case SUB_DEV_REBOOT:
		{
			sprintf(body, "%d|", ERR_NO);
			replySubId = SUB_DEV_REBOOT_RSP;
			paraNum = 1;
			tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
			sys_mag_reboot();
		}break;
		case SUB_GET_CAMERA_POS:
		{
			int chn = atoi(pthis->m_ArrBuf[0]);
			int dfw = dev_get_dfw(chn);
			if (dfw < 0 || dfw > 1)
			{
				sprintf(body, "%d|", ERR_UNKNOW);
				paraNum = 1;
			}
			else
			{
				sprintf(body, "%d|%d|", ERR_NO,dfw);
				paraNum = 2;
			}
			replySubId = SUB_DEV_REBOOT_RSP;
		}break;
		case SUB_SET_CAMERA_POS:
		{
			int chn = atoi(pthis->m_ArrBuf[0]);
			int dfw = atoi(pthis->m_ArrBuf[1]);
			int ret = dev_set_dfw(chn, dfw );
			if (ret < 0)
			{
				sprintf(body, "%d|", ERR_UNKNOW);
				paraNum = 1;
			}
			else
			{
				sprintf(body, "%d|%d|", ERR_NO,dfw);
				paraNum = 2;
			}
			replySubId = SUB_SET_CAMERA_POS_RSP;
		}break;

		// added by TQK 2015.12.28,set the state of camera.
		case SUB_SET_CAMERA_STATE:
		{
			int dfw = atoi(pthis->m_ArrBuf[0]);
			s_SampleCtrl.m_eDfw = dfw;
			tcpClientSetDFWRecSnap(s_SampleCtrl.m_eDfw);
			//s_SampleCtrl.m_iAccRecFlg = 0;
			//s_SampleCtrl.m_iEleRecFlg = 0;
			sprintf(body, "%d|%d|",ERR_NO,dfw);
			paraNum = 2;
			replySubId = SUB_SET_CAMERA_STATE_RSP;
		}break;
		case SUB_GET_CAMERA_STATE:
		{
			int dfw = tcpClientGetDFWRecSnap();//s_SampleCtrl.m_eDfw;
			if (dfw < 0 || dfw > 1)
			{
				sprintf(body, "%d|", ERR_UNKNOW);
				paraNum = 1;
			}
			else
			{
				sprintf(body, "%d|%d|", ERR_NO,dfw);
				paraNum = 2;
			}
			replySubId = SUB_GET_CAMERA_STATE_RSP;
		}break;
		
		default:
		{
			ptf_err("pthis->m_SubId 0x%x",pthis->m_SubId);
			return -1;
		}break;
	}

	return tcpCmdHandlSendCmd2(pthis,replySubId,body,paraNum);
}
/*
* 功能描述	：	处理主ID的命令参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.03
* -----------------------------------------------
* 2015/08/03 V1.0		XXXX		  XXXX
*/
int tcpCmdHandleMainId(CMD_HANDLE* pthis)
{
	int iret = 0;
	ptf_dbg("pthis->m_MainId 0x%x",pthis->m_MainId);
	switch (pthis->m_MainId)
	{
		case MAIN_DEV_PROPERTY:
		{
			iret = tcpCmdDevProperty(pthis);
		}break;
		case MAIN_NET_PARA:
		{
			iret = tcpCmdNetPara(pthis);
		}break;
		case MAIN_FILE_UPDATE:
		{
			iret = tcpCmdFileUpdate(pthis);
			if (iret == 0)
			{
				usleep(1000 * 40);
				sys_mag_reboot();
			}
		}break;
		case MAIN_SAMPLE_BOARD_PARA:
		{
			iret = tcpCmdSamplePara(pthis);
		}break;
		case MAIN_MEDIA_CONTROL:
		{
			iret = tcpCmdMediaCtrl(pthis);
		}break;
		case SUB_DEV_MANAGE:
		{
			iret = tcpCmdDevMag(pthis);
		}break;
		default:
		{
			ptf_err("pthis->m_MainId 0x%x",pthis->m_MainId);
			return -1;
		}break;
	}

	return iret;
} 



/////////////////////////////////
//发送信令
int tcpCmdHandleSendFailure(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum)
{
	int offset = 0;
	pthis->m_ptHead.m_headTag[0] = 'B';
	pthis->m_ptHead.m_headTag[1] = 'N';
	pthis->m_ptHead.m_headTag[2] = 'X';
	pthis->m_ptHead.m_ptType 	= CMD_DATA;
	pthis->m_ptHead.m_ptLen  		= CMD_HEAD_LEN + strlen(Body);
	unsigned char* pBuf = (pthis->m_Buf + PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf));

	pBuf[offset] = (unsigned char)(pthis->m_CheckCode >> 8);
	offset += 1;
	pBuf[offset] = (unsigned char)(pthis->m_CheckCode);
	offset += 1;

	pBuf[offset] = MainId;
	offset += 1;
	pBuf[offset] = SubId;
	offset += 1;

	pBuf[offset] = BodyParaNum;
	offset += 1;

	memcpy(pBuf + offset,Body,strlen(Body));
	offset += strlen(Body);
	pBuf[offset] = CMD_END_FLAG;
	offset += 1;

	int flags = MSG_NOSIGNAL;
	int ret = send(pthis->m_Socket, (void*)(pthis->m_Buf), PT_HEAD_LEN + offset, flags);
	if(ret == -1)
	{
		//ptf_err("send Body = %s, errno = %d",Body, errno);
		if(errno == EPIPE)
		{
			ptf_dbg("+++++ recv signal: SIGPIPE");
		}
		return -1;
	}
	ptf_dbg("Body %s",Body);
	
	return 0;
}
/////////////////////////////////

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										MainId	主ID
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendCmd1(CMD_HANDLE* pthis,unsigned char MainId,unsigned char SubId,char* Body,unsigned char BodyParaNum)
{
	int offset = 0;
	pthis->m_ptHead.m_headTag[0] = 'B';
	pthis->m_ptHead.m_headTag[1] = 'N';
	pthis->m_ptHead.m_headTag[2] = 'X';
	pthis->m_ptHead.m_ptType 	= CMD_DATA;
	pthis->m_ptHead.m_ptLen  		= CMD_HEAD_LEN + strlen(Body);
	unsigned char* pBuf = (pthis->m_Buf + PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf));

	pBuf[offset] = (unsigned char)(pthis->m_CheckCode >> 8);
	offset += 1;
	pBuf[offset] = (unsigned char)(pthis->m_CheckCode);
	offset += 1;

	pBuf[offset] = MainId;
	offset += 1;
	pBuf[offset] = SubId;
	offset += 1;

	pBuf[offset] = BodyParaNum;
	offset += 1;

	memcpy(pBuf + offset,Body,strlen(Body));
	offset += strlen(Body);
	pBuf[offset] = CMD_END_FLAG;
	offset += 1;

	int flags = MSG_NOSIGNAL;
	int ret = send(pthis->m_Socket, (void*)(pthis->m_Buf), PT_HEAD_LEN + offset, flags);
	if(ret == -1)
	{
		//ptf_err("send Body = %s, errno = %d",Body, errno);
		if(errno == EPIPE)
		{
			ptf_dbg("+++++ recv signal: SIGPIPE");
		}
		return -1;
	}
	ptf_dbg("Body %s",Body);

	return 0;
}

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.15
* -----------------------------------------------
* 2015/08/15 V1.0		XXXX		  XXXX
*/
int tcpCmdRegister(CMD_HANDLE* pthis)
{
	char body[256] = {0};
	TCP_IP_PARA sPara;
	netGetTcpPara(&sPara);
	char devId[32];
	devPropertyGetDevId(devId);
	sprintf(body, "%s|%d|%s|",sPara.m_cIPAddr, netGetGroup(),devId);
	return tcpCmdHandlSendCmd1(pthis,SUB_DEV_MANAGE,SUB_DEV_REGISTER,body,3);
}

/*
* 功能描述	：	发送信令
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										SubId	子ID
										Body	消息体
										BodyParaNum	消息参数个数
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendCmd2(CMD_HANDLE* pthis,unsigned char SubId,char* Body,unsigned char BodyParaNum)
{
    return tcpCmdHandlSendCmd1(pthis,pthis->m_MainId,SubId,Body,BodyParaNum);
}

/*
* 功能描述	：	发送图片
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										psVPara	视频描述信息
										picBuf	存放图片的内存地址
										picLen	图片大小
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/

int tcpCmdHandlSendPicture_Elec(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= VIDEO_PICTURE;
	pthis->m_ptHead.m_ptLen  			= picLen + VIDEO_PARAM_LEN;

	struct iovec iov[3];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);

	unsigned char* pBuf = pthis->m_Buf + PT_HEAD_LEN;
	PutVideoPara2Buf(psVPara, pBuf);

	iov[1].iov_base 	= (void*)(pBuf);
	iov[1].iov_len  	= VIDEO_PARAM_LEN;

	iov[2].iov_base 	= (void*)(picBuf);
	iov[2].iov_len  	= picLen;

	int iRet = writev(pthis->m_Socket, iov, 3);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}



int tcpCmdHandlSendPicture(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara,char* picBuf,int picLen)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= VIDEO_PICTURE;
	pthis->m_ptHead.m_ptLen  			= picLen + VIDEO_PARAM_LEN;

	struct iovec iov[3];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);

	unsigned char* pBuf = pthis->m_Buf + PT_HEAD_LEN;
	PutVideoPara2Buf(psVPara, pBuf);

	iov[1].iov_base 	= (void*)(pBuf);
	iov[1].iov_len  	= VIDEO_PARAM_LEN;

	iov[2].iov_base 	= (void*)(picBuf);
	iov[2].iov_len  	= picLen;

	int iRet = writev(pthis->m_Socket, iov, 3);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}



/*
* 功能描述	：	发送视频数据
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										picBuf	存放图片的内存地址
										picLen	图片大小
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoStream(CMD_HANDLE* pthis, char* streamBuf, int streamLen)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= VIDEO_DATA;
	pthis->m_ptHead.m_ptLen  			= streamLen;

	struct iovec iov[2];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);
	iov[1].iov_base 	= (void*)(streamBuf);
	iov[1].iov_len  	= streamLen;

	int iRet = writev(pthis->m_Socket, iov, 2);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	发送视频参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										psVPara	视频参数结构体
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendVideoPara(CMD_HANDLE* pthis, VIDEO_PARAM * psVPara)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= VIDEO_PARA;
	pthis->m_ptHead.m_ptLen 			= VIDEO_PARAM_LEN;
	unsigned char* pBuf = pthis->m_Buf + PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);
	PutVideoPara2Buf(psVPara, pBuf);

	struct iovec iov[1];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= VIDEO_PARAM_LEN + PT_HEAD_LEN;

	int iRet = writev(pthis->m_Socket, iov, 1);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	发送心跳
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendHeartBeat(CMD_HANDLE* pthis)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= HEART_BEAT;
	pthis->m_ptHead.m_ptLen = 0;
	PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);
	struct iovec iov[1];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= PT_HEAD_LEN;

	int iRet = writev(pthis->m_Socket, iov, 1);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	发送采集参数
* 成员更新	:		无
* 输入参数	：	pthis	tcpCmdHandlConstruct返回值
										sampleData		采样数据
										sampleLen		采用数据大小
* 输出参数	：	无
* 返 回 值	：		0			成功
										-1		失败
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
int tcpCmdHandlSendSampleData(CMD_HANDLE* pthis, char* sampleData, int sampleLen)
{
	pthis->m_ptHead.m_headTag[0] 	= 'B';
	pthis->m_ptHead.m_headTag[1] 	= 'N';
	pthis->m_ptHead.m_headTag[2] 	= 'X';
	pthis->m_ptHead.m_ptType 		= SAMPLE_DATA;
	pthis->m_ptHead.m_ptLen  			= sampleLen;

	struct iovec iov[2];
	iov[0].iov_base 	= (void*)(pthis->m_Buf);
	iov[0].iov_len  	= PutPtHead2Buf(&(pthis->m_ptHead),pthis->m_Buf);
	iov[1].iov_base 	= (void*)(sampleData);
	iov[1].iov_len  	= sampleLen;

	int iRet = writev(pthis->m_Socket, iov, 2);
	if (-1 == iRet)
	{
		perror("writev");
		ptf_err("send data errno %d", errno);
		return -1;
	}

	return 0;
}

/*
* 功能描述	：	模块构造函数
* 成员更新	:		无
* 输入参数	：	socketFd	套接字
* 输出参数	：	无
* 返 回 值	：		失败返回NULL
										成功返回地址
* 其它说明	：	无
* 修改日期	:		2015.08.02
* -----------------------------------------------
* 2015/08/02 V1.0		XXXX		  XXXX
*/
CMD_HANDLE* tcpCmdHandlConstruct(int socketFd)
{
	CMD_HANDLE* pthis = (CMD_HANDLE*)malloc(sizeof(CMD_HANDLE));
	if (NULL == pthis)
	{
		ptf_err("malloc err");
		return NULL;
	}
	
	memset(pthis, 0, sizeof(CMD_HANDLE));
	pthis->m_Socket = socketFd;	
	return pthis;
}



