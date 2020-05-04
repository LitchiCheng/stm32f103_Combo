#include "rs232.h"
	 
//接收缓存区 	
u8 RS232_RX_BUF[1];  	//接收缓冲
u8 RS232_RX_CNT=0;


  		  
/*******************************************************************
*	功能：中断方式接收数据
*******************************************************************/  
void USART1_IRQHandler(void)
{
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据	   
	{	 
	 	RS232_RX_BUF[0] =USART_ReceiveData(USART1); 	//读取接收到的数据
		RS232_RX_CNT=1;	
	}										 
} 

			
										 
/********************************************************************
*	功能：RS232初始化
*	参数：bound 波特率
********************************************************************/  
void RS232_Init(u32 BaudRate)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//复位串口1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//停止复位
 
	USART_InitStructure.USART_BaudRate = BaudRate;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口1

  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //使能串口3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 
 
}

/**********************************************************
*	功能：发送数据
*	参数：buf 发送缓冲区首地址
*		  len 待发送的字节数
**********************************************************/
void RS232_Send_Data(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//发送完成  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS232_RX_CNT=0;	  
}

/***********************************************************
*	注意：如果需要使用printf函数，就必须重定义这个函数。
*		可以使任何串口
***********************************************************/
int fputc(int ch, FILE *f)
{ 
   USART_SendData(USART1, (u8) ch); 
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET))
   {
   }
   return ch;
}

