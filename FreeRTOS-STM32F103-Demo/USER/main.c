#include "stm32f10x.h"
#include "sys.h"
#include "PWM.H"
  /**************************************************************************

**************************************************************************/

// FreeRTOS head file, add here.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"

int Encoder_Left;             //���ұ��������������
void encoderHandle_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	Encoder_Init_TIM2();            //=====�������ӿ�
	TIM3_Int_Init(499,7199);
	while(1)
	{
		printf("%d\r\n",Encoder_Left);
		vTaskDelay(500/portTICK_RATE_MS);
	}
}

uint8_t cnt;
void pwmVel_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	PwmInit();
	while(1)
	{
		for(cnt=0;cnt<100;cnt+=5)//ÿ��70MS�ı�һ��ռ�ձ�
		{
			LedPwmCtrl(cnt,cnt);
			vTaskDelay(1000/portTICK_RATE_MS);
		}
		for(cnt=100;cnt>0;cnt-=5)//ÿ��70MS�ı�һ��ռ�ձ�
		{
			LedPwmCtrl(cnt,cnt);
			vTaskDelay(1000/portTICK_RATE_MS);
		}
	}
}

int main(void)
{ 
	uart_init(9600);	            //=====���ڳ�ʼ��
	xTaskCreate( encoderHandle_Task, "encoder", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate( pwmVel_Task, "pwm", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

	vTaskStartScheduler();
	while(1)
	{
		printf("%d\r\n",Encoder_Left);
	} 
}

