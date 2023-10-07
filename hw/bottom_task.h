#ifndef _BOTTOM_TASK_H_
#define _BOTTOM_TASK_H_

#include"config.h"
/*底盘代码*/
#ifdef  BOTTOM_LEVEL
/*FreeRTOS库文件*/
#include "FreeRTOS.h"
#include "task.h"


#include "motor_driver.h"
#include "hwt101.h"
#include "KinematicModel.h"
#include "pid.h"
#include "bottom_task.h"
#include "led.h"
#include "pid.h"
#include "sys.h"
#include "CarTaskConfig.h"

extern Arr_pLedStruct LED;
extern Arr_pStruct_Pos_PID tmp_pid; 

static TaskHandle_t taskStart_handle                       = NULL;
static TaskHandle_t taskMoveDriveOut_handle                = NULL;
static TaskHandle_t taskMoveStaAreaToQRArea_handle         = NULL;
static TaskHandle_t taskMoveQRAreaToTurnPlateArea_handle   = NULL;
static TaskHandle_t taskMoveTurnPlateAreaToStorArea_handle = NULL;
static TaskHandle_t taskMvBetweenColorCir_handle           = NULL;

void bsp_init(void);

void taskMoveDriveOut_start(void);
void taskMoveStaAreaToQRArea_start(void);
void taskMoveQRAreaToTurnPlateArea_start(void);
void taskMoveTurnPlateAreaToStorArea_start(void);
void taskMvBetweenColorCir_start(void);



void taskStart(void* pvParameters); 
void taskMoveDriveOut(void* pvParameters);
void taskMoveStaAreaToQRArea(void* pvParameters);
void taskMoveQRAreaToTurnPlateArea(void* pvParameters);
void taskMoveTurnPlateAreaToStorArea(void* pvParameters);
void taskMvBetweenColorCir(void* pvParameters);

#endif  //_BOTTOM_TASK_H_

#endif //BOTTOM_LEVEL