#include"motor_driver.h"
#include"sys.h"
#include"pid.h"
#include"FreeRTOS.h"
#include"task.h"
#include"hwt101.h"
#include"KinematicModel.h"
#include"stdlib.h"
#include"math.h"

#ifdef BOTTOM_LEVEL
#include"task_schedule.h"

#ifdef STEPPER_MOTOR_DRIVER

#define DIAMETER_WHEEL  76 //mm
#define MicroStep       4        
#define TotalStep       800   
#define MAX_Vel         3 
#define Max_RealVel     2400

Arr_pStruct_Pos_PID tmp_pid = NULL;
Arr_pStruct_Pos_PID xy_pid = NULL;

StepBufferType StepMotorBuffer[4][STEPS_LOOP_MAXNUM+1];
StepBufferType *StepMotorLFBuffer = StepMotorBuffer[MOTOR_LF_ADDR-1];
StepBufferType *StepMotorLRBuffer = StepMotorBuffer[MOTOR_LR_ADDR-1];
StepBufferType *StepMotorRFBuffer = StepMotorBuffer[MOTOR_RF_ADDR-1];
StepBufferType *StepMotorRRBuffer = StepMotorBuffer[MOTOR_RR_ADDR-1];

MotorUartBufferType MotorUartBuffer[5][STEPS_UART_BUFFER_LENTH];
MotorUartBufferType *MotorLFUartBuffer = MotorUartBuffer[MOTOR_LF_ADDR];
MotorUartBufferType *MotorLRUartBuffer = MotorUartBuffer[MOTOR_LR_ADDR];
MotorUartBufferType *MotorRFUartBuffer = MotorUartBuffer[MOTOR_RF_ADDR];
MotorUartBufferType *MotorRRUartBuffer = MotorUartBuffer[MOTOR_RR_ADDR];

bool    isENMotor[5]      = {0};
uint32_t StepCycles[5]     = {0};
uint32_t StepCyclesRema[5] = {0};
uint32_t flag_dma_motor[5] = {0};

void MOTOR_LF_TIM_Init(void){

    GPIO_InitTypeDef  GPIO_InitStructure;

    MOTOR_LF_FUN_EN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LF_EN_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LF_EN_GPIO, &GPIO_InitStructure);

    MOTOR_LF_FUN_DIR_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LF_DIR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LF_DIR_GPIO, &GPIO_InitStructure);

    MOTOR_LF_FUN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LF_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LF_GPIO, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOTOR_LF_GPIO, MOTOR_LF_GPIO_PINSOURCE, MOTOR_LF_AF_TIM);

    MOTOR_LF_FUN_TIM_CLK(ENABLE);
    TIM_DeInit(MOTOR_LF_TIM);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler         = 840-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_LF_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 10;/*占空比*/
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    MOTOR_LF_FUN_TIM_OC_INIT(MOTOR_LF_TIM, &TIM_OCInitStructure);
    MOTOR_LF_FUN_TIM_OC_PRE(MOTOR_LF_TIM, TIM_OCPreload_Enable);

    MOTOR_LF_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_LF_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_LF_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_LF_TIM_CC_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)StepMotorLFBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_LOOP_MAXNUM;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_LF_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_LF_TIM, MOTOR_LF_TIM_DMA_CC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2; 
    NVIC_InitStructure.NVIC_IRQChannel                   = MOTOR_LF_TIM_DMA_IRQn ;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ClearFlag(MOTOR_LF_DMA_STREAM, DMA_IT_TC);        
    DMA_ITConfig(MOTOR_LF_DMA_STREAM, DMA_IT_TC, ENABLE); 
}


void MOTOR_LR_TIM_Init(void){

    GPIO_InitTypeDef  GPIO_InitStructure;

    MOTOR_LR_FUN_EN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LR_EN_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LR_EN_GPIO, &GPIO_InitStructure);

    MOTOR_LR_FUN_DIR_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LR_DIR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LR_DIR_GPIO, &GPIO_InitStructure);

    MOTOR_LR_FUN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_LR_GPIO, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOTOR_LR_GPIO, MOTOR_LR_GPIO_PINSOURCE, MOTOR_LR_AF_TIM);

    MOTOR_LR_FUN_TIM_CLK(ENABLE);
    TIM_DeInit(MOTOR_LR_TIM);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler         = 1680-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_LR_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 10;/*占空比*/
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    MOTOR_LR_FUN_TIM_OC_INIT(MOTOR_LR_TIM, &TIM_OCInitStructure);
    MOTOR_LR_FUN_TIM_OC_PRE(MOTOR_LR_TIM, TIM_OCPreload_Enable);

    MOTOR_LR_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_LR_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_LR_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_LR_TIM_CC_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)StepMotorLRBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_LOOP_MAXNUM;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_LR_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_LR_TIM, MOTOR_LR_TIM_DMA_CC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2; 
    NVIC_InitStructure.NVIC_IRQChannel                   = MOTOR_LR_TIM_DMA_IRQn ;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ClearFlag(MOTOR_LR_DMA_STREAM, DMA_IT_TC);        
    DMA_ITConfig(MOTOR_LR_DMA_STREAM, DMA_IT_TC, ENABLE); 
}

void MOTOR_RF_TIM_Init(void){

    GPIO_InitTypeDef  GPIO_InitStructure;

    MOTOR_RF_FUN_EN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RF_EN_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RF_EN_GPIO, &GPIO_InitStructure);

    MOTOR_RF_FUN_DIR_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RF_DIR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RF_DIR_GPIO, &GPIO_InitStructure);

    MOTOR_RF_FUN_GPIO_CLK(ENABLE);
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RF_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RF_GPIO, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOTOR_RF_GPIO, MOTOR_RF_GPIO_PINSOURCE, MOTOR_RF_AF_TIM);

    MOTOR_RF_FUN_TIM_CLK(ENABLE);
    TIM_DeInit(MOTOR_RF_TIM);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler         = 840-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_RF_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 10;/*占空比*/
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    MOTOR_RF_FUN_TIM_OC_INIT(MOTOR_RF_TIM, &TIM_OCInitStructure);
    MOTOR_RF_FUN_TIM_OC_PRE(MOTOR_RF_TIM, TIM_OCPreload_Enable);
    // TIM_CtrlPWMOutputs(MOTOR_RF_TIM, ENABLE);

    MOTOR_RF_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_RF_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_RF_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_RF_TIM_CC_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)StepMotorRFBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_LOOP_MAXNUM;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RF_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_RF_TIM, MOTOR_RF_TIM_DMA_CC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2; 
    NVIC_InitStructure.NVIC_IRQChannel                   = MOTOR_RF_TIM_DMA_IRQn ;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ClearFlag(MOTOR_RF_DMA_STREAM, DMA_IT_TC);        
    DMA_ITConfig(MOTOR_RF_DMA_STREAM, DMA_IT_TC, ENABLE); 

    //DMA_ClearFlag(MOTOR_RF_DMA_STREAM, DMA_FLAG_TCIF2);
   // DMA_ClearFlag(MOTOR_RF_DMA_STREAM, DMA_IT_TC);        //第一次不进中断
   // DMA_ITConfig(MOTOR_RF_DMA_STREAM, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
}

