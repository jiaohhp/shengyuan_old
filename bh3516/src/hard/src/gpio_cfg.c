//标准头文件包含区域
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//自定义头文件包含区域
#include "printf_func.h"
#include "memmap.h"
#include "gpio_cfg.h"
#include "global_para.h"

//常量定义区

//文件内部使用的宏
#define  	IO_MUX_CONFIG_BASE 		0x200f0000												//管脚复用配置寄存器基地址


#define   	GPIO4_0_FUN			(IO_MUX_CONFIG_BASE + 0x0060)		//muxctrl_reg24,配置为gpio工作方式，对应ov9712的pwdn信号
#define   	GPIO4_1_FUN			(IO_MUX_CONFIG_BASE + 0x0064)		//muxctrl_reg25,配置为gpio工作方式，对应ov9712的reset信号
#define   	GPIO4_2_FUN			(IO_MUX_CONFIG_BASE + 0x0068)		//muxctrl_reg26,配置为gpio工作方式，对应ov2640的reset信号
#define   	GPIO4_3_FUN			(IO_MUX_CONFIG_BASE + 0x006c)		//muxctrl_reg27,配置为gpio工作方式，对应ov2640的strobe信号
#define   	GPIO4_4_FUN			(IO_MUX_CONFIG_BASE + 0x0070)		//muxctrl_reg28,配置为gpio工作方式，对应ov2640的sPWDN信号

#define   	GPIO4_6_FUN			(IO_MUX_CONFIG_BASE + 0x0078)		//muxctrl_reg30,配置为gpio工作方式，对应2640的拍照灯
#define   	GPIO4_7_FUN			(IO_MUX_CONFIG_BASE + 0x007C)		//muxctrl_reg31,配置为gpio工作方式，对应9712的拍照灯


#define   	GPIO9_4_FUN			(IO_MUX_CONFIG_BASE + 0x0124)		//muxctrl_reg73,配置为gpio工作方式，对应phy的reset信号
#define   	GPIO9_5_FUN			(IO_MUX_CONFIG_BASE + 0x0128)		//muxctrl_reg74,配置为gpio工作方式，系统指示灯


//------------------------------------------------------------------gpio管脚写1输出高电平，写0输出低电平


#define  	GPIO_4_BASE        				0x20190000
#define  	GPIO_4_DIRECT   				(GPIO_4_BASE + 0x0400)     	//GPIO10的方向寄存器
#define  	GPIO4_0_9712_PWDN 		(GPIO_4_BASE + 0x04)				//OV9712 PWDN：	对应sensor的电源开关,高电平生效(即断电)
#define  	GPIO4_1_9712_RSTB 		(GPIO_4_BASE + 0x08)				//OV9712 RSTB			置位低电平生效,即进行复位
#define  	GPIO4_2_2640_RESETB		(GPIO_4_BASE + 0x10)				//OV2640 RESETB		置位低电平生效,即进行复位
#define  	GPIO4_3_2640_STROBE 	(GPIO_4_BASE + 0x20)				//OV2640 STROBE
#define  	GPIO4_4_2640_PWDN 		(GPIO_4_BASE + 0x40)				//OV2640 PWDN		对应sensor的电源开关,高电平生效(即断电)

#define  	GPIO4_6_2640_SNAP 		(GPIO_4_BASE + 0x100)			//2640snap		对应抓拍灯输出高电平灯亮
#define  	GPIO4_7_9712_SNAP 		(GPIO_4_BASE + 0x200)			//9712sanp		对应抓拍灯输出高电平灯亮


#define  	GPIO_9_BASE        	0x20210000
#define  	GPIO_9_DIRECT     	(GPIO_9_BASE + 0x0400)     				//GPIO9的方向寄存器
#define  	GPIO_9_4					(GPIO_9_BASE + 0x40)         				//GPIO9_4
#define  	GPIO_9_5 					(GPIO_9_BASE + 0x80)							//GPIO9_5

