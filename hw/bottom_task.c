#include"bottom_task.h"
#include"string.h"
#ifdef BOTTOM_LEVEL

Arr_pLedStruct LED = NULL;
// Arr_pStruct_Pos_PID tmp_pid = NULL; 

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
	CarBottomComUartInit(9600);
	
    // tmp_pid =  create_PosPIDStructure(1);
	LED = Create_Arr_LedStruct(3);

    LED[0]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[1]->SetEnLevel = SET_EN_LOW_LEVEL;
	LED[2]->SetEnLevel = SET_EN_LOW_LEVEL;
	IMU_UART_YawZeroOut();
	
	delay_xms(1000);
}


// void taskStart(void* pvParameters){
   
//     //taskMoveStaAreaToQRArea_start();
	
// 	while (BottomData.TaskState != StartTask_Sta){
// 		vTaskDelay(100);
// 		LED[0]->reverse(LED[0]);
// 	}

// 	taskMoveDriveOut_start();
//     vTaskDelete(taskStart_handle);
//     taskEXIT_CRITICAL();
// }   


// void taskMoveDriveOut_start(void){
// 	xTaskCreate((TaskFunction_t ) taskMoveDriveOut,  		/* 任务入口函数 */
// 			  (const char*    )  "MoveDriveOut",	    /* 任务名字 */
// 			  (uint16_t       )512,  				    /* 任务栈大小 */
// 			  (void*          )NULL,				    /* 任务入口函数参数 */
// 			  (UBaseType_t    )4, 					    /* 任务的优先级 */
// 			  (TaskHandle_t*  )&taskMoveDriveOut_handle);	/* 任务控制块指针 */ 
// }

// void taskMoveStaAreaToQRArea_start(void){
// 	xTaskCreate((TaskFunction_t ) taskMoveStaAreaToQRArea,  		/* 任务入口函数 */
// 			  (const char*    )  "MoveStaAreaToQRArea",	    /* 任务名字 */
// 			  (uint16_t       )512,  				    /* 任务栈大小 */
// 			  (void*          )NULL,				    /* 任务入口函数参数 */
// 			  (UBaseType_t    )4, 					    /* 任务的优先级 */
// 			  (TaskHandle_t*  )&taskMoveStaAreaToQRArea_handle);	/* 任务控制块指针 */ 
// }

// void taskMoveQRAreaToTurnPlateArea_start(void){
// 	xTaskCreate((TaskFunction_t ) taskMoveQRAreaToTurnPlateArea,  		/* 任务入口函数 */
// 			  (const char*    )  "MoveQRAreaToTurnPlateArea",	    /* 任务名字 */
// 			  (uint16_t       )512,  				    /* 任务栈大小 */
// 			  (void*          )NULL,				    /* 任务入口函数参数 */
// 			  (UBaseType_t    )4, 					    /* 任务的优先级 */
// 			  (TaskHandle_t*  )&taskMoveQRAreaToTurnPlateArea_handle);	/* 任务控制块指针 */ 
// }

// void taskMoveTurnPlateAreaToStorArea_start(void){
//     	xTaskCreate((TaskFunction_t )  taskMoveTurnPlateAreaToStorArea,  		/* 任务入口函数 */
// 			  (const char*    )  "MoveTurnPlateAreaToStorArea",	    /* 任务名字 */
// 			  (uint16_t       )512,  				    /* 任务栈大小 */
// 			  (void*          )NULL,				    /* 任务入口函数参数 */
// 			  (UBaseType_t    )4, 					    /* 任务的优先级 */
// 			  (TaskHandle_t*  )&taskMoveTurnPlateAreaToStorArea_handle);	/* 任务控制块指针 */ 
// }

// void taskMvBetweenColorCir_start(void){
// 	xTaskCreate((TaskFunction_t ) taskMvBetweenColorCir,  		/* 任务入口函数 */
// 			  (const char*    )  "MvBetweenColorCir",	    /* 任务名字 */
// 			  (uint16_t       )512,  				    /* 任务栈大小 */
// 			  (void*          )NULL,				    /* 任务入口函数参数 */
// 			  (UBaseType_t    )4, 					    /* 任务的优先级 */
// 			  (TaskHandle_t*  )&taskMvBetweenColorCir_handle);	/* 任务控制块指针 */ 
// }

// void taskMvBetweenColorCir(void* pvParameters){
// 	((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
//     uint8_t loop_i = 12;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}

// 	stop_all_motor();
// 	BottomRetuTask(Lo_RoughL_RoughM_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_Supply_RoughL_1){
// 		vTaskDelay(100);
// 	}
// 	BottomData.TaskState = 0;

// 	loop_i = 12;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(-1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(-1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}

// 	stop_all_motor();
// 	BottomRetuTask(Lo_Supply_RoughL_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_RoughM_RoughR_1){
// 		vTaskDelay(100);
// 	}
// 	BottomData.TaskState = 0;

// 	vTaskDelay(1500);
// 	loop_i = 24;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}
// 	stop_all_motor();
// 	BottomRetuTask(Lo_RoughM_RoughR_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_RoughL_RoughM_1){
// 		vTaskDelay(100);
// 	}


