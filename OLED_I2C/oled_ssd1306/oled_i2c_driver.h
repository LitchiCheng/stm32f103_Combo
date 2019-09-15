#ifndef _OLED_I2C_DRIVER_H
#define _OLED_I2C_DRIVER_H
#include <stm32f10x.h>
#define OLED_ADDRESS 0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);

#endif