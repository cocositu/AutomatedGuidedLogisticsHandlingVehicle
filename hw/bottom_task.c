#include"bottom_task.h"
#include"string.h"
#include"task_schedule.h"

#ifdef BOTTOM_LEVEL

Arr_pLedStruct LED = NULL;

void bsp_init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	
    MOTOR_LF_TIM_Init();
	MOTOR_LR_TIM_Init();
	MOTOR_RF_TIM_Init();
	MOTOR_RR_TIM_Init();
	
    MOTOR_LF_UART_Init(115200);
	MOTOR_LR_UART_Init(115200);
	MOTOR_RF_UART_Init(115200);
	MOTOR_RR_UART_Init(115200);
	
    IMU_uart_init(9600);
	BottomComUartInit(9600);
	XYPos_GPIO_init();
	LED = Create_Arr_LedStruct(3);

    LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;
	IMU_UART_YawZeroOut();
	
	delay_xms(1000);
}

void task_switch(uint8_t task_name){
#ifdef TOP_LEVEL
    if(task_name > 0xA0){
        //串口发送数据开启
        //串口发送数据获取任务运行状态,装入taskSta[task_name]中
    }else{
        switch (task_name){
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

void task_moveSzoneToQRzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveSzoneToQRzone, /* 任务入口函数 */
			  (const char*    )"moveSzoneToQRzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveSzoneToQRzone_handle);	/* 任务控制块指针 */ 
}

void task_moveQRzoneToMzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveQRzoneToMzone,/* 任务入口函数 */
			  (const char*    )"moveQRzoneToMzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveQRzoneToMzone_handle);	/* 任务控制块指针 */ 
}

void task_moveMzoneToEzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveMzoneToEzone, /* 任务入口函数 */
			  (const char*    )"moveMzoneToEzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveMzoneToEzone_handle);	/* 任务控制块指针 */ 
}

void task_moveEzoneToTzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveEzoneToTzone, /* 任务入口函数 */
			  (const char*    )"moveEzoneToTzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveEzoneToTzone_handle);	/* 任务控制块指针 */ 
}

void task_moveTzoneToMzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveTzoneToMzone, /* 任务入口函数 */
			  (const char*    )"moveTzoneToMzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveTzoneToMzone_handle);	/* 任务控制块指针 */ 
}

void task_moveTzoneToSzone_start(void){
	xTaskCreate((TaskFunction_t ) task_moveTzoneToSzone, /* 任务入口函数 */
			  (const char*    )"moveTzoneToSzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveTzoneToSzone_handle);	/* 任务控制块指针 */ 
}

void task_moveBetweenCricle_start(void){
	xTaskCreate((TaskFunction_t ) task_moveBetweenCricle,  		/* 任务入口函数 */
			  (const char*    )"moveBetweenCricle",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveBetweenCricle_handle);	/* 任务控制块指针 */ 
}

void task_moveContorCricle_start(void){
	xTaskCreate((TaskFunction_t ) task_moveContorCricle,  		/* 任务入口函数 */
			  (const char*    )"moveContorCricle",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveContorCricle_handle);	/* 任务控制块指针 */ 
}

void task_moveXYPosition_start(void){
	xTaskCreate((TaskFunction_t ) task_moveXYPosition,  		/* 任务入口函数 */
			  (const char*    )"moveTzoneToSzone",	    /* 任务名字 */
			  (uint16_t       )512,  				    /* 任务栈大小 */
			  (void*          )NULL,				    /* 任务入口函数参数 */
			  (UBaseType_t    )3, 					    /* 任务的优先级 */
			  (TaskHandle_t*  )&task_moveXYPosition_handle);	/* 任务控制块指针 */ 
}


void task_taskSchedule(void* pvParameters){
	//用来处理上层板发来的数据
   	while (1){
		LED[1]->reverse(LED[1]);
		if(BottomData.needStartTask != 0){
			taskSta[BottomData.needStartTask] = TASK_BUSY_STATE;
			task_switch(BottomData.needStartTask);
			BottomData.needStartTask=0;
		}
		if(BottomData.needRelyTask != 0){	
			replyCurTaskStatus(BottomData.needRelyTask);
			BottomData.needRelyTask = 0;
		}
    	vTaskDelay(80);
   	}
	
   	vTaskDelete(task_taskSchedule_handle);
    taskEXIT_CRITICAL();
}

void task_moveSzoneToQRzone(void* pvParameters){
	taskSta[TASK_moveSzoneToQRzone] = TASK_BUSY_STATE;
	TranslationMove_PID(1, SZONE_TO_TMPZONE_DIS, -SZONE_TO_TMPZONE_DIS, True);
	vTaskDelay(100);
	MoveInLine_PID(1, TMPZONE_TO_QRCODE_DIS ,True);
	
	taskSta[TASK_moveSzoneToQRzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveSzoneToQRzone_handle);
    taskEXIT_CRITICAL();
}

void task_moveQRzoneToMzone(void* pvParameters){
	taskSta[TASK_moveQRzoneToMzone] = TASK_BUSY_STATE;

	MoveInLine_PID(1, QRZONE_TO_MZONE_DIS, True);
	vTaskDelay(100);
	TranslationMove(UART_CTRL, 1, 0, 0.18f, True);
	taskSta[TASK_moveQRzoneToMzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveQRzoneToMzone_handle);
    taskEXIT_CRITICAL();
}

void task_moveMzoneToEzone(void* pvParameters){
	taskSta[TASK_moveMzoneToEzone] = TASK_BUSY_STATE;

	TranslationMove(UART_CTRL, 1, 0, -0.2f, True);
	vTaskDelay(300);
	MoveInLine_PID(1, MZONE_TO_FIRCOR_DIS, True);
	vTaskDelay(100);
	AntiClockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);
	IMU_UART_YawZeroOut();
	vTaskDelay(500);
	MoveInLine_PID(1, FIRCOR_TO_EZONE_DIS, False);
	vTaskDelay(100);
	TranslationMove(UART_CTRL, 1, 0, 0.2f, True);
	vTaskDelay(100);
	taskSta[TASK_moveMzoneToEzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveMzoneToEzone_handle);
    taskEXIT_CRITICAL();
}

