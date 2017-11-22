/******************************************************************************

Copyright (C), 2004-2020, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     : hi_comm_vi.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2009/3/9
Last Modified :
Description   : 
Function List :
History       :
1.Date        : 2010/11/16
  Author      : p00123320/w54723/n168968
  Modification: Created file

******************************************************************************/

#ifndef __HI_COMM_VI_H__
#define __HI_COMM_VI_H__

#include "hi_common.h"
#include "hi_errno.h"
#include "hi_comm_video.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#define VI_INVALID_FRMRATE  (-1UL)

typedef enum hiEN_VIU_ERR_CODE_E
{
    ERR_VI_FAILED_NOTENABLE = 64,   /* device or channel not enable*/
    ERR_VI_FAILED_NOTDISABLE,       /* device not disable*/
    ERR_VI_FAILED_CHNOTDISABLE,     /* channel not disable*/
    ERR_VI_CFG_TIMEOUT,             /* config timeout*/
    ERR_VI_NORM_UNMATCH,            /* video norm of ADC and VIU is unmatch*/ 
    ERR_VI_INVALID_WAYID,           /* invlalid way ID     */
    ERR_VI_INVALID_PHYCHNID,        /* invalid phychn id*/
    ERR_VI_FAILED_NOTBIND,          /* device or channel not bind */
    ERR_VI_FAILED_BINDED,           /* device or channel not unbind */
} EN_VIU_ERR_CODE_E;

#define HI_ERR_VI_INVALID_PARA          HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM) 
#define HI_ERR_VI_INVALID_DEVID         HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
#define HI_ERR_VI_INVALID_CHNID         HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
#define HI_ERR_VI_INVALID_NULL_PTR      HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
#define HI_ERR_VI_FAILED_NOTCONFIG      HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
#define HI_ERR_VI_SYS_NOTREADY          HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)
#define HI_ERR_VI_BUF_EMPTY             HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
#define HI_ERR_VI_BUF_FULL              HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
#define HI_ERR_VI_NOMEM                 HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
#define HI_ERR_VI_NOT_SUPPORT          HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
#define HI_ERR_VI_BUSY                  HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)
#define HI_ERR_VI_NOT_PERM              HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)

#define HI_ERR_VI_FAILED_NOTENABLE      HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTENABLE)/* 0xA0108040*/
#define HI_ERR_VI_FAILED_NOTDISABLE     HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTDISABLE)/* 0xA0108041*/
#define HI_ERR_VI_FAILED_CHNOTDISABLE   HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_CHNOTDISABLE)/* 0xA0108042*/
#define HI_ERR_VI_CFG_TIMEOUT           HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_CFG_TIMEOUT)/* 0xA0108043*/
#define HI_ERR_VI_NORM_UNMATCH          HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_NORM_UNMATCH)/* 0xA0108044*/
#define HI_ERR_VI_INVALID_WAYID         HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_WAYID)/* 0xA0108045*/
#define HI_ERR_VI_INVALID_PHYCHNID      HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_INVALID_PHYCHNID)/* 0xA0108046*/
#define HI_ERR_VI_FAILED_NOTBIND        HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_NOTBIND)/* 0xA0108047*/
#define HI_ERR_VI_FAILED_BINDED         HI_DEF_ERR(HI_ID_VIU, EN_ERR_LEVEL_ERROR, ERR_VI_FAILED_BINDED)/* 0xA0108048*/



/* Interface input mode */
typedef enum hiVI_INPUT_MODE_E
{
    VI_MODE_BT656 = 0,            
    VI_MODE_BT601,   
    VI_MODE_DIGITAL_CAMERA, 

    VI_MODE_BUTT
} VI_INPUT_MODE_E;

/*whether an input picture is interlaced or progressive*/
typedef enum hiVI_SCAN_MODE_E
{
    VI_SCAN_INTERLACED  = 0,    
    VI_SCAN_PROGRESSIVE,       
    
    VI_SCAN_BUTT,
} VI_SCAN_MODE_E;

typedef enum hiVI_DATA_TYPE_E
{
    VI_DATA_TYPE_RGB,
    VI_DATA_TYPE_YUV,
    VI_DATA_TYPE_BUTT
} VI_DATA_TYPE_E;

typedef enum hiVI_DATA_RGB_SEQ_E
{
    VI_DATA_SEQ_RGRG = 0,
    VI_DATA_SEQ_GRGR,
    VI_DATA_SEQ_BGBG,
    VI_DATA_SEQ_GBGB,
    
    VI_DATA_RGB_BUTT
} VI_DATA_RGB_SEQ_E;

