/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_comm_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/12/20
  Description   : 
  History       :
  1.Date        : 2010/12/20
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __HI_COMM_ISP_H__
#define __HI_COMM_ISP_H__

#include "hi_type.h"
#include "hi_errno.h"
#include "hi_common.h"
#include "hi_isp_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

#define WEIGHT_ZONE_ROW			7
#define WEIGHT_ZONE_COLUMN		9

#define ISP_REG_BASE	0x20660000
#define ISP_REG_SIZE		0x7fff

#define ISP_WINDOW0_START	0x20660214
#define ISP_WINDOW0_SIZE	0x20660218
#define ISP_WINDOW2_START	0x20660228
#define ISP_WINDOW2_SIZE	0x2066022C

#define ISP_BYPASS_BASE		0x20660080
 

/****************************************************************************
 * GENERAL STRUCTURES                                                       *
 ****************************************************************************/

typedef enum hiISP_ERR_CODE_E
{
    ERR_ISP_NOT_INIT				= 0x40,
    ERR_ISP_TM_NOT_CFG				= 0x41,
    ERR_ISP_ATTR_NOT_CFG			= 0x42,
    ERR_ISP_SNS_UNREGISTER			= 0x43,
    ERR_ISP_INVALID_ADDR			= 0x44,
    
} ISP_ERR_CODE_E;


#define HI_ERR_ISP_NULL_PTR							HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define HI_ERR_ISP_ILLEGAL_PARAM         			HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)

#define HI_ERR_ISP_NOT_INIT         				HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_NOT_INIT)
#define HI_ERR_ISP_TM_NOT_CFG         				HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_TM_NOT_CFG)
#define HI_ERR_ISP_ATTR_NOT_CFG         			HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_ATTR_NOT_CFG)
#define HI_ERR_ISP_SNS_UNREGISTER  	       			HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_SNS_UNREGISTER)
#define HI_ERR_ISP_INVALID_ADDR	   		   			HI_DEF_ERR(HI_ID_ISP, EN_ERR_LEVEL_ERROR, ERR_ISP_INVALID_ADDR)


typedef enum hiISP_BAYER_FORMAT_E
{
	BAYER_RGGB	= 0,
	BAYER_GRBG	= 1,
	BAYER_GBRG	= 2,
	BAYER_BGGR	= 3,
	BAYER_BUTT	
    
} ISP_BAYER_FORMAT_E;

typedef enum hiISP_OP_TYPE_E
{
	OP_TYPE_AUTO	= 0,
	OP_TYPE_MANUAL	= 1,
	OP_TYPE_BUTT
    
} ISP_OP_TYPE_E;

typedef enum hiISP_AE_MODE_E
{
    AE_MODE_LOW_NOISE		= 0,
    AE_MODE_FRAME_RATE		= 1,
    AE_MODE_AGAIN_FIRST		= 2,
    AE_MODE_BUTT
    
} ISP_AE_MODE_E;

typedef enum hiISP_WB_MODE_E
{
    /* all auto*/
    WB_AUTO = 0,
    
    /* half auto */		
    WB_FLUORESCENT,		/*fluorescent*/
    WB_LAMP,				/*lamp*/
    WB_DAYLIGHT,			/*daylight*/
    WB_FLASH,				/*flash light*/
    WB_CLOUDY,				/*cloudy*/
    WB_SHADOW,				/*shadow*/
    WB_BUTT
    
} ISP_WB_MODE_E;

typedef struct hiISP_WINDOW_S
{
	HI_U16 u16Start;
	HI_U16 u16Length;
    
} ISP_WINDOW_S;

typedef enum hiISP_WIND_MODE_E
{
	ISP_WIND_NONE		= 0,
	ISP_WIND_HOR		= 1,
	ISP_WIND_VER		= 2,
	ISP_WIND_ALL		= 3,
	ISP_WIND_BUTT
    
} ISP_WIND_MODE_E;

typedef enum hiISP_IRIS_STATUS_E
{
	ISP_IRIS_KEEP  = 0,       /* Do nothing to Iris */
	ISP_IRIS_OPEN  = 1,       /* Open Iris to the max */
	ISP_IRIS_CLOSE = 2,       /* Close Iris to the min */
	ISP_IRIS_BUTT

} ISP_IRIS_STATUS_E;

typedef enum hiISP_TRIGGER_STATUS_E
{
	ISP_TRIGGER_INIT     = 0,  /* Initial status, before trigger */
	ISP_TRIGGER_SUCCESS  = 1,  /* Trigger finished successfully */
	ISP_TRIGGER_TIMEOUT  = 2,  /* Trigger finished because of time out */
	ISP_TRIGGER_BUTT

} ISP_TRIGGER_STATUS_E;

