/******************************************************************************
  Hisilicon HI3516 sample programs head file.

  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-2 Created
******************************************************************************/

#ifndef __SAMPLE_COMM_H__
#define __SAMPLE_COMM_H__

#include "hi_common.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "hi_comm_vb.h"
#include "mpi_vb.h"
#include "hi_sns_ctrl.h"
#include "hi_comm_isp.h"
#include "mpi_isp.h"
#include "hi_comm_vi.h"
#include "mpi_vi.h"
#include "hi_comm_vo.h"
#include "mpi_vo.h"
#include "hi_comm_venc.h"
#include "mpi_venc.h"
#include "hi_comm_region.h"
#include "mpi_region.h"
#include "hi_comm_vda.h"
#include "mpi_vda.h"
#include "hi_comm_adec.h"
#include "mpi_adec.h"
#include "hi_comm_aenc.h"
#include "mpi_aenc.h"
#include "hi_comm_ai.h"
#include "mpi_ai.h"
#include "hi_comm_ao.h"
#include "mpi_ao.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* if you want to load isp_reg_file, then define SAMPLE_LOAD_ISPREGFILE */
//#define SAMPLE_LOAD_ISPREGFILE
#define SAMPLE_ISPCFG_FILE      "../ispcfg/ispreg.cfg"

#define SAMPLE_GLOBAL_NORM	    VIDEO_ENCODING_MODE_PAL
#define SAMPLE_SYS_ALIGN_WIDTH      16

#define SAMPLE_VO_DEV_HD            0
#define SAMPLE_VO_DEV_SD            2

/* for init global parameter */
#define SAMPLE_ENABLE 		    1
#define SAMPLE_DISABLE 		    0
#define SAMPLE_NOUSE 		    -1

typedef enum sample_rc_e
{
    SAMPLE_RC_CBR = 0,
    SAMPLE_RC_VBR,
    SAMPLE_RC_FIXQP
}SAMPLE_RC_E;

typedef enum sample_ispcfg_opt_e
{
    CFG_OPT_NONE	= 0,
    CFG_OPT_SAVE	= 1,
    CFG_OPT_LOAD	= 2,
    CFG_OPT_BUTT
}SAMPLE_CFG_OPT_E;

typedef enum sample_vi_dev_type_e
{
    APTINA_9P031_DC_1080P_CONTINUES = 0,
    APTINA_9P031_DC_1080P_DEFAULT,
    ALTASENS_DC_1080P,
    OV_2715_DC_1080P,
    OV_9712_DC_1280_800P,
    OV_2640_DC_800_600P,
    TX_15FPS,
    TX_30FPS,
    OV_10630_DC_800P,
    PANASONIC_34041_1080P,
    PANASONIC_34041_1080P_60FPS,
    SONY_IMX036_DC_1080P,
    SONY_IMX122_DC_1080P,
    APTINA_9M034_DC_720P_CONTINUES,
    APTINA_AR0331_DC_1080P_30FPS,
    APTINA_AR0331_DC_1080P_60FPS,
    APTINA_AR0331_DC_3M_30FPS,
    
    OV_2715_BT601_1080P,
    BT656_D1P,
    BT1120_720P,
    BT1120_1080P
}SAMPLE_VI_DEV_TYPE_E;

typedef enum sample_vi_chn_set_e
{
    VI_CHN_SET_NORMAL = 0, /* mirror, filp close */
    VI_CHN_SET_MIRROR,      /* open MIRROR */
    VI_CHN_SET_FILP		/* open filp */
}SAMPLE_VI_CHN_SET_E;

typedef enum sample_osd_change_type_e
{
    OSD_CHANGE_TYPE_FGALPHA = 0,
    OSD_CHANGE_TYPE_BGALPHA,
    OSD_CHANGE_TYPE_LAYER
}SAMPLE_OSD_CHANGE_TYPE_EN;

typedef struct sample_venc_getstream_s
{
     HI_BOOL bThreadStart;
     HI_S32  s32Cnt;
}SAMPLE_VENC_GETSTREAM_PARA_S;

#define BH_DEST_WIDTH		(480)
#define BH_DEST_HEIGHT		(480)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
