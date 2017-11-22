#ifndef _ACODEC_H_
#define _ACODEC_H_

#define IOC_TYPE_ACODEC 'A'

typedef enum hiACODEC_IOCTL_E
{    
    IOC_NR_SOFT_RESET_CTRL = 0x0,       /*reset the audio code to the default config*/
    IOC_NR_SET_I2S1_FS,
    IOC_NR_SET_MIXER_MIC,               /*select the micpga's input,micin or linein*/
    IOC_NR_SEL_DAC_CLK,                 /*clock of dac and adc is reverse or obverse*/
    IOC_NR_SEL_ANA_MCLK,                /*clock of analog part and digital part is reverse or obverse*/
    IOC_NR_SET_GAIN_MICL,
    IOC_NR_SET_GAIN_MICR,
    IOC_NR_SET_DACL_VOL,
    IOC_NR_SET_DACR_VOL,
    IOC_NR_SET_ADCL_VOL,
    IOC_NR_SET_ADCR_VOL,
    IOC_NR_SET_MICL_MUTE,
    IOC_NR_SET_MICR_MUTE,        
    IOC_NR_SET_DACL_MUTE,        
    IOC_NR_SET_DACR_MUTE,        
    IOC_NR_DAC_SOFT_MUTE,        
    IOC_NR_DAC_SOFT_UNMUTE,      
    
    IOC_NR_GET_GAIN_MICL,
    IOC_NR_GET_GAIN_MICR,
    IOC_NR_GET_DACL_VOL,
    IOC_NR_GET_DACR_VOL,
    IOC_NR_GET_ADCL_VOL,
    IOC_NR_GET_ADCR_VOL,
    
    IOC_NR_DACL_SEL_TRACK,       
    IOC_NR_DACR_SEL_TRACK,       
    IOC_NR_ADCL_SEL_TRACK,       
    IOC_NR_ADCR_SEL_TRACK,       
    IOC_NR_SET_DAC_DE_EMPHASIS,  
    IOC_NR_SET_ADC_HP_FILTER,    
    IOC_NR_DAC_POP_FREE,         
    IOC_NR_DAC_SOFT_MUTE_RATE,   
    
    IOC_NR_DAC_SEL_I2S,          
    IOC_NR_ADC_SEL_I2S,          
    IOC_NR_SET_I2S1_DATAWIDTH,   
    IOC_NR_SET_I2S2_DATAWIDTH,   
    IOC_NR_SET_I2S2_FS,          
    IOC_NR_SET_DACR2DACL_VOL,
    IOC_NR_SET_DACL2DACR_VOL,
    IOC_NR_SET_ADCR2DACL_VOL,
    IOC_NR_SET_ADCL2DACR_VOL,
    IOC_NR_SET_ADCR2DACR_VOL,
    IOC_NR_SET_ADCL2DACL_VOL,
}ACODEC_IOCTL_E;

#define ACODEC_SOFT_RESET_CTRL      _IOWR(IOC_TYPE_ACODEC, IOC_NR_SOFT_RESET_CTRL, ACODEC_CTRL)   
#define ACODEC_SET_I2S1_FS          _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S1_FS, ACODEC_CTRL)
#define ACODEC_SET_MIXER_MIC        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MIXER_MIC, ACODEC_CTRL)
#define ACODEC_SET_GAIN_MICL        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_GAIN_MICL, ACODEC_CTRL)
#define ACODEC_SET_GAIN_MICR        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_GAIN_MICR, ACODEC_CTRL)
#define ACODEC_SET_DACL_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACL_VOL, ACODEC_CTRL)
#define ACODEC_SET_DACR_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACR_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCL_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCL_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCR_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCR_VOL, ACODEC_CTRL)
#define ACODEC_SET_MICL_MUTE        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MICL_MUTE, ACODEC_CTRL)
#define ACODEC_SET_MICR_MUTE        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MICR_MUTE, ACODEC_CTRL)
#define ACODEC_SET_DACL_MUTE        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACL_MUTE, ACODEC_CTRL)
#define ACODEC_SET_DACR_MUTE        _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACR_MUTE, ACODEC_CTRL)       
#define ACODEC_DAC_SOFT_MUTE        _IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_MUTE, ACODEC_CTRL)
#define ACODEC_DAC_SOFT_UNMUTE      _IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_UNMUTE, ACODEC_CTRL)

#define ACODEC_GET_GAIN_MICL        _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_GAIN_MICL, ACODEC_CTRL)
#define ACODEC_GET_GAIN_MICR        _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_GAIN_MICR, ACODEC_CTRL)
#define ACODEC_GET_DACL_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_DACL_VOL, ACODEC_CTRL)
#define ACODEC_GET_DACR_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_DACR_VOL, ACODEC_CTRL)
#define ACODEC_GET_ADCL_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_ADCL_VOL, ACODEC_CTRL)
#define ACODEC_GET_ADCR_VOL         _IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_ADCR_VOL, ACODEC_CTRL)