#define 	LEFT_MOV(m,n)     			((m)<<(n))  
#define 	RIGHT_MOV(m,n)     		((m)>>(n))

//文件内部使用的数据类型

//全局变量

//静态局部变量
static int s_9712_snap_led 	= led_off;
static int s_9712_recd_led 	= led_off;
static int s_2640_snap_led  = led_off;
static int s_2640_recd_led  = led_off;
static int s_ov9712_led = led_off;//记录当前ov9712灯的状态
static int s_ov2640_led = led_off;//记录当前ov2640灯的状态


/*
* 功能描述	：	配置两路sensor的使用管脚为gpio工作方式
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void sensor_mux_init(void)
{
	void* pLogicAddr 	= NULL;
	unsigned char ucTmp = 0;
	pLogicAddr = memmap(GPIO4_0_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_1_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_2_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//最低一位置为0，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_3_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//最低一位置为0，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_4_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_6_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//最低一位置为0，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_7_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//最低一位置为0，配置为gpio工作方式
	memunmap(pLogicAddr);
	/*
	pLogicAddr = memmap(GPIO9_5_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);
	*/
	
	
	return ;
}

/*
* 功能描述	：	配置phy的reset信号和系统指示管脚使为gpio工作方式
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void phy_system_mux_init(void)
{
	void* pLogicAddr 	= NULL;
	unsigned char ucTmp = 0;
	pLogicAddr = memmap(GPIO9_4_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO9_5_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//最低两位置为00，配置为gpio工作方式
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 phy复位对应的方向寄存器
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_phy_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 4));//gpio9_4配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	复位phy
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_phy_reset_enable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_4, 0x1);	
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 4));	//点亮sysled指示灯
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	关闭phy复位
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_phy_reset_disable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_4, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,4))));
	memunmap(pLogicAddr);

	return ;
}


/*
* 功能描述	：	配置ov9712 phy复位对应的方向寄存器
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_sysled_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));//gpio9_5配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);

	return ;
}

/****设置led等的方向寄存器*****/
void gpio_set_ov2640_led_output_tqk(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 6));//gpio9_5配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);

	return ;
}

void gpio_set_ov9712_led_output_tqk(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 7));//gpio9_5配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);

	return ;
}



/*
* 功能描述	：	点亮系统指示灯
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_sysled_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_5, 0x1);	
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));	//点亮sysled指示灯
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	熄灭系统指示灯
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_sysled_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_5, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,5))));//关闭系统指示灯
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 pwdn对应的gpio方向寄存器为输出方式
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_pwdn_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1));//gpio4_0配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 reset对应的gpio方向寄存器为输出方式
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_rstb_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 1));	//gpio4_1配置为输出,对应位置为1配置为输出，0为输入
	memunmap(pLogicAddr);
	return ;
}

/*
* 功能描述	：	配置ov9712 pwdn对应的gpio4_0管脚为高电平，从而将ov9712断电
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_pwdn_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_0_9712_PWDN, 0x1);	
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1));	//向ov9712的pwdn管脚写高电平，从而给ov9712断电
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 pwdn对应的gpio4_0管脚为低电平，从而将ov9712上电
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_pwdn_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_0_9712_PWDN, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,0))));//向ov9712的pwdn管脚写低电平，从而给ov9712上电
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 rstb对应的gpio4_0管脚为低电平，从而将ov9712复位
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_rstb_enable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_1_9712_RSTB, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,1))));//向ov9712的reset管脚写低电平，从而给ov9712复位
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov9712 rstb对应的gpio4_0管脚为高电平，从而将ov9712停止复位
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_rstb_disable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_1_9712_RSTB, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 1));//向ov9712的reset管脚写高电平，从而停止ov9712复位
	memunmap(pLogicAddr);

	return ;
}


/*
* 功能描述	：	配置ov2640 pwdn方向寄存器为输出
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_pwdn_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1<<4));//gpio4_4配置为输出
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov2640 rstb方向寄存器为输出
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_rstb_output(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1<<2));//gpio4_2配置为输出
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov2640 rstb方向寄存器为输出
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_strobe_input(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_4_DIRECT, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	
	ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,3))));//(ucTmp |(0x1<<3));//gpio4_3配置为输入
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov2640 pwdn，给ov2640断电
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_pwdn_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_4_2640_PWDN, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 4));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov2640 pwdn，给ov2640上电
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_pwdn_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_4_2640_PWDN, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,4))));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	9712 led灯点亮
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_led_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_7_9712_SNAP, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_4_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 7));
	memunmap(pLogicAddr);

	return ;
}
/*
void gpio_set_sysled_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_5, 0x1);	
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));	//点亮sysled指示灯
	memunmap(pLogicAddr);

	return ;
}
*/

