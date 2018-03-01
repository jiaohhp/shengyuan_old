//��׼ͷ�ļ���������
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//�Զ���ͷ�ļ���������
#include "printf_func.h"
#include "chn_vio.h"


//����������

//�ļ��ڲ�ʹ�õĺ�

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����

//�ڲ�����

//ȫ�ֺ���

/*
* ��������	��	������Ƶ�ɼ��豸
* ��Ա����	:		��
* �������	�� ViDev 		��Ƶ�ɼ��豸���
										enViDevType	��Ƶ�ɼ���ѡ�õ���Ƶ����ʱ��
* �������	�� ��
* �� �� ֵ	��		0	 ����ɹ�
										����ֵ����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_dev(VI_DEV ViDev, SAMPLE_VI_DEV_TYPE_E enViDevType)
{
	HI_S32 s32Ret;
	VI_DEV_ATTR_S    stViDevAttr;

	switch (enViDevType)
	{
		case OV_9712_DC_1280_800P:
		stViDevAttr.enInputMode 							= VI_MODE_DIGITAL_CAMERA; 	/* vi input mode */
		stViDevAttr.enCombineMode 					= VI_COMBINE_SEPARATE; 			/* combine mode */
		stViDevAttr.enCompMode 							= VI_COMP_MODE_SINGLE; 			/* comp mode */
		stViDevAttr.au32CompMask[0] 					= 0xFFC;  										/* comp mask */
		stViDevAttr.au32CompMask[1] 					= 0x0;  											/* comp mask */
		stViDevAttr.enClkEdge 								= VI_CLK_EDGE_SINGLE_UP; 		/* clock edge define */
		stViDevAttr.enScanMode 							= VI_SCAN_PROGRESSIVE;			/* input scan mode */
		stViDevAttr.enDataType 							= VI_DATA_TYPE_RGB;    				/* Date type (RGB or YUV */
		stViDevAttr.unDataSeq.enRgbSeq 			= VI_DATA_SEQ_GRGR;				/* input data seq (Bayer patten) */
		stViDevAttr.bUseInterISP 							= HI_FALSE;		 							/* inside isp or not */

		/* syn timing cfg. BT601/DC interface must config! */
		stViDevAttr.stSynCfg.enVsync 					= VI_VSYNC_PULSE;
		stViDevAttr.stSynCfg.enVsyncNeg 			= VI_VSYNC_NEG_HIGH;
		stViDevAttr.stSynCfg.enHsync 					= VI_HSYNC_VALID_SINGNAL;
		stViDevAttr.stSynCfg.enHsyncNeg 			= VI_HSYNC_NEG_HIGH;
		stViDevAttr.stSynCfg.enVsyncValid 			= VI_VSYNC_NORM_PULSE;
		stViDevAttr.stSynCfg.enVsyncValidNeg 	= VI_VSYNC_VALID_NEG_HIGH;
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncHfb 	= 0; 		/* horizontal begin blank width */
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncAct 	= 1280; 	/* horizontal valid width */
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncHbb 	= 0; 		/* horizontal end blank width */
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncOfb 	= 0; 		/* frame or interleaved odd picture's vertical begin blanking heigh */
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncOAct	= 800; 	/* frame or interleaved odd picture's vertical valid blanking width */
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncObb 	= 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEfb 	= 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEAct = 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEbb 	= 0;

		stViDevAttr.stBT656SynCfg.enFixCode 	= BT656_FIXCODE_1;
		stViDevAttr.stBT656SynCfg.enFieldNeg 	= BT656_FIELD_NEG_STD;
		break;
		case OV_2640_DC_800_600P:
		stViDevAttr.enInputMode 						= VI_MODE_DIGITAL_CAMERA; 	/* vi input mode */
		stViDevAttr.enCombineMode 				= VI_COMBINE_COMPOSITE;		/* combine mode */
		stViDevAttr.enCompMode 						= VI_COMP_MODE_SINGLE; 			/* comp mode */
		stViDevAttr.au32CompMask[0] 				= 0xFF00; 										/* comp mask */
		stViDevAttr.au32CompMask[1] 				= 0x0;  											/* comp mask */
		stViDevAttr.enClkEdge 							= VI_CLK_EDGE_SINGLE_UP; 		/* clock edge define */
		stViDevAttr.enScanMode 						= VI_SCAN_PROGRESSIVE;			/* input scan mode */
		stViDevAttr.enDataType 						= VI_DATA_TYPE_YUV;    				/* Date type (RGB or YUV */
		stViDevAttr.unDataSeq.enRgbSeq 		= VI_INPUT_DATA_YUYV;				/* input data seq (Bayer patten) */
		stViDevAttr.bUseInterISP 						= HI_FALSE;		 							/* inside isp or not */

		/* syn timing cfg. BT601/DC interface must config! */
		stViDevAttr.stSynCfg.enVsync 				= VI_VSYNC_PULSE;
		stViDevAttr.stSynCfg.enVsyncNeg 		= VI_VSYNC_NEG_HIGH;
		stViDevAttr.stSynCfg.enHsync 				= VI_HSYNC_VALID_SINGNAL;
		stViDevAttr.stSynCfg.enHsyncNeg 		= VI_HSYNC_NEG_HIGH;
		stViDevAttr.stSynCfg.enVsyncValid 		= VI_VSYNC_NORM_PULSE;
		stViDevAttr.stSynCfg.enVsyncValidNeg= VI_VSYNC_VALID_NEG_HIGH;
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncHfb 	= 0; 		/* horizontal begin blank width */
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncAct 	= 800; 	/* horizontal valid width */
		//stViDevAttr.stSynCfg.stTimingBlank.u32HsyncAct 	= 1600;
		stViDevAttr.stSynCfg.stTimingBlank.u32HsyncHbb 	= 0; 		/* horizontal end blank width */
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncOfb 	= 0; 		/* frame or interleaved odd picture's vertical begin blanking heigh */
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncOAct	= 600; 	/* frame or interleaved odd picture's vertical valid blanking width */
		//stViDevAttr.stSynCfg.stTimingBlank.u32VsyncOAct	= 1200;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncObb 	= 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEfb 	= 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEAct 	= 0;
		stViDevAttr.stSynCfg.stTimingBlank.u32VsyncEbb 	= 0;

		stViDevAttr.stBT656SynCfg.enFixCode 			= BT656_FIXCODE_1;
		stViDevAttr.stBT656SynCfg.enFieldNeg 			= BT656_FIELD_NEG_STD;
		break;
		default:
		ptf_err("vi input type[%d] is invalid!\n", enViDevType);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VI_SetDevAttr(ViDev, &stViDevAttr);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("%s: HI_MPI_VI_SetDevAttr failed with %#x! ViDev %d",__FUNCTION__,  s32Ret,ViDev);
		return HI_FAILURE;
	}

	s32Ret = HI_MPI_VI_EnableDev(ViDev);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("%s: HI_MPI_VI_EnableDev failed with %#x! ViDev %d",__FUNCTION__,  s32Ret,ViDev);
		return HI_FAILURE;
	}

