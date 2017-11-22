/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_comm_sns.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   : 
  History       :
  1.Date        : 2011/01/05
    Author      : x00100808
    Modification: Created file

******************************************************************************/

#ifndef __HI_COMM_SNS_H__
#define __HI_COMM_SNS_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


typedef struct cmos_inttime_
{
	/* Public */
	HI_U16 exp_ratio;
	HI_U16 vblanking_lines;
	HI_U32 exposure_ashort;
	HI_U32 exposure_along;
	HI_U32 flicker_freq;
	HI_U8  exposure_shift;

	/* Private */
	HI_U16 full_lines_std;
	HI_U16 full_lines_std_30fps;
	HI_U16 full_lines_std_25fps;
	HI_U16 full_lines;
	HI_U16 full_lines_del;
	HI_U16 full_lines_limit;
	HI_U16 exp_ratio_target;
	HI_U16 max_lines_target;
	HI_U16 max_lines;
	HI_U16 min_lines_target;
	HI_U16 min_lines;
	HI_U16 lines_per_500ms;
} cmos_inttime_t;

typedef struct cmos_inttime_ * cmos_inttime_ptr_t;
typedef struct cmos_inttime_ * cmos_inttime_const_ptr_t;


typedef struct cmos_gains_
{
	/* Public */
	HI_U32 dgain;
	HI_U32 again;
	HI_U32 iso;
	HI_U32 dgain_db;
	HI_U32 again_db;
	HI_U32 dgain_fine;
	HI_U16 out_offset_1;
	HI_U16 out_offset_2;

	/* Private */
	HI_U32 max_again_target;
	HI_U32 max_dgain_target;
	HI_U32 max_again;
	HI_U32 max_dgain;
	HI_U8  again_shift;
	HI_U8  dgain_shift;
	HI_U8  dgain_fine_shift;
	HI_U32 dgain_offset;
	HI_U32 max_dgain_step;
} cmos_gains_t;

typedef struct cmos_gains_ * cmos_gains_ptr_t;
typedef const struct cmos_gains_ * cmos_gains_const_ptr_t;


typedef struct cmos_isp_default_
{
	HI_U16 color_matrix[9];		// color matrix

	HI_U16 black_level[4];		// black level

	HI_U16 wb_ref_temp;          //reference color temperature for WB 
	
	HI_U16 gain_offset[4];		// gain offset for white balance


	HI_S32 wb_para[6];          //parameter for wb curve   
	
	HI_U8 hist_thresh[4];		// hist threshold

	HI_U8 iridix_black;
	HI_U8 rggb;					// rggb start sequence

	HI_U16 again_max;
	HI_U16 dgain_max;

	HI_U8 iridix_vs;			// variance space
	HI_U8 iridix_vi;			// variance intensity
	HI_U8 iridix_sm;			// slope max
	HI_U16 iridix_wl;			// white level

	HI_U8 balance_fe;
	HI_U8 ae_compensation;		// ae compensation
	HI_U8 sinter_thresh;		// sinter threshold
	
    //add by z54137. We support two methods to generate noise profile lut. 
    //noise_profile = 0 is for original one, noise_profile = 1 is for latest one. 
	HI_U8 noise_profile;        //two different noise profile 
	HI_U16 nr0;		            //nr0 for noise profile 2
	HI_U16 nr1;		            //nr1 for noise profile 2
	
} cmos_isp_default_t;

typedef struct cmos_isp_default_ * cmos_isp_default_ptr_t;

typedef enum coms_isp_special_alg_
{
	isp_special_alg_0 = 0,		// for altasens only
    isp_special_alg_m034_lin,   // for aptm034 linear only
	isp_special_alg_m034_wdr,   // for aptm034 wdr only
	isp_special_alg_ar0331_lin,   // for aptar0331 linear only
	isp_special_alg_ar0331_wdr,   // for aptar0331 wdr only
	isp_special_alg_awb,  // for sony only
	isp_special_alg_butt
} coms_isp_special_alg_e;


/* Sensor Control Operation */

typedef struct hiSENSOR_EXP_FUNC_S
{
    cmos_inttime_const_ptr_t (*pfn_cmos_inttime_initialize)(void);
    void (*pfn_cmos_inttime_update)				(cmos_inttime_ptr_t p_inttime);
    
    cmos_gains_ptr_t (*pfn_cmos_gains_initialize)(void);
    void (*pfn_cmos_gains_update)						(cmos_gains_const_ptr_t p_gains);
    HI_U16 (*pfn_cmos_gains_update2)					(cmos_gains_const_ptr_t p_gains);
    HI_U32 (*pfn_analog_gain_from_exposure_calculate)	(cmos_gains_ptr_t p_gains, HI_U32 exposure, HI_U32 exposure_max, HI_U32 exposure_shift);
    HI_U32 (*pfn_digital_gain_from_exposure_calculate)	(cmos_gains_ptr_t p_gains, HI_U32 exposure, HI_U32 exposure_max, HI_U32 exposure_shift);

    void (*pfn_cmos_fps_set)				(cmos_inttime_ptr_t p_inttime, const HI_U8 fps);
    HI_U16 (*pfn_vblanking_calculate)		(cmos_inttime_ptr_t p_inttime);
    void (*pfn_cmos_vblanking_front_update)	(cmos_inttime_const_ptr_t p_inttime);

    void (*pfn_setup_sensor)(int isp_mode);
    HI_U8 (*pfn_cmos_get_analog_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U8 (*pfn_cmos_get_digital_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U8 (*pfn_cmos_get_digital_fine_gain)(cmos_gains_ptr_t cmos_gains);
    HI_U32 (*pfn_cmos_get_iso)(cmos_gains_ptr_t cmos_gains);

    HI_U32 (*pfn_cmos_get_isp_default)(cmos_isp_default_ptr_t p_coms_isp_default);
    HI_U32 (*pfn_cmos_get_isp_special_alg)(void);
    
    // ...
    
} SENSOR_EXP_FUNC_S;




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /*__HI_COMM_SNS_H__ */

