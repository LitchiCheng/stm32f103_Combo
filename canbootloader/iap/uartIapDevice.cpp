#include "uartIapDevice.h"
#include "Timer.h"
#include "printf.h"

namespace
{
	const uint16_t UART_RX_DMA_BUF_LEN = 30;
	const uint16_t UART_TX_DMA_BUF_LEN = 150;
	uint8_t uartRxDmaBuff[UART_RX_DMA_BUF_LEN];
	uint8_t uartTxDmaBuff[UART_TX_DMA_BUF_LEN];
	CUsart iapUsart(USART2, uartRxDmaBuff, UART_RX_DMA_BUF_LEN);
};

CUartIapDevice iapDevice(iapUsart);
CCharDev& iap_device(iapDevice);

CUartIapDevice::CUartIapDevice(CUsart& refUsart)
	:CCharDev(300), refUsart_(refUsart)
{
	
}

/**
* @brief  open
* @param  None
* @retval state
*/
int CUartIapDevice::open()
{
	refUsart_.InitSciGpio();
	refUsart_.InitSci();
	return 0;
}

/**
* @brief  close
* @param  None
* @retval 
*/
int CUartIapDevice::close()
{
	return 0;
}

/**
  * @brief  write
	* @param  databuf: the data pointer wants to send
	* @param  len: data length
  * @retval actual send length
  */
int CUartIapDevice::write(const uint8_t* buf, uint32_t len)
{
	return txBufQue_.push_array((uint8_t*)buf, len);
}

/**
  * @brief  read
	* @param  databuf: the data pointer wants to read
	* @param  len: data length
  * @retval actual read length
  */
int CUartIapDevice::read(uint8_t* buf, uint32_t len)
{
	return rxBufQue_.pop_array(buf, len);
}

/**
  * @brief  runReceiver
	* @param  None
  * @retval None
  */
void CUartIapDevice::runReceiver()
{
	static Timer recvTimer(1, 1);
	
	if(USART_GetFlagStatus(refUsart_.getUsartx(), USART_FLAG_ORE) == SET)
	{
		uint16_t tmp;
		tmp = refUsart_.getUsartx()->SR;
		tmp += refUsart_.getUsartx()->DR;
	}
	
	if(refUsart_.get_BytesInRxFifo() == 0)
		return;
	
	update_data_break_flag();
	if(recvTimer.isAbsoluteTimeUp() || refUsart_.get_BytesInRxFifo() > UART_RX_DMA_BUF_LEN/2)
	{
		refUsart_.read_RxFifo(rxBufQue_);

	}
}

void CUartIapDevice::runTransmitter()
{
	if(refUsart_.get_BytesInTxFifo() > 0)
		return;
	
	//refUsart_.get_BytesInTxFifo() == 0
	if(txBufQue_.elemsInQue() > 0)
	{
		uint16_t len = txBufQue_.pop_array(uartTxDmaBuff, UART_TX_DMA_BUF_LEN);
		refUsart_.send_Array(uartTxDmaBuff, len);
	}
}

/**
  * @brief  data_in_read_buf
	* @param  None
  * @retval bytes
  */
uint32_t CUartIapDevice::data_in_read_buf()
{
	return rxBufQue_.elemsInQue();
}

/**
  * @brief  clear read buffer
	* @param  None
  * @retval None
  */
void CUartIapDevice::clear_read_buf()
{
	refUsart_.clear_rxFifo();
	rxBufQue_.clear();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUartIapDevice::data_in_write_buf()
{
	return txBufQue_.elemsInQue();
}

/**
  * @brief  data in write buffer
	* @param  None
  * @retval number of bytes
  */
uint32_t CUartIapDevice::freesize_in_write_buf()
{
	return txBufQue_.emptyElemsInQue();
}

bool CUartIapDevice::isTransmitterIdle()
{ 
	runTransmitter();
	return (txBufQue_.elemsInQue() == 0 && iapUsart.TxDMA(iapUsart.getUsartx())->CNDTR == 0);
}

//end of file
