#if !defined(__OV9712_CMOS_H_)
#define __OV9712_CMOS_H_

#include <stdio.h>
#include <string.h>
#include "hi_comm_sns.h"
#include "hi_sns_ctrl.h"
#include "mpi_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * local variables															*
 ****************************************************************************/

static cmos_inttime_t cmos_inttime;
static cmos_gains_t cmos_gains;


static cmos_isp_default_t st_coms_isp_default = 
{
	// color matrix[9]
	{   
		0x019D, 0x801F, 0x807E,
		0x8047, 0x018E, 0x8048,
		0x8016, 0x80AA, 0x01C0
	},


	// black level
	{29,16,16,38},

	//calibration reference color temperature
	5000,

	//WB gain at 5000, must keep consistent with calibration color temperature
	{0x015b, 0x100, 0x100, 0x0199},

	// WB curve parameters, must keep consistent with reference color temperature.
	{127, -23, -152, 154393, 128, -105036},

	// hist_thresh
	{0xd,0x28,0x60,0x80},
	//{0x10,0x40,0xc0,0xf0},

	0x0,	// iridix_balck
	0x3,	// bggr

	/* limit max gain for reducing noise,	 */
	0x1f,	0x1,

	// iridix
	0x04,	0x08,	0xa0,	0x4ff,

	0x1,	// balance_fe
	0x80,	// ae compensation
	0x15,	// sinter threshold

	0x0,  0,  0  //noise profile=0, use the default noise profile lut, don't need to set nr0 and nr1
};


/*
 * This function initialises an instance of cmos_inttime_t.
 */
static __inline cmos_inttime_const_ptr_t cmos_inttime_initialize()
{
	cmos_inttime.full_lines_std = 810;
	cmos_inttime.full_lines_std_30fps = 810;
	cmos_inttime.full_lines = 810;
	cmos_inttime.full_lines_limit = 65535;
	cmos_inttime.max_lines = 806;
	cmos_inttime.min_lines = 2;
	cmos_inttime.max_lines_target = cmos_inttime.max_lines;
	cmos_inttime.min_lines_target = cmos_inttime.min_lines;

	cmos_inttime.vblanking_lines = 0;

	cmos_inttime.exposure_ashort = 0;
	cmos_inttime.exposure_shift = 0;

	cmos_inttime.lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2; 
	cmos_inttime.flicker_freq = 0;//60*256;//50*256;

	return &cmos_inttime;
}

/*
 * This function applies the new integration time to the ISP registers.
 */
static __inline void cmos_inttime_update(cmos_inttime_ptr_t p_inttime)
{
	HI_U32 _curr = p_inttime->exposure_ashort;

	_curr = ((_curr >= 760) && (_curr < 768))? 768: _curr;
 
	//refresh the sensor setting every frame to avoid defect pixel error
	sensor_write_register(0x10, _curr&0xFF);
	sensor_write_register(0x16, (_curr>>8)&0xFF);

	return;
}

/*
 * This function initialises an instance of cmos_gains_t.
 */
static __inline cmos_gains_ptr_t cmos_gains_initialize()
{
	cmos_gains.max_again = 496;
	cmos_gains.max_dgain = 1;

	cmos_gains.again_shift = 4;
	cmos_gains.dgain_shift = 0;
	cmos_gains.dgain_fine_shift = 0;

	return &cmos_gains;
}

/*
 * This function applies the new gains to the ISP registers.
 */
static __inline void cmos_gains_update(cmos_gains_const_ptr_t p_gains)
{
    sensor_write_register(0x00, p_gains->again_db);

	return;
}


static __inline HI_U32 analog_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
    HI_U32 _again = 1 << p_gains->again_shift;
    HI_U32 _again_new;
    int shift = 0;
    HI_U8 i,j;

    while (exposure > (1<<20))
    {
        exposure >>= 1;
        exposure_max >>= 1;
        ++shift;
    }

    for(i = 0;i < 5; i++)
    {
        for(j = 0; j < 16; j++)
        {
            _again_new = (16 + j) << i;

            if(_again_new > p_gains->max_again_target)
            {                
                goto AGAIN_CALCULATE_LOOP_END;
            }

            _again = _again_new;

            if(((exposure_max * _again) >> 4) >= exposure)
            {
                goto AGAIN_CALCULATE_LOOP_END;
            }
        }
    }

    /* revert i&j to their max values */
    i--;
    j--;
    AGAIN_CALCULATE_LOOP_END:

    p_gains->again = _again;
    p_gains->again_db = (1 << (i + 4)) + j - 16;

	return (exposure << (shift + 4)) / _again;
    
}

static __inline HI_U32 digital_gain_from_exposure_calculate(
		cmos_gains_ptr_t p_gains,
		HI_U32 exposure,
		HI_U32 exposure_max,
		HI_U32 exposure_shift)
{
	p_gains->dgain = 1;

	return exposure;
}
		
