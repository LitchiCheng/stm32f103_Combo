#ifndef _TOOLS_H_
#define _TOOLS_H_
// FreeRTOS head file, add here.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"
#include "sys.h"

#define getCout2ms()	xTaskGetTickCount()
#define getCout2s()		getCout2ms() / 1000
#define getLeftEncoder()	(TIM2->CNT)


#endif //_TOOLS_H_
