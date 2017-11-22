//��׼ͷ�ļ���������
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//�Զ���ͷ�ļ���������
#include "printf_func.h"
#include "memmap.h"
#include "gpio_cfg.h"
#include "global_para.h"

//����������

//�ļ��ڲ�ʹ�õĺ�
#define  	IO_MUX_CONFIG_BASE 		0x200f0000												//�ܽŸ������üĴ�������ַ

#define   	GPIO5_3_I2C_SDA_FUN	(IO_MUX_CONFIG_BASE + 0x008c)			//muxctrl_reg35,Ϊgpio5_3��I2C_SDA���ÿ��ƼĴ���
#define   	GPIO5_4_I2C_SCL_FUN		(IO_MUX_CONFIG_BASE + 0x0090)			//muxctrl_reg36,Ϊgpio5_4��I2C_SCL���ÿ��ƼĴ���

#define   	GPIO4_0_FUN			(IO_MUX_CONFIG_BASE + 0x0060)		//muxctrl_reg24,����Ϊgpio������ʽ����Ӧov9712��pwdn�ź�
#define   	GPIO4_1_FUN			(IO_MUX_CONFIG_BASE + 0x0064)		//muxctrl_reg25,����Ϊgpio������ʽ����Ӧov9712��reset�ź�
#define   	GPIO4_2_FUN			(IO_MUX_CONFIG_BASE + 0x0068)		//muxctrl_reg26,����Ϊgpio������ʽ����Ӧov2640��reset�ź�
#define   	GPIO4_3_FUN			(IO_MUX_CONFIG_BASE + 0x006c)		//muxctrl_reg27,����Ϊgpio������ʽ����Ӧov2640��strobe�ź�
#define   	GPIO4_4_FUN			(IO_MUX_CONFIG_BASE + 0x0070)		//muxctrl_reg28,����Ϊgpio������ʽ����Ӧov2640��sPWDN�ź�

#define   	GPIO4_6_FUN			(IO_MUX_CONFIG_BASE + 0x0078)		//muxctrl_reg30,����Ϊgpio������ʽ����Ӧ2640�����յ�
#define   	GPIO4_7_FUN			(IO_MUX_CONFIG_BASE + 0x007C)		//muxctrl_reg31,����Ϊgpio������ʽ����Ӧ9712�����յ�


#define   	GPIO9_4_FUN			(IO_MUX_CONFIG_BASE + 0x0124)		//muxctrl_reg73,����Ϊgpio������ʽ����Ӧphy��reset�ź�
#define   	GPIO9_5_FUN			(IO_MUX_CONFIG_BASE + 0x0128)		//muxctrl_reg74,����Ϊgpio������ʽ��ϵͳָʾ��


//------------------------------------------------------------------gpio�ܽ�д1����ߵ�ƽ��д0����͵�ƽ


#define  	GPIO_4_BASE        				0x20190000
#define  	GPIO_4_DIRECT   				(GPIO_4_BASE + 0x0400)     	//GPIO10�ķ���Ĵ���
#define  	GPIO4_0_9712_PWDN 		(GPIO_4_BASE + 0x04)				//OV9712 PWDN��	��Ӧsensor�ĵ�Դ����,�ߵ�ƽ��Ч(���ϵ�)
#define  	GPIO4_1_9712_RSTB 		(GPIO_4_BASE + 0x08)				//OV9712 RSTB			��λ�͵�ƽ��Ч,�����и�λ
#define  	GPIO4_2_2640_RESETB		(GPIO_4_BASE + 0x10)				//OV2640 RESETB		��λ�͵�ƽ��Ч,�����и�λ
#define  	GPIO4_3_2640_STROBE 	(GPIO_4_BASE + 0x20)				//OV2640 STROBE
#define  	GPIO4_4_2640_PWDN 		(GPIO_4_BASE + 0x40)				//OV2640 PWDN		��Ӧsensor�ĵ�Դ����,�ߵ�ƽ��Ч(���ϵ�)

#define  	GPIO4_6_2640_SNAP 		(GPIO_4_BASE + 0x100)			//2640snap		��Ӧץ�ĵ�����ߵ�ƽ����
#define  	GPIO4_7_9712_SNAP 		(GPIO_4_BASE + 0x200)			//9712sanp		��Ӧץ�ĵ�����ߵ�ƽ����


