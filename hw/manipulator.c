#include"manipulator.h"
#ifdef TOP_LEVEL

#include"LobotServoController.h"

char MAN_SEP_MSG_Buff[MAN_SEP_MSG_LEN]      = {0};
char MAN_SERVO_MSG_Buff[MAN_SERVO_MSG_LEN]  = {0};

void MAN_SEP_UART_Init(uint32_t bound){

	MAN_SEP_UART_RX_FUN_GPIO_CLK(ENABLE);
	MAN_SEP_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(MAN_SEP_UART_RX_GPIO, MAN_SEP_UART_RX_GPIO_PINSOURCE,  MAN_SEP_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(MAN_SEP_UART_TX_GPIO, MAN_SEP_UART_TX_GPIO_PINSOURCE,  MAN_SEP_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   =  MAN_SEP_UART_RX_GPIO_PIN; 
	GPIO_Init(MAN_SEP_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   =  MAN_SEP_UART_TX_GPIO_PIN; 
	GPIO_Init(MAN_SEP_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MAN_SEP_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init( MAN_SEP_UART, &USART_InitStructure); 

    MAN_SEP_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit( MAN_SEP_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            =  MAN_SEP_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)& MAN_SEP_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MAN_SEP_MSG_Buff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = MAN_SEP_MSG_LEN;
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
    DMA_Init(MAN_SEP_UART_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(MAN_SEP_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MAN_SEP_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MAN_SEP_UART, ENABLE);
}


void  MAN_SERVO_UART_Init(uint32_t bound){

	MAN_SERVO_UART_RX_FUN_GPIO_CLK(ENABLE);
	MAN_SERVO_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig( MAN_SERVO_UART_RX_GPIO, MAN_SERVO_UART_RX_GPIO_PINSOURCE,  MAN_SERVO_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig( MAN_SERVO_UART_TX_GPIO, MAN_SERVO_UART_TX_GPIO_PINSOURCE,  MAN_SERVO_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   =  MAN_SERVO_UART_RX_GPIO_PIN; 
	GPIO_Init( MAN_SERVO_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   =  MAN_SERVO_UART_TX_GPIO_PIN; 
	GPIO_Init( MAN_SERVO_UART_TX_GPIO,&GPIO_InitStructure); 
	
	MAN_SERVO_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init( MAN_SERVO_UART, &USART_InitStructure); 

    MAN_SERVO_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit( MAN_SERVO_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            =  MAN_SERVO_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)& MAN_SERVO_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)MAN_SERVO_MSG_Buff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = MAN_SERVO_MSG_LEN;
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
    DMA_Init(MAN_SERVO_UART_DMA_STREAM, &DMA_InitStructure);

    USART_DMACmd(MAN_SERVO_UART, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(MAN_SERVO_UART_DMA_STREAM, DISABLE);
    USART_Cmd(MAN_SERVO_UART, ENABLE);
}

void ManStepUartCtrl(uint8_t Motor_dir, uint16_t Motor_vel, uint8_t Motor_acc, \
                     uint32_t Motor_clk, uint8_t Abs_or_Rel_Flag){  
   
    MAN_SEP_MSG_Buff[0]  = 0x05;
    MAN_SEP_MSG_Buff[1]  = 0xFD;
    MAN_SEP_MSG_Buff[2]  = Motor_dir;
    MAN_SEP_MSG_Buff[3]  = (uint8_t)(Motor_vel>>8);
    MAN_SEP_MSG_Buff[4]  = (uint8_t)(Motor_vel>>0);
    MAN_SEP_MSG_Buff[5]  = Motor_acc;
    MAN_SEP_MSG_Buff[6]  = (uint8_t)(Motor_clk>>24);
    MAN_SEP_MSG_Buff[7]  = (uint8_t)(Motor_clk>>16);
    MAN_SEP_MSG_Buff[8]  = (uint8_t)(Motor_clk>>8);
    MAN_SEP_MSG_Buff[9]  = (uint8_t)(Motor_clk>>0);
    MAN_SEP_MSG_Buff[10] = (uint8_t)(Abs_or_Rel_Flag);
    MAN_SEP_MSG_Buff[11] = (uint8_t)0x00;
    MAN_SEP_MSG_Buff[12] = (uint8_t)0x6B;
   
    MAN_SEP_UART_DMA_STREAM->NDTR = MAN_SEP_MSG_LEN;  
    DMA_ClearFlag(MAN_SEP_UART_DMA_STREAM, MAN_SEP_UART_DMA_FLAG_TCIF );	
    DMA_Cmd(MAN_SEP_UART_DMA_STREAM, ENABLE);
}


void ManServoSend(uint8_t *buf, uint8_t len){
	MAN_SERVO_UART_DMA_STREAM->CR &= ~DMA_SxCR_EN; // 清除EN位
    MAN_SERVO_UART_DMA_STREAM->NDTR = len;
    MAN_SERVO_UART_DMA_STREAM->M0AR = (uint32_t)buf; 
    DMA_ClearFlag(MAN_SERVO_UART_DMA_STREAM, MAN_SERVO_UART_DMA_FLAG_TCIF);
    MAN_SERVO_UART_DMA_STREAM->CR |= DMA_SxCR_EN; // 设置EN位  
}

#endif //TOP_LEVEL