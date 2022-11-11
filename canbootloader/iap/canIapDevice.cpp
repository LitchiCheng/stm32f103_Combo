#include "canIapDevice.h"
#include "Console.h"

const uint8_t RX_MAILBOX_QUE_SIZE  = 5;
const uint16_t RX_BUFFER_QUE_SIZE = 300;
CanRxMsg rxMailboxBuf[RX_MAILBOX_QUE_SIZE];
uint8_t rxQueueBuffer[RX_BUFFER_QUE_SIZE];

/**
  * @brief  Constructor
	* @param  None
  * @retval bytes it reads
  */
CCanIapDevice::CCanIapDevice(CCanRouter& canBaseRouter,
														uint32_t txNodeId, 
														uint8_t txNodeIde, 
														uint32_t rxNodeId, 
														uint8_t rxNodeIde)
	:CCharDev(300), canBaseRouter_(canBaseRouter),
	rxBufQue_(rxQueueBuffer, RX_BUFFER_QUE_SIZE),
	rxMailbox_(rxMailboxBuf, RX_MAILBOX_QUE_SIZE),
	txNodeId_(txNodeId),txNodeIde_(txNodeIde)//,
//	data_flow_break_timer_(200, 200),
//	prev_elems_rx_que_(0)
{
	if(rxNodeIde == CAN_Id_Standard)
		rxMailbox_.setStdId(rxNodeId);
	else
		rxMailbox_.setExtId(rxNodeId);
}

/**
* @brief  open
* @param  None
* @retval state
*/
int CCanIapDevice::open()
{
	rxMailbox_.attachToRouter(canBaseRouter_);
	return 0;
	//TODO switch socket return type into CDevice return type
}

/**
* @brief  close
* @param  None
* @retval 
*/
int CCanIapDevice::close()
{
	return 0;
}

/**
  * @brief  write
	* @param  databuf: the data pointer wants to send
	* @param  len: data length
  * @retval actual send length
  */
int CCanIapDevice::write(const uint8_t* databuf, uint32_t len)
{
	CanTxMsg tempMsg;
	tempMsg.IDE = txNodeIde_;
	tempMsg.StdId = tempMsg.ExtId = txNodeId_;
	tempMsg.RTR = CAN_RTR_Data;
	tempMsg.DLC = 8;
	
	uint8_t* endptr = (uint8_t*)databuf + len; //the byte at endptr should not be used
	
	while(databuf + 8 < endptr)
	{
		memcpy(tempMsg.Data, databuf, 8);
		databuf += 8;
		canBaseRouter_.putMsg(tempMsg);
	}
	tempMsg.DLC = endptr - databuf;
	memcpy(tempMsg.Data, databuf, tempMsg.DLC);
	canBaseRouter_.putMsg(tempMsg);
	return len;
}

/**
  * @brief  read
	* @param  databuf: the data pointer wants to read
	* @param  len: data length
  * @retval actual read length
  */
int CCanIapDevice::read(uint8_t* databuf, uint32_t len)
{
	uint32_t ret = rxBufQue_.pop_array(databuf, len);
	return ret;
}

/**
  * @brief  runReceiver
	* @param  None
  * @retval None
  */
void CCanIapDevice::runReceiver()
{
	CanRxMsg tempMsg;
	if(0 == rxMailbox_.msgsInQue())
		return;
	
	update_data_break_flag();
	while(rxMailbox_.msgsInQue() > 0)
	{
		rxMailbox_.getMsg(&tempMsg);
		rxBufQue_.push_array(tempMsg.Data, tempMsg.DLC);
	}
	
//	if(rxBufQue_.elemsInQue() > prev_elems_rx_que_)
//	{
//		data_flow_break_timer_.reset();
//	}
//	prev_elems_rx_que_ = rxBufQue_.elemsInQue();
}

/**
  * @brief  data_in_read_buf
	* @param  None
  * @retval bytes
  */
uint32_t CCanIapDevice::data_in_read_buf()
{
	return rxBufQue_.elemsInQue();
}


/**
  * @brief  clear read buffer
	* @param  None
  * @retval None
  */
void CCanIapDevice::clear_read_buf()
{
	rxBufQue_.clear();
}
/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CCanIapDevice::data_in_write_buf()
{
	return 0;
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CCanIapDevice::freesize_in_write_buf()
{
	return canBaseRouter_.getTxQueFreeSize();
}





bool CCanIapDevice::isTransmitterIdle()
{
	return canBaseRouter_.isTransmitterIdel();
}

#define IAP_UPWARD_ID	0x5004
#define IAP_DOWNWARD_ID	0x5005
CCanIapDevice iapDevice(CanRouter1, IAP_UPWARD_ID, CAN_Id_Extended, IAP_DOWNWARD_ID, CAN_Id_Extended);
CCharDev& iap_device(iapDevice);
//end of file