void MOTOR_RR_TIM_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;

    MOTOR_RR_FUN_EN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RR_EN_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RR_EN_GPIO, &GPIO_InitStructure);

    MOTOR_RR_FUN_DIR_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RR_DIR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RR_DIR_GPIO, &GPIO_InitStructure);

	MOTOR_RR_FUN_GPIO_CLK(ENABLE);	
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RR_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(MOTOR_RR_GPIO, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOTOR_RR_GPIO, MOTOR_RR_GPIO_PINSOURCE, MOTOR_RR_AF_TIM);

    MOTOR_RR_FUN_TIM_CLK(ENABLE);
    TIM_DeInit(MOTOR_RR_TIM);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler         = 1680-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_RR_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 10;/*占空比*/
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    MOTOR_RR_FUN_TIM_OC_INIT(MOTOR_RR_TIM, &TIM_OCInitStructure);
    MOTOR_RR_FUN_TIM_OC_PRE(MOTOR_RR_TIM, TIM_OCPreload_Enable);
    
    MOTOR_RR_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_RR_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_RR_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_RR_TIM_CC_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)StepMotorRRBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_LOOP_MAXNUM;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RR_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_RR_TIM, MOTOR_RR_TIM_DMA_CC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2; 
    NVIC_InitStructure.NVIC_IRQChannel                   = MOTOR_RR_TIM_DMA_IRQn ;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ClearFlag(MOTOR_RR_DMA_STREAM, DMA_IT_TC);        
    DMA_ITConfig(MOTOR_RR_DMA_STREAM, DMA_IT_TC, ENABLE); 
}


