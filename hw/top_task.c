#include"top_task.h"
#include"task_schedule.h"
/*顶板代码*/
#ifdef  TOP_LEVEL

Arr_pLedStruct LED = NULL;

void bsp_init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	LCD_Init();
	
	TopComUartInit(9600);
	QR_uart_init(115200);
	MAN_SERVO_UART_Init(9600);
	MAN_STEP_UART_Init(115200);
	ToOpenMV_uart_init(9600);

	delay_xms(500);
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
	LCD_ShowString(0, 0, "Init_is_starting", WHITE, BLACK, 16,0);
   
	LED = Create_Arr_LedStruct(3);
    LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;
	init_MechArm_and_MateTray(False);
	delay_xms(5000);
	LCD_ShowString(0, 0, "Init_Successfull", WHITE, BLACK, 16,0);
	
}

void task_switch(uint8_t task_name){
#ifdef TOP_LEVEL
    if(task_name > 0xA0){
        //串口发送数据开启
        //串口发送数据获取任务运行状态,装入taskSta[task_name]中
		sendStartTask(task_name);
    }else{
        switch (task_name){
		case TASK_taskSchedule:
			task_taskSchedule_start();
			break;
        case TASK_identifyQRcode:
            task_identifyQRcode_start();
            break;
        case TASK_identify_grab_Material:
            task_identify_grab_Material_start();
            break;
        case TASK_indetifyCrileColor:
            task_indetifyCrileColor_start();
            break;
        case TASK_grabMaterial:
            task_grabMaterial_start();
            break;
        case TASK_putMaterial:
            task_putMaterial_start();
            break;
        }
    }
#endif //TOP_LEVEL

#ifdef BOTTOM_LEVEL
    switch (task_name){
    case TASK_moveSzoneToQRzone:
        task_moveSzoneToQRzone_start();
        break;
    case TASK_moveQRzoneToMzone:
        task_moveQRzoneToMzone_start();
        break;
    case TASK_moveMzoneToEzone:
        task_moveMzoneToEzone_start();
        break;
    case TASK_moveEzoneToTzone:
        task_moveEzoneToTzone_start();
        break;
    case TASK_moveTzoneToMzone:
        task_moveTzoneToMzone_start();
        break;   
    case TASK_moveTzoneToSzone:
        task_moveTzoneToSzone_start();
        break;
    case TASK_moveBetweenCricle:
        task_moveBetweenCricle_start();
        break;
    case TASK_moveXYPosition:
        task_moveXYPosition_start();
        break; 
    case TASK_moveContorCricle:
        task_moveContorCricle_start();
        break; 
    }
#endif //BOTTOM_LEVEL
}

void task_taskSchedule_start(void){
	xTaskCreate((TaskFunction_t )task_taskSchedule, /* 任务入口函数 */
			  (const char*    )"taskSchedule",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )2, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_taskSchedule_handle);	/* 任务控制块指针 */ 
}

void task_comUart_start(void){
	xTaskCreate((TaskFunction_t )task_comUart, /* 任务入口函数 */
			  (const char*    )"comUart",	    /* 任务名字 */
			  (uint16_t       )128,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_comUart_handle);	/* 任务控制块指针 */ 
}

void task_identifyQRcode_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )task_identifyQRcode,  		/* 任务入口函数 */
			  (const char*    )"identifyQRcode",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_identifyQRcode_handle);	/* 任务控制块指针 */ 
}

void task_identify_grab_Material_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )task_identify_grab_Material,  		/* 任务入口函数 */
			  (const char*    )"identify_grab_Material",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_identify_grab_Material_handle);	/* 任务控制块指针 */ 	
}

void task_indetifyCrileColor_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t )task_indetifyCrileColor,  		/* 任务入口函数 */
			  (const char*    )"indetifyCrileColor",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_indetifyCrileColor_handle);	/* 任务控制块指针 */ 
}

void task_grabMaterial_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t ) task_grabMaterial,  		/* 任务入口函数 */
			  (const char*    )"grabMaterial",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_grabMaterial_handle);	/* 任务控制块指针 */ 
}

void task_putMaterial_start(void){
	/* 创建task_QrcodeIdentify任务 */
	xTaskCreate((TaskFunction_t ) task_putMaterial,  		/* 任务入口函数 */
			  (const char*    )"putMaterial",			/* 任务名字 */
			  (uint16_t       )512,  				/* 任务栈大小 */
			  (void*          )NULL,				/* 任务入口函数参数 */
			  (UBaseType_t    )3, 					/* 任务的优先级 */
			  (TaskHandle_t*  )&task_putMaterial_handle);	/* 任务控制块指针 */ 
}