void task_moveEzoneToTzone(void* pvParameters){
	taskSta[TASK_moveEzoneToTzone] = TASK_BUSY_STATE;
	vTaskDelay(300);
	//这里还要计算
	TranslationMove(UART_CTRL, 1, 0, -0.2f, True);
	vTaskDelay(100);
	double tmp_ids = (3 - Arr_ABS_ZoneMove[EZONE_SEQ][RingMovSeq][3])*RINGS_BETWEEN_DIS;
	MoveInLine_PID(1, EZONE_TO_SECCOR_DIS+tmp_ids , True);
	vTaskDelay(100);
	AntiClockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);
	IMU_UART_YawZeroOut();
	vTaskDelay(500);
	MoveInLine_PID(1, SECCOR_TO_TZONE_DIS, False);
	delay_ms(100);
	TranslationMove(UART_CTRL, 1, 0, 0.18f, True);
	taskSta[TASK_moveEzoneToTzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveEzoneToTzone_handle);
    taskEXIT_CRITICAL();
}

void task_moveTzoneToMzone(void* pvParameters){
	taskSta[TASK_moveTzoneToMzone] = TASK_BUSY_STATE;

	vTaskDelay(300);
	TranslationMove(UART_CTRL, 1, 0, -0.18f, True);
	vTaskDelay(300);
	double tmp_ids = ((double)Arr_ABS_ZoneMove[TZONE_SEQ][FIRST_SEQ][3]-1)*RINGS_BETWEEN_DIS;
	MoveInLine_PID(-1,TZONE_TO_SECCOR_DIS + tmp_ids, True);
	vTaskDelay(100);

	Clockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);
	IMU_UART_YawZeroOut();
	vTaskDelay(500);
	MoveInLine_PID(-1,SECCOR_TO_FIRCOR_DIS, True);
	Clockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);
	IMU_UART_YawZeroOut();
	vTaskDelay(500);
	MoveInLine_PID(-1,FIRCOR_TO_MZONE_DIS, True);
	TranslationMove(UART_CTRL, 1, 0, 0.2f, True);
	taskSta[TASK_moveTzoneToMzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveTzoneToMzone_handle);
    taskEXIT_CRITICAL();
}
void task_moveTzoneToSzone(void* pvParameters){
	taskSta[TASK_moveTzoneToSzone] = TASK_BUSY_STATE;
	
	TranslationMove(UART_CTRL, 1, 0, -0.2f, True);
	//这里需要重新计算
	double tmp_ids = (3-Arr_ABS_ZoneMove[TZONE_SEQ][SECOND_SEQ][3])*RINGS_BETWEEN_DIS;
	MoveInLine_PID(1, TZONE_TO_THIRCOR_DIS + tmp_ids, True);
	vTaskDelay(100);

	AntiClockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);
	IMU_UART_YawZeroOut();
	vTaskDelay(500);
	MoveInLine_PID(1, THIRCOR_DIS_TO_TMPZONE_DIS, True);
	TranslationMove_PID(1,TMPZONE_TO_SZONE_X_DIS,TMPZONE_TO_SZONE_Y_DIS, True);
	AntiClockwise_90Angle(UART_CTRL);
	vTaskDelay(2000);

	taskSta[TASK_moveTzoneToSzone] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveTzoneToSzone_handle);
    taskEXIT_CRITICAL();
}

