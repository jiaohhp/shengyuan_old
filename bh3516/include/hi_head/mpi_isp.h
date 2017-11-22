/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/12/20
  Description   : 
  History       :
  1.Date        : 2010/12/20
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __MPI_ISP_H__
#define __MPI_ISP_H__

#include "hi_comm_isp.h"
#include "hi_comm_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


HI_S32 HI_MPI_ISP_FreezeFmw(HI_BOOL bFreeze);

HI_S32 HI_MPI_ISP_SetModuleControl(HI_U32 u32ModFlag);
HI_S32 HI_MPI_ISP_GetModuleControl(HI_U32 *pu32ModFlag);

HI_S32 HI_MPI_ISP_GetExtRegAddr(HI_U32 *u32Addr);


/* Exposure Settings */

HI_S32 HI_MPI_ISP_SetExposureType(ISP_OP_TYPE_E enExpType);
HI_S32 HI_MPI_ISP_GetExposureType(ISP_OP_TYPE_E *penExpType);

HI_S32 HI_MPI_ISP_SetAEAttr(const ISP_AE_ATTR_S *pstAEAttr);
HI_S32 HI_MPI_ISP_GetAEAttr(ISP_AE_ATTR_S *pstAEAttr);

HI_S32  HI_MPI_ISP_SetExpStaInfo(ISP_EXP_STA_INFO_S *pstExpStatistic);
HI_S32  HI_MPI_ISP_GetExpStaInfo(ISP_EXP_STA_INFO_S *pstExpStatistic);
HI_S32 HI_MPI_ISP_SetMEAttr(const ISP_ME_ATTR_S *pstMEAttr);
HI_S32 HI_MPI_ISP_GetMEAttr(ISP_ME_ATTR_S *pstMEAttr);

HI_S32 HI_MPI_ISP_SetSlowFrameRate(HI_U8 u8Value);
HI_S32 HI_MPI_ISP_GetSlowFrameRate(HI_U8 *pu8Value);

/* White Balance Settings */

HI_S32 HI_MPI_ISP_SetWBType(ISP_OP_TYPE_E enWBType);
HI_S32 HI_MPI_ISP_GetWBType(ISP_OP_TYPE_E *penWBType);

HI_S32 HI_MPI_ISP_SetAWBAttr(const ISP_AWB_ATTR_S *pstAWBAttr);
HI_S32 HI_MPI_ISP_GetAWBAttr(ISP_AWB_ATTR_S *pstAWBAttr);

HI_S32 HI_MPI_ISP_SetMWBAttr(const ISP_MWB_ATTR_S *pstMWBAttr);
HI_S32 HI_MPI_ISP_GetMWBAttr(ISP_MWB_ATTR_S *pstMWBAttr);

HI_S32 HI_MPI_ISP_SetColorTemp(const HI_U16 u16ColorTemp);     //not support yet
HI_S32 HI_MPI_ISP_GetColorTemp(HI_U16 *pu16ColorTemp);

HI_S32 HI_MPI_ISP_SetWBStaInfo(ISP_WB_STA_INFO_S *pstWBStatistic);
HI_S32 HI_MPI_ISP_GetWBStaInfo(ISP_WB_STA_INFO_S *pstWBStatistic);



/* Focus Settings */

HI_S32 HI_MPI_ISP_SetFocusType(ISP_OP_TYPE_E enFocusType);    //not support yet
HI_S32 HI_MPI_ISP_GetFocusType(ISP_OP_TYPE_E *penFocusType);  //not support yet

