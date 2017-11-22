/*****************************************************************************
*             Copyright 2006 - 2050, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_api_tde.h
* Description:TDE2 API define
*
* History:
* Version   Date          Author        DefectNum       Description
*
*****************************************************************************/

#ifndef _HI_API_TDE2_H_
#define _HI_API_TDE2_H_

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"
#include "hi_tde_type.h"

#define HI_TDE_Open HI_TDE2_Open
#define HI_TDE_Close HI_TDE2_Close
#define HI_TDE_BeginJob HI_TDE2_BeginJob

/****************************************************************************/
/*                             TDE2 API define                               */
/****************************************************************************/

/*****************************************************************************
* Function:      HI_TDE2_Open
* Description:   Opens the TDE device
* Input:         None
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_Open(HI_VOID);

/*****************************************************************************
* Function:      HI_TDE2_Close
* Description:   Closes the TDE device
* Input:         None
* Output:        None
* Return:        None
* Others:        None
*****************************************************************************/
HI_VOID     HI_TDE2_Close(HI_VOID);

/*****************************************************************************
* Function:      HI_TDE2_BeginJob
* Description:   Creates a TDE job, get a TDE2 job handle
* Input:         None
* Output:        None
* Return:        tde handle / Error code
* Others:        None
*****************************************************************************/
TDE_HANDLE  HI_TDE2_BeginJob(HI_VOID);

