#include"RS485.H"
#include "SysTick.h"
#include "math.h"
#include <stdlib.h>
int8_t _increment;
uint8_t TXDData[50] = {0x00};
static void _uart1_dma_configuration();

#define red_channel 0
#define green_channel 2
#define blue_channel 3
#define white_channel 1

void calcRedBreath(uint8_t * data) {
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

void ConstantLightCalculator(uint8_t * data) {
	for (int i = 1; i <= 240; i = i + 4)
	{
		data[i + red_channel] = 200;
		data[i + green_channel] = 150;
		data[i + blue_channel] = 30;
		data[i + white_channel] = 0;
	}
}

void ChargingCalculator(uint8_t * data) {
	/* Orange color is R:255 and B:165 */
	_increment = _increment + 2;
	int final_value = abs(_increment) * 2;
	if (final_value > 255)
	{
		final_value = 255;
	}
	for (int i = 1; i <= 240; i = i + 4)
	{
		data[i + red_channel] = final_value;
		data[i + green_channel] = ((165 * final_value) / 255);
	}
}

void FlowCalculator(uint8_t * data) 
{
	
	static uint8_t next_case = 1;
	static uint8_t next_position = 1;
	//if (flow_speed_control.isAbsoluteTimeUp())
	{
		memset(data,0x00,100);
		next_position += 4;
		if (next_position >= 50) { next_position = 1; }
		data[next_position + red_channel] = 255;
		data[next_position + green_channel] = 255;
		data[next_position + blue_channel] = 0;
		data[next_position + white_channel] = 0;
	}
}

int main()
{
	SysTick_Init(72);
	RS485_Init(256000);	
 
	while(1)
	{	
		memset(TXDData, 0x00, sizeof(TXDData));
		
		//calcRedBreath(TXDData);
		//ConstantLightCalculator(TXDData);
		//ChargingCalculator(TXDData);
		FlowCalculator(TXDData);
		
		GPIO_Tx_Config(0);					
		GPIO_SetBits(GPIOE, GPIO_Pin_4);		
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_ms(10); 							
		GPIO_Tx_Config(1);
		RS485_Send_Data(TXDData,50);	
		delay_ms(20);	
	}
}







