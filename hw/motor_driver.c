#include"motor_driver.h"
#include"sys.h"
#ifdef BOTTOM_LEVEL
#ifdef STEPPER_MOTOR_DRIVER

StepBufferType StepMotorBuffer[4][STEPS_LOOP_MAXNUM+1];
StepBufferType *StepMotorLFBuffer = StepMotorBuffer[0];
StepBufferType *StepMotorLRBuffer = StepMotorBuffer[1];
StepBufferType *StepMotorRFBuffer = StepMotorBuffer[2];
StepBufferType *StepMotorRRBuffer = StepMotorBuffer[3];

MotorUartBufferType MotorUartBuffer[5][STEPS_UART_BUFFER_LENTH];
MotorUartBufferType *MotorLFUartBuffer = MotorUartBuffer[MOTOR_LF_ADDR];
MotorUartBufferType *MotorLRUartBuffer = MotorUartBuffer[MOTOR_LR_ADDR];
MotorUartBufferType *MotorRFUartBuffer = MotorUartBuffer[MOTOR_RF_ADDR];
MotorUartBufferType *MotorRRUartBuffer = MotorUartBuffer[MOTOR_RR_ADDR];

void MOTOR_LF_TIM_Init(void){  
    MOTOR_LF_FUN_GPIO_CLK(ENABLE);	
    GPIO_InitTypeDef  GPIO_InitStructure;
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
    TIM_TimeBaseStructure.TIM_Prescaler         = 84-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_LF_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 50;/*占空比*/
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
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03; //子优先级3
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Stream5_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ClearFlag(MOTOR_LF_DMA_STREAM, DMA_FLAG_TCIF5);
    DMA_ClearFlag(MOTOR_LF_DMA_STREAM, DMA_IT_TC);        //第一次不进中断
    DMA_ITConfig(MOTOR_LF_DMA_STREAM, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
    TIM_DMACmd(MOTOR_LF_TIM, TIM_DMA_Update, ENABLE);
    TIM_DMAConfig(MOTOR_LF_TIM, TIM_DMABase_ARR, TIM_DMABurstLength_1Transfer);
    DMA_Cmd(MOTOR_LF_DMA_STREAM, DISABLE);
    TIM_Cmd(MOTOR_LF_TIM, DISABLE); 
}

    // TIM_GenerateEvent(MOTOR_LF_TIM, TIM_EventSource_Update);
    //TIM_DMACmd(MOTOR_LF_TIM, MOTOR_LF_TIM_DMA_CC, ENABLE);

//DMA_ITConfig(MOTOR_LF_DMA_STREAM, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
//DMA_ClearFlag(MOTOR_LF_DMA_STREAM, DMA_FLAG_TCIF5);
// NVIC_InitTypeDef NVIC_InitStructure;
// NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
// NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0x03; //子优先级3
// NVIC_InitStructure.NVIC_IRQChannel                   = DMA1_Stream5_IRQn;
// NVIC_Init(&NVIC_InitStructure);

void MOTOR_LR_TIM_Init(void){
    MOTOR_LR_FUN_GPIO_CLK(ENABLE);	
    GPIO_InitTypeDef  GPIO_InitStructure;
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
    TIM_TimeBaseStructure.TIM_Prescaler         = 84-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_LR_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 100;/*占空比*/
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
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_LR_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_LR_TIM, MOTOR_LR_TIM_DMA_CC, ENABLE);
}

void MOTOR_RF_TIM_Init(void){
    MOTOR_RF_FUN_GPIO_CLK(ENABLE);
    GPIO_InitTypeDef  GPIO_InitStructure;
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
    TIM_TimeBaseStructure.TIM_Prescaler         = 168-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_RF_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 100;/*占空比*/
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
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RF_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_RF_TIM, MOTOR_RF_TIM_DMA_CC, ENABLE);
}

void MOTOR_RR_TIM_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
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
    TIM_TimeBaseStructure.TIM_Prescaler         = 168-1;
    TIM_TimeBaseStructure.TIM_Period            = 1000-1;
    TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_RR_TIM, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse        = 50;/*占空比*/
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    MOTOR_RR_FUN_TIM_OC_INIT(MOTOR_RR_TIM, &TIM_OCInitStructure);
    MOTOR_RR_FUN_TIM_OC_PRE(MOTOR_RR_TIM, TIM_OCPreload_Enable);
    // TIM_CtrlPWMOutputs(MOTOR_RR_TIM, ENABLE);

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
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(MOTOR_RR_DMA_STREAM, &DMA_InitStructure);
    TIM_DMACmd(MOTOR_RR_TIM, MOTOR_RR_TIM_DMA_CC, ENABLE);
}

void MOTOR_LF_DMA_Init(void){
}
void MOTOR_LR_DMA_Init(void){
}
void MOTOR_RF_DMA_Init(void){
}
void MOTOR_RR_DMA_Init(void){
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


void  MOTOR_LR_UART_Init(uint32_t bound){

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


/**
 * @brief  
 * 
 * @var absolutely relative
 */
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
    }
}

void MotorUartCtrl(                                                                    \
    MOTOR_UART_ADDR_ENUM Motor_addr, uint8_t Motor_dir, uint16_t Motor_vel,         \
    uint8_t Motor_acc, uint32_t Motor_clk, ABS_OR_REL_FLAG Abs_or_Rel_Flag, uint8_t isClog   \
){

    MotorUartBuffer[Motor_addr][0] = Motor_addr;
    MotorUartBuffer[Motor_addr][1] = 0xFD;
    MotorUartBuffer[Motor_addr][2] = Motor_dir;
    MotorUartBuffer[Motor_addr][3] = (uint8_t)(Motor_vel>>8);
    MotorUartBuffer[Motor_addr][4] = (uint8_t)(Motor_vel>>0);
    MotorUartBuffer[Motor_addr][5] = Motor_acc;
    MotorUartBuffer[Motor_addr][6] = (uint8_t)(Motor_clk>>24);
    MotorUartBuffer[Motor_addr][7] = (uint8_t)(Motor_clk>>16);
    MotorUartBuffer[Motor_addr][8] = (uint8_t)(Motor_clk>>8);
    MotorUartBuffer[Motor_addr][9] = (uint8_t)(Motor_clk>>0);
    MotorUartBuffer[Motor_addr][10] = (uint8_t)(Abs_or_Rel_Flag);
    MotorUartBuffer[Motor_addr][11] = (uint8_t)0x00;
    MotorUartBuffer[Motor_addr][12] = (uint8_t)0x6B;
    sendMotorUart_Once(Motor_addr, 13);
    if(isClog == True){
        delay_ms((uint32_t)(Motor_clk / Motor_vel) + 200);
    }
}

#endif //STEPPER_MOTOR_DRIVER
#endif //BOTTOM_LEVEL
