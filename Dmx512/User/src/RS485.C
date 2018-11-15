#include "rs485.h"
	 
//接收缓存区 	
u8 RS485_RX_BUF[1];  	//接收缓冲
u8 RS485_RX_CNT=0;


  		  
/*******************************************************************
*	功能：中断方式接收数据
*******************************************************************/  
void USART3_IRQHandler(void)
{
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据	   
	{	 
	 	RS485_RX_BUF[0] =USART_ReceiveData(USART3); 	//读取接收到的数据
		RS485_RX_CNT=1;	
	}										 
} 

			
										 
/********************************************************************
*	功能：485初始化
*	参数：bound 波特率
********************************************************************/  
void RS485_Init(u32 BaudRate)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PE4端口配置,485收发控制
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOB, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//复位串口3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//停止复位
 
	USART_InitStructure.USART_BaudRate = BaudRate;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口3

  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART3, ENABLE);                    //使能串口 
	RS485_TX_EN(0);			//默认为接收模式
 
}

/**********************************************************
*	功能：发送数据
*	参数：buf 发送缓冲区首地址
*		  len 待发送的字节数
**********************************************************/
void RS485_Send_Data(uint8_t *buf,u16 len)
{
	u8 t;
	RS485_TX_EN(1);			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//发送完成  
		USART_SendData(USART3,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_TX_EN(0);				//设置为接收模式	
}

/*******************************************************************************
* 函数名称：void GPIO_Tx_Normal_Config(void)
* 功能描述：发送引脚变为普通IO
* 参数含义：无
* 备    注: 无
*********************************************************************************/
void GPIO_Tx_Config(u8 Set)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  //设置发送的引脚为普通IO
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	if(0 == Set)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通IO
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用IO
	}
  GPIO_Init(GPIOB, &GPIO_InitStructure); //引脚初始化
}