/*****************************************************************************
* Function:      HI_TDE2_EndJob
* Description:   Submits a TDE2 job
* Input:         s32Handle:  job handle
*                bSync: if synchronous
*                bBlock: if blocked
*                u32TimeOut: timeout value(in 10ms)
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut);

/*****************************************************************************
* Function:      HI_TDE2_CancelJob
* Description:   Cancels a specific TDE job, only successful before calling EndJob
* Input:         s32Handle:  job handle
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_CancelJob(TDE_HANDLE s32Handle);

/*****************************************************************************
* Function:      HI_TDE2_IsCompleted
* Description:   Waits for a submitted job to finish
* Input:         s32Handle:  job handle
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_WaitForDone(TDE_HANDLE s32Handle);

/*****************************************************************************
* Function:      HI_TDE2_QuickCopy
* Description:   Just quick copy, the size of source region and destination region should be the same, so is the color format
* Input:         s32Handle:  job handle
*                pSrc: the source picture information
*                pstSrcRect: the source picture operation region
*                pDst: the destination picture information
*                pstDstRect: the destination picture operation region
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_QuickCopy(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                              TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect);

/*****************************************************************************
* Function:      HI_TDE2_QuickFill
* Description:   Quick fill
* Input:         s32Handle:  job handle
*                pDst: the destination picture information
*                pstDstRect: the destination picture operation region
*                u32FillData: the color value,its format should be the same to the destination picture
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_QuickFill(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstDst, TDE2_RECT_S *pstDstRect,
                              HI_U32 u32FillData);

/*****************************************************************************
* Function:      HI_TDE2_QuickResize
* Description:   Adds the raster bitmap scaling operation to a TDE job
* Input:         s32Handle:  job handle
*                pSrc: the source picture information
*                pstSrcRect: the source picture operation region
*                pDst: the destination picture information
*                pstDstRect: the destination picture operation region
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_QuickResize(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);

/*****************************************************************************
* Function:      HI_TDE2_QuickFlicker
* Description:   Adds the anti-flicker operation to a TDE job
* Input:         s32Handle:  job handle
*                pSrc: the source picture information
*                pstSrcRect: the source picture operation region
*                pDst: the destination picture information
*                pstDstRect: the destination picture operation region
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_QuickDeflicker(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstSrc, TDE2_RECT_S  *pstSrcRect,
                                   TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect);

/*****************************************************************************
* Function:      HI_TDE2_Blit
* Description:   Adds the transfer operation with additional functions performed on
                    the raster bitmap to a TDE task
* Input:         s32Handle:  job handle
*                pstBackGround: the background picture information
*                pstBackGroundRect: the background picture operation region
*                pstForeGround: the foreground picture information
*                pstForeGroundRect: the foreground picture operation region
*                pstDst:  the destination picture information
*                pstDstRect:  the destination picture operation region
*                pOpt:  operation parameter settings
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_Bitblit(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                            TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect, TDE2_SURFACE_S* pstDst,
                            TDE2_RECT_S  *pstDstRect, TDE2_OPT_S* pstOpt);

/*****************************************************************************
* Function:      HI_TDE2_SolidDraw
* Description:   Adds the filling operation with additional functions performed on
                    the raster bitmap to a TDE task
* Input:         s32Handle:  job handle
*                pstForeGround:  the foreground picture information
*                pstForeGroundRect: the source picture operation region
*                pstDst:  the background picture information
*                pstDstRect: the destination picture operation region
*                pstFillColor:  the color value
*                pstOpt:  operation parameter settings
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_SolidDraw(TDE_HANDLE s32Handle, TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                              TDE2_SURFACE_S *pstDst,
                              TDE2_RECT_S  *pstDstRect, TDE2_FILLCOLOR_S *pstFillColor,
                              TDE2_OPT_S *pstOpt);

/*****************************************************************************
* Function:      HI_TDE2_MbBlit
* Description:   Adds the transfer operation with additional functions performed on
                    the macroblock bitmap to a TDE task
* Input:         s32Handle:  job handle
*                pstMB:  Surface of the macroblock
*                pstDst: Operating region of the macroblock
*                pstDstRect:  the destination picture operation region
*                pstMbOpt: operation parameter settings
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_MbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                           TDE2_MBOPT_S* pstMbOpt);

/*****************************************************************************
* Function:      HI_TDE2_BitmapMaskRop
* Description:   Adds the mask raster operation (ROP) operation performed
                    on the raster bitmap to a TDE task.
* Input:         s32Handle:  job handle
*                pstBackGround:  the background picture information
*                pstBackGroundRect: the background picture operation region
*                pstForeGround: the foreground picture information
*                pstForeGroundRect: the source picture operation region
*                pstMask: mask picture information
*                pstMaskRect: operating region of the mask picture
*                pstDst:  the destination picture information
*                pstDstRect:  the destination picture operation region
*                enRopCode_Color: ROP operation code of the color component
*                enRopCode_Alpha: ROP operation code of the alpha component
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_BitmapMaskRop(TDE_HANDLE s32Handle, 
                                  TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                  TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                  TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect, 
                                  TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                  TDE2_ROP_CODE_E enRopCode_Color, TDE2_ROP_CODE_E enRopCode_Alpha);

/*****************************************************************************
* Function:      HI_TDE2_BitmapMaskBlend
* Description:  Adds the mask blending operation performed on the raster
                    bitmap to a TDE task
* Input:         s32Handle:  job handle
*                pstBackGround: the background picture information
*                pstBackGroundRect: the background picture operation region
*                pstForeGround: the foreground picture information
*                pstForeGroundRect: the foreground picture operation region
*                pstMask: mask picture information
*                pstMaskRect: operating region of the mask picture
*                pstDst:  the destination picture information
*                pstDstRect:  the destination picture operation region
*                u8Alpha:  global alpha value during alpha blending
*                enBlendMode: alpha blending mode
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32      HI_TDE2_BitmapMaskBlend(TDE_HANDLE s32Handle, 
                                    TDE2_SURFACE_S* pstBackGround, TDE2_RECT_S  *pstBackGroundRect,
                                    TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S  *pstForeGroundRect,
                                    TDE2_SURFACE_S* pstMask, TDE2_RECT_S  *pstMaskRect,
                                    TDE2_SURFACE_S* pstDst, TDE2_RECT_S  *pstDstRect,
                                    HI_U8 u8Alpha, TDE2_ALUCMD_E enBlendMode);

/*****************************************************************************
* Function:      HI_TDE2_Osd2Mb
* Description:   Adds the operation of blending raster data to the macroblock
                    bitmap to a task
* Input:         s32Handle:  job handle
*                pstBackGround: background surface (macroblock picture)
*                pstBackGroundRect: operating region of the background
*                pstForeGround: the foreground picture information(raster picture)
*                pstForeGroundRect: the foreground picture operation region
*                pstMbOut: target surface (macroblock picture)
*                pstMbOutRect : target operating region
*                pOpt:  operation parameter settings
* Output:        None
* Return:        Success / Error code
* Others:        None
*****************************************************************************/
HI_S32  HI_TDE2_Osd2Mb(TDE_HANDLE s32Handle, TDE2_MB_S* pstBackGround, TDE2_RECT_S* pstBackGroundRect, 
                                TDE2_SURFACE_S* pstForeGround, TDE2_RECT_S* pstForeGroundRect, 
                                TDE2_MB_S* pstMbOut, TDE2_RECT_S* pstMbOutRect, TDE2_OPT_S* pstOpt);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#endif  /* _HI_API_TDE2_H_ */