void  MOTOR_LF_UART_Init(uint32_t bound){

	MOTOR_LF_UART_RX_FUN_GPIO_CLK(ENABLE);
	MOTOR_LF_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(MOTOR_LF_UART_RX_GPIO,MOTOR_LF_UART_RX_GPIO_PINSOURCE, MOTOR_LF_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(MOTOR_LF_UART_TX_GPIO,MOTOR_LF_UART_TX_GPIO_PINSOURCE, MOTOR_LF_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = MOTOR_LF_UART_RX_GPIO_PIN; 
	GPIO_Init(MOTOR_LF_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LF_UART_TX_GPIO_PIN; 
	GPIO_Init(MOTOR_LF_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MOTOR_LF_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(MOTOR_LF_UART, &USART_InitStructure); 

    MOTOR_LF_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_LF_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_LF_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_LF_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MotorLFUartBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_UART_BUFFER_LENTH ;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_LF_UART_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(MOTOR_LF_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MOTOR_LF_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MOTOR_LF_UART, ENABLE);
}


void MOTOR_LR_UART_Init(uint32_t bound){

	MOTOR_LR_UART_RX_FUN_GPIO_CLK(ENABLE);
	MOTOR_LR_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(MOTOR_LR_UART_RX_GPIO,MOTOR_LR_UART_RX_GPIO_PINSOURCE, MOTOR_LR_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(MOTOR_LR_UART_TX_GPIO,MOTOR_LR_UART_TX_GPIO_PINSOURCE, MOTOR_LR_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = MOTOR_LR_UART_RX_GPIO_PIN; 
	GPIO_Init(MOTOR_LR_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = MOTOR_LR_UART_TX_GPIO_PIN; 
	GPIO_Init(MOTOR_LR_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MOTOR_LR_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(MOTOR_LR_UART, &USART_InitStructure); 

    MOTOR_LR_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_LR_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_LR_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_LR_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MotorLRUartBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_UART_BUFFER_LENTH;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_LR_UART_DMA_STREAM, &DMA_InitStructure);
    
    USART_DMACmd(MOTOR_LR_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MOTOR_LR_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MOTOR_LR_UART, ENABLE);
}

void  MOTOR_RF_UART_Init(uint32_t bound){
    MOTOR_RF_UART_RX_FUN_GPIO_CLK(ENABLE);
	MOTOR_RF_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(MOTOR_RF_UART_RX_GPIO,MOTOR_RF_UART_RX_GPIO_PINSOURCE, MOTOR_RF_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(MOTOR_RF_UART_TX_GPIO,MOTOR_RF_UART_TX_GPIO_PINSOURCE, MOTOR_RF_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = MOTOR_RF_UART_RX_GPIO_PIN; 
	GPIO_Init(MOTOR_RF_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RF_UART_TX_GPIO_PIN; 
	GPIO_Init(MOTOR_RF_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MOTOR_RF_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(MOTOR_RF_UART, &USART_InitStructure); 

    MOTOR_RF_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_RF_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_RF_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_RF_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MotorRFUartBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_UART_BUFFER_LENTH;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RF_UART_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(MOTOR_RF_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MOTOR_RF_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MOTOR_RF_UART, ENABLE);
}

void  MOTOR_RR_UART_Init(uint32_t bound){

	MOTOR_RR_UART_RX_FUN_GPIO_CLK(ENABLE);
	MOTOR_RR_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(MOTOR_RR_UART_RX_GPIO,MOTOR_RR_UART_RX_GPIO_PINSOURCE, MOTOR_RR_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(MOTOR_RR_UART_TX_GPIO,MOTOR_RR_UART_TX_GPIO_PINSOURCE, MOTOR_RR_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = MOTOR_RR_UART_RX_GPIO_PIN; 
	GPIO_Init(MOTOR_RR_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = MOTOR_RR_UART_TX_GPIO_PIN; 
	GPIO_Init(MOTOR_RR_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MOTOR_RR_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(MOTOR_RR_UART, &USART_InitStructure); 

    MOTOR_RR_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(MOTOR_RR_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = MOTOR_RR_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&MOTOR_RR_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MotorRRUartBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = STEPS_UART_BUFFER_LENTH;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RR_UART_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(MOTOR_RR_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MOTOR_RR_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MOTOR_RR_UART, ENABLE);
}


void sendMotorUart_Once(MOTOR_UART_ADDR_ENUM Motor_addr, int Msg_Lenth){
    switch (Motor_addr){
    case MOTOR_LF_ADDR:
        MOTOR_LF_UART_DMA_STREAM->NDTR = Msg_Lenth;
        DMA_ClearFlag(MOTOR_LF_UART_DMA_STREAM, DMA_FLAG_TCIF6);
		DMA_Cmd(MOTOR_LF_UART_DMA_STREAM, ENABLE);
        break;
    case MOTOR_LR_ADDR:
        MOTOR_LR_UART_DMA_STREAM->NDTR = Msg_Lenth;
        DMA_ClearFlag(MOTOR_LR_UART_DMA_STREAM, DMA_FLAG_TCIF3);
		DMA_Cmd(MOTOR_LR_UART_DMA_STREAM, ENABLE);
        break;  
    case MOTOR_RF_ADDR:
        MOTOR_RF_UART_DMA_STREAM->NDTR = Msg_Lenth;
        DMA_ClearFlag(MOTOR_RF_UART_DMA_STREAM, DMA_FLAG_TCIF7);
		DMA_Cmd(MOTOR_RF_UART_DMA_STREAM, ENABLE);
        break;
    case MOTOR_RR_ADDR:
        MOTOR_RR_UART_DMA_STREAM->NDTR = Msg_Lenth;
        DMA_ClearFlag(MOTOR_RR_UART_DMA_STREAM, DMA_FLAG_TCIF7);
		DMA_Cmd(MOTOR_RR_UART_DMA_STREAM, ENABLE);
        break;
    case MOTOR_ALL_ADDR:
        MOTOR_LF_UART_DMA_STREAM->NDTR = Msg_Lenth;
        MOTOR_LR_UART_DMA_STREAM->NDTR = Msg_Lenth;
        MOTOR_RF_UART_DMA_STREAM->NDTR = Msg_Lenth;
        MOTOR_RR_UART_DMA_STREAM->NDTR = Msg_Lenth;
        DMA_ClearFlag(MOTOR_LF_UART_DMA_STREAM, DMA_FLAG_TCIF6);
        DMA_ClearFlag(MOTOR_LR_UART_DMA_STREAM, DMA_FLAG_TCIF3);
        DMA_ClearFlag(MOTOR_RF_UART_DMA_STREAM, DMA_FLAG_TCIF7);
        DMA_ClearFlag(MOTOR_RR_UART_DMA_STREAM, DMA_FLAG_TCIF7);
        USART_Cmd(MOTOR_LF_UART, ENABLE);  
		USART_Cmd(MOTOR_LR_UART, ENABLE);
		USART_Cmd(MOTOR_RF_UART, ENABLE);
		USART_Cmd(MOTOR_RR_UART, ENABLE);
		DMA_Cmd(MOTOR_LF_UART_DMA_STREAM, ENABLE);  
		DMA_Cmd(MOTOR_LR_UART_DMA_STREAM, ENABLE);
		DMA_Cmd(MOTOR_RF_UART_DMA_STREAM, ENABLE);
		DMA_Cmd(MOTOR_RR_UART_DMA_STREAM, ENABLE);
        break;
    }
}

void MotorUartCtrl(                                                                         \
    MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, uint16_t Motor_vel,     \
    uint8_t Motor_acc, uint32_t Motor_clk, ABS_OR_REL_FLAG Abs_or_Rel_Flag, bool isSend     \
){  
    uint8_t real_Motor_dir = 0;
    if(Motor_addr == MOTOR_LF_ADDR || Motor_addr == MOTOR_LR_ADDR){
        if(Motor_dir == MOTOR_FORWARD)      real_Motor_dir = 0x01;
        else if(Motor_dir == MOTOR_REVERSE) real_Motor_dir = 0x00;
    }else if(Motor_addr == MOTOR_RF_ADDR || Motor_addr == MOTOR_RR_ADDR){
        if(Motor_dir == MOTOR_FORWARD)      real_Motor_dir = 0x01;
        else if(Motor_dir == MOTOR_REVERSE) real_Motor_dir = 0x00;
    }
    MotorUartBuffer[Motor_addr][0]  = Motor_addr;
    MotorUartBuffer[Motor_addr][1]  = 0xFD;
    MotorUartBuffer[Motor_addr][2]  = real_Motor_dir;
    MotorUartBuffer[Motor_addr][3]  = (uint8_t)(Motor_vel>>8);
    MotorUartBuffer[Motor_addr][4]  = (uint8_t)(Motor_vel>>0);
    MotorUartBuffer[Motor_addr][5]  = Motor_acc;
    MotorUartBuffer[Motor_addr][6]  = (uint8_t)(Motor_clk>>24);
    MotorUartBuffer[Motor_addr][7]  = (uint8_t)(Motor_clk>>16);
    MotorUartBuffer[Motor_addr][8]  = (uint8_t)(Motor_clk>>8);
    MotorUartBuffer[Motor_addr][9]  = (uint8_t)(Motor_clk>>0);
    MotorUartBuffer[Motor_addr][10] = (uint8_t)(Abs_or_Rel_Flag);
    MotorUartBuffer[Motor_addr][11] = (uint8_t)0x00;
    MotorUartBuffer[Motor_addr][12] = (uint8_t)0x6B;
    if(isSend == True){
        sendMotorUart_Once(Motor_addr, 13);
    }
}

void Fun_Dir_StepMotor(MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir){
    GPIO_TypeDef * DIR_GPIO = NULL;
    uint16_t DIR_GPIO_PIN   = NULL;
    switch (Motor_addr){
    case MOTOR_LF_ADDR:
        DIR_GPIO      = MOTOR_LF_DIR_GPIO;
        DIR_GPIO_PIN  = MOTOR_LF_DIR_GPIO_PIN;
        break;
    case MOTOR_LR_ADDR:
        DIR_GPIO      = MOTOR_LR_DIR_GPIO;
        DIR_GPIO_PIN  = MOTOR_LR_DIR_GPIO_PIN;
        break;  
    case MOTOR_RF_ADDR:
        DIR_GPIO      = MOTOR_RF_DIR_GPIO;
        DIR_GPIO_PIN  = MOTOR_RF_DIR_GPIO_PIN;
        break;
    case MOTOR_RR_ADDR:
        DIR_GPIO      = MOTOR_RR_DIR_GPIO;
        DIR_GPIO_PIN  = MOTOR_RR_DIR_GPIO_PIN;
        break;
    }
    if(Motor_dir) GPIO_SetBits(DIR_GPIO, DIR_GPIO_PIN);
    else          GPIO_ResetBits(DIR_GPIO, DIR_GPIO_PIN);
}

void Fun_isGearShift(MOTOR_UART_ADDR_ENUM Motor_addr, bool isGearShift){
    DMA_Stream_TypeDef * DMA_STREAMx = NULL;
    switch (Motor_addr){
        case MOTOR_LF_ADDR:
                DMA_STREAMx   = MOTOR_LF_DMA_STREAM;
            break;
        case MOTOR_LR_ADDR:
                DMA_STREAMx   = MOTOR_LR_DMA_STREAM;
            break;
        case MOTOR_RF_ADDR:
                DMA_STREAMx   = MOTOR_RF_DMA_STREAM;
            break;
        case MOTOR_RR_ADDR:
                DMA_STREAMx   = MOTOR_RR_DMA_STREAM;
            break;
     }
     if(isGearShift) {
        DMA_STREAMx->CR &= ~DMA_SxCR_EN; // 清除EN位
        DMA_STREAMx->CR |= DMA_SxCR_MINC; // 设置MINC位
        DMA_STREAMx->CR |= DMA_SxCR_EN; // 设置EN位      
    }else{
        DMA_STREAMx->CR &= ~DMA_SxCR_EN; // 清除EN位
        DMA_STREAMx->CR &= ~DMA_SxCR_MINC; // 关闭内存地址自增
        DMA_STREAMx->CR |= DMA_SxCR_EN; // 设置EN位      
    }
}


void Fun_En_DMA_Motor(MOTOR_UART_ADDR_ENUM Motor_addr){
    DMA_Stream_TypeDef * DMA_STREAMx = NULL;
    uint32_t DMA_FLAG_TCIF = 0;
    TIM_TypeDef * TIMx = NULL; 
    if(Motor_addr ==  MOTOR_ALL_ADDR){
        if(isENMotor[MOTOR_LF_ADDR]){
            flag_dma_motor[MOTOR_LF_ADDR] = 0;
            DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
            TIM_ARRPreloadConfig(MOTOR_LF_TIM, ENABLE);//ARPE使能
            MOTOR_LF_TIM->EGR |= 0x01;
            DMA_ClearFlag(MOTOR_LF_DMA_STREAM, MOTOR_LF_TIM_DMA_FLAG_TCIF);
            DMA_Cmd(MOTOR_LF_DMA_STREAM, ENABLE);
            TIM_Cmd(MOTOR_LF_TIM, ENABLE);
        }
        if(isENMotor[MOTOR_LR_ADDR]){
            flag_dma_motor[MOTOR_LR_ADDR] = 0;
            DMA_Cmd(MOTOR_LR_DMA_STREAM, DISABLE);
            TIM_ARRPreloadConfig(MOTOR_LR_TIM, ENABLE);//ARPE使能
            MOTOR_LR_TIM->EGR |= 0x01;
            DMA_ClearFlag(MOTOR_LR_DMA_STREAM, MOTOR_LR_TIM_DMA_FLAG_TCIF);
            DMA_Cmd(MOTOR_LR_DMA_STREAM, ENABLE);
            TIM_Cmd(MOTOR_LR_TIM, ENABLE);
            TIM_CtrlPWMOutputs(MOTOR_LR_TIM, ENABLE); 
        }
        if(isENMotor[MOTOR_RF_ADDR]){
            flag_dma_motor[MOTOR_RF_ADDR] = 0;
            DMA_Cmd(MOTOR_RF_DMA_STREAM, DISABLE);
            TIM_ARRPreloadConfig(MOTOR_RF_TIM, ENABLE);//ARPE使能
            MOTOR_RF_TIM->EGR |= 0x01;
            DMA_ClearFlag(MOTOR_RF_DMA_STREAM, MOTOR_RF_TIM_DMA_FLAG_TCIF);
            DMA_Cmd(MOTOR_RF_DMA_STREAM, ENABLE);
            TIM_Cmd(MOTOR_RF_TIM, ENABLE);
        }
        if(isENMotor[MOTOR_RR_ADDR]){
            flag_dma_motor[MOTOR_RR_ADDR] = 0;
            DMA_Cmd(MOTOR_RR_DMA_STREAM, DISABLE);
            TIM_ARRPreloadConfig(MOTOR_RR_TIM, ENABLE);//ARPE使能      
            MOTOR_RR_TIM->EGR |= 0x01;
            DMA_ClearFlag(MOTOR_RR_DMA_STREAM, MOTOR_RR_TIM_DMA_FLAG_TCIF);
            DMA_Cmd(MOTOR_RR_DMA_STREAM, ENABLE);
            TIM_Cmd(MOTOR_RR_TIM, ENABLE); 
            TIM_CtrlPWMOutputs(MOTOR_RR_TIM, ENABLE);
        }
        return;
    }
    switch (Motor_addr){
    case MOTOR_LF_ADDR:
        DMA_STREAMx   = MOTOR_LF_DMA_STREAM;
        DMA_FLAG_TCIF = MOTOR_LF_TIM_DMA_FLAG_TCIF;
        TIMx          = MOTOR_LF_TIM;
        break;
    case MOTOR_LR_ADDR:
        DMA_STREAMx   = MOTOR_LR_DMA_STREAM;
        DMA_FLAG_TCIF = MOTOR_LR_TIM_DMA_FLAG_TCIF;
        TIMx          = MOTOR_LR_TIM;
        break;
    case MOTOR_RF_ADDR:
        DMA_STREAMx   = MOTOR_RF_DMA_STREAM;
        DMA_FLAG_TCIF = MOTOR_RF_TIM_DMA_FLAG_TCIF;
        TIMx          = MOTOR_RF_TIM;
        break;
    case MOTOR_RR_ADDR:
        DMA_STREAMx   = MOTOR_RR_DMA_STREAM;
        DMA_FLAG_TCIF = MOTOR_RR_TIM_DMA_FLAG_TCIF;
        TIMx          = MOTOR_RR_TIM;
        break;
    }

    if(isENMotor[Motor_addr]){
        flag_dma_motor[Motor_addr] = 0;
        DMA_Cmd(DMA_STREAMx, DISABLE);
        TIM_ARRPreloadConfig(TIMx, ENABLE);//ARPE使能
  	    TIMx->EGR |= 0x01;
        DMA_ClearFlag(DMA_STREAMx, DMA_FLAG_TCIF);
	    DMA_Cmd(DMA_STREAMx, ENABLE);
	    TIM_Cmd(TIMx, ENABLE);
        if(TIMx == TIM1 || TIMx == TIM8) TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }
}

void Fun_DataLenth_DMA_Motor(MOTOR_UART_ADDR_ENUM Motor_addr, uint32_t translenth){
    DMA_Stream_TypeDef * DMA_STREAMx = NULL;
    switch (Motor_addr){
    case MOTOR_LF_ADDR:
        DMA_STREAMx   = MOTOR_LF_DMA_STREAM;
        break;
    case MOTOR_LR_ADDR:
        DMA_STREAMx   = MOTOR_LR_DMA_STREAM;   
        break;
    case MOTOR_RF_ADDR:
        DMA_STREAMx   = MOTOR_RF_DMA_STREAM;
        break;
    case MOTOR_RR_ADDR:
        DMA_STREAMx   = MOTOR_RR_DMA_STREAM; 
        break;
    }
    DMA_STREAMx->NDTR = translenth;
}

// bool isENMotor[5]          = {0};
// uint32_t StepCycles[5]     = {0};
// uint32_t StepCyclesRema[5] = {0};


void MotorTIMCtrl(MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, 
    uint32_t Motor_vel,uint32_t Motor_acc, uint32_t Motor_clk, bool isGearShift, bool isEn){ 
    Motor_clk=_abs(Motor_clk);
    if(Motor_vel == 0 || Motor_clk == 0){
        isENMotor[Motor_addr] = 0;
        return;
    }else{
        isENMotor[Motor_addr] = 1;
    }

    if(Motor_vel > 2400) Motor_vel = 2400;

    Fun_Dir_StepMotor(Motor_addr, Motor_dir);

    if(isGearShift == False && Motor_vel != 0) 
        StepMotorBuffer[Motor_addr-1][0] = (StepBufferType)(100000 / Motor_vel);

    if(Motor_clk < STEPS_LOOP_MAXNUM) Fun_DataLenth_DMA_Motor(Motor_addr, Motor_clk);
    else  Fun_DataLenth_DMA_Motor(Motor_addr, STEPS_LOOP_MAXNUM);

    Fun_isGearShift(Motor_addr, isGearShift);   
    
    StepCycles[Motor_addr]     = Motor_clk / STEPS_LOOP_MAXNUM;
    StepCyclesRema[Motor_addr] = Motor_clk % STEPS_LOOP_MAXNUM;  

    if(isEn && isENMotor[Motor_addr] == 1)    Fun_En_DMA_Motor(Motor_addr);    
}

// static uint32_t flag_dma_motor[5] = {0};
void MOTOR_LF_TIM_DMA_IRQHandler(void){
if (DMA_GetITStatus(MOTOR_LF_DMA_STREAM, MOTOR_LF_TIM_DMA_IT_TCIF)){
    if((flag_dma_motor[MOTOR_LF_ADDR] >=  StepCycles[MOTOR_LF_ADDR] && StepCyclesRema[MOTOR_LF_ADDR] == 0) || flag_dma_motor[MOTOR_LF_ADDR] > StepCycles[MOTOR_LF_ADDR] ){
            TIM_Cmd(MOTOR_LF_TIM, DISABLE);
            DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
            flag_dma_motor[MOTOR_LF_ADDR] = 0;
    }else if(flag_dma_motor[MOTOR_LF_ADDR] == StepCycles[MOTOR_LF_ADDR] && StepCyclesRema[MOTOR_LF_ADDR] != 0){
        flag_dma_motor[MOTOR_LF_ADDR]++;
        TIM_Cmd(MOTOR_LF_TIM, DISABLE);
        DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
        MOTOR_LF_TIM->EGR |= 0x01;
	    MOTOR_LF_DMA_STREAM->NDTR = StepCyclesRema[MOTOR_LF_ADDR];
	    DMA_ClearFlag(MOTOR_LF_DMA_STREAM, MOTOR_LF_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_LF_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_LF_TIM, ENABLE);
    }else{
        flag_dma_motor[MOTOR_LF_ADDR]++;
        TIM_Cmd(MOTOR_LF_TIM, DISABLE);
        DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
        MOTOR_LF_TIM->EGR |= 0x01;
	    MOTOR_LF_DMA_STREAM->NDTR = STEPS_LOOP_MAXNUM;
	    DMA_ClearFlag(MOTOR_LF_DMA_STREAM, MOTOR_LF_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_LF_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_LF_TIM, ENABLE);
    }
    DMA_ClearITPendingBit(MOTOR_LF_DMA_STREAM, MOTOR_LF_TIM_DMA_IT_TCIF);
  }
}

void MOTOR_LR_TIM_DMA_IRQHandler(void){
 if (DMA_GetITStatus(MOTOR_LR_DMA_STREAM, MOTOR_LR_TIM_DMA_IT_TCIF)){
    if((flag_dma_motor[MOTOR_LR_ADDR] >=  StepCycles[MOTOR_LR_ADDR] && StepCyclesRema[MOTOR_LR_ADDR] == 0) || flag_dma_motor[MOTOR_LR_ADDR] > StepCycles[MOTOR_LR_ADDR] ){
            TIM_Cmd(MOTOR_LR_TIM, DISABLE);
            DMA_Cmd(MOTOR_LR_DMA_STREAM, DISABLE);
             flag_dma_motor[MOTOR_LR_ADDR] = 0;
    }else if(flag_dma_motor[MOTOR_LR_ADDR] == StepCycles[MOTOR_LR_ADDR] && StepCyclesRema[MOTOR_LR_ADDR] != 0){
        flag_dma_motor[MOTOR_LR_ADDR]++;
        TIM_Cmd(MOTOR_LR_TIM, DISABLE);
        DMA_Cmd(MOTOR_LR_DMA_STREAM, DISABLE);
        MOTOR_LR_TIM->EGR |= 0x01;
	    MOTOR_LR_DMA_STREAM->NDTR = StepCyclesRema[MOTOR_LR_ADDR];
	    DMA_ClearFlag(MOTOR_LR_DMA_STREAM, MOTOR_LR_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_LR_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_LR_TIM, ENABLE);
    }else{
        flag_dma_motor[MOTOR_LR_ADDR]++;
        TIM_Cmd(MOTOR_LR_TIM, DISABLE);
        DMA_Cmd(MOTOR_LR_DMA_STREAM, DISABLE);
        MOTOR_LR_TIM->EGR |= 0x01;
	    MOTOR_LR_DMA_STREAM->NDTR = STEPS_LOOP_MAXNUM;
	    DMA_ClearFlag(MOTOR_LR_DMA_STREAM, MOTOR_LR_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_LR_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_LR_TIM, ENABLE);
    }
    DMA_ClearITPendingBit(MOTOR_LR_DMA_STREAM, MOTOR_LR_TIM_DMA_IT_TCIF);
  }
}


void MOTOR_RF_TIM_DMA_IRQHandler(void){
 if (DMA_GetITStatus(MOTOR_RF_DMA_STREAM, MOTOR_RF_TIM_DMA_IT_TCIF)){
    if((flag_dma_motor[MOTOR_RF_ADDR] >=  StepCycles[MOTOR_RF_ADDR] && StepCyclesRema[MOTOR_RF_ADDR] == 0) || flag_dma_motor[MOTOR_RF_ADDR] > StepCycles[MOTOR_RF_ADDR] ){
            TIM_Cmd(MOTOR_RF_TIM, DISABLE);
            DMA_Cmd(MOTOR_RF_DMA_STREAM, DISABLE);
            flag_dma_motor[MOTOR_RF_ADDR] = 0;
    }else if(flag_dma_motor[MOTOR_RF_ADDR] == StepCycles[MOTOR_RF_ADDR] && StepCyclesRema[MOTOR_RF_ADDR] != 0){
        flag_dma_motor[MOTOR_RF_ADDR]++;
        TIM_Cmd(MOTOR_RF_TIM, DISABLE);
        DMA_Cmd(MOTOR_RF_DMA_STREAM, DISABLE);
        MOTOR_RF_TIM->EGR |= 0x01;
	    MOTOR_RF_DMA_STREAM->NDTR = StepCyclesRema[MOTOR_RF_ADDR];
	    DMA_ClearFlag(MOTOR_RF_DMA_STREAM, MOTOR_RF_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_RF_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_RF_TIM, ENABLE);
    }else{
        flag_dma_motor[MOTOR_RF_ADDR]++;
        TIM_Cmd(MOTOR_RF_TIM, DISABLE);
        DMA_Cmd(MOTOR_RF_DMA_STREAM, DISABLE);
        MOTOR_RF_TIM->EGR |= 0x01;
	    MOTOR_RF_DMA_STREAM->NDTR = STEPS_LOOP_MAXNUM;
	    DMA_ClearFlag(MOTOR_RF_DMA_STREAM, MOTOR_RF_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_RF_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_RF_TIM, ENABLE);
    }
    DMA_ClearITPendingBit(MOTOR_RF_DMA_STREAM, MOTOR_RF_TIM_DMA_IT_TCIF);
  }
}


void MOTOR_RR_TIM_DMA_IRQHandler(void){
 if (DMA_GetITStatus(MOTOR_RR_DMA_STREAM, MOTOR_RR_TIM_DMA_IT_TCIF)){
    if((flag_dma_motor[MOTOR_RR_ADDR] >=  StepCycles[MOTOR_RR_ADDR] && StepCyclesRema[MOTOR_RR_ADDR] == 0) || flag_dma_motor[MOTOR_RR_ADDR] > StepCycles[MOTOR_RR_ADDR] ){
            TIM_Cmd(MOTOR_RR_TIM, DISABLE);
            DMA_Cmd(MOTOR_RR_DMA_STREAM, DISABLE);
            flag_dma_motor[MOTOR_RR_ADDR] = 0;
    }else if(flag_dma_motor[MOTOR_RR_ADDR] == StepCycles[MOTOR_RR_ADDR] && StepCyclesRema[MOTOR_RR_ADDR] != 0){
        flag_dma_motor[MOTOR_RR_ADDR]++;
        TIM_Cmd(MOTOR_RR_TIM, DISABLE);
        DMA_Cmd(MOTOR_RR_DMA_STREAM, DISABLE);
        MOTOR_RR_TIM->EGR |= 0x01;
	    MOTOR_RR_DMA_STREAM->NDTR = StepCyclesRema[MOTOR_RR_ADDR];
	    DMA_ClearFlag(MOTOR_RR_DMA_STREAM, MOTOR_RR_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_RR_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_RR_TIM, ENABLE);
    }else{
        flag_dma_motor[MOTOR_RR_ADDR]++;
        TIM_Cmd(MOTOR_RR_TIM, DISABLE);
        DMA_Cmd(MOTOR_RR_DMA_STREAM, DISABLE);
        MOTOR_RR_TIM->EGR |= 0x01;
	    MOTOR_RR_DMA_STREAM->NDTR = STEPS_LOOP_MAXNUM;
	    DMA_ClearFlag(MOTOR_RR_DMA_STREAM, MOTOR_RR_TIM_DMA_FLAG_TCIF);
	    DMA_Cmd(MOTOR_RR_DMA_STREAM, ENABLE);
	    TIM_Cmd(MOTOR_RR_TIM, ENABLE);
    }
    DMA_ClearITPendingBit(MOTOR_RR_DMA_STREAM, MOTOR_RR_TIM_DMA_IT_TCIF);
  }
}



void stop_all_motor(void){
    DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
    TIM_Cmd(MOTOR_LF_TIM, DISABLE);
    DMA_Cmd(MOTOR_LR_DMA_STREAM, DISABLE);
    TIM_Cmd(MOTOR_LR_TIM, DISABLE);
    DMA_Cmd(MOTOR_RF_DMA_STREAM, DISABLE);
    TIM_Cmd(MOTOR_RF_TIM, DISABLE);
    DMA_Cmd(MOTOR_RR_DMA_STREAM, DISABLE);
    TIM_Cmd(MOTOR_RR_TIM, DISABLE);
}


void AntiClockwise_90Angle(MOTOR_TYPE_ENUM Motor_ctrl){
    switch (Motor_ctrl){
    case UART_CTRL:
    //1214
        MotorUartCtrl(MOTOR_LF_ADDR,  MOTOR_REVERSE, 0x52, 0x80, 1217, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_LR_ADDR,  MOTOR_REVERSE, 0x52, 0x80, 1217, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RF_ADDR,  MOTOR_FORWARD, 0x52, 0x80, 1217, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RR_ADDR,  MOTOR_FORWARD, 0x52, 0x80, 1217, REL_FLAG, False);
	    sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
        break;
    case TIM_CTRL:
    //1214
        MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, 800, 0, 1217, False, False);
	    MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, 800, 0, 1217, False, False);
	    MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, 800, 0, 1217, False, False);
	    MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 800, 0, 1217, False, False);
        sendMotorTim_Once(MOTOR_ALL_ADDR);
        break;
    }
}


void Clockwise_90Angle(MOTOR_TYPE_ENUM Motor_ctrl){
    switch (Motor_ctrl){
    case UART_CTRL:
    //1211
        MotorUartCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 0x40, 0x60, 1211 + 3, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, 0x40, 0x60, 1211 + 3, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, 0x40, 0x60, 1211 + 3, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, 0x40, 0x60, 1211 + 3, REL_FLAG, False);
	    sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
        break;
    case TIM_CTRL:
    //1211
        MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 800, 0, 1210 + 4, False, False);
	    MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, 800, 0, 1210 + 4, False, False);
	    MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, 800, 0, 1210 + 4, False, False);
	    MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, 800, 0, 1210 + 4, False, False);
        sendMotorTim_Once(MOTOR_ALL_ADDR);
        break;
    }
}

void AntiClockwise_180Angle(MOTOR_TYPE_ENUM Motor_ctrl){
    switch (Motor_ctrl){
    case UART_CTRL:
    //1214
        MotorUartCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, 0x40, 0x60, (1214+3)*2, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, 0x40, 0x60, (1214+3)*2, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, 0x40, 0x60, (1214+3)*2, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 0x40, 0x60, (1214+3)*2, REL_FLAG, False);
	    sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
        break;
    case TIM_CTRL:
    //1214
        MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_REVERSE, 800, 0, (1214+3)*2+1, False, False);
	    MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_REVERSE, 800, 0, (1214+3)*2+1, False, False);
	    MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_FORWARD, 800, 0, (1214+3)*2+1, False, False);
	    MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_FORWARD, 800, 0, (1214+3)*2+1, False, False);
        sendMotorTim_Once(MOTOR_ALL_ADDR);
        break;
    }
}


void Clockwise_180Angle(MOTOR_TYPE_ENUM Motor_ctrl){
    switch (Motor_ctrl){
    case UART_CTRL:
    //1213
        MotorUartCtrl(MOTOR_LF_ADDR,  MOTOR_FORWARD, 0x40, 0x60, (1211+3)*2+1, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_LR_ADDR,  MOTOR_FORWARD, 0x40, 0x60, (1211+3)*2+1, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RF_ADDR,  MOTOR_REVERSE, 0x40, 0x60, (1211+3)*2+1, REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RR_ADDR,  MOTOR_REVERSE, 0x40, 0x60, (1211+3)*2+1, REL_FLAG, False);
	    sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
        break;
    case TIM_CTRL:
        MotorTIMCtrl(MOTOR_LF_ADDR, MOTOR_FORWARD, 800, 0, (1211+3)*2-1, False, False);
	    MotorTIMCtrl(MOTOR_LR_ADDR, MOTOR_FORWARD, 800, 0, (1211+3)*2-1, False, False);
	    MotorTIMCtrl(MOTOR_RF_ADDR, MOTOR_REVERSE, 800, 0, (1211+3)*2-1, False, False);
	    MotorTIMCtrl(MOTOR_RR_ADDR, MOTOR_REVERSE, 800, 0, (1211+3)*2-1, False, False);
        sendMotorTim_Once(MOTOR_ALL_ADDR);
        break;
    }
}

void pid_init(void){
     tmp_pid = create_PosPIDStructure(1);
     xy_pid  = create_PosPIDStructure(2);
}

void MoveInLine_PID(double vel, double distance, bool isOSTime){
    ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
    uint32_t loop_i = (uint32_t)(distance / 0.05);
    double  re    = (uint32_t)(distance -  loop_i * 0.05);
    for(int i = 0;i <=loop_i; i++){
		double yaw = HWT101_Struct.YawAngle;
		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
		if(_abs(tmp_dw) > 200) tmp_dw = 200 *_sign(tmp_dw);
        //if(i == loop_i && vel < 0) Kinematic_Analysis_Pos(vel-0.6, 0, 0, (re+0.05)*_sign_f(vel), 0,0);
		if(i == loop_i) Kinematic_Analysis_Pos(vel, 0, 0, re*_sign_f(vel), 0,0);
        // else if (i % 2 == 0 && vel>0) Kinematic_Analysis_Pos(vel, 0.0075 ,-tmp_dw /100.0, 0.4*_sign_f(vel), 0.1, 0.1*_sign_f(-tmp_dw));
        // else if (i % 2 == 0 && vel<0) Kinematic_Analysis_Pos(vel, 0.0075 ,-tmp_dw /100.0, 0.4*_sign_f(vel), 0.1, 0.1*_sign_f(-tmp_dw));
        else Kinematic_Analysis_Pos(vel, 0, -tmp_dw /100.0, 0.4*_sign_f(vel), 0, 0.1*_sign_f(-tmp_dw));
		MotorTIMCtrl(MOTOR_LF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[0])+1)/2), (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, False);
		MotorTIMCtrl(MOTOR_LR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[1])+1)/2), (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, False);
		MotorTIMCtrl(MOTOR_RF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[2])+1)/2), (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, False);
		MotorTIMCtrl(MOTOR_RR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[3])+1)/2), (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, False);
		Fun_En_DMA_Motor(MOTOR_ALL_ADDR);
        if(isOSTime)    vTaskDelay((uint32_t)(1000*0.05/_abs_f(vel)));
        else            delay_xms((uint32_t)(1000*0.05/_abs_f(vel)));    
	}
    stop_all_motor();
}