typedef struct hiISP_INPUT_TIMING_S
{
	ISP_WIND_MODE_E enWndMode;
	HI_U16 u16HorWndStart;
	HI_U16 u16HorWndLength;
	HI_U16 u16VerWndStart;
	HI_U16 u16VerWndLength;
    
} ISP_INPUT_TIMING_S;

typedef struct hiISP_IMAGE_ATTR_S		// sensor information: list?
{
	HI_U16 u16Width;
	HI_U16 u16Height;
	HI_U16 u16FrameRate;			
	ISP_BAYER_FORMAT_E  enBayer;
    
} ISP_IMAGE_ATTR_S;

typedef enum hiISP_MOD_BYPASS_E
{
	ISP_MOD_SHARPEN		= 0x8000,
	ISP_MOD_GAMMARGB	= 0x4000,
	ISP_MOD_COLORMATRIX	= 0x2000,
	ISP_MOD_DEMOSAIC	= 0x1000,

	ISP_MOD_GAMMAPOST	= 0x0800,
	ISP_MOD_GAMMAPRE	= 0x0200,
	ISP_MOD_SHADING		= 0x0100,

	ISP_MOD_IRIDIX		= 0x0080,
	ISP_MOD_GAIN		= 0x0040,
	ISP_MOD_SINTER		= 0x0008,
	ISP_MOD_HOTPIXEL	= 0x0004,
	ISP_MOD_GAMMAFE		= 0x0002,	
	ISP_MOD_BALANCEFE	= 0x0001,		
	ISP_MOD_BUTT
    
} ISP_MOD_BYPASS_E;


typedef enum hiISP_AE_FRAME_END_UPDATE_E
{
        ISP_AE_FRAME_END_UPDATE_0  = 0x0, //isp update gain and exposure  in the  same frame
        ISP_AE_FRAME_END_UPDATE_1  = 0x1, //isp update exposure one frame before  gain
       
        ISP_AE_FRAME_END_BUTT

}ISP_AE_FRAME_END_UPDATE_E;
/* 4A settings                                                              */
typedef struct hiISP_AE_ATTR_S
{
    /* base parameter */
    ISP_AE_MODE_E enAEMode;		/*AE mode(lownoise/framerate)(onvif)*/
    HI_U16 u16ExpTimeMax;       /*the exposure time's max and min value .(unit :line)*/
    HI_U16 u16ExpTimeMin; 
    HI_U16 u16DGainMax;         /* the Dgain's min and max value*/
    HI_U16 u16DGainMin;
    HI_U16 u16AGainMax;			/* the Again's min and max value*/
    HI_U16 u16AGainMin;
    
    HI_U8  u8ExpStep;			/*AE adjust step*/
    HI_S16 s16ExpTolerance;		/*AE adjust tolerance*/
    HI_U8  u8ExpCompensation;	/*AE compensation*/ 
    ISP_AE_FRAME_END_UPDATE_E  enFrameEndUpdateMode;
    HI_BOOL bByPassAE;
    /*AE weighting table*/
    HI_U8 u8Weight[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];
    
} ISP_AE_ATTR_S;

typedef struct hiISP_EXP_STA_INFO_S
{
    HI_U8  u8ExpHistThresh[4];				/*RW, Histogram threshold for bin 0/1 1/2 2/3 3/4 boundary*/
    HI_U16 u16ExpStatistic[WEIGHT_ZONE_ROW ][WEIGHT_ZONE_COLUMN][5];	/*RO, zone exposure statistics*/
    HI_U16 u16Exp_Hist256Value[256];		/*RO, 256 bins histogram*/
    HI_U16 u16Exp_Hist5Value[5];			/*RO, 5 bins histogram*/
    HI_U8   u8AveLum;						/*RO, average lum*/


}ISP_EXP_STA_INFO_S;

typedef struct hiISP_ME_ATTR_S
{
	HI_S32 s32AGain;       		/* sensor analog gain (unit: times) */
	HI_S32 s32DGain;       		/* sensor digital gain (unit: times) */
	HI_U32 u32ExpLine;			/* sensor exposure time (unit: line ) */

	HI_BOOL bManualExpLineEnable;
	HI_BOOL bManualAGainEnable;
	HI_BOOL bManualDGainEnable;

} ISP_ME_ATTR_S;

typedef struct hiISP_AF_ATTR_S
{
    HI_S32 s32FocusDistanceMax;		/* the focuse range*/
    HI_S32 s32FocusDistanceMin;

    /*weighting table*/
    HI_U8 u8Weight[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];
    
} ISP_AF_ATTR_S;

