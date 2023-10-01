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
#endif //TOP_LEVEL

/*底盘代码*/
#ifdef  BOTTOM_LEVEL
#include"motor_driver.h"
#include"hwt101.h"
#endif //BOTTOM_LEVEL

/*两层通用文件*/
#include "led.h"
#include"comm_uart.h"

// #define UART6_PRINTF
// #include"uart.h"
// float EcgDiscrList[28];
// float stdResultDiffECG;
// arm_std_f32(EcgDiscrList,28-4,&stdResultDiffECG);

#ifdef  TOP_LEVEL6
Arr_pLedStruct LED = NULL;
static TaskHandle_t task_Led_handle = NULL;
static TaskHandle_t task_QrcodeIdentify_handle = NULL;
/* LED任务 */ 
static void task_Led(void* pvParameters); 
/* QRCode任务 */ 
static void task_QRcodeIdentify(void* pvParameters);  

int fputc(int ch, FILE *p) {
	#ifdef UART1_PRINTF
 		USART_SendData(USART1,(uint8_t)ch);
 		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
 	#endif
	#ifdef UART6_PRINTF
 		USART_SendData(USART6,(uint8_t)ch);
 		while(USART_GetFlagStatus(USART6,USART_FLAG_TXE)==RESET);
 	#endif
 return ch;
}

int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	QR_uart_init(115200);
	
	LED = Create_Arr_LedStruct(2);
	LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	
	LCD_ShowString(0, 0, "WAITING...", WHITE, BLACK, 24,0);
	delay_xms(1000);
	LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
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
		#ifdef UART1_PRINTF
			printf("Hello World!\r\n");
		#endif
		#ifdef UART6_PRINTF
			printf("Goodbye World!\r\n");
		#endif
		vTaskDelay(300);
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

