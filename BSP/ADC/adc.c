/**********************************************************************************
 * 文件名  ：adc.c
 * 描述    ：adc 应用函数库         
 * 实验平台：NiRen_STM32mini板
 * 硬件连接：PA0 -> 待进行AD转换的信号       
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "adc.h"
	   
/*******************************************************************************
* 函数名  : ADC1_Init_Config
* 描述    : ADC1初始化配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 只是初始化了ADC1但未启动ADC1转换
*******************************************************************************/																   
void ADC1_Init_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE ); //使能ADC1和GPIOA外设时钟 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC时钟分频因子为6(72M/6=12M),ADC最大工作频率为14M

	/*设置PA0模拟通道输入引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//选择要初始化的GPIOA的PA0引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//工作模式为模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化GPIOB中的PB5,PB6,PB7,PB8引脚

	/*初始化配置ADC1*/
	ADC_DeInit(ADC1); //复位ADC1,即将ADC1的所有寄存器设置为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC1工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//扫描模式设置，多通道下使用使能，单通道下使用失能
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//由软件来触发转换启动,也可以设置成外设启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);									//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器  
  
	ADC_Cmd(ADC1, ENABLE);						//使能ADC1，但还没启动ADC1	
	ADC_ResetCalibration(ADC1);					//使能ADC1复位校准寄存器	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位完成	
	ADC_StartCalibration(ADC1);					//开启AD校准 
	while(ADC_GetCalibrationStatus(ADC1));		//等待校准完成
	 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

/*******************************************************************************
* 函数名  : ADC1_Get_AdcValue
* 描述    : 读取并返回ADC对应通道的AD转换值
* 输入    : ch：ADC1转换通道
* 输出    : 无
* 返回    : u16：ADC1转换通道单次转换返回的AD值 
* 说明    : 本例程只能转换ADC1的通道0，即ch=0
*******************************************************************************/
u16 ADC1_Get_AdcValue(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//设置ADC1的转换通道ch,一个序列,采样时间为239.5周期	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);								//软件启动ADC1开始转换	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));						//等待AD转换结束
	return ADC_GetConversionValue(ADC1);								//返回最近一次ADC1规则组转换的AD值
}

