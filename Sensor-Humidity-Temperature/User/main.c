#include"RS232.H"
#include "stdio.h"
#include "SysTick.h"
#include "sht.h"
void gpio_init(void);
int main()
{
	RS232_Init(9600);
	gpio_init();
	SysTick_Init(72);
	printf("³ÌÐò¿ªÊ¼£¡£¡£¡£¡£¡£¡ \n");
    while (sht_probe() != STATUS_OK) 
	{
		printf("SHT sensor probing failed\n"); 
    }
		printf("SHT sensor probing successful\n");
    while (1) 
	{
        s32 temperature, humidity;
        s8 ret = sht_measure_blocking_read(&temperature, &humidity);
        if (ret == STATUS_OK) {
            printf("measured temperature: %0.2f degreeCelsius, "
                      "measured humidity: %0.2f percentRH\n",
                      temperature / 1000.0f,
                      humidity / 1000.0f);
        } 
		else 
		{
            printf("error reading measurement\n");
        }
        delay_ms(200);
    }
    
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_input;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_input.GPIO_Pin = GPIO_Pin_0;
	GPIO_input.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE,&GPIO_input);
}
