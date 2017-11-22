#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//自定义头文件包含区域
#include "printf_func.h"
#include "chn_vio.h"
#include "chn_venc.h"
#include "project_func.h"
#include "chn_vio.h"
#include "global_para.h"
#include "sys_mag.h"
#include "tcp_client.h"

//常量定义区

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部变量
TDataFIFO*  			s_pSnap0_FIFO 		= NULL;	//抓拍fifo
TDataFIFO*  			s_pSnap1_FIFO 		= NULL;	//抓拍fifo


//内部函数

/*
* 功能描述	：	初始化mpp系统
* 成员更新	:		无
* 输入参数	： 	无
* 输出参数	： 	无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 hi_mpp_vb_init(HI_VOID)
{
	MPP_SYS_CONF_S stSysConf = {0};
	HI_S32 s32Ret = HI_FAILURE;

	VB_CONF_S VbConf;
	memset(&VbConf, 0, sizeof(VbConf));

	int width;
	int height;
	int bufsize;

	width	= 1280;
	height 	= 800;
	bufsize = CEILING_2_POWER(height,SAMPLE_SYS_ALIGN_WIDTH) * CEILING_2_POWER(width,SAMPLE_SYS_ALIGN_WIDTH) * 2;
	VbConf.astCommPool[0].u32BlkCnt 		= 10;
	VbConf.astCommPool[0].u32BlkSize 	= bufsize;

	width 	= 800;
	height 	= 600;
	bufsize = CEILING_2_POWER(height,SAMPLE_SYS_ALIGN_WIDTH) * CEILING_2_POWER(width,SAMPLE_SYS_ALIGN_WIDTH) * 2;
	VbConf.astCommPool[1].u32BlkCnt 	= 10;
	VbConf.astCommPool[1].u32BlkSize 	= bufsize;

	s32Ret = HI_MPI_VB_SetConf(&VbConf);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VB_SetConf failed s32Ret 0x%x", s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VB_Init();
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VB_Init failed s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	stSysConf.u32AlignWidth = SAMPLE_SYS_ALIGN_WIDTH;
	s32Ret = HI_MPI_SYS_SetConf(&stSysConf);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_SetConf failed");
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_SYS_Init();
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_Init failed");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

/*
* 功能描述	：	反初始化mpp系统
* 成员更新	:		无
* 输入参数	： 	无
* 输出参数	： 	无
* 返 回 值	：		0		代表成功
										-1	代表失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 hi_mpp_vb_uninit(void)
{
	HI_S32 s32Ret = HI_FAILURE;
	s32Ret = HI_MPI_SYS_Exit();
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_Exit failed s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VB_Exit();
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VB_Exit failed s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

/*
* 功能描述	：	创建并启动视频编码通道
* 输入参数	：	VencGrp	视频编码通道组
										VencChn	视频编码通道
										ViChn	视频采集通道
* 输出参数	：	无
* 返 回 值	：		0		代表成
										-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2015/08/09     V1.0	    
*/
HI_S32 venc_chn_creat_and_start(VENC_GRP VencGrp, VENC_CHN VencChn, VI_CHN ViChn)
{

	HI_S32 s32Ret;
	VENC_CHN_ATTR_S 		stVencChnAttr;
	VENC_ATTR_H264_S 		stH264Attr;
	VENC_ATTR_H264_VBR_S  stH264Vbr;
	//ptf_dbg("---------------------");
	//sleep(5);

	W_H stPicSize;
	stPicSize.u32Width 		= BH_DEST_WIDTH;
	stPicSize.u32Height 	= BH_DEST_HEIGHT;
	MPP_CHN_S stSrcChn, stDestChn;

	s32Ret = HI_MPI_VENC_CreateGroup(VencGrp);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VENC_CreateGroup s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	
	//ptf_dbg("---------------------");
	//sleep(5);

	stVencChnAttr.stVeAttr.enType = PT_H264;
	stH264Attr.u32PicWidth 	= stPicSize.u32Width;		/*the picture width*/
	stH264Attr.u32PicHeight = stPicSize.u32Height;	/*the picture height*/
	stH264Attr.u32BufSize  	= stPicSize.u32Width * stPicSize.u32Height * 2;/*stream buffer size*/
	stH264Attr.u32Profile  	= 0;									/*0: baseline; 1:MP; 2:HP  */
	stH264Attr.bByFrame 	= HI_TRUE;						/*get stream mode is slice mode or frame mode?*/
	stH264Attr.bField 			= HI_FALSE; 					/* surpport frame code only for hi3516, bfield = HI_FALSE */
	stH264Attr.bMainStream 	= HI_TRUE; 				/* surpport main stream only for hi3516, bMainStream = HI_TRUE */
	stH264Attr.u32Priority 	= 0; 								/*channels precedence level. invalidate for hi3516*/
	stH264Attr.bVIField 		= HI_FALSE;					/*the sign of the VI picture is field or frame. Invalidate for hi3516*/
	memcpy(&stVencChnAttr.stVeAttr.stAttrH264e, &stH264Attr, sizeof(VENC_ATTR_H264_S));
	//ptf_dbg("---------------------");
	//sleep(5);

	stVencChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
	stH264Vbr.u32Gop 				= 30;
	stH264Vbr.u32StatTime 		= 1;
	stH264Vbr.u32ViFrmRate 	= 30;
	stH264Vbr.fr32TargetFrmRate 	= 30;
	stH264Vbr.u32MinQp 			= 10;
	stH264Vbr.u32MaxQp 		= 51;
	stH264Vbr.u32MaxBitRate 	= 512;
	memcpy(&stVencChnAttr.stRcAttr.stAttrH264Vbr, &stH264Vbr, sizeof(VENC_ATTR_H264_VBR_S));

	
	//ptf_dbg("---------------------");
	//sleep(5);

	s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr, HI_NULL);

	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VENC_CreateChn s32Ret 0x%x",s32Ret);
		return s32Ret;
	}
	//ptf_dbg("---------------------");
	//sleep(5);

	s32Ret = HI_MPI_VENC_RegisterChn(VencGrp, VencChn);
	//ptf_dbg("---------------------");
	//sleep(5);

	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}
	//ptf_dbg("---------------------");
	//sleep(5);

	stSrcChn.enModId 		= HI_ID_VIU;
	stSrcChn.s32ChnId 	= ViChn;
	stDestChn.enModId 	= HI_ID_GROUP;
	stDestChn.s32ChnId 	= VencGrp;
	//ptf_dbg("---------------------");
	//sleep(5);

	s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
			
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_Bind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}
	//ptf_dbg("-----------------VencChn %d -",VencChn);
	//return 0;