//static int task_index = 0;
void task_taskSchedule(void* pvParameters){
   	taskSta[TASK_taskSchedule] = TASK_BUSY_STATE;
	
   	while (task_index < 66){
		LCD_ShowString(0, 0, "last_task_j_bs      ", WHITE, BLACK, 16,0);
        while (get_task_status(taskList[task_index]) != TASK_IDLE_STATE && task_index != 0){
            LCD_ShowIntNum(32, 16, taskSta[taskList[task_index]], 2, WHITE, BLACK, 16);
			vTaskDelay(100);
        }
		task_index++;
		LCD_ShowString(0, 0, "task_j_is          ", WHITE, BLACK, 16, 0);
		LCD_ShowIntNum(0, 16, task_index, 2, WHITE, BLACK, 16);
        while (get_task_status(taskList[task_index]) == TASK_IDLE_STATE){
			vTaskDelay(100);
            //启动当前任务,如何发现没有开启，再次发送开启指令
            task_switch(taskList[task_index]);
        }
		LCD_ShowString(0, 0, "task_j_sta           ", WHITE, BLACK, 16, 0);
		
   	}


	taskSta[TASK_taskSchedule] = TASK_IDLE_STATE;
   	vTaskDelete(task_taskSchedule_handle);
	taskEXIT_CRITICAL();
}


void task_comUart(void* pvParameters){
	while (1){
		if(TopData.xy_pos_sta == 1){
			//读取openMVd的值
			//转发给底板
			TopData.xy_pos_sta = 0;
		}

		vTaskDelay(100);
	}
	
}


void task_identifyQRcode(void* pvParameters){
	taskSta[TASK_identifyQRcode] = TASK_BUSY_STATE;
	LED[0]->off(LED[0]);
	while (1){
		if(QRCode.GetITSta == 1){
			//delay_xms(1000);
			for(int i = 0; i < 3; i++){ 
				Arr_CarryColorSeq[FIRST_SEQ][i+1]  = QRCode.MSG_Buff[i] - '0';
				Arr_CarryColorSeq[SECOND_SEQ][i+1] = QRCode.MSG_Buff[i+4] - '0';
			}
			showQRCodeMessage(QRCode.MSG_Buff);
			//向底板发送搬运顺序
			// while (TopData.sta_CarrySeq == 1){
			// 	sendCarrySeq();
			// 	vTaskDelay(80);
			// }
			
			int tmp_i = 6;
			while (tmp_i--){
				sendCarrySeq();
				vTaskDelay(60);
			}
			TopData.sta_CarrySeq = 0;
			
			break;
		}else{
			//未识别到
			LCD_ShowString(0, 0, "Failed       ", WHITE, BLACK, 16, 0);
		}
		vTaskDelay(100);
	}
	LED[0]->on(LED[0]);
	taskSta[TASK_identifyQRcode] = TASK_IDLE_STATE;
	vTaskDelete(task_identifyQRcode_handle);
    taskEXIT_CRITICAL();
}

int task_u = 0;
void task_identify_grab_Material(void* pvParameters){
	taskSta[TASK_identify_grab_Material] = TASK_BUSY_STATE;
	RingMovSeq = task_u;
	for(int i=1;i<=3;++i){
		LCD_ShowIntNum(0,48,RingMovSeq,2,WHITE,BLACK,16);
		LCD_ShowIntNum(36,48,Arr_CarryColorSeq[RingMovSeq][i],2,WHITE,BLACK,16);
		OV_Struct.TaskState = 0;
		while (OV_Struct.TaskState != 'y'){
			LED[0]->reverse(LED[0]);
			OV_Struct.TaskState = 0;	
			OV_SendData(Arr_CarryColorSeq[RingMovSeq][i]);
			vTaskDelay(80);
		}
		OV_Struct.TaskState = 0;
		GrabMate_to_MT(Arr_CarryColorSeq[RingMovSeq][i], True);
		vTaskDelay(200);
	}
	LED[0]->on(LED[0]);
	task_u++;
	taskSta[TASK_identify_grab_Material] = TASK_IDLE_STATE;
	vTaskDelete(task_identify_grab_Material_handle);
    taskEXIT_CRITICAL();
}

