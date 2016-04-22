/**********************************************************************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库         
 * 实验平台：NiRen_STM32mini板
 * 硬件连接：DJ1/3短接
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "Led.h"
	   
/*******************************************************************************
* 函数名  : LED_GPIO_Config
* 描述    : LED IO配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : LED的IO口是:PA8
*******************************************************************************/
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//使能GPIOA的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = LED;				    //选择要初始化的GPIOB引脚(PA8)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置引脚工作模式为通用推挽输出 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);			//调用库函数中的GPIO初始化函数，初始化GPIOB中的PB5,PB6,PB7,PB8引脚


	LED_OFF();														//关闭LED					
}

/*点亮LED.PA8*/
void IO_ON(void) 
{
	GPIO_SetBits(hwjIO_PORT,hwjIO);
}

/*关闭LED.PA8*/
void IO_OFF(void)
{
	GPIO_ResetBits(hwjIO_PORT,hwjIO);
}

/*点亮LED.PA8*/
void LED_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED);
}

/*关闭LED.PA8*/
void LED_OFF(void)
{
	GPIO_ResetBits(LED_PORT,LED);
}



void KEY_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = hwjIO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(hwjIO_PORT, &GPIO_InitStructure);
}