/* Set fps base */
static __inline void cmos_fps_set(
		cmos_inttime_ptr_t p_inttime,
		const HI_U8 fps
		)
{
	switch(fps)
	{
		case 30:
			p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 30 / 2; 
			sensor_write_register(0x2a, 0x98);
			sensor_write_register(0x2b, 0x6);
		break;

		case 25:
			/* do not change full_lines_std */
			p_inttime->lines_per_500ms = cmos_inttime.full_lines_std_30fps * 25 / 2;
			sensor_write_register(0x2a, 0xe8);
			sensor_write_register(0x2b, 0x7);
		break;

		default:
		break;
	}
	
	return;
}
		
static __inline HI_U16 vblanking_calculate(
		cmos_inttime_ptr_t p_inttime)
{
	if(p_inttime->exposure_ashort >= p_inttime->full_lines - 4)
	{
		p_inttime->exposure_ashort = p_inttime->full_lines - 4;
	}

	p_inttime->vblanking_lines = p_inttime->full_lines - p_inttime->full_lines_std_30fps;

	return p_inttime->exposure_ashort;
}
		
/*
 * This function applies the new vert blanking porch to the ISP registers.
 */
static __inline void cmos_vblanking_update(cmos_inttime_const_ptr_t p_inttime)
{
	sensor_write_register(0x2d, p_inttime->vblanking_lines & 0xff);
	sensor_write_register(0x2e, (p_inttime->vblanking_lines & 0xff00) >> 8);

	return;
}


static void setup_sensor(int isp_mode)
{
	if(0 == isp_mode) /* ISP 'normal' isp_mode */
	{
	}
	else if(1 == isp_mode) /* ISP pixel calibration isp_mode */
	{
		sensor_write_register(0x350C, 0x15);
		sensor_write_register(0x350D, 0x90);
		sensor_write_register(0x3500, 0x01);  
		sensor_write_register(0x3501, 0x80);
		sensor_write_register(0x3502, 0x00);
		sensor_write_register(0x350A, 0x00);
		sensor_write_register(0x350B, 0x00);
	}
}

static HI_U32 cmos_gains_lin_to_db_convert(HI_U32 data, HI_U32 shift_in)
{
    #define PRECISION 8
    HI_U32 _res = 0;
    if(0 == data)
        return _res;

    data = data << PRECISION; // to ensure precision.
    for(;;)
    {
        /* Note to avoid endless loop here. */
        data = (data * 913) >> 10;
        // data = (data*913 + (1<<9)) >> 10; // endless loop when shift_in is 0. */
        if(data <= ((1<<shift_in) << PRECISION))
        {
            break;
        }
        ++_res;
    }
    return _res;
}

static HI_U8 cmos_get_analog_gain(cmos_gains_ptr_t p_gains)
{
    return cmos_gains_lin_to_db_convert(p_gains->again, p_gains->again_shift);
}

static HI_U8 cmos_get_digital_gain(cmos_gains_ptr_t p_gains)
{
    return 0;
}

static __inline HI_U32 cmos_get_ISO(cmos_gains_ptr_t p_gains)
{
	HI_U32 _again = p_gains->again == 0 ? 1 : p_gains->again;
	HI_U32 _dgain = p_gains->dgain == 0 ? 1 : p_gains->dgain;

	p_gains->iso =  ((_again * _dgain * 100) >> (p_gains->again_shift + p_gains->dgain_shift));

	return p_gains->iso;
}

static HI_U32 cmos_get_isp_default(cmos_isp_default_ptr_t p_coms_isp_default)
{
	if (NULL == p_coms_isp_default)
	{
	    printf("null pointer when get isp default value!\n");
	    return -1;
	}
    memcpy(p_coms_isp_default, &st_coms_isp_default, sizeof(cmos_isp_default_t));
    return 0;
}


/****************************************************************************
 * callback structure                                                       *
 ****************************************************************************/

SENSOR_EXP_FUNC_S stSensorExpFuncs =
{
    .pfn_cmos_inttime_initialize = cmos_inttime_initialize,
    .pfn_cmos_inttime_update = cmos_inttime_update,

    .pfn_cmos_gains_initialize = cmos_gains_initialize,
    .pfn_cmos_gains_update = cmos_gains_update,
    .pfn_cmos_gains_update2 = NULL,
    .pfn_analog_gain_from_exposure_calculate = analog_gain_from_exposure_calculate,
    .pfn_digital_gain_from_exposure_calculate = digital_gain_from_exposure_calculate,

    .pfn_cmos_fps_set = cmos_fps_set,
    .pfn_vblanking_calculate = vblanking_calculate,
    .pfn_cmos_vblanking_front_update = cmos_vblanking_update,

    .pfn_setup_sensor = setup_sensor,

	.pfn_cmos_get_analog_gain = cmos_get_analog_gain,
	.pfn_cmos_get_digital_gain = cmos_get_digital_gain,
	.pfn_cmos_get_digital_fine_gain = NULL,
    .pfn_cmos_get_iso = cmos_get_ISO,

	.pfn_cmos_get_isp_default = cmos_get_isp_default,
	.pfn_cmos_get_isp_special_alg = NULL,
};


int OV9712SensorRegCallback(void)
{
	int ret;
	ret = HI_MPI_ISP_SensorRegCallBack(&stSensorExpFuncs);
	if (ret)
	{
	    printf("sensor register callback function failed!\n");
	    return ret;
	}
	
	return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // __OV9712_CMOS_H_

