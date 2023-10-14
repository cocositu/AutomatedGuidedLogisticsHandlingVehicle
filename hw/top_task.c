
#include"top_task.h"
/*顶板代码*/
#ifdef  TOP_LEVEL

Arr_pLedStruct LED = NULL;
uint8_t FirSequence[3] = {0};
uint8_t SecSequence[3] = {0};

void bsp_init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LCD_Init();
	
	CarTopComUartInit(9600);
	QR_uart_init(115200);
	MAN_SERVO_UART_Init(9600);
	MAN_STEP_UART_Init(115200);
	ToOpenMV_uart_init(9600);
	delay_xms(500);
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    LCD_ShowString(0, 0, "WAITING...", WHITE, BLACK, 24,0);
    
	LED = Create_Arr_LedStruct(3);
    LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;

	ManServo_StepMoter_Init();
	delay_xms(1000);
	
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

void taskManCrawl_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )taskManCrawl,  		/* 任务入口函数 */
			  (const char*    )"ManCrawl",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&taskManCrawl_handle);	/* 任务控制块指针 */ 
}

void taskPutItem_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )taskPutItem,  		/* 任务入口函数 */
			  (const char*    )"ManCrawl",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&taskPutItem_handle);	/* 任务控制块指针 */ 
}

void taskPutItem(void* pvParameters){
	delay_xms(1000);
	Item_tray_Turn(1);
	RoughingArea_Action();

	TopSendTask(Lo_Supply_RoughL_1 , StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}
	TopData.TaskState = 0;
	delay_xms(1000);
	Item_tray_Turn(2);
	RoughingArea_Action();
	
	TopSendTask(Lo_RoughM_RoughR_1 , StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}

	TopData.TaskState = 0;
	delay_xms(1000);
	Item_tray_Turn(3);
	RoughingArea_Action();
	

	TopSendTask(Lo_RoughL_RoughM_1, StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}
	TopData.TaskState = 0;
	delay_xms(1000);
	Item_tray_Turn(1);
	Catch_StagingArea_Action_();
	vTaskDelay(500);

	TopSendTask(Lo_Supply_RoughL_1 , StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}
	TopData.TaskState = 0;
	delay_xms(1000);
	Item_tray_Turn(2);
	Catch_StagingArea_Action_();
	vTaskDelay(500);

	TopSendTask(Lo_RoughM_RoughR_1 , StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}

	TopData.TaskState = 0;
	delay_xms(1000);
	Item_tray_Turn(3);
	vTaskDelay(500);
	Catch_StagingArea_Action_();
	
	

	vTaskDelete(taskPutItem_handle);
    taskEXIT_CRITICAL();
}

void taskManCrawl(void* pvParameters){
	LCD_ShowString(0, 0, "Color     ", WHITE, BLACK, 24, 0);
	for(int i=0;i<3;i++){
		while (1){
			OV_Struct.TaskNum = FirSequence[i];
			LCD_ShowString(0, 0, "            ", WHITE, BLACK, 24, 0);
			LCD_ShowIntNum(0, 0,FirSequence[i],2, WHITE, BLACK, 24);
			OV_SendData(FirSequence[i]);
			vTaskDelay(100);
			if(OV_Struct.TaskState == 'y'){
				OV_Struct.TaskState = 0;
				Item_tray_Turn(FirSequence[i]);
				Catch_Itemtray_Action();
				break;
			}
		}
		LCD_ShowString(0, 0, "Finsh       ", WHITE, BLACK, 24, 0);
		vTaskDelay(500);
	}

	TopSendTask(Lo_RoughL_RoughM_1, StartTask_Sta);
	
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}
	TopData.TaskState = 0;
	taskPutItem_start();
	vTaskDelete(taskManCrawl_handle);
    taskEXIT_CRITICAL();
}


static void taskQRcodeIdentify(void* pvParameters){
	while (1){
		if(QRCode.GetITSta == 1){
			for(int i = 0; i < 3; i++){
				FirSequence[i] = QRCode.MSG_Buff[i] - '0';
				SecSequence[i] = QRCode.MSG_Buff[i+4] -'0';
			}
			LCD_ShowString(0, 0, "Success    ", WHITE, BLACK, 24, 0);
			showQRCodeMessage(QRCode.MSG_Buff);
		
			memset(QRCode.MSG_Buff, 0, sizeof(QRCode.MSG_Buff));
			QRCode.GetITSta = 0;
			QRCode.MSG_CNT  = 0;
			break;
		}else if(QRCode.GetITSta == -1){
			LCD_ShowString(0, 0, "Failed     ", WHITE, BLACK, 24, 0);
			QRCode.GetITSta = 0;
		}else if(QRCode.GetITSta == 0){
			LCD_ShowString(0, 0, "Hello      ", WHITE, BLACK, 24, 0);
		}
		vTaskDelay(300);
	}


	TopSendQRMsg(QRCode.MSG_Buff);
	TopSendTask(Lo_QRcode_Supply, StartTask_Sta);
	LCD_ShowString(0, 0, "NNNNNNN ", WHITE, BLACK, 24, 0);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
		LED[0]->reverse(LED[0]);
 	}
	TopData.TaskState = 0;
	LED[1]->off(LED[1]);
	LCD_ShowString(0, 0, "MMMMMMMM ", WHITE, BLACK, 24, 0);
	taskManCrawl_start();
	vTaskDelete(taskQrcodeIdentify_handle);
    taskEXIT_CRITICAL();
}


void taskStart(void* pvParameters){
	LCD_ShowString(0, 0, "start     ", WHITE, BLACK, 24, 0);
	TopSendTask(Lo_zero_QRcode, StartTask_Sta);
	while (TopData.TaskState != FinishTask_Sta){
		vTaskDelay(100);
 	}
	LCD_ShowString(0, 0, "stQR     ", WHITE, BLACK, 24, 0);
	TopData.TaskState = 0;
	LED[1]->off(LED[1]);
	taskQrcodeIdentify_start();
    vTaskDelete(taskStart_handle);
    taskEXIT_CRITICAL();
}   

static void taskLed(void* pvParameters){	
	while(1){
		// LED[0]->reverse(LED[0]);
		// LED[1]->reverse(LED[1]);
		vTaskDelay(1000);
	}
}   


#endif //TOP_LEVEL