//	sleep(5);
	s32Ret = HI_MPI_VENC_StartRecvPic(VencChn);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_VENC_StartRecvPic s32Ret 0x%x",s32Ret);
		//printf("error\n");
		return HI_FAILURE;
	}else
	{
		//printf("succ\n");
	}
	
	//ptf_dbg("---------------------");
//	sleep(5);

	return HI_SUCCESS;

}

/*
* 功能描述	：	停止并销毁视频编码通道
* 输入参数	：	VencGrp	视频编码通道组
										VencChn	视频编码通道
* 输出参数	：	无
* 返 回 值	：		0		代表成
										-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2015/08/111     V1.0	    
*/
HI_S32 venc_chn_stop_and_destroy(VENC_GRP VencGrp, VENC_CHN VencChn)
{
	HI_S32 		s32Ret;
	MPP_CHN_S 	stDestChn;

	s32Ret = HI_MPI_VENC_StopRecvPic(VencChn);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	stDestChn.enModId 	= HI_ID_GROUP;
	stDestChn.s32ChnId 	= VencGrp;

	s32Ret = HI_MPI_SYS_UnBind(NULL, &stDestChn);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VENC_UnRegisterChn(VencChn);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}


	s32Ret = HI_MPI_VENC_DestroyChn(VencChn);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VENC_DestroyGroup(VencGrp);
	if (HI_SUCCESS != s32Ret)
	{
		ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

/*
* 功能描述	：	创建并启动抓拍编码通道
* 输入参数	：	VencGrp	视频编码通道组
										VencChn	视频编码通道
										ViChn	视频采集通道
* 输出参数	：	无
* 返 回 值	：		0		代表成
										-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
HI_S32 venc_snap_chn_start(VENC_GRP VencGrp,VENC_CHN VencChn, VI_CHN ViChn)
{
    HI_S32 s32Ret;
    VENC_CHN_ATTR_S stVencChnAttr;
    W_H stPicSize;
    VENC_ATTR_JPEG_S stJpegAttr;
	MPP_CHN_S stSrcChn, stDestChn;

	stPicSize.u32Width 		= BH_DEST_WIDTH;
	stPicSize.u32Height 	= BH_DEST_HEIGHT;

    s32Ret = HI_MPI_VENC_CreateGroup(VencGrp);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_VENC_CreateGroup s32Ret 0x%x VencGrp %d",s32Ret,VencGrp);
        return HI_FAILURE;
    }

    stVencChnAttr.stVeAttr.enType 	= PT_JPEG;
    stJpegAttr.u32PicWidth  		= stPicSize.u32Width;
    stJpegAttr.u32PicHeight 		= stPicSize.u32Height;
    stJpegAttr.u32BufSize 			= stPicSize.u32Width * stPicSize.u32Height * 2;
    stJpegAttr.bByFrame 				= HI_TRUE;	/*get stream mode is field mode  or frame mode*/
    stJpegAttr.bVIField 				= HI_FALSE;	/*the sign of the VI picture is field or frame?*/
    stJpegAttr.u32Priority 			= 0;		/*channels precedence level*/
    memcpy(&stVencChnAttr.stVeAttr.stAttrJpeg, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));

    s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_VENC_CreateChn s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

	s32Ret = HI_MPI_VENC_RegisterChn(VencGrp, VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_VENC_RegisterChn s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

    stSrcChn.enModId 		= HI_ID_VIU;
    stSrcChn.s32ChnId 	= ViChn;
    stDestChn.enModId 	= HI_ID_GROUP;
    stDestChn.s32ChnId 	= VencChn;
    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_SYS_Bind s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VENC_StartRecvPic(VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_VENC_StartRecvPic s32Ret 0x%x",s32Ret);
        return s32Ret;
    }
		
    return HI_SUCCESS;
}

