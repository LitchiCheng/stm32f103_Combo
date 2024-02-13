#include "RS232.H"
#include "stdio.h"
#include "SysTick.h"
#include "i2c-master.h"
#include "i2c-slave.h"

/*
适用于stm32f103vet6
硬件I2C: PB6 -- SCL; PB7 -- SDA 
*/

/*常用功能
delay_ms
printf
*/

int main()
{
	RS232_Init(9600);
	SysTick_Init(72);
	printf("program start...\n");
#if MASTER
	i2cMasterInit();
	printf("i2c master init...\n");
#else
	i2cSlaveInit();
	printf("i2c slave init...\n");
#endif
	while(1)
	{
		//如果I2C设备不正常，会卡在这个位置
		#if MASTER
			uint8_t data = i2cRead(0x12);
			printf("Master alive... 0x%02x\n", data);
		#else
			printf("Slave alive...\n");
		#endif 
		//i2cWrite(0x01, 0xaa);
		delay_ms(1000);
	}
}
