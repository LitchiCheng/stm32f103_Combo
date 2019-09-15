#include "RS232.h"
#include "stdio.h"
#include "ADC.h"
#include "stm32f10x.h"
#include "SysTick.h"
void JOYSTICK_K_KEY_init();

int main()
{
	RS232_Init(9600); 
	AdcInit();
	JOYSTICK_K_KEY_init();
	while(1)
	{
		if(ADCConvertedValue[0] > 0x820 && ADCConvertedValue[0] < 0x840)
			printf("X�м�, 0x%04X\n",ADCConvertedValue[0]);//��ӡ
		else if(ADCConvertedValue[0] < 0x820)
			printf("X����, 0x%04X\n",ADCConvertedValue[0]);//��ӡ
		else if(ADCConvertedValue[0] > 0X840)
			printf("X����, 0x%04X\n",ADCConvertedValue[0]);//��ӡ
		
		printf("\n");//����
		
		if(ADCConvertedValue[1] > 0x7B0 && ADCConvertedValue[1] < 0x7D0)
			printf("Y�м�, 0x%04X\n",ADCConvertedValue[1]);//��ӡ
		else if(ADCConvertedValue[1] < 0x7B0)
			printf("Y����, 0x%04X\n",ADCConvertedValue[1]);//��ӡ
		else if(ADCConvertedValue[1] > 0x7D0)
			printf("Y����, 0x%04X\n",ADCConvertedValue[1]);//��ӡ
		
		printf("\n");
		
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7))
			printf("����̧��\r\n");
		else
			printf("��������\r\n");	
	}
}

void JOYSTICK_K_KEY_init()
{
	GPIO_InitTypeDef key;
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	key.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	key.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &key);
}