#if 1
	VI_CSC_ATTR_S s_ViDevCsc;
	switch (enViDevType)
		{
			case OV_9712_DC_1280_800P:
			//VI_CSC_ATTR_S s_ViDevCsc;
			s_ViDevCsc.u32LumaVal 	= 50;	  //���ȵ���50
			s_ViDevCsc.u32HueVal		= 50; //ɫ������50
			s_ViDevCsc.u32ContrVal	= 70;     //�Աȶȵ���50
			s_ViDevCsc.u32SatuVal		= 50;//0Ϊ�ڰ�ͼ��
			s_ViDevCsc.enViCscType 	= VI_CSC_TYPE_709;
			s32Ret =  HI_MPI_VI_SetCscAttr(ViDev, 0, &s_ViDevCsc);
			if (s32Ret != HI_SUCCESS)
			{
				ptf_err(" HI_MPI_VI_SetCscAttr err 0x%x",__FUNCTION__, s32Ret);
				return s32Ret;
			}
			break;
			case OV_2640_DC_800_600P:
			//VI_CSC_ATTR_S s_ViDevCsc;
			s_ViDevCsc.u32LumaVal 	= 50;		//���ȵ���50
			s_ViDevCsc.u32HueVal		= 50;	//ɫ������50
			s_ViDevCsc.u32ContrVal	= 40;		//�Աȶȵ���50
			s_ViDevCsc.u32SatuVal		= 50;//0Ϊ�ڰ�ͼ��
			s_ViDevCsc.enViCscType 	= VI_CSC_TYPE_709;
			s32Ret =  HI_MPI_VI_SetCscAttr(ViDev, 0, &s_ViDevCsc);
			if (s32Ret != HI_SUCCESS)
			{
				ptf_err(" HI_MPI_VI_SetCscAttr err 0x%x",__FUNCTION__, s32Ret);
				return s32Ret;
			}
			break;
			default:
				return HI_FAILURE;
		}
	//VI_CSC_ATTR_S s_ViDevCsc;
	//s_ViDevCsc.u32LumaVal 	= 50;
	//s_ViDevCsc.u32HueVal		= 50;
	//s_ViDevCsc.u32ContrVal	= 50;
	//s_ViDevCsc.u32SatuVal		= 0;//0Ϊ�ڰ�ͼ��
	//s_ViDevCsc.enViCscType 	= VI_CSC_TYPE_709;
	//s32Ret =  HI_MPI_VI_SetCscAttr(ViDev, 0, &s_ViDevCsc);
	//if (s32Ret != HI_SUCCESS)
	//{
	//	ptf_err(" HI_MPI_VI_SetCscAttr err 0x%x",__FUNCTION__, s32Ret);
	//	return s32Ret;
	//}