/*
* 功能描述	：	销毁抓拍通道
* 输入参数	：	VencGrp	视频编码通道组
										VencChn	视频编码通道
										ViChn	视频采集通道
* 输出参数	：	无
* 返 回 值	：		0		代表成
										-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
HI_S32 venc_snap_destroy_chn(VENC_GRP VeGroup, VENC_CHN VencChn, VI_CHN ViChn)
{
	MPP_CHN_S stSrcChn, stDestChn;
	HI_S32 s32Ret;
    s32Ret = HI_MPI_VENC_StopRecvPic(VencChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VENC_StopRecvPic s32Ret 0x%x",s32Ret);
        return s32Ret;
    }
	
    stSrcChn.enModId 		= HI_ID_VIU;
    stSrcChn.s32ChnId 	= ViChn;
    stDestChn.enModId 	= HI_ID_GROUP;
    stDestChn.s32ChnId 	= VencChn;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (HI_SUCCESS != s32Ret)
    {
        ptf_err("HI_MPI_SYS_UnBind s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VENC_UnRegisterChn(VencChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VENC_UnRegisterChn s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VENC_DestroyChn(VencChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VENC_DestroyChn s32Ret 0x%x",s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_MPI_VENC_DestroyGroup(VeGroup);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VENC_DestroyGroup s32Ret 0x%x",s32Ret);
        return s32Ret;
    }

	return HI_SUCCESS;
}


/*
* 功能描述	：	视频编码线程
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		
-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2015/08/11              V1.0	    
*/
HI_VOID* venc_get_stream_proc(HI_VOID *args)
{
	VENC_CTRL_INFO * psVencCtrl = (VENC_CTRL_INFO *)args;
	HI_S32  i = 0;
	HI_S32 maxfd 	= 0;
	HI_S32  s32Ret;

	struct timeval TimeoutVal;
	fd_set read_fds;

	VENC_CHN_STAT_S 	stStat;
	VENC_STREAM_S 		stStream;

	char* pack 	= (char*)AllocatorAlloc(g_psArpAllocator,1024 * 1024);
	HI_U32 VencFd = HI_MPI_VENC_GetFd(psVencCtrl->m_VencChn);
	if (VencFd < 0)
	{
		ptf_err("HI_MPI_VENC_GetFd VencFd 0x%x",VencFd);
		return NULL;
	}

	int count = 0;
	int vlen 	= 0;
	DataPakg *vpkg	= NULL;

	//printf("venc_OK\n");

	struct timeval sCurTime;
	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(VencFd, &read_fds);
		TimeoutVal.tv_sec 	= 2;
		TimeoutVal.tv_usec 	= 0;
		maxfd = VencFd;
		s32Ret = select(maxfd + 1, &read_fds, NULL, NULL, &TimeoutVal);
		if (s32Ret < 0)
		{
			ptf_err("select s32Ret 0x%x",s32Ret);
			break;
		}
		else if (s32Ret == 0)
		{
			ptf_dbg("time out psVencCtrl->m_VencChn %d",psVencCtrl->m_VencChn);
			//发送摄像头错误命令
			CMD_PARA cmdPara;
			cmdPara.m_eType = CMD_FAILURE_CAMERA;
						//cmdPara.m_eType = CMD_ELEC_ALARM_STOP_PUSH;
		    cmdPara.m_para1 	= psVencCtrl->m_ViDev;
						//ele_stop_snap_dfw = s_SampleCtrl.m_eDfw;
			tcpClientHandlePushCmd(&cmdPara);
			continue;
		}
		else
		{
			if (FD_ISSET(VencFd, &read_fds))
			{
				memset(&stStream, 0, sizeof(stStream));
				s32Ret = HI_MPI_VENC_Query(psVencCtrl->m_VencChn, &stStat);
				if (HI_SUCCESS != s32Ret)
				{
					ptf_err("HI_MPI_VENC_Query s32Ret 0x%x",s32Ret);
					break;
				}

				stStream.pstPack = (VENC_PACK_S*)(pack);
				stStream.u32PackCount = stStat.u32CurPacks;
				s32Ret = HI_MPI_VENC_GetStream(psVencCtrl->m_VencChn, &stStream, HI_TRUE);
				if (HI_SUCCESS != s32Ret)
				{
					ptf_err("HI_MPI_VENC_GetStream s32Ret 0x%x",s32Ret);
					break;
				}

				vlen = 0;
				for (i = 0; i < stStream.u32PackCount; i++)
				{
					vlen += stStream.pstPack[i].u32Len[0] + stStream.pstPack[i].u32Len[1];
				}

				vpkg = DataPkgCreate(g_psArpAllocator,vlen);
				if (NULL == vpkg)
				{
					ptf_err("DataPackageCreate err len %d",vlen);
					break;
				}

				PoolDataVideo *pVSInfo = (PoolDataVideo *)(vpkg->m_datainfo);
				pVSInfo->m_VFrameType = H264E_NALU_PSLICE;
				gettimeofday(&sCurTime, NULL);
				pVSInfo->m_PTS = sCurTime.tv_usec + sCurTime.tv_sec * 1000000;//stStream.pstPack[0].u64PTS;    //帧模式
				pVSInfo->m_VCount = count++;
				if (count > 65535)
				{
					count = 0;
				}
				for (i = 0; i < stStream.u32PackCount; i++)
				{
					if (H264E_NALU_PSLICE != stStream.pstPack[i].DataType.enH264EType)
					{
						pVSInfo->m_VFrameType = H264E_NALU_ISLICE;
					}
					if (stStream.pstPack[i].u32Len[0] > 0)
					{
						memcpy(vpkg->m_bData + vpkg->m_iDataSize, stStream.pstPack[i].pu8Addr[0], stStream.pstPack[i].u32Len[0]);
						vpkg->m_iDataSize += stStream.pstPack[i].u32Len[0];
					}
					if (stStream.pstPack[i].u32Len[1] > 0)
					{
						memcpy(vpkg->m_bData + vpkg->m_iDataSize, stStream.pstPack[i].pu8Addr[1], stStream.pstPack[i].u32Len[1]);
						vpkg->m_iDataSize += stStream.pstPack[i].u32Len[1];
					}
				}
				PushData(vpkg, psVencCtrl->m_DaceCenter);
				//ptf_dbg("psVencCtrl->m_ViDev %d vpkg->m_iDataSize %d count %d",psVencCtrl->m_ViDev,vpkg->m_iDataSize,count);

				DataPkgRelease(vpkg);
				s32Ret = HI_MPI_VENC_ReleaseStream(psVencCtrl->m_VencChn, &stStream);
				if (HI_SUCCESS != s32Ret)
				{
					ptf_err("HI_MPI_VENC_ReleaseStream s32Ret 0x%x",s32Ret);
					break;
				}
			}
		}

		usleep(1000);
	}

	AllocatorFree((void*)pack);
	//重启系统
	sys_mag_reboot();
	return NULL;
}

