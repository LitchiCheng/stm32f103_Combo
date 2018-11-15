
#include "key.h" 

 /*不精确的延时 */
 void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

 /*	 配置按键用到的I/O口 */
void Key_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //开启按键端口PB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启按键端口PA的时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化端口

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为上拉输入
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化端口
}

 /* 检测是否有按键按下 */
u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
		/*检测是否有按键按下 */
   	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
	  {	   
	 	 /*延时消抖*/
	  	Delay(50000);		
	   		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
					{	 
						/*等待按键释放 */
						while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			 			return 	KEY_ON;	 
					}
			else
					return KEY_OFF;
		}
	else
		return KEY_OFF;
}