/*
* 功能描述	：	9712 led灯熄灭
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov9712_led_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_7_9712_SNAP, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,7))));
	memunmap(pLogicAddr);

	return ;
}
/*
void gpio_set_sysled_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO_9_5, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_4_7_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,5))));//关闭系统指示灯
	memunmap(pLogicAddr);

	return ;
}
*/


/*
* 功能描述	：	2640 led灯点亮
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_led_on(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_6_2640_SNAP, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 6));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	2640 led灯熄灭
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.09.26
* -----------------------------------------------
* 2015/09/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_led_off(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_6_2640_SNAP, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,6))));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	控制led灯亮灭
* 成员更新	:		无
* 输入参数	：	eLedLight		灯类型
										eLedSwitch	开关控制
										eLedAction	操作类型
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.09.27
* -----------------------------------------------
* 2015/09/27	 V1.0		XXXX		  XXXX
*/
void gpio_led_ctrl(LED_LIGHT eLedLight, LED_SWITCH eLedSwitch,LED_SWITCH_TYPE eType)
{
	//eLedLight=led_9712;  //2017_07_31    双改单添加
	if (led_on == eLedSwitch)
	{	
		//直接点亮
		if (led_9712 == eLedLight)
		{
			s_ov9712_led=led_on;
			gpio_set_ov9712_led_on();
			/*if(led_on==s_ov2640_led)
			{
				gpio_set_ov2640_led_on();
			}else
			{
				gpio_set_ov2640_led_off();
			}
			//gpio_set_ov2640_led_off();*/
			if(led_recd == eType)
			{
				s_9712_recd_led = led_on;
			}
			else if(led_snap == eType)
			{
				s_9712_snap_led = led_on;
			}
		}
		else
		{
			s_ov2640_led=led_on;
			gpio_set_ov2640_led_on();
			/*if(led_on==s_ov9712_led)
			{
				gpio_set_ov9712_led_on();
			}else
			{
				gpio_set_ov9712_led_off();
			}
			//gpio_set_ov9712_led_off();*/
			if(led_recd == eType)
			{
				s_2640_recd_led = led_on;
			}else if(led_snap == eType)
			{
				s_2640_snap_led = led_on;
			}
		}
		/*//记录亮灭
		if (led_sanp == eType)
		{
			s_snap_led = led_on;
		}
		else
		{
			s_recd_led = led_on;
		}*/
	}
	else
	{
		/*if (led_recd == eType)
		{
			s_recd_led 	= led_off;
			if ((led_off == s_snap_led) && (led_off == s_recd_led))
			{
				gpio_set_ov2640_led_off();
				gpio_set_ov9712_led_off();
				s_ov2640_led=led_off;
				s_ov9712_led=led_off;
			}
		}
		else
		{
			s_snap_led = led_off;
			if ((led_off == s_snap_led) && (led_off == s_recd_led))
			{
				gpio_set_ov2640_led_off();
				gpio_set_ov9712_led_off();
				s_ov2640_led=led_off;
				s_ov9712_led=led_off;
			}
		}*/
		/*if(led_recd == eType)
		{
			s_9712_recd_led = led_off;
		}else if(led_9712_snap ==eType)
		{
			s_9712_snap_led = led_off;
		}else if(led_2640_recd == eType)
		{
			s_2640_recd_led = led_off;
		}else if(led_2640_snap == eType)
		{
			s_2640_snap_led = led_off;
		}*/
		if(led_9712==eLedLight)
		{
			if(led_recd == eType)
			{
				s_9712_recd_led = led_off;
			}else if(led_snap ==eType)
			{
				s_9712_snap_led = led_off;
			}
			if(s_9712_recd_led==led_off && s_9712_snap_led== led_off)
			{
				gpio_set_ov9712_led_off();
				s_ov9712_led=led_off;
			}
		}else
		{
			if(led_recd == eType)
			{
				s_2640_recd_led = led_off;
			}else if(led_snap == eType)
			{
				s_2640_snap_led = led_off;
			}
			if(s_2640_recd_led==led_off && s_2640_snap_led== led_off)
			{
				gpio_set_ov2640_led_off();
				s_ov2640_led=led_off;
			}
		}
		
	}
}
/*
* 功能描述	：	配置ov2640 resetb，停止ov2640复位
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_resetb_disable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_2_2640_RESETB, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 2));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	配置ov2640 resetb，启动ov2640复位
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void gpio_set_ov2640_reset_enable(void)
{
	void* pLogicAddr = NULL;
	unsigned char ucTmp = 0;

	pLogicAddr = memmap(GPIO4_2_2640_RESETB, 0x1);
	if (pLogicAddr == NULL)
	{
		ptf_err("memmap GPIO_9_DIRECT failed!\n");
		return ;
	}
	 ucTmp= *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,2))));
	memunmap(pLogicAddr);

	return ;
}

/*
* 功能描述	：	闪烁状态灯
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.01
* -----------------------------------------------
* 2015/08/01	 V1.0		XXXX		  XXXX
*/
static void* sys_led_on_off(void* arg)
{
	while (1)
	{
		gpio_set_sysled_on();
		sleep(1);
		gpio_set_sysled_off();
		sleep(1);
	}

	return NULL;
}

