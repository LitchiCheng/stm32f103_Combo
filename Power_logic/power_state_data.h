#ifndef _POWER_STATE_DATA_H
#define _POWER_STATE_DATA_H

#include "stm32f10x.h"
#include "Singleton.h"

class PowerStateGlobalData
{
private:
    /* data */
public:
    PowerStateGlobalData(/* args */){};
    ~PowerStateGlobalData(){};

    uint32_t board_power_state_address_now;
	uint32_t pc_power_state_address_now;

	bool is_last_board_power_state_error;
	bool is_last_pc_power_state_error;
};


typedef NormalSingleton<PowerStateGlobalData> PSGlobalData;

#endif