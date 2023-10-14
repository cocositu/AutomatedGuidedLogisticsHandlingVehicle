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
#include"CarTaskConfig.h"


static TaskHandle_t taskStart_handle  = NULL;
static TaskHandle_t taskLed_handle = NULL;
static TaskHandle_t taskQrcodeIdentify_handle = NULL;
static TaskHandle_t taskManCrawl_handle = NULL;
static TaskHandle_t taskPutItem_handle =NULL;

void bsp_init(void);

void taskStart_start(void);
void taskLed_start(void);
void taskQrcodeIdentify_start(void);
void taskPutItem_start(void);

void taskStart(void* pvParameters); 
void taskLed(void* pvParameters); 
void taskQRcodeIdentify(void* pvParameters);  
void taskManCrawl(void* pvParameters);
void taskPutItem(void* pvParameters);


#endif //TOP_LEVEL
#endif //_TOP_TASK_H_