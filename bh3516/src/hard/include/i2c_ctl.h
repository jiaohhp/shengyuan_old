#ifndef __I2C_CTL_H__
#define __I2C_CTL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

int i2c_read_data(	unsigned int device_addr , 
					unsigned int reg_addr ,
					unsigned int sensor_addr_byte,
					unsigned int sensor_data_byte);

int i2c_write_data(	unsigned int device_addr , 
					 	unsigned int reg_addr,
					 	unsigned int reg_value,
					 	unsigned int sensor_addr_byte,
						unsigned int sensor_data_byte);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

