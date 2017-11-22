/******************************************************************************
*
*  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : hi_comm_vpp.h
*  Version       : Initial Draft
*  Author        : j00169368
*  Created       : 2010/12/13
*  Last Modified :
*  Description   : include VPP struct , Marco and Error information
*  Function List :
*
*
*  History:
* 
*       1.  Date         : 2010/12/13
*           Author       : j00169368
*           Modification : Created file
*
******************************************************************************/


#ifndef __HI_COMM_VPP_H__
#define __HI_COMM_VPP_H__

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_errno.h"
#include "hi_defines.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef __KERNEL__ 
	#define HI_TRACE_VPP(level, fmt...)	\
	do{\
		if(level<=HI_DBG_ERR)\
		{\
			//printf("[Func]:%s [Line]:%d [Info]:", __FUNCTION__, __LINE__);\
		    printf(fmt);\
		}\
	} while(0);

#else
#define HI_TRACE_VPP(level, fmt...)\
	do{\
		HI_TRACE(level, HI_ID_VPP, "[%s]: %d,", __FUNCTION__, __LINE__);\
		HI_TRACE(level, HI_ID_VPP, ##fmt);\
	} while(0);
#endif	


/* invlalid device ID */
#define HI_ERR_VPP_INVALID_DEVID     HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_DEVID)
/* invlalid channel ID */
#define HI_ERR_VPP_INVALID_CHNID     HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_INVALID_CHNID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define HI_ERR_VPP_ILLEGAL_PARAM     HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_ILLEGAL_PARAM)
/* channel exists */
#define HI_ERR_VPP_EXIST             HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_EXIST)
/*UN exist*/
#define HI_ERR_VPP_UNEXIST           HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_UNEXIST)
/* using a NULL point */
#define HI_ERR_VPP_NULL_PTR          HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configing attribute */
#define HI_ERR_VPP_NOT_CONFIG        HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_CONFIG)
/* operation is not supported by NOW */
#define HI_ERR_VPP_NOT_SUPPORT      HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define HI_ERR_VPP_NOT_PERM          HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NOT_PERM)
/* failure caused by malloc memory */
#define HI_ERR_VPP_NOMEM             HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NOMEM)
/* failure caused by malloc buffer */
#define HI_ERR_VPP_NOBUF             HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_NOBUF)
/* no data in buffer */
#define HI_ERR_VPP_BUF_EMPTY         HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_EMPTY)
/* no buffer for new data */
#define HI_ERR_VPP_BUF_FULL          HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_BUF_FULL)
/* bad address, eg. used for copy_from_user & copy_to_user */
#define HI_ERR_VPP_BADADDR           HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_BADADDR)
/* resource is busy, eg. destroy a venc chn without unregistering it */
#define HI_ERR_VPP_BUSY              HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_BUSY)

/* System is not ready,maybe not initialed or loaded.
 * Returning the error code when opening a device file failed.
 */
#define HI_ERR_VPP_NOTREADY          HI_DEF_ERR(HI_ID_VPP, EN_ERR_LEVEL_ERROR, EN_ERR_SYS_NOTREADY)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_COMM_VPP_H__ */