#define  	GPIO_9_BASE        	0x20210000
#define  	GPIO_9_DIRECT     	(GPIO_9_BASE + 0x0400)     				//GPIO9�ķ���Ĵ���
#define  	GPIO_9_4					(GPIO_9_BASE + 0x40)         				//GPIO9_4
#define  	GPIO_9_5 					(GPIO_9_BASE + 0x80)							//GPIO9_5

#define 	LEFT_MOV(m,n)     			((m)<<(n))  
#define 	RIGHT_MOV(m,n)     		((m)>>(n))

//�ļ��ڲ�ʹ�õ���������

//ȫ�ֱ���

//��̬�ֲ�����
static int s_9712_snap_led 	= led_off;
static int s_9712_recd_led 	= led_off;
static int s_2640_snap_led  = led_off;
static int s_2640_recd_led  = led_off;
static int s_ov9712_led = led_off;//��¼��ǰov9712�Ƶ�״̬
static int s_ov2640_led = led_off;//��¼��ǰov2640�Ƶ�״̬

//�ڲ�����

//ȫ�ֺ���

/*
* ��������	��	���ø��üĴ���ΪI2C������ʽ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
* -----------------------------------------------
* 2015/07/26	 V1.0		XXXX		  XXXX
*/
void i2c_mux_init_I2c(void)
{
	void* pLogicAddr 	= NULL;
	unsigned char ucTmp = 0;
	pLogicAddr = memmap(GPIO5_3_I2C_SDA_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | 0x1); 		//�ܽŸ���,SDA�����λ��Ϊ1����Ϊsda,Ϊ0����Ϊgpio
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO5_4_I2C_SCL_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr = (ucTmp | 0x1); 		//�ܽŸ���,SCL�����λ��Ϊ1����Ϊscl,Ϊ0����Ϊgpio
	memunmap(pLogicAddr);
	return ;
}

/*
* ��������	��	������·sensor��ʹ�ùܽ�Ϊgpio������ʽ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_1_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_2_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//���һλ��Ϊ0������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_3_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//���һλ��Ϊ0������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_4_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_6_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//���һλ��Ϊ0������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO4_7_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFE); 		//���һλ��Ϊ0������Ϊgpio������ʽ
	memunmap(pLogicAddr);
	/*
	pLogicAddr = memmap(GPIO9_5_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);
	*/
	
	
	return ;
}

/*
* ��������	��	����phy��reset�źź�ϵͳָʾ�ܽ�ʹΪgpio������ʽ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	pLogicAddr = memmap(GPIO9_5_FUN, 0x4);			
	if (NULL == pLogicAddr)
	{
		//printf("memmap GPIO_UART2_RXD failed!\n");
		return ;
	}
	ucTmp = *(unsigned char*)pLogicAddr;
	*(unsigned char*)pLogicAddr =  (ucTmp & 0xFC); 		//�����λ��Ϊ00������Ϊgpio������ʽ
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 phy��λ��Ӧ�ķ���Ĵ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 4));//gpio9_4����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	��λphy
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 4));	//����sysledָʾ��
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	�ر�phy��λ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
* ��������	��	����ov9712 phy��λ��Ӧ�ķ���Ĵ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));//gpio9_5����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
	memunmap(pLogicAddr);

	return ;
}

/****����led�ȵķ���Ĵ���*****/
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 6));//gpio9_5����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 7));//gpio9_5����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
	memunmap(pLogicAddr);

	return ;
}



