#include "logic_out.h"
#include "power_process.h"
#include "power_state_data.h"

Timer openpc(500,500);
Timer close_pc_key_timer(2000,2000);
Timer closepc(500,500);
Timer force_close_pc_timer(40000,40000);

bool is_openpc_ready = true;
bool is_closepc_ready = false;
bool is_open_process_timer_open = true;
bool is_close_key_timer_open = true;
bool is_close_process_timer_open = true;
bool is_close_key_time_enough = false;
bool is_can_close_pc_process = false;
bool is_can_open_pc_process = false;
bool is_force_close_pc = false;
bool is_over_time_monitor = false;

void powerProcess()
{
	if (is_openpc_ready)              //按了下开机按键，开机开始按下后是高
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1)
		{
			launch_board();
			enable_battery();
			is_can_open_pc_process = true;
		}
		if(is_can_open_pc_process)
		{	
			/*****************************************open pc process************************************/
			pc_en_line_low();
			if (is_open_process_timer_open)             
			{
				openpc.reset();
				is_open_process_timer_open = false;
			}
			if (openpc.isAbsoluteTimeUp())
			{
				pc_en_line_high();
				is_open_process_timer_open = true;
				is_can_open_pc_process = false;
				is_openpc_ready = false;
				is_closepc_ready = true;
				PSGlobalData::Instance()->is_last_board_power_state_error = true;
			}
			/*********************************************************************************************/
		}
	}

	if(is_closepc_ready)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)                   //PC已经关机了，所以直接断开电源GPIOA, GPIO_Pin_0		关机是高
		{
			BaseTimer::Instance()->delay_ms(1000);
			shutdown_board();
			disable_battery();
			BaseTimer::Instance()->delay_ms(250);					//防止开机键在24V光耦衰减时的高检测，误开机
			/********************************************/
			is_openpc_ready = true;
			is_closepc_ready = false;
			is_open_process_timer_open = true;
			is_close_key_timer_open = true;
			is_close_process_timer_open = true;
			is_close_key_time_enough = false;
			is_can_close_pc_process = false;
			is_can_open_pc_process = false;
			is_force_close_pc = false;
			is_over_time_monitor = false;
		}
		
		if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1) && !is_close_key_time_enough)
		{
			if (is_close_key_timer_open)
			{
				close_pc_key_timer.reset();
				is_close_key_timer_open = false;
			} 
			if(close_pc_key_timer.isAbsoluteTimeUp())
			{
				is_close_key_timer_open = true;
				is_over_time_monitor = true;
			}
		}
		else if ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0))				//如果按下就松掉，那么时钟也要重新开
		{
			is_close_key_timer_open = true;
			is_close_key_time_enough = false;
		}
		
		if(is_over_time_monitor)												//超过2s后松掉才执行后面的关机流程
		{
			if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0))
			{
				is_over_time_monitor = true;
				/****enter in normal close pc process********/
				is_close_key_time_enough = true;
				is_can_close_pc_process = true;
				/*******************************************/
				/****start force close pc process****/
				is_force_close_pc = true;
				force_close_pc_timer.reset();
				/***************************************/
			}
		}

		if(is_can_close_pc_process)                     //正常关机流程，关机按住500ms
		{
			pc_en_line_low();
			if (is_close_process_timer_open)
			{
				closepc.reset();
				is_close_process_timer_open = false;
			}
			if (closepc.isAbsoluteTimeUp())
			{
				pc_en_line_high();
				is_close_process_timer_open = true;
				is_can_close_pc_process = false;
			}
		}

		if(is_force_close_pc && force_close_pc_timer.isAbsoluteTimeUp())            //强制关机流程，40s后关机
		{
			BaseTimer::Instance()->delay_ms(1000);
			shutdown_board();
			disable_battery();
			BaseTimer::Instance()->delay_ms(250);
			/********************************************/
			is_openpc_ready = true;
			is_closepc_ready = false;
			is_open_process_timer_open = true;
			is_close_key_timer_open = true;
			is_close_process_timer_open = true;
			is_close_key_time_enough = false;
			is_can_close_pc_process = false;
			is_can_open_pc_process = false;
			is_force_close_pc = false;
			is_over_time_monitor = false;
		}
	}
}

