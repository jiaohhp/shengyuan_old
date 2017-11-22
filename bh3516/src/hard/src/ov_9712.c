//标准头文件包含区域

//自定义头文件包含区域
#include "i2c_ctl.h"
#include "ov_9712.h"
#include "printf_func.h"

//常量定义区

//文件内部使用的宏

//文件内部使用的数据类型

//全局变量

//静态局部变量
static const unsigned char 	ov9712_i2c_addr		= 0x60;
static const unsigned int  	ov9712_addr_byte		= 1;
static const unsigned int  	ov9712_data_byte		= 1;	

//内部函数

/*
* 功能描述	：	读9712寄存器
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		寄存器数据内容
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09 V1.0		XXXX		  XXXX
*/
int ov9712_read_register(int addr)
{
	return i2c_read_data(ov9712_i2c_addr, addr, ov9712_addr_byte, ov9712_data_byte);
}

/*
* 功能描述	：	写9712寄存器
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		0		成功
										-1	失败
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09 V1.0		XXXX		  XXXX
*/
int ov9712_write_register(int addr, int data)
{
 	return i2c_write_data(ov9712_i2c_addr ,addr, data, ov9712_addr_byte, ov9712_data_byte);
}


//外部函数


/*
* 功能描述	：	模块初始化
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.08
* -----------------------------------------------
* 2015/08/08 V1.0		XXXX		  XXXX
*/

void ov_9712_init(void)
{
	//读取sensor id
	int id1,id2,id3,id4;
	ov9712_write_register(0xFF, 0x01);
	id1 = ov9712_read_register(0x1C);
	id2 = ov9712_read_register(0x1D);
	id3 = ov9712_read_register(0x0A);
	id4 = ov9712_read_register(0x0B);
	ptf_dbg("9712 id: %x %x %x %x\r\n",id1,id2,id3,id4);

	return ;
}



