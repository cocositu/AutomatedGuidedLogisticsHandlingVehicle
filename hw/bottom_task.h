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

static TaskHandle_t task_taskSchedule_handle = NULL;
static TaskHandle_t task_moveSzoneToQRzone_handle = NULL;
static TaskHandle_t task_moveQRzoneToMzone_handle = NULL;
static TaskHandle_t task_moveMzoneToEzone_handle = NULL;
static TaskHandle_t task_moveEzoneToTzone_handle = NULL;
static TaskHandle_t task_moveTzoneToMzone_handle = NULL;
static TaskHandle_t task_moveTzoneToSzone_handle = NULL;
static TaskHandle_t task_moveBetweenCricle_handle = NULL;
static TaskHandle_t task_moveXYPosition_handle = NULL;
static TaskHandle_t task_moveContorCricle_handle = NULL;

void bsp_init(void);
void task_switch(uint8_t task_name);
void task_taskSchedule_start(void);
void task_moveSzoneToQRzone_start(void);
void task_moveQRzoneToMzone_start(void);
void task_moveMzoneToEzone_start(void);
void task_moveEzoneToTzone_start(void);
void task_moveTzoneToMzone_start(void);
void task_moveTzoneToSzone_start(void);
void task_moveBetweenCricle_start(void);
void task_moveContorCricle_start(void);
void task_moveXYPosition_start(void);

void task_taskSchedule(void* pvParameters);
void task_moveSzoneToQRzone(void* pvParameters);
void task_moveQRzoneToMzone(void* pvParameters);
void task_moveMzoneToEzone(void* pvParameters);
void task_moveEzoneToTzone(void* pvParameters);
void task_moveTzoneToMzone(void* pvParameters);
void task_moveTzoneToSzone(void* pvParameters);
void task_moveBetweenCricle(void* pvParameters);
void task_moveContorCricle(void* pvParameters);
void task_moveXYPosition(void* pvParameters);

#endif  //_BOTTOM_TASK_H_

#endif //BOTTOM_LEVEL