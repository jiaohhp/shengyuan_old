/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_vo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/01/12
  Description   : Vou API
  History       :
  1.Date        : 2009/01/21
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __MPI_VO_H__
#define __MPI_VO_H__

#include "hi_comm_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


HI_S32 HI_MPI_VO_SetPubAttr(VO_DEV VoDev, const VO_PUB_ATTR_S *pstPubAttr);
HI_S32 HI_MPI_VO_GetPubAttr(VO_DEV VoDev, VO_PUB_ATTR_S *pstPubAttr);

HI_S32 HI_MPI_VO_Enable (VO_DEV VoDev);
HI_S32 HI_MPI_VO_Disable(VO_DEV VoDev);

HI_S32 HI_MPI_VO_SetVideoLayerAttr(VO_DEV VoDev, const VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);
HI_S32 HI_MPI_VO_GetVideoLayerAttr(VO_DEV VoDev, VO_VIDEO_LAYER_ATTR_S *pstLayerAttr);

HI_S32 HI_MPI_VO_EnableVideoLayer (VO_DEV VoDev);
HI_S32 HI_MPI_VO_DisableVideoLayer(VO_DEV VoDev);

HI_S32 HI_MPI_VO_SetChnAttr(VO_DEV VoDev, VO_CHN VoChn, const VO_CHN_ATTR_S *pstChnAttr);
HI_S32 HI_MPI_VO_GetChnAttr(VO_DEV VoDev, VO_CHN VoChn, VO_CHN_ATTR_S *pstChnAttr);

HI_S32 HI_MPI_VO_EnableChn (VO_DEV VoDev, VO_CHN VoChn);
HI_S32 HI_MPI_VO_DisableChn(VO_DEV VoDev, VO_CHN VoChn);


HI_S32 HI_MPI_VO_SetChnField(VO_DEV VoDev, VO_CHN VoChn, const VO_DISPLAY_FIELD_E enField);
HI_S32 HI_MPI_VO_GetChnField(VO_DEV VoDev, VO_CHN VoChn, VO_DISPLAY_FIELD_E *pField);

HI_S32 HI_MPI_VO_GetChnFrame (VO_DEV VoDev, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame);
HI_S32 HI_MPI_VO_ReleaseChnFrame(VO_DEV VoDev, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstFrame);

HI_S32 HI_MPI_VO_SetZoomInWindow(VO_DEV VoDev, VO_CHN VoChn, const VO_ZOOM_ATTR_S *pstZoomAttr);
HI_S32 HI_MPI_VO_GetZoomInWindow(VO_DEV VoDev, VO_CHN VoChn, VO_ZOOM_ATTR_S *pstZoomAttr);

HI_S32 HI_MPI_VO_SetZoomInRatio(VO_DEV VoDev, VO_CHN VoChn, const VO_ZOOM_RATIO_S *pstZoomRatio);
HI_S32 HI_MPI_VO_GetZoomInRatio(VO_DEV VoDev, VO_CHN VoChn, VO_ZOOM_RATIO_S *pstZoomRatio);

HI_S32 HI_MPI_VO_SendFrame(VO_DEV VoDev, VO_CHN VoChn, VIDEO_FRAME_INFO_S *pstVFrame);

HI_S32 HI_MPI_VO_GetScreenFrame (VO_DEV VoDev, VIDEO_FRAME_INFO_S *pstVFrame);
HI_S32 HI_MPI_VO_ReleaseScreenFrame(VO_DEV VoDev, VIDEO_FRAME_INFO_S *pstVFrame);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MPI_VO_H__ */