void TranslationMove(MOTOR_TYPE_ENUM Motor_ctrl, double V, double Dx, double Dy, bool isOSTime){
    V = _abs_f(V);
    double tmp_sqrt = sqrt(Dx*Dx + Dy*Dy);
    double Vx = V * Dx / tmp_sqrt;
    double Vy = V * Dy / tmp_sqrt;
    Kinematic_Analysis_Pos(Vx, Vy, 0, Dx, Dy, 0);

	switch (Motor_ctrl){
    case UART_CTRL:
        MotorUartCtrl(MOTOR_LF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[0])+1)/2), (uint32_t)(vel_weel[0]*60), 0x50, (uint32_t)(800*_abs_f(pos_weel[0])), REL_FLAG, False);
	    MotorUartCtrl(MOTOR_LR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[1])+1)/2), (uint32_t)(vel_weel[1]*60), 0x50, (uint32_t)(800*_abs_f(pos_weel[1])), REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[2])+1)/2), (uint32_t)(vel_weel[2]*60), 0x50, (uint32_t)(800*_abs_f(pos_weel[2])), REL_FLAG, False);
	    MotorUartCtrl(MOTOR_RR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[3])+1)/2), (uint32_t)(vel_weel[3]*60), 0x50, (uint32_t)(800*_abs_f(pos_weel[3])), REL_FLAG, False);
	    sendMotorUart_Once(MOTOR_ALL_ADDR, STEPS_UART_BUFFER_LENTH);
        if(isOSTime) vTaskDelay((uint32_t)(1200*tmp_sqrt/V));
        else         delay_xms((uint32_t)(1200*tmp_sqrt/V));
        break;
    case TIM_CTRL:
        MotorTIMCtrl(MOTOR_LF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[0])+1)/2), (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, False);
		MotorTIMCtrl(MOTOR_LR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[1])+1)/2), (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, False);
		MotorTIMCtrl(MOTOR_RF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[2])+1)/2), (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, False);
		MotorTIMCtrl(MOTOR_RR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[3])+1)/2), (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, False);
        sendMotorTim_Once(MOTOR_ALL_ADDR);
        if(isOSTime) vTaskDelay((uint32_t)(1000*tmp_sqrt/V));
        else         delay_xms((uint32_t)(1000*tmp_sqrt/V));
        stop_all_motor();
        break;
    }
}


