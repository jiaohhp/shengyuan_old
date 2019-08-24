#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef HI_GPIO_I2C
#include "gpio_i2c.h"
#else
#include "i2c.h"
#endif

static const unsigned int sensor_i2c_addr	=	0x60;		/* I2C Address of OV9712 */
static const unsigned int sensor_addr_byte	=	1;
static const unsigned int sensor_data_byte	=	1;


int sensor_read_register(int addr)
{
#ifdef HI_GPIO_I2C
    int fd = -1;
    int ret;
    int value;

    fd = open("/dev/gpioi2c", 0);
    if(fd<0)
    {
        printf("Open gpioi2c error!\n");
        return -1;
    }

    value = ((sensor_i2c_addr&0xff)<<24) | ((addr&0xff)<<16);

    ret = ioctl(fd, GPIO_I2C_READ, &value);
    if (ret)
    {
        printf("GPIO-I2C read faild!\n");
        return -1;
    }

    value &= 0xff;

    close(fd);
    return value;
#else
    int fd = -1;
    int ret;
    I2C_DATA_S i2c_data;

    fd = open("/dev/i2c", 0);
    if(fd<0)
    {
        printf("Open i2c error!\n");
        return -1;
    }

    i2c_data.dev_addr = sensor_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte = sensor_addr_byte;
    i2c_data.data_byte = sensor_data_byte;

    ret = ioctl(fd, I2C_CMD_READ, &i2c_data);

    if (ret)
    {
        printf("i2c read faild!\n");
        return -1;
    }

    close(fd);
	return i2c_data.data;
#endif

}

int sensor_write_register(int addr, int data)
{
#ifdef HI_GPIO_I2C
    int fd = -1;
    int ret;
    int value;

    fd = open("/dev/gpioi2c", 0);
    if(fd<0)
    {
        printf("Open gpioi2c error!\n");
        return -1;
    }

    value = ((sensor_i2c_addr&0xff)<<24) | ((addr&0xff)<<16) | (data&0xff);

    ret = ioctl(fd, GPIO_I2C_WRITE, &value);

    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return -1;
    }

    close(fd);
#else
    int fd = -1;
    int ret;
    I2C_DATA_S i2c_data;

    fd = open("/dev/i2c", 0);
    if(fd<0)
    {
        printf("Open i2c error!\n");
        return -1;
    }

    i2c_data.dev_addr = sensor_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte = sensor_addr_byte;
    i2c_data.data = data;
    i2c_data.data_byte = sensor_data_byte;

    ret = ioctl(fd, I2C_CMD_WRITE, &i2c_data);

    if (ret)
    {
        printf("i2c write faild!\n");
        return -1;
    }

    close(fd);
#endif
	return 0;
}

int sensor_write_register_bit(int addr, int data, int mask)
{
#ifdef HI_GPIO_I2C
    int fd = -1;
    int ret;
    int value;

    fd = open("/dev/gpioi2c", 0);
    if(fd<0)
    {
        printf("Open gpioi2c error!\n");
        return -1;
    }

    value = ((sensor_i2c_addr&0xff)<<24) | ((addr&0xff)<<16);

    ret = ioctl(fd, GPIO_I2C_READ, &value);
    if (ret)
    {
        printf("GPIO-I2C read faild!\n");
        return -1;
    }

    value &= 0xff;
    value &= ~mask;
    value |= data & mask;

    value = ((sensor_i2c_addr&0xff)<<24) | ((addr&0xff)<<16) | (value&0xff);

    ret = ioctl(fd, GPIO_I2C_WRITE, &value);
    if (ret)
    {
        printf("GPIO-I2C write faild!\n");
        return -1;
    }

    close(fd);
#else
    int fd = -1;
    int ret;
    int value;
    I2C_DATA_S i2c_data;

    fd = open("/dev/i2c", 0);
    if(fd<0)
    {
        printf("Open i2c error!\n");
        return -1;
    }

    i2c_data.dev_addr = sensor_i2c_addr;
    i2c_data.reg_addr = addr;
    i2c_data.addr_byte = sensor_addr_byte;
    i2c_data.data_byte = sensor_data_byte;

    ret = ioctl(fd, I2C_CMD_READ, &i2c_data);
    if (ret)
    {
        printf("i2c read faild!\n");
        return -1;
    }

    value = i2c_data.data;
    value &= ~mask;
    value |= data & mask;

    i2c_data.data = value;

    ret = ioctl(fd, I2C_CMD_WRITE, &i2c_data);
    if (ret)
    {
        printf("i2c write faild!\n");
        return -1;
    }

    close(fd);
#endif
	return 0;
}


static void delay_ms(int ms)
{
    usleep(ms*1000);
}

void sensor_prog(int* rom)
{
    int i = 0;
    while (1)
    {
        int u32Lookup = rom[i++];
        int addr = (u32Lookup >> 16) & 0xFFFF;
        int data = u32Lookup & 0xFFFF;
        if (addr == 0xFFFE)
        {
            delay_ms(data);
        }
        else if (addr == 0xFFFF)
        {
            return;
        }
        else
        {
			sensor_write_register(addr, data);
        }
    }
}

