/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_venc.h
  Version       : Initial Draft
  Author        : Hisilicon Hi3511 MPP Team
  Created       : 2006/11/22
  Last Modified :
  Description   : mpi functions declaration
  Function List :
  History       :
  1.Date        : 2006/11/22
    Author      : z50929
    Modification: Create
  2.Date        : 2007/11/22
    Author      : z50825
    Modification: Create
  3.Date        : 2008/7/18
    Author      : l64467
    Modification: Add interface for CR20080716031
    			  HI_MPI_VENC_SetMaxStreamCnt;
				  HI_MPI_VENC_GetMaxStreamCnt;
******************************************************************************/
#ifndef __MPI_VENC_H__
#define __MPI_VENC_H__

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_venc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


HI_S32 HI_MPI_VENC_CreateGroup(VENC_GRP VeGroup);
HI_S32 HI_MPI_VENC_DestroyGroup(VENC_GRP VeGroup);

HI_S32 HI_MPI_VENC_CreateChn(VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr, const VENC_WM_ATTR_S *pstWm);
HI_S32 HI_MPI_VENC_DestroyChn(VENC_CHN VeChn);

HI_S32 HI_MPI_VENC_RegisterChn(VENC_GRP VeGroup,VENC_CHN VeChn );
HI_S32 HI_MPI_VENC_UnRegisterChn(VENC_CHN VeChn);

HI_S32 HI_MPI_VENC_StartRecvPic(VENC_CHN VeChn);
HI_S32 HI_MPI_VENC_StopRecvPic(VENC_CHN VeChn);
HI_S32 HI_MPI_VENC_Query(VENC_CHN VeChn,VENC_CHN_STAT_S *pstStat);

HI_S32 HI_MPI_VENC_GetStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream, HI_BOOL bBlockFlag);
HI_S32 HI_MPI_VENC_ReleaseStream(VENC_CHN VeChn, VENC_STREAM_S *pstStream);

HI_S32 HI_MPI_VENC_InsertUserData(VENC_CHN VeChn, HI_U8 *pu8Data, HI_U32 u32Len);

HI_S32 HI_MPI_VENC_SendFrame(VENC_GRP VeGrp, VIDEO_FRAME_INFO_S *pstFrame);

HI_S32 HI_MPI_VENC_GetFd(VENC_CHN VeChn);
HI_S32 HI_MPI_GRP_GetFd(VENC_CHN VeChn);

HI_S32 HI_MPI_VENC_SetChnAttr( VENC_CHN VeChn, const VENC_CHN_ATTR_S *pstAttr);
HI_S32 HI_MPI_VENC_GetChnAttr( VENC_CHN VeChn, VENC_CHN_ATTR_S *pstAttr);

HI_S32 HI_MPI_VENC_SetMaxStreamCnt(VENC_CHN VeChn,HI_U32 u32MaxStrmCnt);
HI_S32 HI_MPI_VENC_GetMaxStreamCnt(VENC_CHN VeChn,HI_U32 *pu32MaxStrmCnt);

HI_S32 HI_MPI_VENC_SetH264SliceSplit(VENC_CHN VeChn, const VENC_PARAM_H264_SLICE_SPLIT_S *pstSliceSplit);
HI_S32 HI_MPI_VENC_GetH264SliceSplit(VENC_CHN VeChn, VENC_PARAM_H264_SLICE_SPLIT_S *pstSliceSplit);

HI_S32 HI_MPI_VENC_SetH264InterPred(VENC_CHN VeChn, const VENC_PARAM_H264_INTER_PRED_S *pstH264InterPred);
HI_S32 HI_MPI_VENC_GetH264InterPred(VENC_CHN VeChn, VENC_PARAM_H264_INTER_PRED_S *pstH264InterPred);

HI_S32 HI_MPI_VENC_SetH264IntraPred(VENC_CHN VeChn, const VENC_PARAM_H264_INTRA_PRED_S *pstH264InterPred);
HI_S32 HI_MPI_VENC_GetH264IntraPred(VENC_CHN VeChn, VENC_PARAM_H264_INTRA_PRED_S *pstH264InterPred);

