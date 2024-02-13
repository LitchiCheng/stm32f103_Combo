#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "misc.h"

#include "i2c-define.h"

void i2cSlaveInit(void);

#endif