/*
* 功能描述	：	使能9712 经过验证已经生效
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_enable(void)
{
	//关闭复位
	gpio_set_ov9712_rstb_disable();
	//给sennsor上电
	gpio_set_ov9712_pwdn_on();
	return ;
}

/*
* 功能描述	：	失效9712 经过验证已经生效
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_disable(void)
{
	gpio_set_ov9712_pwdn_off();
	return ;
}

/*
* 功能描述	：	使能2640经过验证已经生效
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_enable(void)
{
	//关闭复位
	gpio_set_ov2640_resetb_disable();
	//给sennsor上电
	gpio_set_ov2640_pwdn_on();

	return ;
}

/*
* 功能描述	：	失效2640经过验证已经生效
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_disable(void)
{
	gpio_set_ov2640_pwdn_off();
	return ;
}
/*
* 功能描述	：	初始化gpio模块
* 成员更新	:		无
* 输入参数	：	
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void gpio_init(void)
{
	sensor_mux_init();
	phy_system_mux_init();
	gpio_set_phy_output();
	gpio_set_sysled_output();
	gpio_set_ov2640_led_output_tqk();
	gpio_set_ov9712_led_output_tqk();
	gpio_set_ov9712_pwdn_output();
	gpio_set_ov9712_rstb_output();
	gpio_set_ov2640_pwdn_output();
	gpio_set_ov2640_rstb_output();
	gpio_set_ov2640_strobe_input();
	gpio_set_ov9712_led_on();
	gpio_set_ov2640_led_on();
	sleep(1);
	gpio_set_ov9712_led_off();
	gpio_set_ov2640_led_off();
	//启动闪烁状态灯的线程
	ThreadPoolAddWorkLimit((void*)g_psThreadPool, sys_led_on_off, (void *)NULL);
	return ;
}