void TranslationMove_PID(double V, double Dx, double Dy, bool isOSTime){
    
    V = _abs_f(V);
    if(V < esp) return;
    double Ds = sqrt(Dx*Dx + Dy*Dy);
    double Vx = V * Dx / Ds;
    double Vy = V * Dy / Ds;

    ((pStruct_PID)tmp_pid[0])->setPar(tmp_pid[0], 5, 0, 0);
    uint32_t loop_i = (uint32_t)(Ds / 0.05);
    double  re_x    = (uint32_t)(Dx -  loop_i * 0.05 * Dx / Ds);
    double  re_y    = (uint32_t)(Dy -  loop_i * 0.05 * Dy / Ds);
    for(uint32_t i = 0;i <=loop_i; i++){
		double yaw = HWT101_Struct.YawAngle;
		int tmp_dw = tmp_pid[0]->run(tmp_pid[0], 0, yaw);		
		if(_abs(tmp_dw) > 200) tmp_dw = 200 *_sign(tmp_dw);
		if(i == loop_i) Kinematic_Analysis_Pos(Vx, Vy, 0, re_x, re_y, 0);
		else Kinematic_Analysis_Pos(Vx, Vy, -tmp_dw /100.0, 0.8*_sign_f(Dx), 0.8*_sign_f(Dy), 0.2*_sign_f(-tmp_dw));

		MotorTIMCtrl(MOTOR_LF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[0])+1)/2), (uint32_t)(800*_abs_f(vel_weel[0])), 0, (uint32_t)(800*_abs_f(pos_weel[0])), False, False);
		MotorTIMCtrl(MOTOR_LR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[1])+1)/2), (uint32_t)(800*_abs_f(vel_weel[1])), 0, (uint32_t)(800*_abs_f(pos_weel[1])), False, False);
		MotorTIMCtrl(MOTOR_RF_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[2])+1)/2), (uint32_t)(800*_abs_f(vel_weel[2])), 0, (uint32_t)(800*_abs_f(pos_weel[2])), False, False);
		MotorTIMCtrl(MOTOR_RR_ADDR, (MOTOR_DIR_ENUM)((_sign_f(vel_weel[3])+1)/2), (uint32_t)(800*_abs_f(vel_weel[3])), 0, (uint32_t)(800*_abs_f(pos_weel[3])), False, False);
		Fun_En_DMA_Motor(MOTOR_ALL_ADDR);
        if(isOSTime)    vTaskDelay((uint32_t)(1000*0.05/_abs_f(V)));
        else            delay_xms((uint32_t)(1000*0.05/_abs_f(V)));    
	}
    stop_all_motor();
}

