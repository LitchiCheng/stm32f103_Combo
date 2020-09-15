#ifndef _PWM_H_
#define _PWM_H_
#include "stm32f10x.h"

void LedIoInit(void);//配置相关引脚
void PwmInit(void);//PWM初始化

/**************************************************
*  功能：2路PWM控制
*  参数：PWM1和PWM2为0-100之间的数字
*************************************************/
void LedPwmCtrl(uint8_t PWM1,uint8_t PWM2);
#endif