void task_moveContorCricle(void* pvParameters){
	taskSta[TASK_moveContorCricle] = TASK_BUSY_STATE;

	TranslationMove(UART_CTRL, 1, RINGS_BETWEEN_DIS, 0, True);
	vTaskDelay(500);

	taskSta[TASK_moveContorCricle] = TASK_IDLE_STATE;
	vTaskDelete(task_moveContorCricle_handle);
    taskEXIT_CRITICAL();
}


void task_moveBetweenCricle(void* pvParameters){
	taskSta[TASK_moveBetweenCricle] = TASK_BUSY_STATE;
	
	RingMovSeq  = (RingMovCount_total / 9) & 0x01;
	
	if(RingMovCount_total == 0){
		RingMovZone = 0;
		RingMovCount = 0;
		CalMovBetweenRings(RingMovZone, RingMovSeq, 2);
	}else if(RingMovCount_total == 6){
		RingMovZone = 1;
		RingMovCount = 0;
		CalMovBetweenRings(RingMovZone, RingMovSeq, 2);
	}
	else if(RingMovCount_total == 9) {
		RingMovZone = 0;
		RingMovCount = 0;
		CalMovBetweenRings(RingMovZone, RingMovSeq, 1);
	}else if( RingMovCount_total == 15){
		RingMovZone = 1;
		RingMovCount = 0;
		CalMovBetweenRings(RingMovZone, RingMovSeq, 1);
	}else{
		RingMovCount++;
	}

	int8_t tmp_i = Arr_REL_ZoneMove[RingMovZone][RingMovSeq][RingMovCount];
	
	TranslationMove(UART_CTRL, 1, RINGS_BETWEEN_DIS * tmp_i, 0, True);
	vTaskDelay(2400);

	RingMovCount_total++;
	taskSta[TASK_moveBetweenCricle] = TASK_IDLE_STATE;
	vTaskDelete(task_moveBetweenCricle_handle);
    taskEXIT_CRITICAL();
}

void task_moveXYPosition(void* pvParameters){
	taskSta[TASK_moveXYPosition] = TASK_BUSY_STATE;

	//调整xy位置
	//320*240   160 120
	//160*120   80  60
	AdjustXYPostion(150, 120, True);
	//vTaskDelay(2000);
	taskSta[TASK_moveXYPosition] = TASK_IDLE_STATE ;
	vTaskDelete(task_moveXYPosition_handle);
    taskEXIT_CRITICAL();
}

void AdjustXYPostion(int t_x, int t_y, bool isOSTime){
    ((pStruct_PID)xy_pid[0])->setPar(xy_pid[0], 1, 0, 0);
    ((pStruct_PID)xy_pid[1])->setPar(xy_pid[1], 1, 0, 0);
    int c_x =0, c_y=0 ;
    int tmp_i = 15;
    while (tmp_i--){
		//vTaskSuspend(task_taskSchedule_handle);
        while (BottomData.sta_xy !=1){
			LED[0]->reverse(LED[0]);
            // inqCurXYPos();
			XY_GPIO_CTRL(1);
            if(isOSTime)    vTaskDelay(200);
            else            delay_xms(200);    
        }
		//vTaskResume(task_taskSchedule_handle);
		XY_GPIO_CTRL(0);
        BottomData.sta_xy = 0;
		
        c_x = BottomData.px;
        c_y = BottomData.py;
	
        double dx = (double)xy_pid[1]->run(xy_pid[1], (double)c_x, (double)t_x);
        double dy = (double)xy_pid[0]->run(xy_pid[0], (double)c_y, (double)t_y);	
		dx = dx*0.0008;
		dy = dy*0.0008;
	    if(_abs_f(dx) > 1) dx = 1 *_sign_f(dx);
        if(_abs_f(dy)> 1) dy = 1 *_sign_f(dy);
		
        TranslationMove(TIM_CTRL, 0.12, dx, dy, isOSTime);
		vTaskDelay(200);
    }
    stop_all_motor();
}


