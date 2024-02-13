#include "i2c-master.h"

#define I2C_SLAVE_ADDRESS 0x78

void i2cMasterInit(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);//使用I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2CMASTER_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C1_CLOCK_FRQ;

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}

void i2cWrite(uint8_t addr,uint8_t data)
{
#ifdef DEBUG
	printf("Enter in send\n");
#endif
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
#ifdef DEBUG
	printf("Write start\n");
#endif	
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
#ifdef DEBUG
	printf("Writer device addr 0x%02x\n", I2CSLAVE_ADDR);
#endif
	I2C_Send7bitAddress(I2C1, I2CSLAVE_ADDR, I2C_Direction_Transmitter);//器件地址
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#ifdef DEBUG
	printf("Write addr 0x%02x\n", addr);
#endif
	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
#ifdef DEBUG
	printf("Write data 0x%02x\n", data);
#endif
	I2C_SendData(I2C1, data);//发送数据
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
#ifdef DEBUG
	printf("Write stop\n");
#endif	
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}

uint8_t i2cRead(uint8_t addr)
{
	uint8_t data = 0x00;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
#ifdef DEBUG
	printf("Enter in read\n");
#endif
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
#ifdef DEBUG
	printf("Read send device addr\n");
#endif	
	I2C_Send7bitAddress(I2C1, I2CSLAVE_ADDR, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
#ifdef DEBUG
	printf("Read send addr\n");
#endif	
	I2C_SendData(I2C1, addr);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
#ifdef DEBUG
	printf("Read restart \n");
#endif	
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
#ifdef DEBUG
	printf("Read send device addr\n");
#endif	
	I2C_Send7bitAddress(I2C1, I2CSLAVE_ADDR | 0x01, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
#ifdef DEBUG
	printf("Read data\n");
#endif	
	data = I2C_ReceiveData(I2C1);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
#ifdef DEBUG
	printf("Read stop 0x%02x\n", data);
#endif	
	//I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	return data;
}

