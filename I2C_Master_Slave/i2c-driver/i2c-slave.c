#include "i2c-slave.h"


#define I2C1_RAM_SIZE           256        // RAM Size in Byte

#define I2C1_MODE_WAITING       0          // Waiting for commands
#define I2C1_MODE_SLAVE_ADR_WR  1          // Received slave address (writing)
#define I2C1_MODE_ADR_BYTE      2          // Received ADR byte
#define I2C1_MODE_DATA_BYTE_WR  3          // Data byte (writing)
#define I2C1_MODE_SLAVE_ADR_RD  4          // Received slave address (to read)
#define I2C1_MODE_DATA_BYTE_RD  5          // Data byte (to read)

uint8_t i2c1_mode = I2C1_MODE_WAITING;
uint8_t i2c1_ram_adr = 0;
uint8_t i2c1_ram[I2C1_RAM_SIZE];

uint8_t Get_I2C1_Ram(uint8_t adr) 
{
    return i2c1_ram[adr];
}

void Set_I2C1_Ram(uint8_t adr, uint8_t val) 
{
    i2c1_ram[adr] = val;
    return;
}

void I2C1_Ram_Init(void) 
{
    uint16_t i;
    for (i = 0; i < 256; i++)
    {
        Set_I2C1_Ram(i, i+1);
    }
}

void i2cSlaveInit(void) 
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Configure I2C_EE pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure the I2C event priority */
    NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure I2C error interrupt to have the higher priority */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2CSLAVE_ADDR;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C1_CLOCK_FRQ;

    /* I2C Peripheral Enable */
    I2C_Cmd(I2C1, ENABLE);

    /* Apply I2C configuration after enabling it */
    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE); //Part of the STM32 I2C driver
    I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_ERR, ENABLE); //Part of the STM32 I2C driver

    I2C1_Ram_Init();
		printf("slave init config\n");
}

void I2C1_ClearFlag(void) 
{
	printf("I2C1_ClearFlag \n");
    /* ADDR Flag clear */
    while((I2C1->SR1 & I2C_SR1_ADDR) == I2C_SR1_ADDR) 
    {
			printf("I2C1_ClearFlag \n");
        I2C1->SR1;
        I2C1->SR2;
    }

    /* STOPF Flag clear */
    while((I2C1->SR1&I2C_SR1_STOPF) == I2C_SR1_STOPF) 
    {
			printf("I2C1_ClearFlag \n");
        I2C1->SR1;
        I2C1->CR1 |= 0x1;
    }
}

void I2C1_EV_IRQHandler(void) 
{
    uint8_t wert;
    uint32_t event;

    /* Reading last event */
    event = I2C_GetLastEvent(I2C1);

    /* Event handle */
		//EV1
    if(event == I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) 
    {
        // Master has sent the slave address to send data to the slave
        i2c1_mode = I2C1_MODE_SLAVE_ADR_WR;
			printf("I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED \n");
    }
		//Slave RECEIVER mode EV2
    else if(event == I2C_EVENT_SLAVE_BYTE_RECEIVED) 
    {
			printf("I2C_EVENT_SLAVE_BYTE_RECEIVED \n");
        // Master has sent a byte to the slave
        wert = I2C_ReceiveData(I2C1);
        // Check address
        if(i2c1_mode == I2C1_MODE_SLAVE_ADR_WR) 
        {
            i2c1_mode = I2C1_MODE_ADR_BYTE;
            // Set current ram address
            i2c1_ram_adr = wert;
        }
        else 
        {
            i2c1_mode = I2C1_MODE_DATA_BYTE_WR;
            // Store data in RAM
            Set_I2C1_Ram(i2c1_ram_adr, wert);
            // Next ram adress
            i2c1_ram_adr++;
        }
    }
		//EV1
    else if(event == I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) 
    {
			printf("I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED \n");
        // Master has sent the slave address to read data from the slave
        i2c1_mode = I2C1_MODE_SLAVE_ADR_RD;
        // Read data from RAM
        wert = Get_I2C1_Ram(i2c1_ram_adr);
        // Send data to the master
        I2C_SendData(I2C1, wert);
        // Next ram adress
        i2c1_ram_adr++;
    }
		//Slave TRANSMITTER mode EV3
    else if(event == I2C_EVENT_SLAVE_BYTE_TRANSMITTED) 
    {
			static uint8_t one = 1;
			if(one){
				one = 0;
				printf("I2C_EVENT_SLAVE_BYTE_TRANSMITTED 1\n");
					// Master wants to read another byte of data from the slave
					i2c1_mode = I2C1_MODE_DATA_BYTE_RD;
					// Read data from RAM
					wert = Get_I2C1_Ram(i2c1_ram_adr);
					// Send data to the master
					I2C_SendData(I2C1, wert);
				printf("I2C_EVENT_SLAVE_BYTE_TRANSMITTED 2\n");
					// Next ram adress
					i2c1_ram_adr++;
			}
			
    }
		//Slave RECEIVER mode EV4
    else if(event == I2C_EVENT_SLAVE_STOP_DETECTED) 
    {
			printf("I2C_EVENT_SLAVE_STOP_DETECTED \n");
        // Master has STOP sent
        I2C1_ClearFlag();
        i2c1_mode = I2C1_MODE_WAITING;
    }
}

void I2C1_ER_IRQHandler(void) 
{
	printf("I2C1_ER_IRQHandler \n");
    if (I2C_GetITStatus(I2C1, I2C_IT_AF)) 
    {
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
    }
}
