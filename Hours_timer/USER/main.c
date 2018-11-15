
#include "stm32f10x.h"
#include "SysTick.h"
#include "led.h"
#include "key.h" 
void delay_h(int hour);
void openGPIO();
void closeallGPIO();

int main(void)
{
	
	SystemInit();		
	LED_GPIO_Config(); 
	Key_GPIO_Config();
	SysTick_Init();
	int key =0;
	int hours =0;
	bool isKey1 = FALSE;
	bool isSetbit = FALSE;
	while (1)
		{	
				if( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 0  )
			{
				if (!isKey1)
				{
					hours = 0;
				}
				if (isSetbit)
					{
						key = 3;
					}
					else{key =2;}
			}
			else if( Key_Scan(GPIOA,GPIO_Pin_1) == 0  )
			{
				key =1;
				isKey1 = TRUE;
			}
			else
			{
				key=0;
			}
		
			switch(key)
			{
				case 1:
					GPIO_ResetBits(GPIOB,GPIO_Pin_9);
						if (hours++ <6)
					{
						if (1==hours)
						{GPIO_SetBits(GPIOA,GPIO_Pin_3);}
						else if (2==hours)
						{GPIO_SetBits(GPIOA,GPIO_Pin_4);}
						else if (3==hours)
						{GPIO_SetBits(GPIOB,GPIO_Pin_5);}
						else if (4==hours)
						{GPIO_SetBits(GPIOB,GPIO_Pin_6);}
						else if (5==hours)
						{GPIO_SetBits(GPIOB,GPIO_Pin_7);}
						else if (6==hours)
						{GPIO_SetBits(GPIOB,GPIO_Pin_8);}
					}
					else 
						{
							hours=0;
							closeallGPIO();
						}
				break;
		    case 2:
						delay_h(hours);
						GPIO_SetBits(GPIOB,GPIO_Pin_9);
						isSetbit = TRUE;
						hours = 0;
						closeallGPIO();				
				break;
				case 3:
						break;
				default:
						isSetbit = FALSE;
						GPIO_ResetBits(GPIOB,GPIO_Pin_9);
				break;
			}
		}

	}




void delay_h(int hour)
{
		int t=1;
		while(t <= hour)
		{
			Delay_us(36*100000);
			t++;
		}
}


void closeallGPIO()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}




