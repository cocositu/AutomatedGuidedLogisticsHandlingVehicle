/*arm_math.h ,math.h这两个头文件是dsp库，用来加速浮点运算使用*/
// #include "arm_math.h"
// #include "math.h"
/*STM32F407ZGT6标准库头文件*/
#include "stm32f4xx.h" 
/*FreeRTOS库文件*/
#include "FreeRTOS.h"
#include "task.h"
/*标准C库文件*/
#include"stdio.h"
#include"string.h"
/*配置以及自定义系统文件*/
#include "config.h"
#include "sys.h"

/*顶部代码*/
#ifdef  TOP_LEVEL
#include"QRcode.h"
#include "lcd_init.h"
#include "lcd.h"
#include"manipulator.h"
#include"openmv.h"
#include"top_task.h"
#endif //TOP_LEVEL

/*底盘代码*/
#ifdef  BOTTOM_LEVEL
#include "motor_driver.h"
#include "hwt101.h"
#include "KinematicModel.h"
#include "pid.h"
#include "bottom_task.h"
#endif //BOTTOM_LEVEL

/*两层通用文件*/
#include "led.h"
#include"CarTaskConfig.h"


// #include"uart.h"
// float EcgDiscrList[28];
// float stdResultDiffECG;
// arm_std_f32(EcgDiscrList,28-4,&stdResultDiffECG);


#ifdef  TOP_LEVEL

int main(void){
	bsp_init();


	// while (1){
	// 	OV_Struct.TaskNum = 0x01;
	// 	OV_SendData(0x01);
	// 	delay_xms(100);
	// 	if(OV_Struct.TaskState == 'y'){
	// 		LCD_ShowString(0, 0, "red", WHITE, BLACK, 24, 0);
	// 	}
	// }	
	// ManStepUartCtrl(0,50, 40, 4000, 1);
	taskStart_start();
	// taskQrcodeIdentify_start();
	// taskLed_start();
	/* 开启任务调度 */
	vTaskStartScheduler(); 
}


#endif //TOP_LEVEL

#ifdef BOTTOM_LEVEL

int fputc(int ch,FILE *p) {
 	USART_SendData(USART6, (uint8_t)ch);
 	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE)==RESET);
 	return ch;
}
static TaskHandle_t taskled_handle = NULL;
static void task_Led(void* pvParameters);
int main(void){
	bsp_init();
	pid_init();
	delay_xms(5000);
	TranslationMove_PID(1, 0.6272, -0.6272, False);
	delay_xms(5000);
	MoveInLine_PID(1, 6.955, False);
	delay_xms(5000);
	delay_xms(5000);
	AntiClockwise_90Angle(UART_CTRL);
	while (1)
	{
		/* code */
	}
	
	

	
	// /* 创建taskStart任务 */
	// xTaskCreate((TaskFunction_t )taskStart,  	/* 任务入口函数 */
	// 		  (const char*  )"taskStart",		/* 任务名字 */
	// 		  (uint16_t     )512,  				/* 任务栈大小 */
	// 		  (void*        )NULL,				/* 任务入口函数参数 */
	// 		  (UBaseType_t  )4, 				/* 任务的优先级 */
	// 		  (TaskHandle_t*)&taskStart_handle);	/* 任务控制块指针 */ 
		/* 创建taskStart任务 */
	// xTaskCreate((TaskFunction_t ) task_Led,  	/* 任务入口函数 */
	// 		  (const char*  )"taskLed",		/* 任务名字 */
	// 		  (uint16_t     )512,  				/* 任务栈大小 */
	// 		  (void*        )NULL,				/* 任务入口函数参数 */
	// 		  (UBaseType_t  )4, 				/* 任务的优先级 */
	// 		  (TaskHandle_t*)&taskled_handle);	/* 任务控制块指针 */ 

	// /* 开启任务调度 */
	// vTaskStartScheduler(); 
}



	// Fun_En_DMA_Motor(MOTOR_ALL_ADDR);


static void task_Led(void* pvParameters){	
	while(1){
		
		LED[0]->reverse(LED[0]);
		LED[1]->reverse(LED[1]);
		MoveInLine_PID(1, 7.8, True);
		vTaskDelay(7000);
	}
}   

#endif //BOTTOM_LEVEL

