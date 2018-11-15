

#include "led.h"

void LED_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOB, GPIO_Pin_9);	 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOB, GPIO_Pin_7);	 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOB, GPIO_Pin_6);	 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOB, GPIO_Pin_5);	 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);	 
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_ResetBits(GPIOA, GPIO_Pin_3);	 
	
	
}



