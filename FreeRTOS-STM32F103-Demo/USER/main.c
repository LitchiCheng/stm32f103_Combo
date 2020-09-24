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
#include "tools.h"

uint64_t Encoder_Left;            
double rpm;
static uint32_t last_cout = 0;
static uint32_t last_time = 0;

double filterRpm(double rpm){
	const int SAMPLE_NUM = 100;
	static double filter_rpm[SAMPLE_NUM] = {0.0f};
	static uint8_t index = 0;
	double sum = 0;
	if(index <= SAMPLE_NUM-1){
		filter_rpm[index] = rpm;
		index++;
		sum = rpm * SAMPLE_NUM;
	}else{
		for(int i=0;i<SAMPLE_NUM-1;i++){
			filter_rpm[i] = filter_rpm[i+1];
			sum += filter_rpm[i+1];
		}
		filter_rpm[SAMPLE_NUM-1] = rpm;
		sum+=rpm;
	}
	return sum/SAMPLE_NUM;
}

#define kp 20
#define kd	0.01
#define ki	0.001
static int32_t printdiff = 0;
void encoderHandle_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	Encoder_Init_TIM2();   
	TIM3_Int_Init(499,7199);
	
	while(1)
	{
		uint32_t now_cout = getLeftEncoder();
		uint32_t now_time = getCout2ms();
		if(now_time - last_time >= 2){
			//printf("1s up \r\n");
			rpm = ((now_cout - last_cout) / 400.0f) * 100.0f * 60.0f;
			int32_t diff = (20000-last_cout);
			diff = (diff < 0) ? -diff : diff;
			printdiff = diff;
			static uint32_t last_diff = 0;
			int32_t diff_speed = (diff - last_diff);
			diff_speed = (diff_speed < 0) ? -diff_speed:diff_speed;
			static uint32_t diff_sum =0;
			diff_sum += diff_speed;
			last_diff = diff;
			int pwm = 100-((kp*diff+ki*diff_sum+kp*diff_speed))/10000;
			if(diff >= 10 && pwm>98) pwm = 98;
			LedPwmCtrl(pwm,50);
			last_cout = now_cout;
			last_time = now_time;
			rpm = filterRpm(rpm);
			
		}
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
		printf("%d\r\n",printdiff);
		vTaskDelay(100/portTICK_RATE_MS);
	}
}

int main(void)
{ 
	uart_init(128000);	            //=====´®¿Ú³õÊ¼»¯
	xTaskCreate( encoderHandle_Task, "encoder", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate( pwmVel_Task, "pwm", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
 
	vTaskStartScheduler();
}

