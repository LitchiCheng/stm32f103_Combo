#ifndef _I2C_DEFINE_H
#define _I2C_DEFINE_H

#include "stdio.h"

#define I2CMASTER_ADDR           0x30 << 1  //主机的I2C地址,随便写的
#define I2CSLAVE_ADDR           0x40 << 1  // address is 0x40
#define I2C1_CLOCK_FRQ          100000     // I2C-Frq in Hz (100 kHz)

#define MASTER 0

#endif //_I2C_DEFINE_H