typedef struct hiISP_FOUCS_STA_INFO_S
{
	HI_U16 u16FocusMetrics;      /*RO, The integrated and normalized measure of contrast*/
	HI_U16 u16ThresholdRead;     /*RO, The ISP recommend value of AF threshold*/
	HI_U16 u16ThresholdWrite;    /*RW, The user defined value of AF threshold (or 0 to use threshold from previous frame)*/
	HI_U16 u16FocusIntensity;    /*RO, The average brightness*/
	HI_U16 u16ZoneMetrics[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN]; /*RO, The zoned measure of contrast*/
    
} ISP_FOCUS_STA_INFO_S;

typedef struct hiISP_MF_ATTR_S
{
    HI_S32 s32DefaultSpeed;		/*1,default speed(unit:m/s).(onvif)*/		
    
} ISP_MF_ATTR_S;

typedef struct hiISP_AWB_ATTR_S
{
    HI_U8 u8RGStrength;
    HI_U8 u8BGStrength;
    HI_U8 u8ZoneSel;           /* A value of 0 or 0x3F means global AWB, A value between 0 and 0x3F means zoned AWB */
    /* weighting table*/
    HI_U8 u8Weight[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];
    
} ISP_AWB_ATTR_S;

typedef struct hiISP_WB_ZONE_STA_INFO_S
{
    HI_U16 u16Rg;         /*RO, Zoned WB output G/R*/
    HI_U16 u16Bg;         /*RO, Zoned WB output G/B*/
    HI_U32 u32Sum;        /*RO, Zoned WB output population*/
  
} ISP_WB_ZONE_STA_INFO_S;

typedef struct hiISP_WB_STA_INFO_S
{
    HI_U16 u16WhiteLevel;  /*RW, Upper limit of valid data for white region*/
    HI_U16 u16BlackLevel;  /*RW, Lower limit of valid data for white region*/
    HI_U16 u16CbMax;       /*RW, Maximum value of B/G for white region*/
    HI_U16 u16CbMin;       /*RW, Minimum value of B/G for white region*/
    HI_U16 u16CrMax;       /*RW, Maximum value of R/G for white region*/
    HI_U16 u16CrMin;       /*RW, Minimum value of R/G for white region*/
    
    HI_U16 u16GRgain;      /*RO, Global WB output G/R*/
    HI_U16 u16GBgain;      /*RO, Global WB output G/B*/
    HI_U32 u32GSum;        /*RO, Global WB output population*/
 
    ISP_WB_ZONE_STA_INFO_S stZoneSta[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];  /*RO, Zoned WB statistics*/
  
} ISP_WB_STA_INFO_S;

typedef struct hiISP_MWB_ATTR_S		
{
    HI_U16 u16Rgain;      /*RW, Multiplier for R color channel*/
	HI_U16 u16Ggain;      /*RW, Multiplier for G color channel*/
    HI_U16 u16Bgain;      /*RW, Multiplier for B color channel*/
    
} ISP_MWB_ATTR_S;

typedef struct hiISP_COLORMATRIX_S
{
    HI_U16 u16CorrMatrix[9]; 
} ISP_COLORMATRIX_S;

typedef struct hiISP_AI_ATTR_S
{
	HI_BOOL bIrisEnable;			/*Auto iris  on/off*/
	HI_BOOL bIrisCalEnable;			/*iris calibration on/off*/
	HI_U32  u32IrisHoldValue;       /*iris hold value*/

	ISP_IRIS_STATUS_E enIrisStatus;         /*status of Iris*/
	ISP_TRIGGER_STATUS_E enTriggerStatus;   /*status of trigger*/
	HI_U16 u16IrisStopValue;                /*the initial stop value for AI trigger*/
	HI_U16 u16IrisCloseDrive;               /*the drive value to close Iris, [700, 900]. A larger value means faster.*/
	HI_U16 u16IrisTriggerTime;              /*frame numbers of AI trigger lasting time. > 600*/
    HI_U8  u8IrisInertiaValue;              /*frame numbers of  AI moment of inertia */
    
} ISP_AI_ATTR_S;

typedef struct hiISP_MI_ATTR_S
{
    HI_S32 s32FixAttenuation;		
    
} ISP_MI_ATTR_S;


