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

static int32_t EncCnt = 0;
void encoderCnt_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	static int encoder[2] = {0 , 0};// 两次编码器读值，用以计算溢出方向
	static int N = 0;// 圈数
	while(1){
		encoder[1] = TIM2->CNT;
		if((encoder[1] - encoder[0] ) > 0x7FFF){
			N--;
		}
		else if( (encoder[0] - encoder[1] ) > 0x7FFF){
			N++;
		}
		EncCnt = N * 0xFFFF + encoder[1];
		encoder[0] = encoder[1];
		vTaskDelay(1/portTICK_RATE_MS);
	}
}

uint64_t Encoder_Left;            
double rpm;
static int32_t last_cout = 0;
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
void encoderHandle_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	Encoder_Init_TIM2();   
	TIM3_Int_Init(499,7199);
	
	while(1)
	{
		int64_t now_cout = EncCnt;
		uint32_t now_time = getCout2ms();
		if(now_time - last_time >= 2){
			rpm = ((now_cout - last_cout) / 400.0f) * 100.0f * 60.0f;
			int64_t diff = ((int)50-last_cout);
			//diff = (diff < 0) ? -diff : diff;
			static int32_t last_diff = 0;
			int32_t diff_speed = (diff - last_diff);
			diff_speed = (diff_speed < 0) ? -diff_speed:diff_speed;
			static int32_t diff_sum =0;
			diff_sum += diff_speed;
			last_diff = diff;
			int pwm = 100-((kp*diff+ki*diff_sum+kp*diff_speed)/ 20000);
			if(diff >= 5 && pwm>98) pwm = 98;
			//LedPwmCtrl(pwm,50);
			//printf("%lld,%d\n",diff,pwm);
			last_cout = now_cout;
			last_time = now_time;
			rpm = filterRpm(rpm);
		}
	}
}



xQueueHandle xQueuePwm;

void pwmVel_Task(void *pvParameters)
{
	static u8 state = 0;
	pvParameters = pvParameters;
	PwmInit();
	LedIoInit();
	xQueuePwm = xQueueCreate( 10, sizeof(uint8_t) );
	while(1)
	{
		
		static uint8_t pwm = 0;
		if( xQueueReceive( xQueuePwm, &( pwm ), ( TickType_t ) 10 ) )
        {
            LedPwmCtrl(pwm,50);
        }
		vTaskDelay(100/portTICK_RATE_MS);
		printf("%d,%f\r\n",pwm,rpm);
	}
}

xQueueHandle xQueueRx;
void uartCmd_Task(void *pvParameters)
{
	portCHAR cChar;
    xQueueRx = xQueueCreate(20, sizeof(u8));/*创建一个深度为20的队列*/
    while(1)
    {
        if(xQueueReceive(xQueueRx, &cChar, 10/portTICK_RATE_MS)==pdTRUE) /*队列中有数据*/
        {
            printf("%c",cChar);/*输出数据*/
			if(xQueuePwm != 0)
				xQueueSend( xQueuePwm, ( void * ) &cChar, ( TickType_t ) 0 );
        }
    }
}



int main(void)
{ 
	uart_init(128000);	            //=====串口初始化
	xTaskCreate( encoderHandle_Task, "encoder", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate( encoderCnt_Task, "cnt", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );
	xTaskCreate( pwmVel_Task, "pwm", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
	xTaskCreate( uartCmd_Task, "uart_cmd", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+4, NULL );
 
	vTaskStartScheduler();
}

