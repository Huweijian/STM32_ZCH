/**********************************************************************************
 * ������  ��ADCʵ��
 * ����    ����PA0����ΪADC1��ת��ͨ����������ת����12λADֵ�Ӵ���1���
 * ʵ��ƽ̨��NiRen_STM32mini��
 * ��汾  ��ST_v3.5
 * ����    ������ͨ��ģ�鿪���Ŷ�
 * ����    ��http://nirenelec.blog.163.com
 * �Ա�    ��http://nirenelec.taobao.com
**********************************************************************************/

#include "stm32f10x.h"
#include "adc.h"
#include "usart.h"	
#include "SysTick.h" 
#include "led.h"
#include "pwm.h"
void RCC_Configuration(void);


/*******************************************************************************
* ������  : main
* ����    : ���������û������main������ʼ����
* ����    : ��
* ���    : ��
* ����ֵ  : int:����ֵΪһ��16λ������
* ˵��    : ��
*******************************************************************************/
int main(void)
{
	u16 adcvalue = 0;			//ADCת��ֵ
	u8 buff[4];					//���ڷ��ͻ�����

	//SystemInit();
	RCC_Configuration();		//����ϵͳʱ��Ϊ72MHZ
	ADC1_Init_Config();			//ADC1��ʼ������
	USART1_Init_Config(9600);	//USART1��ʼ������
	USART2_Init_Config(9600);
	SysTick_Init_Config();		//��ʼ��ϵͳ�δ�ʱ��SysTick
	LED_GPIO_Config();
	KEY_GPIO_Config();
	TIM3_PWM_Init();
	
	
	while (1)
	{/*
		LED_ON();
		Delay_nS(1);
		LED_OFF();
		Delay_nS(1);*/
		
		
	
		adcvalue = ADC1_Get_AdcValue(0); //��ȡ������adc��Ӧͨ����adת��ֵ   
		buff[0] = (u8)(adcvalue>>8);
		buff[1] = (u8)(adcvalue);
		buff[2] = 0x0d;
		buff[3] = 0x0a;
		USART2_SendData(buff, 4);	
		Delay_nS(1); //��ʱ1s
		
		if(Usart1_R_State == 1)//һ֡���ݽ������
		{
			USART1_SendData((u8 *)Usart1_R_Buff, Usart1_R_Count);//usart1�������ݻ���������(���͸ս�����ɵ�һ֡����)
			Usart1_R_State =0;
			Usart1_R_Count =0;
		}
		
	}
}

/*******************************************************************************
* ������  : RCC_Configuration
* ����    : ����ϵͳʱ��Ϊ72MHZ(������Ը�����Ҫ��)
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : STM32F107x��STM32F105xϵ��MCU��STM32F103xϵ��MCUʱ������������ͬ
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;               //�ⲿ����ʱ��(HSE)�Ĺ���״̬����
  
  RCC_DeInit();                               //��������ʱ����صļĴ�������ΪĬ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);                  //�����ⲿ����ʱ��HSE 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); //�ȴ��ⲿ����ʱ��(HSE)�ȶ�

  if(SUCCESS == HSEStartUpStatus)             //����ⲿ����ʱ���Ѿ��ȶ�
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //Flash����
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    
  
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //����AHBʱ�ӵ���ϵͳʱ��(1��Ƶ)/72MHZ
    RCC_PCLK2Config(RCC_HCLK_Div1);  //����APB2ʱ�Ӻ�HCLKʱ�����/72MHz(���Ϊ72MHz)
    RCC_PCLK1Config(RCC_HCLK_Div2);  //����APB1ʱ����HCLKʱ�ӵ�2��Ƶ/36MHz(���Ϊ36MHz)
  
#ifndef STM32F10X_CL                 //���ʹ�õĲ���STM32F107x��STM32F105xϵ��MCU,PLL��������  
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLLCLK = 8MHz * 9 = 72 MHz 
#else                                //���ʹ�õ���STM32F107x��STM32F105xϵ��MCU,PLL��������
    /***** ����PLLx *****/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    RCC_PLL2Cmd(ENABLE); //ʹ��PLL2 
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);//�ȴ�PLL2�ȶ�

    /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

    RCC_PLLCmd(ENABLE); //ʹ��PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //�ȴ�PLL�ȶ�

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);          //����ϵͳʱ�ӵ�ʱ��ԴΪPLL

    while(RCC_GetSYSCLKSource() != 0x08);               //���ϵͳ��ʱ��Դ�Ƿ���PLL
    RCC_ClockSecuritySystemCmd(ENABLE);                 //ʹ��ϵͳ��ȫʱ�� 
    
     }
}
