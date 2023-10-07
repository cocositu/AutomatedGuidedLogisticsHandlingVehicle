
#include"top_task.h"
/*顶板代码*/
#ifdef  TOP_LEVEL

Arr_pLedStruct LED = NULL;

void bsp_init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LCD_Init();
	QR_uart_init(115200);
	MAN_STEP_UART_Init(115200);
	ToOpenMV_uart_init(115200);
	CarTopComUartInit(9600);

    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    LCD_ShowString(0, 0, "WAITING...", WHITE, BLACK, 24,0);
    
	LED = Create_Arr_LedStruct(3);
    LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;
	delay_xms(5000);
}

void taskStart_start(void){
	xTaskCreate((TaskFunction_t )taskStart,  		/* 任务入口函数 */
			  (const char*    )"Start",			/* 任务名字 */
			  (uint16_t       )256,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )4, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&taskStart_handle);	/* 任务控制块指针 */ 
}

void taskLed_start(void){
	xTaskCreate((TaskFunction_t )taskLed,  		/* 任务入口函数 */
			  (const char*    )"Led",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )4, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&taskLed_handle);	/* 任务控制块指针 */ 
}

void taskQrcodeIdentify_start(void){
		/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )taskQRcodeIdentify,  		/* 任务入口函数 */
			  (const char*    )"QRcodeIdentify",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&taskQrcodeIdentify_handle);	/* 任务控制块指针 */ 
}

void taskStart(void* pvParameters){
	
	while (1){
		TopSendTask(Lo_zero_QRcode, StartTask_Sta);
		vTaskDelay(200);
	}
	
	

	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(50);
	}
	taskQrcodeIdentify_start();
    vTaskDelete(taskStart_handle);
    taskEXIT_CRITICAL();
}   

static void taskLed(void* pvParameters){	
	while(1){
		LED[0]->reverse(LED[0]);
		LED[1]->reverse(LED[1]);
		vTaskDelay(1000);
	}
}   

static void taskQRcodeIdentify(void* pvParameters){
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
