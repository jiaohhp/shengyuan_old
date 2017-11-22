#ifndef __MPI_VI_H__
#define __MPI_VI_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include "hi_comm_vi.h"

HI_S32 HI_MPI_VI_SetDevAttr(VI_DEV ViDev, const VI_DEV_ATTR_S *pstDevAttr);
HI_S32 HI_MPI_VI_GetDevAttr(VI_DEV ViDev, VI_DEV_ATTR_S *pstDevAttr);
HI_S32 HI_MPI_VI_SetDevExAttr(VI_DEV  ViDev, const VI_DEV_EX_ATTR_S *pstDevExAttr);

HI_S32 HI_MPI_VI_SetCscAttr(VI_DEV ViDev, VI_WAY ViWay, VI_CSC_ATTR_S *pstCscAttr);
HI_S32 HI_MPI_VI_GetCscAttr(VI_DEV ViDev, VI_WAY ViWay, VI_CSC_ATTR_S *pstCscAttr);

HI_S32 HI_MPI_VI_EnableDev(VI_DEV ViDev);

HI_S32 HI_MPI_VI_DisableDev(VI_DEV ViDev);

HI_S32 HI_MPI_VI_ChnBind(VI_CHN ViChn, const VI_CHN_BIND_ATTR_S *pstChnBindAttr);
HI_S32 HI_MPI_VI_ChnUnBind(VI_CHN ViChn);
HI_S32 HI_MPI_VI_GetChnBind(VI_CHN ViChn, VI_CHN_BIND_ATTR_S *pstChnBindAttr);

HI_S32 HI_MPI_VI_SetChnAttr(VI_CHN ViChn, const VI_CHN_ATTR_S *pstAttr);
HI_S32 HI_MPI_VI_GetChnAttr(VI_CHN ViChn, VI_CHN_ATTR_S *pstAttr);

HI_S32 HI_MPI_VI_EnableChn(VI_CHN ViChn);
HI_S32 HI_MPI_VI_DisableChn(VI_CHN ViChn);

HI_S32 HI_MPI_VI_GetFrame(VI_CHN ViChn, VI_FRAME_INFO_S *pstFrameInfo);
HI_S32 HI_MPI_VI_ReleaseFrame(VI_CHN ViChn, VI_FRAME_INFO_S *pstFrameInfo);
HI_S32 HI_MPI_VI_SetFrameDepth(VI_CHN ViChn, HI_U32 u32Depth);

HI_S32 HI_MPI_VI_SetSrcFrameRate(VI_CHN ViChn, HI_U32 u32ViFramerate);
HI_S32 HI_MPI_VI_GetSrcFrameRate(VI_CHN ViChn, HI_U32 *pu32ViFramerate);
HI_S32 HI_MPI_VI_SetFrameRate(VI_CHN ViChn, HI_U32 u32ViFramerate);
HI_S32 HI_MPI_VI_GetFrameRate(VI_CHN ViChn, HI_U32 *pu32ViFramerate);

HI_S32 HI_MPI_VI_SetVppCfg(VI_CHN ViChn, const VI_VPP_CFG_S *pstVppCfg);
HI_S32 HI_MPI_VI_GetVppCfg(VI_CHN ViChn, VI_VPP_CFG_S *pstVppCfg);
HI_S32 HI_MPI_VI_SetVppCfgEx(VI_CHN ViChn, const VPP_CFG_EX_S *pstVppCfgEx);
HI_S32 HI_MPI_VI_GetVppCfgEx(VI_CHN ViChn, VPP_CFG_EX_S *pstVppCfgEx);

HI_S32 HI_MPI_VI_Query(VI_CHN ViChn, VI_CHN_STAT_S *pstStat);
HI_S32 HI_MPI_VI_GetFd(VI_CHN ViChn);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__MPI_VI_H__ */