void USART6_IRQHandler(void){
	if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET){		
		DMA_Cmd(DMA2_Stream1, DISABLE); /* 关闭DMA ，防止干扰 */ 
		uint16_t reDataLenth = DMA_GetCurrDataCounter(DMA2_Stream1);
		if(reDataLenth==0&&BottomData.RxBuff[0]==0x66&&BottomData.RxBuff[COM_MSG_LEN-1]==0x88){	 
          
            switch (BottomData.RxBuff[1]){
            // case 0xA1:
            //     if(BottomData.RxBuff[2] == 0xD4){
			// 		  replyRecCarrySta();
			// 	}else if(BottomData.RxBuff[2] == 0xE5) replyRecEzoneRingSta();
            //     else if(BottomData.RxBuff[2] == 0xF6) replyRecTzoneRingSta();
            //     break;
            case 0xB2:
                if(BottomData.RxBuff[2]!= 0 && taskSta[BottomData.RxBuff[2]] != TASK_BUSY_STATE){
			        taskSta[BottomData.RxBuff[2]] = TASK_BUSY_STATE;
			        BottomData.needStartTask = BottomData.RxBuff[2];
                }
                break;
            case 0xC3://查询任务状态
                BottomData.needRelyTask = BottomData.RxBuff[2];
                //replyCurTaskStatus(BottomData.RxBuff[2]);
                break;
            case 0xD4://搬运顺序
                Arr_CarryColorSeq[FIRST_SEQ][1] = BottomData.RxBuff[2];
				Arr_CarryColorSeq[FIRST_SEQ][2] = BottomData.RxBuff[3];
				Arr_CarryColorSeq[FIRST_SEQ][3] = BottomData.RxBuff[4];
                Arr_CarryColorSeq[SECOND_SEQ][1] = BottomData.RxBuff[5];
				Arr_CarryColorSeq[SECOND_SEQ][2] = BottomData.RxBuff[6];
				Arr_CarryColorSeq[SECOND_SEQ][3] = BottomData.RxBuff[7];
                BottomData.sta_CarrySeq = 1;
				replyRecCarrySta();
                break;
            case 0xE5://粗放顺序
                Arr_ZoneColorSeq[EZONE_SEQ][1] = BottomData.RxBuff[2];
				Arr_ZoneColorSeq[EZONE_SEQ][2] = BottomData.RxBuff[3];
				Arr_ZoneColorSeq[EZONE_SEQ][3] = BottomData.RxBuff[4];
                BottomData.sta_EzoneSeq = 1;
				replyRecEzoneRingSta();
                break;
            case 0xF6://暂存顺序
				Arr_ZoneColorSeq[TZONE_SEQ][1] = BottomData.RxBuff[2];
				Arr_ZoneColorSeq[TZONE_SEQ][2] = BottomData.RxBuff[3];
                Arr_ZoneColorSeq[TZONE_SEQ][3] = BottomData.RxBuff[4];
                BottomData.sta_TzoneSeq = 1;
				replyRecTzoneRingSta();
                break;
			case 0x11:
				BottomData.sta_xy = 1;
				BottomData.px=(BottomData.RxBuff[2]-'0')*100+(BottomData.RxBuff[3]-'0')*10+(BottomData.RxBuff[4]-'0');
				BottomData.py=(BottomData.RxBuff[5]-'0')*100+(BottomData.RxBuff[6]-'0')*10+(BottomData.RxBuff[7]-'0');
            default:
                memset(BottomData.RxBuff,0,sizeof(BottomData.RxBuff));
                break;
            }
        }
		DMA2_Stream1->NDTR = COM_MSG_LEN;
		DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1);  /* 清DMA标志位 */
		DMA_Cmd(DMA2_Stream1, ENABLE);      
		USART_ClearFlag(USART6, USART_FLAG_TC); //清除发送完成标志
		USART_ReceiveData(USART6);// 清除空闲中断标志位
	}
}

		
#endif  //BOTTOM_LEVEL