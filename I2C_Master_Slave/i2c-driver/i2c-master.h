#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H
#include <stm32f10x.h>

#include "i2c-define.h"

void i2cMasterInit(void);
void i2cWrite(uint8_t addr,uint8_t data);
uint8_t i2cRead(uint8_t addr);

#endif
