#ifndef _POWER_STATE_RECODER_MANAGER_H
#define _POWER_STATE_RECODER_MANAGER_H

#include "stmflash.h"
#include "stm32f10x.h"

void traverseLastPostionAndStateInFlash(const uint32_t block_first_address, const uint32_t block_end_address, uint32_t & address_now, bool & last_state);
void transferStateIfLastError(bool is_error);
void writeStateInPositionInFlash(uint32_t & address_now, uint8_t state);

#endif