//全局函数


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
int venc_request_idr(int iLogicChn)
{
    HI_S32 s32Ret;
    VENC_CHN s32VChn = 0;

	if (iLogicChn <0 || iLogicChn > 1)
	{
		ptf_err("iLogicChn %d",iLogicChn);
		return -1;
	}
	
	s32VChn = g_sSensorVenc[iLogicChn].m_VencChn;
    s32Ret = HI_MPI_VENC_RequestIDR(s32VChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VENC_GetChnAttr s32Ret 0x%x",s32Ret);
        return HI_FAILURE;
    }
	
    return HI_SUCCESS;
}

/*
* 功能描述	：	抓拍线程
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		
-1	代表失败
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2015/08/11              V1.0	    
*/
HI_VOID* venc_get_snap_proc(HI_VOID *args)
{
	VENC_CTRL_INFO * pInfo = (VENC_CTRL_INFO *)args;
	struct timeval 		TimeoutVal;
	fd_set 		read_fds;
	HI_S32 	s32VencFd;
	VENC_CHN_STAT_S 	stStat;
	VENC_STREAM_S 		stStream;
	memset(&stStream, 0, sizeof(stStream));
	HI_S32 				s32Ret;
	int 		vlen 	= 0;
	HI_U32 	i 		= 0;
	DataPakg 	*vpkg = NULL;

	s32VencFd = HI_MPI_VENC_GetFd(pInfo->m_SnapExtVencChn);
	if (s32VencFd < 0)
	{
		ptf_err("HI_MPI_VENC_GetFd s32VencFd 0x%x",s32VencFd);
		return NULL;
	}

	stStream.pstPack = (VENC_PACK_S*)AllocatorAlloc(g_psArpAllocator,1024 * 1024);
	if (NULL == stStream.pstPack)
	{
		ptf_err("%s: malloc memory failed!\n", __FUNCTION__);
		return NULL;
	}

	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(s32VencFd, &read_fds);

		TimeoutVal.tv_sec  = 2;
		TimeoutVal.tv_usec = 0;
		s32Ret = select(s32VencFd+1, &read_fds, NULL, NULL, &TimeoutVal);
		if (s32Ret < 0)
		{
			ptf_err("%s: snap select failed!\n", __FUNCTION__);
		}
		else if (0 == s32Ret)
		{
			ptf_dbg("%s: snap time out!\n", __FUNCTION__);
		}
		else
		{
			if (FD_ISSET(s32VencFd, &read_fds))
			{
				s32Ret = HI_MPI_VENC_Query(pInfo->m_SnapExtVencChn, &stStat);
				if (s32Ret != HI_SUCCESS)
				{
					ptf_err("HI_MPI_VENC_Query s32Ret 0x%x",s32Ret);
					return NULL;
				}

				stStream.u32PackCount = stStat.u32CurPacks;
				s32Ret = HI_MPI_VENC_GetStream(pInfo->m_SnapExtVencChn, &stStream, HI_TRUE);
				if (HI_SUCCESS != s32Ret)
				{
					ptf_err("HI_MPI_VENC_GetStream s32Ret 0x%x",s32Ret);
					break;
				}

				vlen = 0;
				for (i = 0; i < stStream.u32PackCount; i++)
				{
					vlen += stStream.pstPack[i].u32Len[0] + stStream.pstPack[i].u32Len[1];
				}
				vpkg = DataPkgCreate(g_psArpAllocator,vlen);
				if (NULL == vpkg)
				{
					ptf_err("DataPackageCreate err len %d",vlen);
					break;
				}

				for (i = 0; i < stStream.u32PackCount; i++)
				{
					if (stStream.pstPack[i].u32Len[0] > 0)
					{
						memcpy(vpkg->m_bData + vpkg->m_iDataSize, stStream.pstPack[i].pu8Addr[0], stStream.pstPack[i].u32Len[0]);
						vpkg->m_iDataSize += stStream.pstPack[i].u32Len[0];
					}
					if (stStream.pstPack[i].u32Len[1] > 0)
					{
						memcpy(vpkg->m_bData + vpkg->m_iDataSize, stStream.pstPack[i].pu8Addr[1], stStream.pstPack[i].u32Len[1]);
						vpkg->m_iDataSize += stStream.pstPack[i].u32Len[1];
					}
				}

				//放入fifo
				if (pInfo->m_ViDev == 0)
					DataFifoPush(vpkg, s_pSnap0_FIFO);
				else
					DataFifoPush(vpkg, s_pSnap1_FIFO);
				//ptf_dbg("pInfo->m_ViDev %d vpkg->m_iDataSize %d",pInfo->m_ViDev,vpkg->m_iDataSize);
				DataPkgRelease(vpkg);
				s32Ret = HI_MPI_VENC_ReleaseStream(pInfo->m_SnapExtVencChn, &stStream);
				if (HI_SUCCESS != s32Ret)
				{
					DataPkgRelease((void*)vpkg);
					ptf_err("HI_MPI_VENC_ReleaseStream s32Ret 0x%x",s32Ret);
					break;
				}
				
			}
		}
	}
	sys_mag_reboot();

	return NULL;
}
/*
* 功能描述	：	获取抓拍图片
* 输入参数	：	Venc	编码逻辑通道
* 输出参数	：
* 返 回 值	：		失败返回空指针
* 其它说明	：
* 修改日期 
* -----------------------------------------------
* 2013/04/25     V1.0	    
*/
DataPakg* venc_get_snap_picture(int  logicChn)
{

	if (logicChn <0 || logicChn > 1)
	{
		ptf_err("logicChn %d",logicChn);
		return NULL;
	}
	//logicChn=0;  //2017_07_31  双改单添加
	DataPakg* pkg;
	if (logicChn == 0)
	{
		DataFifoClear(s_pSnap0_FIFO);
		while (1)
		{
			pkg = DataFifoGet(s_pSnap0_FIFO);
			if (pkg != NULL)
			{
				break;
			}
		}
	}
	else
	{
		DataFifoClear(s_pSnap1_FIFO);
		while (1)
		{
			pkg = DataFifoGet(s_pSnap1_FIFO);
			if (pkg != NULL)
			{
				break;
			}
		}
	}
	return pkg;
}



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
HI_S32 venc_init(void)
{
	//ptf_dbg("---------------------");
	HI_S32 		s32Ret;
	s32Ret = hi_mpp_vb_uninit();
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}else
	{
		//printf("uinit OK\n");
	}
	
	//ptf_dbg("---------------------");
	s32Ret = hi_mpp_vb_init();
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("s32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}else
	{
		//printf("init OK\n");
	}
	

	//ptf_dbg("---------------------");


	s_pSnap0_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 1);
	s_pSnap1_FIFO 		= DataFifoConstruct(FIFO_PRI_CYC, 1);
	//ptf_dbg("---------------------");

	VENC_CTRL_INFO *pInfo;

	int num;
	for (num = 0; num < 2; num++)
	{
		pInfo = &(g_sSensorVenc[num]);
		vi_start_dev(pInfo->m_ViDev, pInfo->m_Sensor);
		vi_start_base_chn(pInfo->m_ViDev, pInfo->m_ViChn, pInfo->m_Sensor);
		venc_chn_creat_and_start(pInfo->m_VencGrp, pInfo->m_VencChn, pInfo->m_ViChn);

		ThreadPoolAddWorkUnlimit((void*)g_psThreadPool,venc_get_stream_proc,(HI_VOID*)pInfo);
		vi_start_snap_ext_chn(pInfo->m_ViChn, pInfo->m_SnapExtViChn);
	   	venc_snap_chn_start(pInfo->m_SnapExtVencGrp, pInfo->m_SnapExtVencChn, pInfo->m_SnapExtViChn);
		
		ThreadPoolAddWorkUnlimit((void*)g_psThreadPool,venc_get_snap_proc,(HI_VOID*)pInfo);
	}
	return HI_SUCCESS;
}