#endif
	//printf("end\n");
	return HI_SUCCESS;
}

/*
* ��������	��	ֹͣ��Ƶ�ɼ��豸
* ��Ա����	:		��
* �������	�� 	ViDev ��Ƶ�ɼ��豸���
* �������	�� ��
* �� �� ֵ	��		0		����ɹ�
										-1	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_dev(VI_DEV ViDev)
{
	HI_S32 s32Ret = HI_MPI_VI_DisableDev(ViDev);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("HI_MPI_VI_DisableDevs32Ret 0x%x",s32Ret);
		return HI_FAILURE;
	}
	return HI_SUCCESS;
}

/*
* ��������	��	������Ƶ�ɼ�ͨ��
* ��Ա����	:		��
* �������	�� 	ViDev 		��Ƶ�ɼ��豸���
										ViChn 		��Ƶ�ɼ��豸��ͨ���Ĳɼ�ͨ����
										enViDevType	��Ƶ�ɼ��豸����
* �������	�� ��
* �� �� ֵ	��		0		����ɹ�
										-1	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_base_chn(VI_DEV ViDev, VI_CHN ViChn, SAMPLE_VI_DEV_TYPE_E enViDevType)
{
	HI_S32 s32Ret;
	int capWidth,capHeight;
	//code by tqk
	int capRect_X,capRect_Y;
	VI_CHN_ATTR_S stChnAttr;
	VI_CHN_BIND_ATTR_S stChnBindAttr;

	switch(enViDevType)
	{
		case OV_9712_DC_1280_800P:																																										
			capRect_X	= 290;
			capRect_Y	= 50;
			capWidth 	= 700;//1280;
			capHeight 	= 700;//800;
		break;
		case OV_2640_DC_800_600P:
			
			capRect_X	= 160;
			capRect_Y   = 60;
			capWidth 	= 480;//800;
			capHeight 	= 480;//600;
			
			//capRect_X	= 320;
			//capRect_Y   = 120;	
			//capWidth 	= 960;//800;
			//capHeight 	= 960;//600;
		break;

		default:
		ptf_err("%s: get capture picture size[%d] failed!\n",__FUNCTION__, enViDevType);
		return HI_FAILURE;
	}

	/* step  5: config & start vicap dev */
	/*code by tqk*/
	stChnAttr.stCapRect.s32X 			= capRect_X;
	stChnAttr.stCapRect.s32Y 			= capRect_Y;
	stChnAttr.stCapRect.u32Width 	= capWidth;
	stChnAttr.stCapRect.u32Height 	= capHeight;
	/*code by tqk*/
	stChnAttr.enCapSel 						= VI_CAPSEL_BOTH;
	
	/* to show scale. this is a sample only, we want to show dist_size = D1 onley */
	stChnAttr.stDestSize.u32Width 		= BH_DEST_WIDTH;	//�̶�Ϊ480
	stChnAttr.stDestSize.u32Height 	= BH_DEST_HEIGHT;	//�̶�Ϊ480
	stChnAttr.enPixFormat 					= PIXEL_FORMAT_YUV_SEMIPLANAR_422;   /* sp420 or sp422 */
	stChnAttr.bMirror 	= HI_FALSE;
	stChnAttr.bFilp 		= HI_FALSE;

	/* we only use phy chn, and phychnid=vichn */
	stChnBindAttr.enBindType	 			= VI_CHN_BIND_PHYCHN;
	stChnBindAttr.unBindAttr.stBindPhyChn.s32PhyChn 	= ViChn;
	stChnBindAttr.unBindAttr.stBindPhyChn.ViDev 			= ViDev;
	stChnBindAttr.unBindAttr.stBindPhyChn.ViWay 			= 0;
	//��vi����ͨ��
	s32Ret = HI_MPI_VI_ChnBind(ViChn, &stChnBindAttr);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("HI_MPI_VI_ChnBind s32Ret 0x%x ViDev %d ViChn %d",s32Ret,ViDev,ViChn);
		return HI_FAILURE;
	}
	#if 1
	//����viͨ������
	s32Ret = HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("VI_Start_BaseChn u32Width %d u32Height %d ViChn %d s32Ret 0x%x", stChnAttr.stCapRect.u32Width, 
			stChnAttr.stCapRect.u32Height,ViChn,s32Ret);
		return HI_FAILURE;
	}	
	#endif
	//ʹ��vi ͨ��
	s32Ret = HI_MPI_VI_EnableChn(ViChn);
	if (s32Ret != HI_SUCCESS)
	{
		ptf_err("HI_MPI_VI_EnableChn s32Ret 0x%x ViDev %d ViChn %d",s32Ret,ViDev,ViChn);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

/*
* ��������	��	ֹͣ��Ƶ�ɼ�ͨ��
* ��Ա����	:		��
* �������	�� 	ViChn 		��Ƶ�ɼ��豸��ͨ���Ĳɼ�ͨ����
* �������	�� 	��
* �� �� ֵ	��		0	����ɹ�
										-1	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_base_chn(VI_CHN ViChn)
{
    HI_S32 s32Ret;
    s32Ret = HI_MPI_VI_DisableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_SYS_Bind s32Ret 0x%x",s32Ret);
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_VI_ChnUnBind(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_SYS_Bind s32Ret 0x%x",s32Ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/*
* ��������	��	����ץ��ͨ��
* ��Ա����	:		��
* �������	�� 	ViBaseChn 		��Ƶ�ɼ�baseͨ��
										ViExtChn			��Ƶ�ɼ���չͨ��
* �������	�� ��
* �� �� ֵ	��		0		����ɹ�
										-1	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_start_snap_ext_chn(VI_CHN ViBaseChn, VI_CHN ViExtChn)
{
    VI_CHN_BIND_ATTR_S stChnBindAttr;
    HI_S32 s32Ret;

    stChnBindAttr.enBindType 	= VI_CHN_BIND_VICHN;
    stChnBindAttr.unBindAttr.stBindViChn.ViChn 	= ViBaseChn;

    s32Ret = HI_MPI_VI_ChnBind(ViExtChn, &stChnBindAttr);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VI_ChnBind ret 0x%x ",s32Ret);
        return HI_FAILURE;
    }
    s32Ret = HI_MPI_VI_EnableChn(ViExtChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VI_EnableChn ret 0x%x ",s32Ret);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*
* ��������	��	ֹͣץ��ͨ��
* ��Ա����	:		��
* �������	�� 	ViExtChn	��Ƶ�ɼ���չͨ��
* �������	�� ��
* �� �� ֵ	��		0		����ɹ�
										-1	����ʧ��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09		 V1.0		XXXX		  XXXX
*/
HI_S32 vi_stop_snap_ext_chn(VI_CHN ViExtChn)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VI_DisableChn(ViExtChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VI_DisableChn ret 0x%x ",s32Ret);

        return HI_FAILURE;
    }
	
    s32Ret = HI_MPI_VI_ChnUnBind(ViExtChn);
    if (s32Ret != HI_SUCCESS)
    {
        ptf_err("HI_MPI_VI_ChnUnBind ret 0x%x ",s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

