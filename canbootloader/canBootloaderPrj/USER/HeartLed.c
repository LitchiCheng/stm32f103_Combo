#include "stm32f10x.h"
#include "HeartLed_def.h"

void Initial_HeartLED(void)
{
	uint32_t RCC_AHB1Periphx;
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA<< (((uint32_t)HEART_LED_GPIO-(uint32_t)GPIOA)/0x400), ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = HEART_LED_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  
  GPIO_Init(HEART_LED_GPIO, &GPIO_InitStructure);
}

void HeartLed_Run()
{
	static uint32_t cnt = 0;
	static bool LedOn = true;
	if(++cnt > HEART_LED_PERIOD)
	{
		cnt = 0;
		LedOn = !LedOn;
	}
	
	if(LedOn)
		GPIO_SetBits(HEART_LED_GPIO, HEART_LED_PIN);
	else
		GPIO_ResetBits(HEART_LED_GPIO, HEART_LED_PIN);
}