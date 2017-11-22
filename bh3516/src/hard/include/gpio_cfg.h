#ifndef __GPIO_CFG_H__
#define __GPIO_CFG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum LED_LIGHT_E
{
	led_9712,
	led_2640,
}LED_LIGHT;

typedef enum LED_SWITCH_E
{
	led_on,
	led_off,
}LED_SWITCH;

typedef enum LED_SWITCH_TYPE_E
{
	led_snap,
	led_recd,
}LED_SWITCH_TYPE;

/*
* 功能描述	：	使能9712
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_enable(void);

/*
* 功能描述	：	失效9712
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov9712_disable(void);

/*
* 功能描述	：	使能2640
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_enable(void);

/*
* 功能描述	：	失效2640
* 成员更新	:		无
* 输入参数	：	无
* 输出参数	：	无
* 返 回 值	：		无
* 其它说明	：	无
* 修改日期	:		2015.08.09
* -----------------------------------------------
* 2015/08/09	 V1.0		XXXX		  XXXX
*/
void ov2640_disable(void);

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
void gpio_set_ov9712_led_on(void);

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
void gpio_set_ov9712_led_off(void);

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
void gpio_set_ov2640_led_on(void);

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
void gpio_set_ov2640_led_off(void);


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
void gpio_led_ctrl(LED_LIGHT eLedLight, LED_SWITCH eLedSwitch,LED_SWITCH_TYPE eType);

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
void gpio_init(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif



