#include "power_state_recoder_manager.h"
#include "spi.h"



void traverseLastPostionAndStateInFlash(const uint32_t block_first_address, const uint32_t block_end_address, uint32_t & address_now, bool & last_state)
{
    uint32_t address_temp = block_first_address;
    uint8_t state_temp[2];
	address_now = address_temp;
    while(address_temp <= (block_end_address - 1))                         //end the cycle while at the end of block 
    {
        flasher::Instance()->Read(address_temp, (u16*)state_temp, 2);
        if ((state_temp[0] != 0xFF) && (state_temp[1] != 0xFF)) 
		{
			address_temp += 2;      //go on traverse
			//last_board_power_state is ON not OFF, there is a wrong
			if(state_temp[0] == 0x01)	last_state = true;
			else if(state_temp[0] == 0x02)	last_state = false;
        }
		else
        {
            address_now = address_temp;
            break;
        } 
    }
}

void transferStateIfLastError(bool is_error)
{
    if (true == is_error)
    {
        //transfer the error to 407 by spi
		//spiTest(0xAA);
    }
	else
	{
		//spiTest(0xBB);

	}
}

void writeStateInPositionInFlash(uint32_t & address_now, uint8_t state)
{
    if (1 == state)
	{
		flasher::Instance()->Write(address_now, (u16*)BOARD_POWER_ON_RECORD_BUFFER, 1);        //record ON state
		address_now = address_now + 2;	
	}		
    else if (0 == state) 
	{
		flasher::Instance()->Write(address_now, (u16*)BOARD_POWER_OFF_RECORD_BUFFER, 1);    //record OFF state
		address_now = address_now + 2;
	}
}