/*
* ��������	��	����ϵͳָʾ��
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));	//����sysledָʾ��
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	Ϩ��ϵͳָʾ��
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,5))));//�ر�ϵͳָʾ��
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 pwdn��Ӧ��gpio����Ĵ���Ϊ�����ʽ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1));//gpio4_0����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 reset��Ӧ��gpio����Ĵ���Ϊ�����ʽ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1 << 1));	//gpio4_1����Ϊ���,��Ӧλ��Ϊ1����Ϊ�����0Ϊ����
	memunmap(pLogicAddr);
	return ;
}

/*
* ��������	��	����ov9712 pwdn��Ӧ��gpio4_0�ܽ�Ϊ�ߵ�ƽ���Ӷ���ov9712�ϵ�
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1));	//��ov9712��pwdn�ܽ�д�ߵ�ƽ���Ӷ���ov9712�ϵ�
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 pwdn��Ӧ��gpio4_0�ܽ�Ϊ�͵�ƽ���Ӷ���ov9712�ϵ�
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,0))));//��ov9712��pwdn�ܽ�д�͵�ƽ���Ӷ���ov9712�ϵ�
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 rstb��Ӧ��gpio4_0�ܽ�Ϊ�͵�ƽ���Ӷ���ov9712��λ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,1))));//��ov9712��reset�ܽ�д�͵�ƽ���Ӷ���ov9712��λ
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov9712 rstb��Ӧ��gpio4_0�ܽ�Ϊ�ߵ�ƽ���Ӷ���ov9712ֹͣ��λ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 1));//��ov9712��reset�ܽ�д�ߵ�ƽ���Ӷ�ֹͣov9712��λ
	memunmap(pLogicAddr);

	return ;
}


/*
* ��������	��	����ov2640 pwdn����Ĵ���Ϊ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1<<4));//gpio4_4����Ϊ���
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov2640 rstb����Ĵ���Ϊ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp |(0x1<<2));//gpio4_2����Ϊ���
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov2640 rstb����Ĵ���Ϊ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,3))));//(ucTmp |(0x1<<3));//gpio4_3����Ϊ����
	memunmap(pLogicAddr);

	return ;
}

/*
* ��������	��	����ov2640 pwdn����ov2640�ϵ�
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
* ��������	��	����ov2640 pwdn����ov2640�ϵ�
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
* ��������	��	9712 led�Ƶ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.09.26
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
	*(unsigned char*)pLogicAddr = (ucTmp | (0x1 << 5));	//����sysledָʾ��
	memunmap(pLogicAddr);

	return ;
}
*/

/*
* ��������	��	9712 led��Ϩ��
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.09.26
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
	*(unsigned char*)pLogicAddr = (ucTmp & (~(LEFT_MOV(0x1,5))));//�ر�ϵͳָʾ��
	memunmap(pLogicAddr);

	return ;
}
*/


/*
* ��������	��	2640 led�Ƶ���
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.09.26
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
* ��������	��	2640 led��Ϩ��
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.09.26
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
* ��������	��	����led������
* ��Ա����	:		��
* �������	��	eLedLight		������
										eLedSwitch	���ؿ���
										eLedAction	��������
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.09.27
* -----------------------------------------------
* 2015/09/27	 V1.0		XXXX		  XXXX
*/
void gpio_led_ctrl(LED_LIGHT eLedLight, LED_SWITCH eLedSwitch,LED_SWITCH_TYPE eType)
{
	//eLedLight=led_9712;  //2017_07_31    ˫�ĵ����
	if (led_on == eLedSwitch)
	{	
		//ֱ�ӵ���
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
		/*//��¼����
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
* ��������	��	����ov2640 resetb��ֹͣov2640��λ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
* ��������	��	����ov2640 resetb������ov2640��λ
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.07.26
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
* ��������	��	��˸״̬��
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.01
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
* ��������	��	ʹ��9712 ������֤�Ѿ���Ч
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_enable(void)
{
	//�رո�λ
	gpio_set_ov9712_rstb_disable();
	//��sennsor�ϵ�
	gpio_set_ov9712_pwdn_on();
	return ;
}

/*
* ��������	��	ʧЧ9712 ������֤�Ѿ���Ч
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_disable(void)
{
	gpio_set_ov9712_pwdn_off();
	return ;
}

/*
* ��������	��	ʹ��2640������֤�Ѿ���Ч
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_enable(void)
{
	//�رո�λ
	gpio_set_ov2640_resetb_disable();
	//��sennsor�ϵ�
	gpio_set_ov2640_pwdn_on();

	return ;
}

/*
* ��������	��	ʧЧ2640������֤�Ѿ���Ч
* ��Ա����	:		��
* �������	��	��
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	��
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_disable(void)
{
	gpio_set_ov2640_pwdn_off();
	return ;
}
/*
* ��������	��	��ʼ��gpioģ��
* ��Ա����	:		��
* �������	��	
* �������	��	��
* �� �� ֵ	��		��
* ����˵��	��	
* �޸�����	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void gpio_init(void)
{
	i2c_mux_init_I2c();	
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
	//������˸״̬�Ƶ��߳�
	ThreadPoolAddWorkLimit((void*)g_psThreadPool, sys_led_on_off, (void *)NULL);
	return ;
}