typedef enum hiVI_DATA_YUV_SEQ_E
{
   /*The input sequence of the second component(only contains u and v) in BT.1120 mode */
    VI_INPUT_DATA_VUVU = 0,    
    VI_INPUT_DATA_UVUV,        

   /* The input sequence for yuv */
    VI_INPUT_DATA_UYVY = 0,
    VI_INPUT_DATA_VYUY,
    VI_INPUT_DATA_YUYV,
    VI_INPUT_DATA_YVYU,
    
    VI_DATA_YUV_BUTT
} VI_DATA_YUV_SEQ_E;

typedef union hiVI_DATA_SEQ_U
{
    VI_DATA_RGB_SEQ_E enRgbSeq;
    VI_DATA_YUV_SEQ_E enYuvSeq;    
} VI_DATA_SEQ_U;

typedef enum hiVI_CLK_EDGE_E
{
    /* single-edge mode and in rising edge */
    VI_CLK_EDGE_SINGLE_UP = 0,  

    /* single-edge mode and in falling edge */
    VI_CLK_EDGE_SINGLE_DOWN, 

    /* Double edge mode */
    VI_CLK_EDGE_DOUBLE ,                 
    
    VI_CLK_EDGE_BUTT   
} VI_CLK_EDGE_E;

typedef enum hiVI_COMP_MODE_E
{
    VI_COMP_MODE_SINGLE = 0,      /* in single component mode */
    VI_COMP_MODE_DOUBLE = 1,      /* in double component mode */
    VI_COMP_MODE_BUTT,   
}VI_COMP_MODE_E;

typedef enum hiVI_COMBINE_MODE_E
{
    VI_COMBINE_COMPOSITE = 0,       /* Composite mode */
    VI_COMBINE_SEPARATE,            /* Separate mode */
    VI_COMBINE_BUTT,
} VI_COMBINE_MODE_E;

/* Attribute of the vertical synchronization signal */
typedef enum hiVI_VSYNC_E
{
    VI_VSYNC_FIELD = 0,          /*Field/toggle mode:a signal reversal means a new frame or a field/frame */
    VI_VSYNC_PULSE,              /*Pusle/effective mode:a pusle or an effective signal means a new frame or a field */
} VI_VSYNC_E;

/* Polarity of the vertical synchronization signal */
typedef enum hiVI_VSYNC_NEG_E
{
    VI_VSYNC_NEG_HIGH = 0,     /*if VIU_VSYNC_E = VIU_VSYNC_FIELD,then the vertical synchronization signal of even field is high-level, 
                                     if VIU_VSYNC_E = VIU_VSYNC_PULSE,then the vertical synchronization pulse is positive pulse.*/
    VI_VSYNC_NEG_LOW           /*if VIU_VSYNC_E = VIU_VSYNC_FIELD,then the vertical synchronization signal of even field is low-level,
                                     if VIU_VSYNC_E = VIU_VSYNC_PULSE,then the vertical synchronization pulse is negative pulse.*/
} VI_VSYNC_NEG_E;

/* Attribute of the horizontal synchronization signal */
typedef enum hiVI_HSYNC_E
{
    VI_HSYNC_VALID_SINGNAL = 0,  /* the horizontal synchronization is valid signal mode */
    VI_HSYNC_PULSE,              /* the horizontal synchronization is pulse mode, a new pulse means the beginning of a new line */
} VI_HSYNC_E;

/* Polarity of the horizontal synchronization signal */
typedef enum hiVI_HSYNC_NEG_E
{
    VI_HSYNC_NEG_HIGH = 0,    /*if VI_HSYNC_E = VI_HSYNC_VALID_SINGNAL,then the horizontal synchronization signal of even field is high-level;
                                    if VI_HSYNC_E = VI_HSYNC_PULSE,then the horizontal synchronization pulse is positive pulse */
    VI_HSYNC_NEG_LOW          /*if VI_HSYNC_E = VI_HSYNC_VALID_SINGNAL,then the horizontal synchronization signal of even field is low-level; 
                                    if VI_HSYNC_E = VI_HSYNC_PULSE,then the horizontal synchronization pulse is negative pulse */
} VI_HSYNC_NEG_E;

/* Attribute of the valid vertical synchronization signal */
typedef enum hiVI_VSYNC_VALID_E
{
    VI_VSYNC_NORM_PULSE = 0,         /* the vertical synchronization is pusle mode, a pusle means a new frame or field  */
    VI_VSYNC_VALID_SINGAL,           /* the vertical synchronization is effective mode, a effective signal means a new frame  */
} VI_VSYNC_VALID_E;

