#include "rs232.h"
	 
//���ջ����� 	
u8 RS232_RX_BUF[1];  	//���ջ���
u8 RS232_RX_CNT=0;


  		  
/*******************************************************************
*	���ܣ��жϷ�ʽ��������
*******************************************************************/  
void USART1_IRQHandler(void)
{
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����	   
	{	 
	 	RS232_RX_BUF[0] =USART_ReceiveData(USART1); 	//��ȡ���յ�������
		RS232_RX_CNT=1;	
	}										 
} 

			
										 
/********************************************************************
*	���ܣ�RS232��ʼ��
*	������bound ������
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
 

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//��λ����1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//ֹͣ��λ
 
	USART_InitStructure.USART_BaudRate = BaudRate;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1

  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //ʹ�ܴ���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
   
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
 
}

/**********************************************************
*	���ܣ���������
*	������buf ���ͻ������׵�ַ
*		  len �����͵��ֽ���
**********************************************************/
void RS232_Send_Data(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//�������  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS232_RX_CNT=0;	  
}

/***********************************************************
*	ע�⣺�����Ҫʹ��printf�������ͱ����ض������������
*		����ʹ�κδ���
***********************************************************/
int fputc(int ch, FILE *f)
{ 
   USART_SendData(USART1, (u8) ch); 
   while(!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET))
   {
   }
   return ch;
}

