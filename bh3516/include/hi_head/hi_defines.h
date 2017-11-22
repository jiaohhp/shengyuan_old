/******************************************************************************
 Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_defines.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2005/4/23
Last Modified :
Description   : The common data type defination
Function List :
History       :
1.Date        :   2009/03/03
  Author      :   z44949
  Modification:   Create the file

2.Date        :   2009/07/01
  Author      :   z44949
  Modification:   Move MPP_VER_PRIX from hi_comm.h

******************************************************************************/
#ifndef __HI_DEFINES_H__
#define __HI_DEFINES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef HICHIP
    #define HICHIP HI3516_V100
#endif

#if HICHIP==HI3516_V100
    #define CHIP_NAME    "Hi3516"
    #define MPP_VER_PRIX "_MPP_V"
#elif HICHIP==HI35xx_Vxxx
    #error HuHu, I am an dummy chip
#else
    #error CHIP define may be error
#endif

#define DEFAULT_ALIGN           16

/* For VIU */
#define VIU_MAX_DEV_NUM         2
#define VIU_MAX_WAY_NUM_PER_DEV 1
#define VIU_MAX_CHN_NUM         16
#define VIU_MAX_PHYCHN_NUM      5
#define VIU_CHNID_DEV_FACTOR    2
#define VIU_USERFRM_TIMEOUT     10

/* For VENC */
#define VENC_PIC_ALIGN          16
#define VENC_MAX_CHN_NUM        64
#define VENC_MAX_GRP_NUM        64
#define H264E_MAX_WIDTH         5632
#define H264E_MAX_HEIGHT        4224
#define H264E_MIN_WIDTH         112
#define H264E_MIN_HEIGHT        64
#define MPEG4E_MAX_WIDTH        1280
#define MPEG4E_MAX_HEIGHT       720
#define MPEG4E_MIN_WIDTH        112
#define MPEG4E_MIN_HEIGHT       64
#define JPEGE_MAX_WIDTH         8192
#define JPEGE_MAX_HEIGHT        8192
#define JPEGE_MIN_WIDTH         32
#define JPEGE_MIN_HEIGHT        32
#define VENC_MAX_ROI_NUM        8
#define H264E_MIN_HW_INDEX      0
#define H264E_MAX_HW_INDEX      9
#define H264E_MIN_VW_INDEX      0
#define H264E_MAX_VW_INDEX      4
#define MPEG4E_MAX_HW_INDEX     1
#define MPEG4E_MAX_VW_INDEX     0

/* For VPP */
#define VPP_DN_MAX_COS_STH      3
#define VPP_DN_MAX_INC_STH      255
#define VPP_DN_MAX_TFY_STH      4
#define VPP_SP_MAX_STH          5
#define VPP_SP_MIN_STH          -4
#define VPP_PIC_ALIGN           16

/* For Region */
#define RGN_HANDLE_MAX          1024
#define OVERLAY_MAX_NUM         8
#define VICOVER_MAX_NUM         8
#define OVERLAYEX_MAX_NUM       16

/* For VDA */
#define VDA_MAX_NODE_NUM        16
#define VDA_MAX_INTERNAL        256
#define VDA_CHN_NUM_MAX         16
#define VDA_MAX_WIDTH           960
#define VDA_MAX_HEIGHT          576

/* For VOU */
#define VO_MAX_DEV_NUM          6       /* we have three VO physical device(HD,AD,SD) and three virtual device(VD1,VD2,VD3) */
#define VO_MAX_PHY_DEV          3       /* max physical device number(HD,AD,SD) */
#define VO_MAX_CHN_NUM          64      /* max channel number of each device */
#define VO_CSCD_MAX_PAT         128     /* cascade pattern max number */
#define VO_CSCD_MAX_POS         32      /* cascade position max number */
#define VO_SYNC_MAX_GRP         16      /* we limit total sync group as 16 on three device */
#define VO_SYNC_MAX_CHN         64      /* each sync group can accommodate 64 channels */
#define VO_MIN_CHN_WIDTH        16      /* channel minimal width */
#define VO_MIN_CHN_HEIGHT       16      /* channel minimal height */
#define VO_MIN_TOLERATE         1       /* min play toleration 1ms */
#define VO_MAX_TOLERATE         100000  /* max play toleration 100s */
#define VO_MAX_SOLIDDRAW        128     /* max draw region number */
#define VO_MAX_ZOOM_RATIO       1000    /* max zoom ratio, 1000 means 100% scale */
#define VO_MIN_DISP_BUF         5       /* min display buffer number */
#define VO_MAX_DISP_BUF         15      /* max display buffer number */
#define VO_MIN_VIRT_BUF         3       /* min virtual device buffer number */
#define VO_MAX_VIRT_BUF         15      /* max virtual device buffer number */

/* For AUDIO */
#define SIO_MAX_NUM             3
#define AIO_MAX_CHN_NUM         16
#define AENC_MAX_CHN_NUM        32
#define ADEC_MAX_CHN_NUM        32


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_DEFINES_H__ */

