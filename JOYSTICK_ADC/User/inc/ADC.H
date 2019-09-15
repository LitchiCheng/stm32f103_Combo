#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
extern volatile uint16_t ADCConvertedValue[2];
/******************************************
*	功能：ADC1初始化，使用PC04引脚采集
******************************************/
void AdcInit(void);

/****************************************************
*	功能：获取采集到的AD值
*	返回值：采集到的值
****************************************************/
uint16_t AdcGetVal(void);


#endif
