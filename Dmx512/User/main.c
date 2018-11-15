#include"RS485.H"
#include "SysTick.h"
#include "math.h"
#include <stdlib.h>
int8_t _increment;
uint8_t TXDData[50] = {0x00};
void calc(uint8_t * data);
static void _uart1_dma_configuration();
int main()
{
	SysTick_Init(72);
	RS485_Init(256000);	
 
	while(1)
	{	
		memset(TXDData, 0x00, sizeof(TXDData));
		calc(TXDData);
		GPIO_Tx_Config(0);					
		GPIO_SetBits(GPIOE, GPIO_Pin_4);		
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_ms(10); 							
		GPIO_Tx_Config(1);
		RS485_Send_Data(TXDData,50);	
		delay_ms(10);	
	}
}


void calc(uint8_t * data) {
	_increment = _increment + 1;
	int final_value = abs(_increment) * 1;
	if (final_value > 255)
	{
		final_value = 255;
	}
	for (int i = 1; i <= 50; i = i + 4)
	{
		data[i] = (uint8_t)final_value;
	}
}