typedef struct hiISP_DRC_ATTR_S
{
    HI_BOOL bDRCEnable;        
    HI_BOOL bDRCManualEnable;        
    HI_U32  u32StrengthTarget;  /* Range: [0, 0xFF]. It is not the final strength used by ISP. 
                                         * It will be clipped to reasonable value when image is noisy. */
    HI_U32  u32SlopeMax;        /* Range: [0, 0xFF]. Not recommended to change. */
    HI_U32  u32SlopeMin;        /* Range: [0, 0xFF]. Not recommended to change. */
    HI_U32  u32WhiteLevel;      /* Range: [0, 0xFFFF]. Not recommended to change. */
    HI_U32  u32BlackLevel;      /* Range: [0, 0xFFFF]. Not recommended to change. */
} ISP_DRC_ATTR_S;

typedef enum hiISP_ANTIFLICKER_MODE_E
{
      ISP_ANTIFLICKER_MODE_0 = 0x0,    /*The exposure time is fixed at several values. The minimum exposure time is 1/120 sec. (60 Hz) or 1/100 sec. (50 Hz). Since the exposure time is synchronized with the fluorescent lighting,
                                                                   rolling bar noise can be suppressed. In high-luminance environments, however, the exposure time is fixed to the minimum exposure time, it may lead to over exposure*/

      ISP_ANTIFLICKER_MODE_1 = 0x1,    /*In this mode, the exposure time is not fixed in high-luminance environments and the exposure time can be reduced  to the minimum . Use it when the exposure is to be controlled in high-luminance environments.
                                                                  in low lumiance enviroment, the effect is the same as the above one*/
      
      ISP_ANTIFLICKER_MODE_BUTT
      
}ISP_ANTIFLICKER_MODE_E;

typedef struct hiISP_ANTIFLICKER_S
{
	HI_BOOL bEnable;
       
	HI_U8 u8Frequency;

       ISP_ANTIFLICKER_MODE_E  enMode;
    
} ISP_ANTIFLICKER_S;

typedef struct hiISP_SATURATION_ATTR_S
{
    HI_BOOL bSatManualEnable;
    
}ISP_SATURATION_ATTR_S;

typedef struct hiISP_DP_ATTR_S
{
	HI_BOOL bEnableStatic;
	HI_BOOL bEnableDynamic;
	HI_BOOL bEnableCluster;
	HI_BOOL bEnableDetect;
	ISP_TRIGGER_STATUS_E enTriggerStatus;  /*status of bad pixel trigger*/
	HI_U8   u8BadPixelStartThresh;
	HI_U8   u8BadPixelFinishThresh;
	HI_U16  u16BadPixelCountMax;           /*limit of max number of bad pixel*/
	HI_U16  u16BadPixelCountMin;            /*limit of min number of bad pixel*/
	HI_U16  u16BadPixelCount;
	HI_U16  u16BadPixelTriggerTime;     /*time limit for bad pixel trigger, in frame number */
	HI_U32  u32BadPixelTable[1024];
	
} ISP_DP_ATTR_S;

typedef struct hiISP_DIS_ATTR_S
{
    HI_BOOL bEnable;
    
} ISP_DIS_ATTR_S;

typedef enum hiISP_SHADING_SCALE_E
{
	ISP_SHADING_SCALE_2 = 0x0,   /* the max compensation gain is 2 */
	ISP_SHADING_SCALE_4 = 0x1,   /* the max compensation gain is 4 */
	ISP_SHADING_SCALE_8 = 0x2,   /* the max compensation gain is 8 */
	ISP_SHADING_SCALE_16 = 0x3,   /* the max compensation gain is 16 */
	ISP_SHADING_SCALE_BUTT
}ISP_SHADING_SCALE_E;

typedef struct hiISP_SHADING_ATTR_S
{
    HI_BOOL Enable;
} ISP_SHADING_ATTR_S;

typedef enum hiISP_SHADING_TAB_E
{
    SHADING_TAB_R		= 0,
    SHADING_TAB_G		= 1,
    SHADING_TAB_B		= 2,
    SHADING_TAB_BUTT
} ISP_SHADING_TAB_E;

typedef struct hiISP_SHADINGTAB_S
{
    ISP_SHADING_SCALE_E enScale;
    ISP_SHADING_TAB_E enMesh_R;
    ISP_SHADING_TAB_E enMesh_G;
    ISP_SHADING_TAB_E enMesh_B;
    HI_U8 u8ShadingTable_R[64*64]; 
    HI_U8 u8ShadingTable_G[64*64]; 
    HI_U8 u8ShadingTable_B[64*64]; 

} ISP_SHADINGTAB_S;

typedef struct hiISP_DENOISE_ATTR_S
{
	HI_BOOL bEnable;
	HI_BOOL bManualEnable;
	HI_U8 u8ThreshTarget;			// Noise reduction effect for high spacial frequencies 
	HI_U8 u8ThreshMax;			// Noise reduction effect for high spacial frequencies 
	
} ISP_DENOISE_ATTR_S;