void AdjustXYPostion(int t_x, int t_y, bool isOSTime){
    ((pStruct_PID)xy_pid[0])->setPar(xy_pid[0], 5, 0, 0);
    ((pStruct_PID)xy_pid[1])->setPar(xy_pid[0], 5, 0, 0);
    int c_x =0, c_y=0 ;
    int tmp_i = 30;
    while (tmp_i--){
        while (BottomData.sta_xy !=1){
            inqCurXYPos();
            if(isOSTime)    vTaskDelay(50);
            else            delay_xms(50);    
        }
        BottomData.sta_xy = 0;
        c_x = BottomData.px;
        c_y = BottomData.px;
        if(_abs(c_x - t_x) <= 1 && _abs(t_y - t_y) <= 1)
            break;
        int dx = xy_pid[0]->run(xy_pid[0], c_x, t_x);
        int dy = xy_pid[0]->run(xy_pid[0], c_y, t_y);	
	    if(_abs(dx) > 400) dx = 400 *_sign(dx);
        if(_abs(dy) > 400) dy = 400 *_sign(dy);
        TranslationMove(TIM_CTRL, 0.5, dx, dy, isOSTime);
    }
    stop_all_motor();
}

#endif //STEPPER_MOTOR_DRIVER
#endif //BOTTOM_LEVEL
