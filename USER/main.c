/**********************************************************************************
 * 工程名  ：ADC实验
 * 描述    ：将PA0口作为ADC1的转换通道，将单次转换的12位AD值从串口1输出
 * 实验平台：NiRen_STM32mini板
 * 库版本  ：ST_v3.5
 * 作者    ：泥人通信模块开发团队
 * 博客    ：http://nirenelec.blog.163.com
 * 淘宝    ：http://nirenelec.taobao.com
**********************************************************************************/

#include "stm32f10x.h"
#include "adc.h"
#include "usart.h"	
#include "SysTick.h" 
#include "led.h"
#include "pwm.h"
void RCC_Configuration(void);


/*******************************************************************************
* 函数名  : main
* 描述    : 主函数，用户程序从main函数开始运行
* 输入    : 无
* 输出    : 无
* 返回值  : int:返回值为一个16位整形数
* 说明    : 无
*******************************************************************************/
int main(void)
{
	u16 adcvalue = 0;			//ADC转换值
	u8 buff[4];					//串口发送缓冲区

	//SystemInit();
	RCC_Configuration();		//设置系统时钟为72MHZ
	ADC1_Init_Config();			//ADC1初始化配置
	USART1_Init_Config(9600);	//USART1初始化配置
	USART2_Init_Config(9600);
	SysTick_Init_Config();		//初始化系统滴答时钟SysTick
	LED_GPIO_Config();
	KEY_GPIO_Config();
	TIM3_PWM_Init();
	
	
	while (1)
	{/*
		LED_ON();
		Delay_nS(1);
		LED_OFF();
		Delay_nS(1);*/
		
		
	
		adcvalue = ADC1_Get_AdcValue(0); //读取并返回adc对应通道的ad转换值   
		buff[0] = (u8)(adcvalue>>8);
		buff[1] = (u8)(adcvalue);
		buff[2] = 0x0d;
		buff[3] = 0x0a;
		USART2_SendData(buff, 4);	
		Delay_nS(1); //延时1s
		
		if(Usart1_R_State == 1)//一帧数据接收完成
		{
			USART1_SendData((u8 *)Usart1_R_Buff, Usart1_R_Count);//usart1发送数据缓冲区数据(发送刚接收完成的一帧数据)
			Usart1_R_State =0;
			Usart1_R_Count =0;
		}
		
	}
}

/*******************************************************************************
* 函数名  : RCC_Configuration
* 描述    : 设置系统时钟为72MHZ(这个可以根据需要改)
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : STM32F107x和STM32F105x系列MCU与STM32F103x系列MCU时钟配置有所不同
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;               //外部高速时钟(HSE)的工作状态变量
  
  RCC_DeInit();                               //将所有与时钟相关的寄存器设置为默认值
  RCC_HSEConfig(RCC_HSE_ON);                  //启动外部高速时钟HSE 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); //等待外部高速时钟(HSE)稳定

  if(SUCCESS == HSEStartUpStatus)             //如果外部高速时钟已经稳定
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Flash设置
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
  
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //设置AHB时钟等于系统时钟(1分频)/72MHZ
    RCC_PCLK2Config(RCC_HCLK_Div1);  //设置APB2时钟和HCLK时钟相等/72MHz(最大为72MHz)
    RCC_PCLK1Config(RCC_HCLK_Div2);  //设置APB1时钟是HCLK时钟的2分频/36MHz(最大为36MHz)
  
#ifndef STM32F10X_CL                 //如果使用的不是STM32F107x或STM32F105x系列MCU,PLL以下配置  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 8MHz * 9 = 72 MHz 
#else                                //如果使用的是STM32F107x或STM32F105x系列MCU,PLL以下配置
    /***** 配置PLLx *****/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    RCC_PLL2Cmd(ENABLE); //使能PLL2 
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);//等待PLL2稳定

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

    RCC_PLLCmd(ENABLE); //使能PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待PLL稳定

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);          //设置系统时钟的时钟源为PLL

    while(RCC_GetSYSCLKSource() != 0x08);               //检查系统的时钟源是否是PLL
    RCC_ClockSecuritySystemCmd(ENABLE);                 //使能系统安全时钟 
    
     }
}
