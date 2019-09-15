#include "RS232.H"
#include "stdio.h"
#include "SysTick.h"
#include "oled_bll.h"
#include "oled_i2c_driver.h"
//#include "codetab.h"
unsigned char BMP1[] = {
	0xFF,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xBD,0xBD,0x81,0xE7,0x3C,//C:\Users\97478\Desktop\oled\电池电量\电池电量\电池.BMP0,
};

void gpio_init(void);
void led_inverse();
char ss[] = "oled";

int main()
{
	RS232_Init(9600);
	gpio_init();
	SysTick_Init(72);
	printf("程序开始！！！！！！ \n");
  
	I2C_Configuration();
	OLED_Init();
	OLED_P8x16Str(0,3,"l Love Alice !");//第一行 -- 8x16的显示单元显示ASCII码
	OLED_P16x16Ch(0,0,0);
	OLED_P16x16Ch(16,0,1);
	OLED_P16x16Ch(32,0,2);
	Draw_BMP(0,0,0,0,BMP1);
	while(1)
	{
		led_inverse();
	}
    
}

void gpio_init(void)
{
	//PE0是按键0,上拉输入
	//pa10是led0,低电平点亮
	GPIO_InitTypeDef GPIO_input;
	//GPIO_InitTypeDef GPIO_Led0; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_input.GPIO_Pin = GPIO_Pin_0;
	GPIO_input.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE,&GPIO_input);
	
	GPIO_input.GPIO_Pin = GPIO_Pin_10;
	GPIO_input.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_input.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_input);
}

void led_inverse()
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)){
		//GPIO_SetBits(GPIOA, GPIO_Pin_10);
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
		//OLED_OFF();
	}
	else{
		//GPIO_ResetBits(GPIOA, GPIO_Pin_10);
		GPIO_SetBits(GPIOA, GPIO_Pin_10);
		//OLED_ON();
		//OLED
	}
}
