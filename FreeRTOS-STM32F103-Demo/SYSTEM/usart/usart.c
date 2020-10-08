#include "sys.h"
#include "usart.h"	  
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
		
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

#define Max_BUFF_Len 128
unsigned char Uart2_Buffer[Max_BUFF_Len];
unsigned int Uart2_Rx=0;
void USART1_IRQHandler(void)      //����1 �жϷ������
{
	uint8_t Clear = 0;
	u8 rx_dat;
	/*������յ�һ�ֽ�����*/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	    //�ж϶��Ĵ����Ƿ�ǿ�
	{	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	        //����жϱ�־
		Uart2_Buffer[Uart2_Rx] = USART_ReceiveData(USART1);     //���մ���1���ݵ�buff������
        Uart2_Rx++;
    /*������յ�һ֡����*/
	}else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET){		
		Clear = USART1->SR;
		Clear = USART1->DR;		//���IDLE��־
		Uart2_Buffer[Uart2_Rx] = 0x00;
		Uart2_Rx=0;
		printf("%s\r\n", Uart2_Buffer);
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)        //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 
	{ 
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);			//��ֹ�����������жϣ� 
	}	
}
