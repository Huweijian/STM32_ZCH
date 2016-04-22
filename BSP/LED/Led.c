/**********************************************************************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��NiRen_STM32mini��
 * Ӳ�����ӣ�DJ1/3�̽�
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "Led.h"
	   
/*******************************************************************************
* ������  : LED_GPIO_Config
* ����    : LED IO����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : LED��IO����:PA8
*******************************************************************************/
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;				//����һ��GPIO_InitTypeDef���͵�GPIO��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//ʹ��GPIOA������ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = LED;				    //ѡ��Ҫ��ʼ����GPIOB����(PA8)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������Ź���ģʽΪͨ��������� 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);			//���ÿ⺯���е�GPIO��ʼ����������ʼ��GPIOB�е�PB5,PB6,PB7,PB8����


	LED_OFF();														//�ر�LED					
}

/*����LED.PA8*/
void IO_ON(void) 
{
	GPIO_SetBits(hwjIO_PORT,hwjIO);
}

/*�ر�LED.PA8*/
void IO_OFF(void)
{
	GPIO_ResetBits(hwjIO_PORT,hwjIO);
}

/*����LED.PA8*/
void LED_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED);
}

/*�ر�LED.PA8*/
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