int idfCC_conunt = 0;
void task_indetifyCrileColor(void* pvParameters){
	taskSta[TASK_indetifyCrileColor] = TASK_BUSY_STATE;

	//识别色环颜色
	//Arr_ZoneColorSeq[idfCC_conunt/2][idfCC_conunt%2 + 1] = /**/;
	idfCC_conunt++;
	
	if(idfCC_conunt == 2){
		Arr_ZoneColorSeq[EZONE_SEQ][3] = 6 - Arr_ZoneColorSeq[EZONE_SEQ][1] - Arr_ZoneColorSeq[EZONE_SEQ][2];
		Arr_ZoneColorSeq[EZONE_SEQ][1] = 2;
		Arr_ZoneColorSeq[EZONE_SEQ][2] = 1;
		Arr_ZoneColorSeq[EZONE_SEQ][3] = 3;
		CalMovBetweenRings(0,0,2);
		LCD_ShowString(0,48,"                               ",WHITE,BLACK,16,0);
		for(int i=0;i<=6;i++){
			if(Arr_REL_ZoneMove[0][0][i]<0){
				LCD_ShowChar(i*32,48,'-',WHITE,BLACK,16,0);
				LCD_ShowIntNum(i*32+8,48,_abs(Arr_REL_ZoneMove[0][0][i]),1,WHITE,BLACK,16);
			}else
				LCD_ShowIntNum(i*32,48,Arr_REL_ZoneMove[0][0][i],2,WHITE,BLACK,16);
		}
		//并向下层板发送颜色
		// while (TopData.sta_EzoneSeq != 1)
		// {
		// 	sendEzoneSeq();
		// 	vTaskDelay(100);
		// }

		int tmp_i =6;
		while (tmp_i--)
		{
			sendEzoneSeq();
			vTaskDelay(80);
		}
		
	}else if(idfCC_conunt == 4){
		Arr_ZoneColorSeq[TZONE_SEQ][3] = 6 - Arr_ZoneColorSeq[TZONE_SEQ][1] - Arr_ZoneColorSeq[TZONE_SEQ][2];
		Arr_ZoneColorSeq[TZONE_SEQ][1] = 2;
		Arr_ZoneColorSeq[TZONE_SEQ][2] = 1;
		Arr_ZoneColorSeq[TZONE_SEQ][3] = 3;
		CalMovBetweenRings(0,1,2);
		LCD_ShowString(0,48,"                               ",WHITE,BLACK,16,0);
		for(int i=0;i<=6;i++){
			if(Arr_REL_ZoneMove[0][1][i]<0){
				LCD_ShowChar(i*32,48,'-',WHITE,BLACK,16,0);
				LCD_ShowIntNum(i*32+8,48,_abs(Arr_REL_ZoneMove[0][0][i]),1,WHITE,BLACK,16);
			}else
				LCD_ShowIntNum(i*32,48,Arr_REL_ZoneMove[0][0][i],2,WHITE,BLACK,16);
		}
		//并向下层板发送颜色
		// while (TopData.sta_TzoneSeq != 1)
		// {
		// 	sendTzoneSeq();
		// 	vTaskDelay(100);
		// }
		int tmp_i =6;
		while (tmp_i--)
		{
			sendTzoneSeq();
			vTaskDelay(100);
		}
		
	}
	vTaskDelay(1000);
	taskSta[TASK_indetifyCrileColor] = TASK_IDLE_STATE;
	vTaskDelete(task_indetifyCrileColor_handle);
    taskEXIT_CRITICAL();
}

int grabM_count = 0;
void task_grabMaterial(void* pvParameters){
	taskSta[TASK_grabMaterial] = TASK_BUSY_STATE;

	//抓取物料
	GrabMate_to_G(Arr_CarryColorSeq[RingMovSeq][grabM_count % 3 + 1],True);
	grabM_count++;

	taskSta[TASK_grabMaterial] = TASK_IDLE_STATE;
	vTaskDelete(task_grabMaterial_handle);
    taskEXIT_CRITICAL();
}

int putM_count = 0;
void task_putMaterial(void* pvParameters){
	taskSta[TASK_putMaterial] = TASK_BUSY_STATE;

	// 0,1,2,3,4,5,6,7,8,9,10,11
	// 1,2,3,4,5,6,7,8,9,10,11,12
	//放置物料
	
	if(putM_count < 9)  PutMate_to_G(Arr_CarryColorSeq[RingMovSeq][putM_count % 3 + 1],True);
	else  				PutMate_to_M(Arr_CarryColorSeq[RingMovSeq][putM_count % 3 + 1],True);
	
	putM_count++;


	taskSta[TASK_putMaterial] = TASK_IDLE_STATE;
	vTaskDelete(task_putMaterial_handle);
    taskEXIT_CRITICAL();
}

void USART1_IRQHandler(void){
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        TopData.RxBuff[TopData.RxCnt++] = USART1->DR;
        if((TopData.RxBuff[0] == 0x66)&&(TopData.RxBuff[9] == 0x88)){
			
            switch (TopData.RxBuff[1]){
            case 0xC3:
                //TopData.tmp_taskName = TopData.RxBuff[2];
                //TopData.tmp_taskSta = TopData.RxBuff[3];
				taskSta[TopData.RxBuff[2]] = TopData.RxBuff[3];
				//TopData.tmp_taskName = 0;
				//TopData.tmp_taskSta = 0;
                break;
            case 0xD4:
                TopData.sta_CarrySeq = TopData.RxBuff[2];
                break;
            case 0xE5:
                TopData.sta_EzoneSeq = TopData.RxBuff[2];
                break;
            case 0xF6:
                TopData.sta_TzoneSeq = TopData.RxBuff[2];
                break;
            case 0x11:
                TopData.xy_pos_sta = TopData.RxBuff[2];
                break;
            }
        }
        if (TopData.RxCnt == 10){
            TopData.RxCnt = 0;
            memset(TopData.RxBuff,0,sizeof(TopData.RxBuff));
        }
        
	}
}
#endif //TOP_LEVEL
