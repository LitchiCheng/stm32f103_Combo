#ifndef CAN_IAP_DEVICE_H
#define CAN_IAP_DEVICE_H
#include "chardev.h"
#include "CCan.h"

class CCanIapDevice
	:public CCharDev
{
	public:
		CCanIapDevice(CCanRouter& canBaseRouter,
									uint32_t txNodeId, 
									uint8_t txNodeIde, 
									uint32_t rxNodeId, 
									uint8_t rxNodeIde);	
		virtual int open();
		virtual int close();
	
		virtual int write(const uint8_t*, uint32_t);
		virtual int read(uint8_t*, uint32_t);
	
		virtual uint32_t data_in_write_buf();
		virtual uint32_t freesize_in_write_buf();
		virtual uint32_t data_in_read_buf();
		virtual void clear_read_buf();
		virtual bool isTransmitterIdle();
	
		virtual void runTransmitter() {}
		virtual void runReceiver();
			
	private:
		CCanRouter& canBaseRouter_;
		ringque<uint8_t> rxBufQue_;
		CCanRxMailbox rxMailbox_;
		uint32_t txNodeId_;
		uint8_t txNodeIde_;
//		Timer data_flow_break_timer_;
//		uint16_t prev_elems_rx_que_;
};

extern CCanIapDevice iapDevice;

#endif
//end of file
