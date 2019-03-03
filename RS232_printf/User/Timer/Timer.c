#include "Timer.h"
#include "stdlib.h"

//#define ENCODER_PPR           (u16)(1000)   // number of pulses per revolution
u16 hEncoder_Timer_Overflow=0;


void Tim4_Init(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;			//TIM4?CH1?CH2?????????A?B
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOD,&GPIO_InitStructure);
GPIO_ResetBits(GPIOD,GPIO_Pin_12);

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOD,&GPIO_InitStructure);
GPIO_ResetBits(GPIOD,GPIO_Pin_13);

GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);

TIM_DeInit(TIM4);
TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
 
TIM_TimeBaseStructure.TIM_Period=0xC350;
TIM_TimeBaseStructure.TIM_Prescaler=0x0;
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

      
TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                            TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 
TIM_ICStructInit(&TIM_ICInitStructure);
//TIM_ICInitStructure.TIM_ICFilter = 0X0F;        
//TIM_ICInit(TIM4, &TIM_ICInitStructure);  

NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); 

TIM4->CNT =0;
TIM_ClearFlag(TIM4, TIM_FLAG_Update);
TIM_ITConfig( TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);
TIM_Cmd(TIM4,ENABLE );                             
 
}
             

void TIM4_IRQHandler(void)
{
 TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  
   if (hEncoder_Timer_Overflow != 65535)  
   {
			hEncoder_Timer_Overflow++;
   }
}


