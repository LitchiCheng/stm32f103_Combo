#ifndef _PWM_H_
#define _PWM_H_
#include "stm32f10x.h"

void LedIoInit(void);//�����������
void PwmInit(void);//PWM��ʼ��

/**************************************************
*  ���ܣ�2·PWM����
*  ������PWM1��PWM2Ϊ0-100֮�������
*************************************************/
void LedPwmCtrl(uint8_t PWM1,uint8_t PWM2);
#endif