HI_S32 HI_MPI_ISP_SetAFAttr(const ISP_AF_ATTR_S *pstAFAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetAFAttr(ISP_AF_ATTR_S *pstAFAttr);        //not support yet

HI_S32 HI_MPI_ISP_SetMFAttr(const ISP_MF_ATTR_S *pstMFAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetMFAttr(ISP_MF_ATTR_S *pstMFAttr);        //not support yet

HI_S32 HI_MPI_ISP_ManualFocusMove(HI_S32 s32MoveSteps);       //not support yet

HI_S32 HI_MPI_ISP_SetFocusStaInfo(ISP_FOCUS_STA_INFO_S *pstFocusStatistic);
HI_S32 HI_MPI_ISP_GetFocusStaInfo(ISP_FOCUS_STA_INFO_S *pstFocusStatistic);


/* Iris Settings */

HI_S32 HI_MPI_ISP_SetIrisType(ISP_OP_TYPE_E enIrisType);//not support yet
HI_S32 HI_MPI_ISP_GetIrisType(ISP_OP_TYPE_E *penIrisType);    //not support yet

HI_S32 HI_MPI_ISP_SetAIAttr(const ISP_AI_ATTR_S *pstAIAttr);
HI_S32 HI_MPI_ISP_GetAIAttr(ISP_AI_ATTR_S *pstAIAttr);

HI_S32 HI_MPI_ISP_SetMIAttr(const ISP_MI_ATTR_S *pstMIAttr);  //not support yet
HI_S32 HI_MPI_ISP_GetMIAttr(ISP_MI_ATTR_S *pstMIAttr);        //not support yet


/* General Function Settings */

HI_S32 HI_MPI_ISP_SetDRCAttr(const ISP_DRC_ATTR_S *pstDRC);
HI_S32 HI_MPI_ISP_GetDRCAttr(ISP_DRC_ATTR_S *pstDRC);

HI_S32 HI_MPI_ISP_SetDefectPixelAttr(const ISP_DP_ATTR_S *pstDPAttr);
HI_S32 HI_MPI_ISP_GetDefectPixelAttr(ISP_DP_ATTR_S *pstDPAttr);

HI_S32 HI_MPI_ISP_SetDISAttr(const ISP_DIS_ATTR_S *pstDISAttr);
HI_S32 HI_MPI_ISP_GetDISAttr(ISP_DIS_ATTR_S *pstDISAttr);

HI_S32 HI_MPI_ISP_SetShadingAttr(const ISP_SHADING_ATTR_S *pstShadingAttr);
HI_S32 HI_MPI_ISP_GetShadingAttr(ISP_SHADING_ATTR_S *pstShadingAttr);

HI_S32 HI_MPI_ISP_SetShadingTable(const ISP_SHADINGTAB_S *pstShadingTab);
HI_S32 HI_MPI_ISP_GetShadingTable(ISP_SHADINGTAB_S *pstShadingTab);

HI_S32 HI_MPI_ISP_SetDenoiseAttr(const ISP_DENOISE_ATTR_S *pstDenoiseAttr);
HI_S32 HI_MPI_ISP_GetDenoiseAttr(ISP_DENOISE_ATTR_S *pstDenoiseAttr);

HI_S32 HI_MPI_ISP_SetGammaAttr(const ISP_GAMMA_ATTR_S* pstGammaAttr);
HI_S32 HI_MPI_ISP_GetGammaAttr(ISP_GAMMA_ATTR_S* pstGammaAttr);

HI_S32 HI_MPI_ISP_SetGammaTable(ISP_GAMMA_TABLE_S* pstGammaAttr);
HI_S32 HI_MPI_ISP_GetGammaTable(ISP_GAMMA_TABLE_S* pstGammaAttr);  //not support yet

//for WDR sensor
HI_S32 HI_MPI_ISP_SetGammaFEAttr(const ISP_GAMMA_FE_ATTR_S* pstGammaFEAttr);
HI_S32 HI_MPI_ISP_GetGammaFEAttr(ISP_GAMMA_FE_ATTR_S* pstGammaFEAttr);
HI_S32 HI_MPI_ISP_SetGammaFETable(const ISP_GAMMA_FE_TABLE_S* pstGammaFETable);
HI_S32 HI_MPI_ISP_GetGammaFETable(ISP_GAMMA_FE_TABLE_S* pstGammaFETable);

//for WDR sensor
HI_S32 HI_MPI_ISP_SetGammaPostAttr(const ISP_GAMMA_POST_ATTR_S* pstGammaPostAttr);
HI_S32 HI_MPI_ISP_GetGammaPostAttr(ISP_GAMMA_POST_ATTR_S* pstGammaPostAttr);
HI_S32 HI_MPI_ISP_SetGammaPostTable(const ISP_GAMMA_POST_TABLE_S* pstGammaPostTable);

HI_S32 HI_MPI_ISP_SetSharpenAttr(const ISP_SHARPEN_ATTR_S *pstSharpenAttr);
HI_S32 HI_MPI_ISP_GetSharpenAttr(ISP_SHARPEN_ATTR_S *pstSharpenAttr);

HI_S32 HI_MPI_ISP_SetCropAttr(const ISP_CROP_ATTR_S *pstCropAttr);
HI_S32 HI_MPI_ISP_GetCropAttr(ISP_CROP_ATTR_S *pstCropAttr);

HI_S32 HI_MPI_ISP_SetAntiFlickerAttr(const ISP_ANTIFLICKER_S *pstAntiflicker);
HI_S32 HI_MPI_ISP_GetAntiFlickerAttr(ISP_ANTIFLICKER_S *pstAntiflicker);

HI_S32 HI_MPI_ISP_SetCCM(const ISP_COLORMATRIX_S *pstColorMatrix);
HI_S32 HI_MPI_ISP_GetCCM(ISP_COLORMATRIX_S *pstColorMatrix);

HI_S32 HI_MPI_ISP_SetSaturationAttr(ISP_SATURATION_ATTR_S *pstSatAttr);
HI_S32 HI_MPI_ISP_GetSaturationAttr(ISP_SATURATION_ATTR_S *pstSatAttr);

HI_S32 HI_MPI_ISP_SetSaturation(HI_U32 u32Value);
HI_S32 HI_MPI_ISP_GetSaturation(HI_U32 *pu32Value);

HI_S32 HI_MPI_ISP_SetCfg(HI_U32 u32Addr, HI_U32 u32Value);
HI_S32 HI_MPI_ISP_GetCfg(HI_U32 u32Addr, HI_U32 *pu32Value);

HI_S32 HI_MPI_ISP_SetCRStrength(const ISP_CR_STRENGTH_S *pstCRStrength);
HI_S32 HI_MPI_ISP_GetCRStrength(ISP_CR_STRENGTH_S *pstCRStrength);


/* Sensor Relative Operation */

HI_S32 HI_MPI_ISP_SensorRegCallBack(SENSOR_EXP_FUNC_S *pstSensorExpFuncs);


/* Firmware Main Operation */

HI_S32 HI_MPI_ISP_SetInputTiming(const ISP_INPUT_TIMING_S *pstInputTiming);
HI_S32 HI_MPI_ISP_GetInputTiming(ISP_INPUT_TIMING_S *pstInputTiming);

HI_S32 HI_MPI_ISP_SetImageAttr(const ISP_IMAGE_ATTR_S *pstImageAttr);
HI_S32 HI_MPI_ISP_GetImageAttr(ISP_IMAGE_ATTR_S *pstImageAttr);

HI_S32 HI_MPI_ISP_Init(void);

HI_S32 HI_MPI_ISP_Run(void);

HI_S32 HI_MPI_ISP_Exit(void);

HI_S32 HI_MPI_ISP_QueryInnerStateInfo(ISP_INNER_STATE_INFO_S *pstInnerStateInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__MPI_ISP_H__ */

