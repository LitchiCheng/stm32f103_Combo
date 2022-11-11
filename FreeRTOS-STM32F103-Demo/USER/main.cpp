#include "stm32f10x.h"
#include "sys.h"
#include "PWM.H"
#include "Timer.h"

// FreeRTOS head file, add here.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"

uint64_t Encoder_Left;             //左右编码器的脉冲计数
float rpm;
void encoderHandle_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	Encoder_Init_TIM2();            //=====编码器接口
	//TIM3_Int_Init(499,7199);
	while(1)
	{
		Encoder_Left=(short)TIM2->CNT;
		//printf("%lld\r\n",Encoder_Left);
		printf("%f\r\n",rpm);
		//vTaskDelay(500/portTICK_RATE_MS);
	}
}

uint8_t cnt;
void pwmVel_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	PwmInit();
	LedIoInit();
	while(1)
	{
		LedPwmCtrl(50,50);
		vTaskDelay(70/portTICK_RATE_MS);
	}
}

int main(void)
{ 
	BaseTimer::Instance()->initialize();
	uart_init(115200);	            //=====串口初始化
	xTaskCreate( encoderHandle_Task, "encoder", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate( pwmVel_Task, "pwm", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
 
	vTaskStartScheduler();
}