/* Don't need to set, the driver will set a default value */
#define ACODEC_SEL_DAC_CLK          _IOWR(IOC_TYPE_ACODEC, IOC_NR_SEL_DAC_CLK, ACODEC_CTRL)
#define ACODEC_SEL_ANA_MCLK         _IOWR(IOC_TYPE_ACODEC, IOC_NR_SEL_ANA_MCLK, ACODEC_CTRL)
#define ACODEC_DACL_SEL_TRACK       _IOWR(IOC_TYPE_ACODEC, IOC_NR_DACL_SEL_TRACK, ACODEC_CTRL)       
#define ACODEC_DACR_SEL_TRACK       _IOWR(IOC_TYPE_ACODEC, IOC_NR_DACR_SEL_TRACK, ACODEC_CTRL)       
#define ACODEC_ADCL_SEL_TRACK       _IOWR(IOC_TYPE_ACODEC, IOC_NR_ADCL_SEL_TRACK, ACODEC_CTRL)       
#define ACODEC_ADCR_SEL_TRACK       _IOWR(IOC_TYPE_ACODEC, IOC_NR_ADCR_SEL_TRACK, ACODEC_CTRL) 
#define ACODEC_SET_DAC_DE_EMPHASIS  _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DAC_DE_EMPHASIS, ACODEC_CTRL)
#define ACODEC_SET_ADC_HP_FILTER    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADC_HP_FILTER, ACODEC_CTRL)
#define ACODEC_DAC_POP_FREE         _IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_POP_FREE, ACODEC_CTRL)
#define ACODEC_DAC_SOFT_MUTE_RATE   _IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_MUTE_RATE, ACODEC_CTRL)

/* Reserved ioctl cmd */
#define ACODEC_DAC_SEL_I2S          _IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SEL_I2S, ACODEC_CTRL)          
#define ACODEC_ADC_SEL_I2S          _IOWR(IOC_TYPE_ACODEC, IOC_NR_ADC_SEL_I2S, ACODEC_CTRL)       
#define ACODEC_SET_I2S1_DATAWIDTH   _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S1_DATAWIDTH, ACODEC_CTRL)   
#define ACODEC_SET_I2S2_DATAWIDTH   _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S2_DATAWIDTH, ACODEC_CTRL)          
#define ACODEC_SET_I2S2_FS          _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S2_FS, ACODEC_CTRL)
#define ACODEC_SET_DACR2DACL_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACR2DACL_VOL, ACODEC_CTRL)
#define ACODEC_SET_DACL2DACR_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACL2DACR_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCL2DACL_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCL2DACL_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCR2DACL_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCR2DACL_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCL2DACR_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCL2DACR_VOL, ACODEC_CTRL)
#define ACODEC_SET_ADCR2DACR_VOL    _IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCR2DACR_VOL, ACODEC_CTRL)

typedef enum hiACODEC_FS_E
{
    ACODEC_FS_48000 =   0x1a,
    ACODEC_FS_24000 =   0x19,
    ACODEC_FS_12000 =   0x18,

    ACODEC_FS_44100 =   0x1a,
    ACODEC_FS_22050 =   0x19,
    ACODEC_FS_11025 =   0x18,

    ACODEC_FS_32000 =   0x1a,
    ACODEC_FS_16000 =   0x19,
    ACODEC_FS_8000  =   0x18,

    ACODEC_FS_BUTT,
} ACODEC_FS_E;

typedef enum hiACODEC_MIXER_E
{
    ACODEC_MIXER_MUTE   = 0x0,
    ACODEC_MIXER_LINEIN = 0x1,
    ACODEC_MIXER_MICIN  = 0x2,

    ACODEC_MIXER_BUTT,
} ACODEC_MIXER_E;

typedef struct 
{
    unsigned int vol_ctrl:7;
    unsigned int vol_ctrl_mute:1;           /*volume control*/

    unsigned int soft_mute_en:1;            /*soft mute control*/
    unsigned int soft_unmute_en:1;
    unsigned int soft_mute_rate:2;
    
    unsigned int adc_hpf_en:1;              /*high frequency filter*/
    unsigned int dac_deemphasis:2;          /*de-emphasis control*/

    unsigned int i2s_select:1;              /*select i2s interface,just support i2s1 now*/
    unsigned int track_select:1;            /*select track for adc or dac*/
    unsigned int i2s_data_bits:2;
    unsigned int i2s_fs_sel:5;              /*value is enum ACODEC_FS_E*/
    
    unsigned int mute_ctrl:1;               /*mute control of adc/dac/micpga*/
    unsigned int mixer_mic_ctrl:2;          /*select micin or linein, value is enum ACODEC_MIXER_E*/
    
    unsigned int clk_sel:1;
    unsigned int gain_mic:5;                /*the gain of micpga, value is 0~0x1f*/
    unsigned int pop_dac_ctrl:1;
}ACODEC_CTRL;

#endif /* End of #ifndef _ACODEC_H_ */