// 	loop_i = 12;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(-1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(-1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}

// 	stop_all_motor();
// 	BottomRetuTask(Lo_RoughL_RoughM_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_Supply_RoughL_1){
// 		vTaskDelay(100);
// 	}
// 	BottomData.TaskState = 0;

// 	loop_i = 12;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(-1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(-1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}

// 	stop_all_motor();
// 	BottomRetuTask(Lo_Supply_RoughL_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_RoughM_RoughR_1){
// 		vTaskDelay(100);
// 	}
// 	BottomData.TaskState = 0;

// 	vTaskDelay(1500);
// 	loop_i = 24;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}
// 	stop_all_motor();
// 	BottomRetuTask(Lo_RoughM_RoughR_1, FinishTask_Sta);
// 	vTaskDelay(300);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_RoughL_RoughM_1){
// 		vTaskDelay(100);
// 	}

	
// 	vTaskDelete(taskMvBetweenColorCir_handle);
//     taskEXIT_CRITICAL();
// }

// void taskMoveTurnPlateAreaToStorArea(void* pvParameters){
//     ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
//     vTaskDelay(1000);
//     uint8_t loop_i = 9;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.2, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.8, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(200);
// 	}
//     vTaskDelay(1000);
//     Kinematic_Analysis_Pos(0, 0, 1, 0, 1.5126);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
	
// 	vTaskDelay(2500);
// 	stop_all_motor();
// 	IMU_UART_YawZeroOut();
// 	vTaskDelay(1000);
// 	loop_i = 61;
// 	while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /160.0, 0.2, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(51);
// 	}
// 	stop_all_motor();

// 	Kinematic_Analysis_Pos(0, 1, 0, 0.22, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 	vTaskDelay(630);

// 	taskMvBetweenColorCir_start();
// 	vTaskDelete(taskMoveTurnPlateAreaToStorArea_handle);
//     taskEXIT_CRITICAL();
// }

// void taskMoveQRAreaToTurnPlateArea(void* pvParameters){
//     ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
//     uint8_t loop_i = 14;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.2, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.8, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(200);
// 	}
// 	vTaskDelay(1000);
// 	Kinematic_Analysis_Pos(0, 1, 0, 0.1, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 	vTaskDelay(630);
// 	stop_all_motor();

// 	BottomRetuTask(Lo_QRcode_Supply, FinishTask_Sta);
// 	vTaskDelay(300);
// 	LED[1]->on(LED[1]);
// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_RoughL_RoughM_1){
// 		vTaskDelay(100);
// 		LED[0]->reverse(LED[0]);
// 	}
// 	BottomData.TaskState = 0;

//     taskMoveTurnPlateAreaToStorArea_start();
//     vTaskDelete(taskMoveQRAreaToTurnPlateArea_handle);
//     taskEXIT_CRITICAL();
// }

// void taskMoveStaAreaToQRArea(void* pvParameters){
//     ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
//     vTaskDelay(3000);
//     uint8_t loop_i = 13*4 - 1;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(1, 0, 0, 0.05, 0);
// 		else Kinematic_Analysis_Pos(1, 0, -tmp_dw /200.0, 0.8, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(50);
// 	}
// 	stop_all_motor();

// 	BottomRetuTask(Lo_zero_QRcode, FinishTask_Sta);

// 	while (BottomData.TaskState != StartTask_Sta || BottomData.TaskNum != Lo_QRcode_Supply){
// 		vTaskDelay(100);
// 		LED[0]->reverse(LED[0]);
// 	}
// 	BottomData.TaskState = 0;

//     taskMoveQRAreaToTurnPlateArea_start();
//     vTaskDelete(taskMoveStaAreaToQRArea_handle);
//     taskEXIT_CRITICAL();
// }


// void taskMoveDriveOut(void* pvParameters){	
//     ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
//     vTaskDelay(3000);
//     uint8_t loop_i = 8;
//     while (loop_i--) {
// 		double yaw = HWT101_Struct.YawAngle;
// 		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
// 		if(_abs(tmp_dw) > 160) tmp_dw = 160 *_sign(tmp_dw);
// 		if(loop_i == 0) Kinematic_Analysis_Pos(0.7071/2, -0.7071/2, 0, 0.1, 0);
// 		else Kinematic_Analysis_Pos(0.7071/2, -0.7071/2, -tmp_dw /200.0, 0.4, 0);
// 		if(vel_weel[0] > 0)
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[0]), 0, (uint32_t)(800*pos_weel[0]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, True);
// 		if(vel_weel[1] > 0)
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[1]), 0, (uint32_t)(800*pos_weel[1]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, True);
// 		if(vel_weel[2] > 0)
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[2]), 0, (uint32_t)(800*pos_weel[2]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, True);
// 		if(vel_weel[3] > 0)
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, (uint32_t)(800*vel_weel[3]), 0, (uint32_t)(800*pos_weel[3]), False, True);
// 		else
// 			MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, True);
// 		vTaskDelay(200);
// 	}
// 	stop_all_motor();
//     taskMoveStaAreaToQRArea_start();
//     vTaskDelete(taskMoveDriveOut_handle);
//     taskEXIT_CRITICAL();
// }  


#endif  //BOTTOM_LEVEL