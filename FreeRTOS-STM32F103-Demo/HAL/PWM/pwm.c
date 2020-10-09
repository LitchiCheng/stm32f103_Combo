#include "PWM.H"

TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0;

void PwmInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE|
                         RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
  /* ARR寄存器用来配置频率，此处配置为 10 Khz */
  TimerPeriod = (SystemCoreClock / 3600 ) - 1;


  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  LedPwmCtrl(100,100);

  /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void LedIoInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* TIM1的PWM1和PWM2通道对应引脚是PA8和PA9，这些可以在硬件手册中找到*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 /*| GPIO_Pin_11*/;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**************************************************
*  功能：2路PWM控制
*  参数：PWM1和PWM2为0-100之间的数字
*************************************************/
void LedPwmCtrl(uint8_t PWM1,uint8_t PWM2)
{
	Channel1Pulse = (uint16_t) (((uint32_t) PWM1 * (TimerPeriod - 1)) / 100);
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	
//  Channel2Pulse = (uint16_t) (((uint32_t) PWM2 * (TimerPeriod - 1)) / 100);
//  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
//  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
}

