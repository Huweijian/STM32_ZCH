#ifndef __ADC_H
#define __ADC_H	

#include "stm32f10x.h"

void ADC1_Init_Config(void);
u16 ADC1_Get_AdcValue(u8 ch);
 
#endif 