HI_S32 HI_MPI_VENC_SetH264Trans(VENC_CHN VeChn, const VENC_PARAM_H264_TRANS_S *pstH264Trans);
HI_S32 HI_MPI_VENC_GetH264Trans(VENC_CHN VeChn, VENC_PARAM_H264_TRANS_S *pstH264Trans);

HI_S32 HI_MPI_VENC_SetH264Entropy(VENC_CHN VeChn, const VENC_PARAM_H264_ENTROPY_S *pstH264EntropyEnc);
HI_S32 HI_MPI_VENC_GetH264Entropy(VENC_CHN VeChn, VENC_PARAM_H264_ENTROPY_S *pstH264EntropyEnc);

HI_S32 HI_MPI_VENC_SetH264Poc(VENC_CHN VeChn, const VENC_PARAM_H264_POC_S *pstH264Dpb);
HI_S32 HI_MPI_VENC_GetH264Poc(VENC_CHN VeChn, VENC_PARAM_H264_POC_S *pstH264Dpb);

HI_S32 HI_MPI_VENC_SetH264Dblk(VENC_CHN VeChn, const VENC_PARAM_H264_DBLK_S *pstH264Dblk);
HI_S32 HI_MPI_VENC_GetH264Dblk(VENC_CHN VeChn, VENC_PARAM_H264_DBLK_S *pstH264Dblk);

HI_S32 HI_MPI_VENC_SetH264Vui(VENC_CHN VeChn, const VENC_PARAM_H264_VUI_S *pstH264Vui);
HI_S32 HI_MPI_VENC_GetH264Vui(VENC_CHN VeChn, VENC_PARAM_H264_VUI_S *pstH264Vui);

HI_S32 HI_MPI_VENC_SetJpegParam(VENC_CHN VeChn, const VENC_PARAM_JPEG_S *pstJpegParam);
HI_S32 HI_MPI_VENC_GetJpegParam(VENC_CHN VeChn, VENC_PARAM_JPEG_S *pstJpegParam);

HI_S32 HI_MPI_VENC_SetMjpegParam(VENC_CHN VeChn, const VENC_PARAM_MJPEG_S *pstMjpegParam);
HI_S32 HI_MPI_VENC_GetMjpegParam(VENC_CHN VeChn, VENC_PARAM_MJPEG_S *pstMjpegParam);

HI_S32 HI_MPI_VENC_SetMpeg4Param(VENC_CHN VeChn, const VENC_PARAM_MPEG4_S *pstMpeg4Param);
HI_S32 HI_MPI_VENC_GetMpeg4Param(VENC_CHN VeChn, VENC_PARAM_MPEG4_S *pstMpeg4Param);

HI_S32 HI_MPI_VENC_GetVppCfg(VENC_GRP VeGroup, GROUP_VPP_CFG_S *pstGrpVppCfg);
HI_S32 HI_MPI_VENC_SetVppCfg(VENC_GRP VeGroup, const GROUP_VPP_CFG_S *pstGrpVppCfg);

HI_S32 HI_MPI_VENC_SetVppCfgEx(VENC_GRP VeGroup, VPP_CFG_EX_S *pstVppCfgEx);
HI_S32 HI_MPI_VENC_GetVppCfgEx(VENC_GRP VeGroup, VPP_CFG_EX_S *pstVppCfgEx);

HI_S32 HI_MPI_VENC_SetRoiCfg(VENC_CHN VeChn, VENC_ROI_CFG_S *pstVencRoiCfg);
HI_S32 HI_MPI_VENC_GetRoiCfg(VENC_CHN VeChn, HI_U32 u32Index, VENC_ROI_CFG_S *pstVencRoiCfg);

HI_S32 HI_MPI_VENC_RequestIDR(VENC_CHN VeChn);

HI_S32 HI_MPI_VENC_GetRcPara(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcPara);
HI_S32 HI_MPI_VENC_SetRcPara(VENC_CHN VeChn, VENC_RC_PARAM_S *pstRcPara);

HI_S32 HI_MPI_VENC_SetH264eRefMode( VENC_CHN VeChn, VENC_ATTR_H264_REF_MODE_E enRefMode );
HI_S32 HI_MPI_VENC_GetH264eRefMode( VENC_CHN VeChn, VENC_ATTR_H264_REF_MODE_E* penRefMode );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_VENC_H__ */