/* Polarity of the valid vertical synchronization signal */
typedef enum hiVI_VSYNC_VALID_NEG_E
{
    VI_VSYNC_VALID_NEG_HIGH = 0,     /*if VI_VSYNC_VALID_E = VI_VSYNC_NORM_PULSE,a positive pulse means vertical synchronization pulse; 
                                            if VI_VSYNC_VALID_E = VI_VSYNC_VALID_SINGAL,the vertical synchronization signal is high-level */
    VI_VSYNC_VALID_NEG_LOW           /*if VI_VSYNC_VALID_E = VI_VSYNC_NORM_PULSE,a negative pulse means vertical synchronization pulse; 
                                            if VI_VSYNC_VALID_E = VI_VSYNC_VALID_SINGAL,the vertical synchronization signal is low-level */
} VI_VSYNC_VALID_NEG_E;

/* Blank information of the input timing */
typedef struct hiVIU_TIMING_BLANK
{
    HI_U32 u32HsyncHfb ;    /* Horizontal front blanking width */
    HI_U32 u32HsyncAct ;    /* Horizontal effetive width */
    HI_U32 u32HsyncHbb ;    /* Horizontal back blanking width */
    HI_U32 u32VsyncOfb ;    /* Odd-field vertical front blanking height when the input mode is frame or field */ 
    HI_U32 u32VsyncOAct ;   /* Odd-field vertical front effective blanking width when the input mode is frame or field */
    HI_U32 u32VsyncObb ;    /* Odd-field vertical back blanking height when the input mode is frame or field */
    HI_U32 u32VsyncEfb ;    /* Even-field vertical front blanking height when the input mode is field */ 
    HI_U32 u32VsyncEAct ;   /* Even-field vertical back blanking width when the input mode is field */
    HI_U32 u32VsyncEbb ;    /* Even-field vertical back blanking height when the input mode is field */ 
}VI_TIMING_BLANK;

/* synchronization information on the BT.601 or DC timing */
typedef struct hiVI_SYNC_CFG_S
{
    VI_VSYNC_E             enVsync;
    VI_VSYNC_NEG_E         enVsyncNeg;
    VI_HSYNC_E             enHsync;
    VI_HSYNC_NEG_E         enHsyncNeg;
    VI_VSYNC_VALID_E       enVsyncValid;
    VI_VSYNC_VALID_NEG_E   enVsyncValidNeg;
    VI_TIMING_BLANK        stTimingBlank;
} VI_SYNC_CFG_S;

/* the highest bit of the BT.656 timing code */
typedef enum hiBT656_FIXCODE_E
{
    /* The highest bit of the EAV/SAV data over the
    BT.656 protocol is always 1.*/
    BT656_FIXCODE_1 = 0, 

    /* The highest bit of the EAV/SAV data over the
    BT.656 protocol is always 0.*/
    BT656_FIXCODE_0            
}BT656_FIXCODE_E;

/* Polarity of the field indicator bit (F) of the BT.656 timing code */
typedef enum hiBT656_FIELD_NEG_E
{
    BT656_FIELD_NEG_STD = 0,      /* the standard BT.656 mode,the first filed F=0,the second filed F=1*/
    BT656_FIELD_NEG_NSTD          /* the non-standard BT.656 mode,the first filed F=0,the second filed F=1*/
}BT656_FIELD_NEG_E;

typedef struct hiVI_BT656_SYNC_CFG_S
{
    BT656_FIXCODE_E           enFixCode;
    BT656_FIELD_NEG_E         enFieldNeg;
}VI_BT656_SYNC_CFG_S;

typedef struct hiVI_FRAME_INFO_S
{
    VIDEO_FRAME_INFO_S  stVFrmInfo;
} VI_FRAME_INFO_S;

/* Device attributes */
typedef struct hiVI_DEV_ATTR_S
{
    VI_INPUT_MODE_E     enInputMode;        /* input mode */
    VI_COMBINE_MODE_E     enCombineMode;    /* Composite/separate mode */

    VI_COMP_MODE_E      enCompMode;         /* Component mode (single or double) */
    HI_U32              au32CompMask[2];    /* Component mask */

    VI_CLK_EDGE_E       enClkEdge;          /* Clock edge mode */
    VI_SCAN_MODE_E      enScanMode;         /* Input scanning mode */    
    VI_DATA_TYPE_E      enDataType;         /* Input data type (RGB or YUV) */
    VI_DATA_SEQ_U       unDataSeq;          /* Input data sequence */
    HI_BOOL             bUseInterISP;       /* Whether an embedded ISP is used */
    
    VI_SYNC_CFG_S       stSynCfg;           /* Synchronizing timing (a BT.601/DC interface
                                                     must be configured) */
    VI_BT656_SYNC_CFG_S stBT656SynCfg;      /* Synchronizing timing (a BT.656 interface must
                                                     be configured) */
} VI_DEV_ATTR_S;

