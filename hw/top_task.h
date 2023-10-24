#ifndef _TOP_TASK_H_
#define _TOP_TASK_H_

#include"config.h"

/*顶板代码*/
#ifdef  TOP_LEVEL
/*FreeRTOS库文件*/
#include "FreeRTOS.h"
#include "task.h"
/*标准C库文件*/
#include"stdio.h"
#include"string.h"
/*顶部代码*/
#include"QRcode.h"
#include "lcd_init.h"
#include "lcd.h"
#include"manipulator.h"
#include"openmv.h"
#include"manipulator.h"
/*两层通用文件*/
#include "led.h"

static TaskHandle_t task_taskSchedule_handle            = NULL;
static TaskHandle_t task_comUart_handle                 = NULL;
static TaskHandle_t task_identifyQRcode_handle          = NULL;
static TaskHandle_t task_identify_grab_Material_handle  = NULL;
static TaskHandle_t task_indetifyCrileColor_handle      = NULL;
static TaskHandle_t task_grabMaterial_handle            = NULL;
static TaskHandle_t task_putMaterial_handle             = NULL;


void bsp_init(void);
void task_switch(uint8_t task_name);

void task_taskSchedule_start(void);
void task_comUart_start(void);
void task_identifyQRcode_start(void);
void task_identify_grab_Material_start(void);
void task_indetifyCrileColor_start(void);
void task_grabMaterial_start(void);
void task_putMaterial_start(void);

void task_taskSchedule(void* pvParameters);
void task_comUart(void* pvParameters);
void task_identifyQRcode(void* pvParameters);
void task_identify_grab_Material(void* pvParameters);
void task_indetifyCrileColor(void* pvParameters);
void task_grabMaterial(void* pvParameters);
void task_putMaterial(void* pvParameters);


#endif //TOP_LEVEL
#endif //_TOP_TASK_H_