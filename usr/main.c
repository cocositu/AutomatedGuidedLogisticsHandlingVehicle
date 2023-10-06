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
// #include"comm_uart.h"
#include"uart.h"


// #include"uart.h"
// float EcgDiscrList[28];
// float stdResultDiffECG;
// arm_std_f32(EcgDiscrList,28-4,&stdResultDiffECG);

#ifdef  TOP_LEVEL
Arr_pLedStruct LED = NULL;
static TaskHandle_t task_Led_handle = NULL;
static TaskHandle_t task_QrcodeIdentify_handle = NULL;
/* LED任务 */ 
static void task_Led(void* pvParameters); 
/* QRCode任务 */ 
static void task_QRcodeIdentify(void* pvParameters);  



int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	QR_uart_init(115200);
	MAN_SEP_UART_Init(115200);
	LED = Create_Arr_LedStruct(2);
	LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	
	LCD_ShowString(0, 0, "WAITING...", WHITE, BLACK, 24,0);
	delay_xms(5000);
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);

	/**
	 * 
	 */
	ManStepUartCtrl(0,50, 40, 4000, 1);
	
	
	
		/* 创建app_task1任务 */
	xTaskCreate((TaskFunction_t )task_Led,  		/* 任务入口函数 */
			  (const char*    )"task_Led",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )4, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_Led_handle);	/* 任务控制块指针 */ 
		/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )task_QRcodeIdentify,  		/* 任务入口函数 */
			  (const char*    )"task_QRcodeIdentify",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_QrcodeIdentify_handle);	/* 任务控制块指针 */ 
	
	/* 开启任务调度 */
	vTaskStartScheduler(); 

}

static void task_Led(void* pvParameters){	
	while(1){
		LED[0]->reverse(LED[0]);
		LED[1]->reverse(LED[1]);
		vTaskDelay(500);
	}
}   

static void task_QRcodeIdentify(void* pvParameters){
	while (1){
		if(QRCode.GetITSta == 1){
			LCD_ShowString(0, 0, "Success    ", WHITE, BLACK, 24, 0);
			showQRCodeMessage(QRCode.MSG_Buff);
			memset(QRCode.MSG_Buff, 0, sizeof(QRCode.MSG_Buff));
			QRCode.GetITSta = 0;
			QRCode.MSG_CNT  = 0;
	}else if(QRCode.GetITSta == -1){
			LCD_ShowString(0, 0, "Failed     ", WHITE, BLACK, 24, 0);
			QRCode.GetITSta = 0;
	}else if(QRCode.GetITSta == 0){
			LCD_ShowString(0, 0, "Hello      ", WHITE, BLACK, 24, 0);
	}
		vTaskDelay(1000);
	}
}

#endif //TOP_LEVEL

#ifdef BOTTOM_LEVEL

int fputc(int ch, FILE *p) {
	USART_SendData(USART6,(uint8_t)ch);
	while(USART_GetFlagStatus(USART6,USART_FLAG_TXE)==RESET);
 return ch;
}

int main(void){
	bsp_init();
	uart6_init(9600);
		// MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 800, 0, 800*10, False, True);
		// MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD,800, 0,800*10, False, True);
		// MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD,800, 0, 800*10, False, True);
		// MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 800, 0, 800*10, False, True);
	
	while (1)
	{
		/* code */
	}
	
	/* 创建taskStart任务 */
	xTaskCreate((TaskFunction_t )taskStart,  	/* 任务入口函数 */
			  (const char*  )"taskStart",		/* 任务名字 */
			  (uint16_t     )512,  				/* 任务栈大小 */
			  (void*        )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t  )4, 				/* 任务的优先级 */
			  (TaskHandle_t*)&taskStart_handle);	/* 任务控制块指针 */ 

	/* 开启任务调度 */
	vTaskStartScheduler(); 
}




	// MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 800*3, 0, 800*10, False, False);
	// MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, 800*3, 0, 800*10, False, False);
	// MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, 800*3, 0, 800*10, False, False);
	// MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 800*3, 0, 800*10, False, False);
	// Fun_En_DMA_Motor(MOTOR_ALL_ADDR);


// static void task_Led(void* pvParameters){	
// 	while(1){
// 		TickType_t lasttick = xTaskGetTickCount();
// 		LED[0]->reverse(LED[0]);
// 		LED[1]->reverse(LED[1]);
// 		// Kinematic_Analysis_Pos(1, 0, 0, 3, 0);
// 		// if(vel_weel[0] > 0)
// 		// 	MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		// else
// 		// 	MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
		
// 		// MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*3), 0, (uint32_t)(800*3), False, False);
// 		// MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*3), 0, (uint32_t)(800*3), False, False);
// 		// MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*3), 0, (uint32_t)(800*3), False, False);
// 		// Fun_En_DMA_Motor(MOTOR_ALL_ADDR);
// 		// MotorUartCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 60, 0xF0, (uint32_t)(800/2), REL_FLAG, False);
// 		// MotorUartCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, 60, 0xF0, (uint32_t)(800/2), REL_FLAG, False);
// 		// MotorUartCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, 60, 0xF0, (uint32_t)(800/2), REL_FLAG, False);
// 		// MotorUartCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 60, 0xF0, (uint32_t)(800/2), REL_FLAG, False);
// 		// sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);

// 		xTaskDelayUntil(&lasttick, 1000);
// 	}
// }   

#endif //BOTTOM_LEVEL

