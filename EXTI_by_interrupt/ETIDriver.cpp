

void KEY_Init(void) 
{ 
     GPIO_InitTypeDef GPIO_InitStructure;
    
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PE2~4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
     GPIO_Init(GPIOE, &GPIO_InitStructure);//GPIOE2,3,4
     // WK_UP-->GPIOA.0  
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOA.0

}

GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);//开启中断线
GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

EXTI_InitTypeDef EXTI_InitStructure;
EXTI_InitStructure.EXTI_Line = EXTI_Line2 ; //PE2 
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    
EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  
EXTI_Init(&EXTI_InitStructure);

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;   // PE2
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
NVIC_Init(&NVIC_InitStructure); 


void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {

    }
    EXTI_ClearITPendingBit(EXTI_Line4);
}