typedef enum hiVI_CSC_TYPE_E
{
    VI_CSC_TYPE_601 = 0,
    VI_CSC_TYPE_709,
    VI_CSC_TYPE_BUTT,
} VI_CSC_TYPE_E;

typedef struct hiVI_CSC_ATTR_S
{
    VI_CSC_TYPE_E enViCscType;
    HI_U32 u32LumaVal;                     /* luminance:   [0 ~ 100] */
    HI_U32 u32ContrVal                     /* contrast :   [0 ~ 100] */; 
    HI_U32 u32HueVal;                      /* hue      :   [0 ~ 100] */
    HI_U32 u32SatuVal;                     /* satuature:   [0 ~ 100] */ 
} VI_CSC_ATTR_S;

typedef enum hiVI_DSAC_MODE_E
{
	DSAC_MODE_CUT_OFF = 0,        
	DSAC_MODE_ROUND_DOWN,
	DSAC_MODE_FEED_BACK,	
	DSAC_MODE_BUTT
}VI_DSAC_MODE_E;

typedef struct hiVI_DEV_EX_ATTR_S
{
	VI_DSAC_MODE_E enDsacMode;
	HI_BOOL        bCscEn;
	HI_BOOL        bCdsEn;
}VI_DEV_EX_ATTR_S;

typedef enum hiVI_BIND_TYPE_E
{
    VI_CHN_BIND_PHYCHN = 0,                 /*bind physical channel*/
    VI_CHN_BIND_VICHN,                      /*bind another vichn*/
    VI_CHN_BIND_BUTT
}VI_BIND_TYPE_E;

typedef struct hiVI_BIND_PHYCHN_S
{
    HI_S32 s32PhyChn; 
    VI_DEV ViDev;
    VI_WAY ViWay;
} VI_BIND_PHYCHN_S;

typedef struct hiVI_BIND_VICHN_S
{
    VI_CHN ViChn;
} VI_BIND_VICHN_S;

typedef struct hiVI_CHN_BIND_ATTR_S
{
    VI_BIND_TYPE_E enBindType;
    union 
    {
        VI_BIND_PHYCHN_S stBindPhyChn;
        VI_BIND_VICHN_S  stBindViChn;    
    } unBindAttr;    
} VI_CHN_BIND_ATTR_S;

/* captrue selection of video input */
typedef enum hiVI_CAPSEL_E
{
    VI_CAPSEL_TOP = 0,        /* top field */
    VI_CAPSEL_BOTTOM,        /* bottom field */
    VI_CAPSEL_BOTH,            /* top and bottom field */
    VI_CAPSEL_BUTT
} VI_CAPSEL_E;


/* channel attributes */
typedef struct hiVI_CHN_ATTR_S
{
    RECT_S          stCapRect;      /* the capture rect (corresponding to the 
                                            size of the picture captured by a VI device) */
    VI_CAPSEL_E     enCapSel;       /* Frame field selection,this member is applicable only to
                                            the interlaced mode */
    SIZE_S          stDestSize;     /* Size of the target picture */ 
    PIXEL_FORMAT_E  enPixFormat;    /* Formats of storing pixels: sp420 and sp422 */
    HI_BOOL         bMirror;        /* Whether to mirror the picture */
    HI_BOOL         bFilp;          /* Whether to clip the picture */
} VI_CHN_ATTR_S;

typedef struct hiVI_SCALE_CFG_S
{
    HI_BOOL bScaleEn;

    HI_S32  s32DstWidth;    /* the dest width after scale */
    HI_S32  s32DstHeight;   /* the dest height after scale */
} VI_SCALE_CFG_S;

typedef struct hiVI_VPP_CFG_S
{
    HI_S32          VppJobMaxW;
    HI_S32          VppJobMaxH;
    VPP_CFG_S       stVppCfg;

    HI_U32          u32SrcFrmRate;
    HI_U32          u32TarFrmRate;     /* the dest send rate support fraction rate */

    VI_SCALE_CFG_S  stScaleCfg;
}VI_VPP_CFG_S;

typedef struct hiVI_CHN_STAT_S
{
    HI_BOOL bEnable;   /* Whether this channel is enabled */
    HI_U32 u32IntCnt;  /* The video frame interrupt count */
    HI_U32 u32FrmRate; /* current frame rate */ 
    HI_U32 u32LostInt; /* The interrupt is received but nobody care */
    HI_U32 u32VbFail;  /* Video buffer malloc failure */
    HI_U32 u32PicWidth;/* curren pic width */
    HI_U32 u32PicHeight;/* current pic height */
} VI_CHN_STAT_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef__HI_COMM_VIDEO_IN_H__ */


