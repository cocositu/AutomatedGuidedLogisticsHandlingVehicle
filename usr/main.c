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
#include"LobotServoController.h"
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
#include "task_schedule.h"

// #include"uart.h"
// float EcgDiscrList[28];
// float stdResultDiffECG;
// arm_std_f32(EcgDiscrList,28-4,&stdResultDiffECG);


#ifdef  TOP_LEVEL

int main(void){
	bsp_init();

	task_comUart_start();
	task_taskSchedule_start();
	/* 开启任务调度 */
	LCD_ShowString(0, 0, "task_will_start   ", WHITE, BLACK, 16,0);
	vTaskStartScheduler(); 
	
}


#endif //TOP_LEVEL

#ifdef BOTTOM_LEVEL

// int fputc(int ch,FILE *p) {
//  	USART_SendData(USART6, (uint8_t)ch);
//  	while(USART_GetFlagStatus(USART6, USART_FLAG_TXE)==RESET);
//  	return ch;
// }

int main(void){
	bsp_init();
	pid_init();

	
	task_taskSchedule_start();
	// TranslationMove(UART_CTRL, 1, 0.607, 0, False);
	// delay_xms(5000);

	// MoveInLine_PID(1, 2.1645, False);
	// delay_xms(3000);
	// AntiClockwise_90Angle(UART_CTRL);
	// delay_xms(3000);
	// IMU_UART_YawZeroOut();
	// delay_xms(1000);
	// MoveInLine_PID(1, 2.730, False);
	// delay_xms(5000);
	// TranslationMove(UART_CTRL, 1, 0.606, 0, False);
	// delay_xms(5000);
	// TranslationMove(UART_CTRL, 1, 0.606, 0, False);
	// delay_xms(5000);

	// MoveInLine_PID(1, 3.3378, False);
	// delay_xms(3000);
	// AntiClockwise_90Angle(UART_CTRL);
	// delay_xms(3000);
	// IMU_UART_YawZeroOut();
	// delay_xms(1000);
	// MoveInLine_PID(1, 5.4821, False);
	// delay_xms(6000);
	// TranslationMove_PID(1, 1.5375 + 0.02,0.304 + 0.02, False);
	// delay_xms(2000);
	// AntiClockwise_90Angle(UART_CTRL);
	// while (1){}
	
	

	/* 开启任务调度 */
	vTaskStartScheduler(); 
}




#endif //BOTTOM_LEVEL

