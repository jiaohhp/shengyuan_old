#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "i2c_ctl.h"


//addr_len 单位为字节
//data_len 单位为字节
typedef struct hiI2C_DATA_S
{
	unsigned char dev_addr; 
	unsigned int reg_addr; 
	unsigned int addr_byte; 
	unsigned int data; 
	unsigned int data_byte;
    
} I2C_DATA_S;

#define I2C_CMD_WRITE      	0x01
#define I2C_CMD_READ       	0x03

int i2c_read_data(	unsigned int device_addr , 
					unsigned int reg_addr ,
					unsigned int sensor_addr_byte,
					unsigned int sensor_data_byte)
{
	int fd = open("/dev/i2c", 0);
	if(fd < 0)
	{
		//printf("Open i2c device error!\n");
		return -1;
	}
	
	I2C_DATA_S i2c_data;
	i2c_data.dev_addr 	= device_addr;
	i2c_data.reg_addr 	= reg_addr;
	i2c_data.addr_byte 	= sensor_addr_byte;
	i2c_data.data_byte 	= sensor_data_byte;

	int ret = ioctl(fd, I2C_CMD_READ, &i2c_data);
	if(ret)
	{  
		//printf("i2c read failed!\n");
		return -1 ;
	}

	unsigned int reg_value = i2c_data.data;

	//printf("read: device_addr:0x%x; reg_addr:0x%x; reg_value:0x%x.\n", device_addr, reg_addr, reg_value);

	close(fd);

	return reg_value;
}

int i2c_write_data(	unsigned int device_addr , 
					 	unsigned int reg_addr,
					 	unsigned int reg_value,
					 	unsigned int sensor_addr_byte,
						unsigned int sensor_data_byte)
{
	int fd = -1;
	int ret = 0;
	I2C_DATA_S i2c_data;	

	fd = open("/dev/i2c", 0);
	if(fd < 0)
	{
		//printf("Open i2c device error!\n");
		return -1;
	}

	i2c_data.dev_addr 	= device_addr;
	i2c_data.reg_addr 	= reg_addr;
	i2c_data.addr_byte 	= sensor_addr_byte;
	i2c_data.data 		= reg_value;
	i2c_data.data_byte 	= sensor_data_byte;

	////printf("write: device_addr:0x%2x; reg_addr:0x%2x; reg_value:0x%2x.\n", device_addr, reg_addr, reg_value);

	ret = ioctl(fd, I2C_CMD_WRITE, &i2c_data);
	if(ret)
	{  
		//printf("i2c write failed!\n");
		return -1 ;
	}

	close(fd);

	return 0;
}