typedef struct hiISP_GAMMA_ATTR_S
{
	HI_BOOL bEnable;
    
} ISP_GAMMA_ATTR_S;

typedef enum hiISP_GAMMA_CURVE_E
{
	ISP_GAMMA_CURVE_1_6 = 0x0,           /* 1.6 Gamma curve */
	ISP_GAMMA_CURVE_1_8 = 0x1,           /* 1.8 Gamma curve */
	ISP_GAMMA_CURVE_2_0 = 0x2,           /* 2.0 Gamma curve */
	ISP_GAMMA_CURVE_2_2 = 0x3,           /* 2.2 Gamma curve */
	ISP_GAMMA_CURVE_USER_DEFINE = 0x4,   /* user defined Gamma curve, Gamma Table must be correct */
	ISP_GAMMA_CURVE_BUTT
	
} ISP_GAMMA_CURVE_E;

typedef struct hiISP_GAMMA_TABLE_S
{
	ISP_GAMMA_CURVE_E enGammaCurve;
	HI_U16 u16Gamma[65];
   
} ISP_GAMMA_TABLE_S;

typedef struct hiISP_GAMMA_FE_ATTR_S
{
	HI_BOOL bEnable;
    
} ISP_GAMMA_FE_ATTR_S;

typedef struct hiISP_GAMMA_FE_TABLE_S
{
	HI_U16 u16Gamma_FE[129];
    
} ISP_GAMMA_FE_TABLE_S;

typedef struct hiISP_GAMMA_POST_ATTR_S
{
	HI_BOOL bEnable;
    
} ISP_GAMMA_POST_ATTR_S;

typedef struct hiISP_GAMMA_POST_TABLE_S
{
	HI_U16 u16GammaPost[65];
    
} ISP_GAMMA_POST_TABLE_S;

typedef struct hiISP_SHARPEN_ATTR_S
{
	HI_BOOL bEnable;
	HI_BOOL bManualEnable;
	HI_U32 u32StrengthTarget;   /* Range:[0, 0xFF]. */
	HI_U8 u8StrengthMin;      /* Range:[0, u32StrengthTarget]. */
} ISP_SHARPEN_ATTR_S;

typedef struct hiCROP_RECT_S
{
    HI_U16 u16StartX;
    HI_U16 u16StartY;
    HI_U16 u16Width;
    HI_U16 u16Height;
    
} CROP_RECT_S;

typedef struct hiISP_CROP_ATTR_S
{
	HI_BOOL bEnable;
    CROP_RECT_S stCropRect;
    
} ISP_CROP_ATTR_S;

typedef struct hiISP_PARA_REC_S
{
	HI_BOOL bInit;
	HI_BOOL bTmCfg;
	HI_BOOL bAttrCfg;
	
    ISP_INPUT_TIMING_S stInputTiming;
    ISP_IMAGE_ATTR_S stImageAttr;

	HI_U32 u32ModFlag;

	/* Exposure */
	ISP_OP_TYPE_E enExpType;
	ISP_AE_ATTR_S stAEAttr;
	ISP_ME_ATTR_S stMEAttr;

	/* White Balance */
	ISP_OP_TYPE_E enWBType;
	ISP_AWB_ATTR_S stAWBAttr;
	ISP_MWB_ATTR_S stMWBAttr;
    
} ISP_PARA_REC_S;

/*Crosstalk Removal Strength*/
typedef struct hiISP_CR_STRENGTH_S
{
	HI_U8 u8HorStrength;       /* Range:[0, 0xFF]. */
	HI_U8 u8VerStrength;       /* Range:[0, 0xFF]. */
} ISP_CR_STRENGTH_S;

    /*users query ISP state information*/
typedef struct hiISP_INNER_STATE_INFO_S
{
	HI_U32 u32ExposureTime;				
	HI_U32 u32AnalogGain;					
	HI_U32 u32DigitalGain;				
	HI_U32 u32Exposure;					
	HI_U16 u16AE_Hist256Value[256];		//256 bins histogram
	HI_U16 u16AE_Hist5Value[5];			//5 bins histogram
	HI_U8 u8AveLum;						
	HI_BOOL bExposureIsMAX;
}ISP_INNER_STATE_INFO_S;


#define ISP_CHECK_POINTER(ptr)\
    do {\
        if (NULL == ptr)\
        {\
        	HI_PRINT("Null Pointer!\n");\
            return HI_ERR_ISP_NULL_PTR;\
        }\
    }while(0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __HI_COMM_ISP_H__ */