int flag = 0;
Arr_pLedStruct LED = NULL;
pLedStruct LED3 =NULL;
static TaskHandle_t task_Led_handle = NULL;
static void task_Led(void* pvParameters); 
void DMA_Enable(void);
int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	IMU_uart_init(9600);
	//uart1_init(115200);
	//uart6_init(115200);
	LED = Create_Arr_LedStruct(3);
	LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;

	LED[0]->off(LED[0]);
	// MOTOR_LF_TIM_Init();
	// MOTOR_LR_TIM_Init();
	// MOTOR_RF_TIM_Init();
	// MOTOR_RR_TIM_Init();
	// MOTOR_LF_DMA_Init();
	// MOTOR_LR_DMA_Init();
	// MOTOR_RF_DMA_Init();
	// MOTOR_RR_DMA_Init();
	MOTOR_LF_UART_Init(115200);
	MOTOR_LR_UART_Init(115200);
	MOTOR_RF_UART_Init(115200);
	MOTOR_RR_UART_Init(115200);
	delay_xms(5000);
	//TIM_SetCompare1(MOTOR_LF_TIM, 30);
	// TIM_SetCompare1(TIM1, 30);
	// TIM_SetCompare1(TIM3, 30);
	// TIM_SetCompare1(TIM8, 30);

	// TIM_CtrlPWMOutputs(TIM1, ENABLE);
	// TIM_CtrlPWMOutputs(TIM8, ENABLE);
	// for(int i = 0; i < STEPS_LOOP_MAXNUM; ++i)
	// 	StepMotorLFBuffer[i] = 10 * i + 1000;
	// StepMotorLFBuffer[STEPS_LOOP_MAXNUM] = 0xFFFF;
	// DMA_Enable();
	// TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能
	//TIM_Cmd(MOTOR_LF_TIM, ENABLE); 
	// TIM_Cmd(TIM1, ENABLE); 
	// TIM_Cmd(TIM3, ENABLE); 
	// TIM_Cmd(TIM8, ENABLE); 
	// TIM_CtrlPWMOutputs(TIM2, ENABLE);
	// for(int j =0;j<4;j++){
	// 	for(int i = 0; i < 8; i++){
	// 		MotorUartBuffer[j][i] = 'a' + i;
	// 	}
	// }
	// while (1)
	// {
	// 	LED[0]->reverse(LED[0]);
		
    // 	//while (DMA_GetFlagStatus(MOTOR_LF_UART_DMA_STREAM, DMA_FLAG_TCIF6) == RESET);
    // 	// 清除DMA传输完成标志
   	// 	DMA_ClearFlag(MOTOR_LF_UART_DMA_STREAM, DMA_FLAG_TCIF6);
	// 	DMA_Cmd(MOTOR_LF_UART_DMA_STREAM, ENABLE);
	// 	DMA_ClearFlag(MOTOR_LR_UART_DMA_STREAM, DMA_FLAG_TCIF3);
	// 	DMA_Cmd(MOTOR_LR_UART_DMA_STREAM, ENABLE);
	// 	DMA_ClearFlag(MOTOR_RF_UART_DMA_STREAM, DMA_FLAG_TCIF7);
	// 	DMA_Cmd(MOTOR_RF_UART_DMA_STREAM, ENABLE);
	// 	DMA_ClearFlag(MOTOR_RR_UART_DMA_STREAM, DMA_FLAG_TCIF7);
	// 	DMA_Cmd(MOTOR_RR_UART_DMA_STREAM, ENABLE);
	// 	delay_xms(1000);
	// }
	while (1){
		if(HWT101_Struct.GetITSta == 1){
			LED[0]->reverse(LED[0]);
			HWT101_Struct.GetITSta = 0;
			memset(HWT101_Struct.MSG_Rx_Buff,0,sizeof(HWT101_Struct.MSG_Rx_Buff));
		}else if(HWT101_Struct.GetITSta == -1){
			HWT101_Struct.GetITSta = 0;
			LED[2]->reverse(LED[2]);
		}
		// MotorUartCtrl(MOTOR_LF_ADDR, 0, 80, 50, (uint32_t)(800 * 4), REL_FLAG, False);
		// MotorUartCtrl(MOTOR_LR_ADDR, 0, 80, 50, (uint32_t)(800 * 4), REL_FLAG, False);
		// MotorUartCtrl(MOTOR_RF_ADDR, 1, 80, 50, (uint32_t)(800 * 4), REL_FLAG, False);
		// MotorUartCtrl(MOTOR_RR_ADDR, 1, 80, 50, (uint32_t)(800 * 4), REL_FLAG, False);
		//8sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
		//delay_xms((uint32_t)(800.0 / 80 + 2.0 * 80.0 / 50) * 1000);
		delay_xms(300);
	}
	
	
	/* 创建app_task1任务 */
	xTaskCreate((TaskFunction_t )task_Led,  		/* 任务入口函数 */
			  (const char*    )"task_Led",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )4, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_Led_handle);	/* 任务控制块指针 */ 
		/* 创建task_QrcodeIdentify任务 */
	
	/* 开启任务调度 */
	vTaskStartScheduler(); 
}

static void task_Led(void* pvParameters){	
	while(1){
		//LED[0]->reverse(LED[0]);
		LED[1]->reverse(LED[1]);
		LED[2]->reverse(LED[2]);
		// #ifdef UART1_PRINTF
		// 	printf("Hello World!\r\n");
		// #endif
		// #ifdef UART6_PRINTF
		// 	printf("Goodbye World!\r\n");
		// #endif
		// if(flag >= 1){
		// 	LED[0]->on(LED[0]);
		// }
		vTaskDelay(1000);
	}
}   

void DMA_Enable(void){
	MOTOR_LF_DMA_STREAM->NDTR = STEPS_LOOP_MAXNUM + 1;
	DMA_ClearFlag(MOTOR_LF_DMA_STREAM, DMA_FLAG_TCIF5);
 	//DMA_SetCurrDataCounter(MOTOR_LF_DMA_STREAM, STEPS_LOOP_MAXNUM+1);
	DMA_Cmd(MOTOR_LF_DMA_STREAM, ENABLE);
	TIM_Cmd(MOTOR_LF_TIM, ENABLE); 
}	 

//TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE使能
// TIM2->EGR |= 0x01;

#endif //BOTTOM_LEVEL


// HIGHT
void DMA1_Stream5_IRQHandler(void){
  if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5)){
    flag++;
    DMA_Cmd(DMA1_Stream5, DISABLE);
    TIM_CCxCmd(TIM2, TIM_Channel_1, TIM_CCx_Disable);
    DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
    TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
  }
}