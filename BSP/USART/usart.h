#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define USART1_REC_MAXLEN 200	//最大接收数据长度

void USART1_Init_Config(u32 bound);
void USART1_SendData(u8* buff, u16 len);
void USART2_Init_Config(u32 bound);
void USART2_SendData(u8* buff, u16 len);
extern vu8 Usart1_R_Buff[USART1_REC_MAXLEN];	//串口1数据接收缓冲区 
extern vu8 Usart1_R_State;					//串口1接收状态
extern vu16 Usart1_R_Count;					//当前接收数据的字节数 	  

#endif


