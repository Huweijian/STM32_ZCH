/**********************************************************************************
 * �ļ���  ��adc.c
 * ����    ��adc Ӧ�ú�����         
 * ʵ��ƽ̨��NiRen_STM32mini��
 * Ӳ�����ӣ�PA0 -> ������ADת�����ź�       
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "adc.h"
	   
/*******************************************************************************
* ������  : ADC1_Init_Config
* ����    : ADC1��ʼ������
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ֻ�ǳ�ʼ����ADC1��δ����ADC1ת��
*******************************************************************************/																   
void ADC1_Init_Config(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE ); //ʹ��ADC1��GPIOA����ʱ�� 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //����ADCʱ�ӷ�Ƶ����Ϊ6(72M/6=12M),ADC�����Ƶ��Ϊ14M

	/*����PA0ģ��ͨ����������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			//ѡ��Ҫ��ʼ����GPIOA��PA0����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//����ģʽΪģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//���ÿ⺯���е�GPIO��ʼ����������ʼ��GPIOB�е�PB5,PB6,PB7,PB8����

	/*��ʼ������ADC1*/
	ADC_DeInit(ADC1); //��λADC1,����ADC1�����мĴ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//ADC1�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//ɨ��ģʽ���ã���ͨ����ʹ��ʹ�ܣ���ͨ����ʹ��ʧ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�����������ת������,Ҳ�������ó���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);									//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
  
	ADC_Cmd(ADC1, ENABLE);						//ʹ��ADC1������û����ADC1	
	ADC_ResetCalibration(ADC1);					//ʹ��ADC1��λУ׼�Ĵ���	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λ���	
	ADC_StartCalibration(ADC1);					//����ADУ׼ 
	while(ADC_GetCalibrationStatus(ADC1));		//�ȴ�У׼���
	 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

/*******************************************************************************
* ������  : ADC1_Get_AdcValue
* ����    : ��ȡ������ADC��Ӧͨ����ADת��ֵ
* ����    : ch��ADC1ת��ͨ��
* ���    : ��
* ����    : u16��ADC1ת��ͨ������ת�����ص�ADֵ 
* ˵��    : ������ֻ��ת��ADC1��ͨ��0����ch=0
*******************************************************************************/
u16 ADC1_Get_AdcValue(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//����ADC1��ת��ͨ��ch,һ������,����ʱ��Ϊ239.5����	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);								//�������ADC1��ʼת��	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));						//�ȴ�ADת������
	return ADC_GetConversionValue(ADC1);								//�������һ��ADC1������ת����ADֵ
}