void OV2640RewriteAddr()
{
	sensor_write_register(0xFF, 0x0);
	sensor_write_register(0xF7, 0x40);
}
void OV9712SensorInit()
{
    //Reset 
    sensor_write_register(0x12, 0x80);
    sensor_write_register(0x09, 0x10);

    //Core Settings
    sensor_write_register(0x1e, 0x07);
    sensor_write_register(0x5f, 0x18);
    sensor_write_register(0x69, 0x04);
    sensor_write_register(0x65, 0x2a);
    sensor_write_register(0x68, 0x0a);
    sensor_write_register(0x39, 0x28);
    sensor_write_register(0x4d, 0x90);
    sensor_write_register(0xc1, 0x80);
    sensor_write_register(0x0c, 0x30);
    sensor_write_register(0x6d, 0x02);

    //DSP
    //sensor_write_register(0x96, 0xf1);
    sensor_write_register(0x96, 0x01);
    sensor_write_register(0xbc, 0x68);

    //Resolution and Format
    //Sensor Vertical: 			0x1a*4 + 0x03[3:2] = 0xCA*4 + 0b10 = 	810
    //Sensor Horizontal : 		0x18*8 + 0x32[5:3] = 0xA2*8 + 0b000 = 	1296
    //Sensor Vertical start:	0x19*4 + 0x03[1:0] = 0x01*4 + 0b10 = 	6
    //Sensor Horizontal start: 	0x17*8 + 0x32[2:0] = 0x01*8 + 0b001 = 	9
    //Dsp Vertical:				0x58*4 + 0x57[1:0] = 0xC8*4 + 0b00 = 	800
    //Dsp Horizontal:			0x59*8 + 0x57[4:2] = 0xA0*8 + 0b00 = 	1280
    sensor_write_register(0x12, 0x00);
    sensor_write_register(0x3b, 0x00);
    sensor_write_register(0x97, 0x80);
    sensor_write_register(0x17, 0x26);
    sensor_write_register(0x18, 0xA2);
    sensor_write_register(0x19, 0x01);
    sensor_write_register(0x1a, 0xC8);
    sensor_write_register(0x03, 0x02);
    sensor_write_register(0x32, 0x01);
    sensor_write_register(0x57, 0x00);
    sensor_write_register(0x58, 0xC8);
    sensor_write_register(0x59, 0xA0);
    sensor_write_register(0x98, 0x00);
    sensor_write_register(0x99, 0x28);
    sensor_write_register(0x9a, 0x00);
    sensor_write_register(0x4c, 0x13);
    sensor_write_register(0x4b, 0x36);
    sensor_write_register(0x3d, 0x3c);
    sensor_write_register(0x3e, 0x03);
    sensor_write_register(0xbd, 0xA0);
    sensor_write_register(0xbe, 0xb4);
    sensor_write_register(0x37, 0x02);
    sensor_write_register(0x60, 0x9d);

    //YAVG
    sensor_write_register(0x4e, 0x55);
    sensor_write_register(0x4f, 0x55);
    sensor_write_register(0x50, 0x55);
    sensor_write_register(0x51, 0x55);
    sensor_write_register(0x24, 0x55);
    sensor_write_register(0x25, 0x40);
    sensor_write_register(0x26, 0xa1);

    //Clock
    sensor_write_register(0x5c, 0x52);
    sensor_write_register(0x5d, 0x00);
    sensor_write_register(0x11, 0x01);
    sensor_write_register(0x2a, 0x9c);
    sensor_write_register(0x2b, 0x06);
    sensor_write_register(0x2d, 0x00);
    sensor_write_register(0x2e, 0x00);

    //General
    sensor_write_register(0x13, 0xA5);
    sensor_write_register(0x14, 0x40);

    //Banding
    sensor_write_register(0x4a, 0x00);
    sensor_write_register(0x49, 0xce);
    sensor_write_register(0x22, 0x03);
    sensor_write_register(0x09, 0x00);

    //close AE_AWB
    sensor_write_register(0x13, 0x80);
    sensor_write_register(0x16, 0x00);
    sensor_write_register(0x10, 0xf0);
    sensor_write_register(0x00, 0x3f);
    sensor_write_register(0x38, 0x00);
    sensor_write_register(0x01, 0x40);
    sensor_write_register(0x02, 0x40);
    sensor_write_register(0x05, 0x40);
    sensor_write_register(0x06, 0x00);
    sensor_write_register(0x07, 0x00);

    //BLC
    sensor_write_register(0x41, 0x84);

	
	int id1 =0, id2 = 0, id3 = 0, id4 = 0;
	id1 = sensor_read_register(0x1C);
	id2 = sensor_read_register(0x1D);
	id3 = sensor_read_register(0x0A);
	id4 = sensor_read_register(0x0B);
	printf("ov9712 id: %x %x %x %x\r\n",id1,id2,id3,id4);
	
	return ;
}


