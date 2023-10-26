#include"manipulator.h"
#ifdef TOP_LEVEL

#include"LobotServoController.h"
#include"sys.h"
#include"FreeRTOS.h"
#include"task.h"
#define istime(ti, isOstime)  (isOstime? vTaskDelay(ti):delay_xms(ti))

char MAN_SEP_MSG_Buff[MAN_SEP_MSG_LEN]      = {0};
char MAN_SERVO_MSG_Buff[MAN_SERVO_MSG_LEN]  = {0};

void MAN_STEP_UART_Init(uint32_t bound){

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
    DMA_ClearFlag(MAN_SEP_UART_DMA_STREAM, DMA_FLAG_TCIF6); 
    DMA_Cmd(MAN_SEP_UART_DMA_STREAM, ENABLE);
}

void ManServoSend(uint8_t *buf, uint8_t len){
    MAN_SERVO_UART_DMA_STREAM->CR &= ~DMA_SxCR_EN; // 清除EN位
    MAN_SERVO_UART_DMA_STREAM->NDTR = len;
    MAN_SERVO_UART_DMA_STREAM->M0AR = (uint32_t)buf; 
    DMA_ClearFlag(MAN_SERVO_UART_DMA_STREAM, MAN_SERVO_UART_DMA_FLAG_TCIF);
    MAN_SERVO_UART_DMA_STREAM->CR |= DMA_SxCR_EN; // 设置EN位  
}


void init_MechArm_and_MateTray(bool isOSTime){
    moveServo(1, Claw_p3, 500);
    istime(500, isOSTime);
    moveServo(2, CP_p1, 500);
    istime(500, isOSTime);
    moveServo(3, MT_p2, 500);
    istime(500, isOSTime);
}

//materiel tray
void GrabMate_to_MT(uint8_t x, bool isOSTime){
    alone_Rotate_MateTray(x,isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 1320, 1);//物料在转盘抓取
    istime(500, isOSTime);
    moveServo(1, Claw_p1, 150);
    istime(170, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//升到最上方
    istime(50, isOSTime);
    moveServo(2, CP_p2, 800);//放物料盘的位置//“延时不要改！！！”
    istime(820, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 700, 1);//物料放盘
    istime(300, isOSTime);
    moveServo(1, Claw_p2, 200);//物料松开
    istime(220, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//升到最上方
    istime(100, isOSTime);
    moveServo(1, Claw_p3, 300); //爪子张开
    istime(300, isOSTime);
    moveServo(2, CP_p1, 500);//回到识别转盘位置
    istime(520, isOSTime);
}


//ground
void GrabMate_to_G(uint8_t x, bool isOSTime){
    alone_Rotate_MateTray(x,isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 2950, 1);//从地面抓取
    istime(800, isOSTime);
    moveServo(1, Claw_p1, 300);//
    istime(310, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//升到最上方
    istime(800, isOSTime);
    moveServo(2, CP_p2, 800);//放物料盘的位置//“延时不要改！！！”
    istime(820, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 700, 1);//物料放盘1
    istime(500, isOSTime);
    moveServo(1, Claw_p2, 300);//物料松开
    istime(320, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//升到最上方
    istime(500, isOSTime);
    moveServo(1, Claw_p3, 300); //爪子张开
    istime(320, isOSTime);
    moveServo(2, CP_p1, 500);//回到识别转盘位置
}
//物料从物料盘抓到地面  （物料盘抓到地面的时候）
void PutMate_to_G(uint8_t x, bool isOSTime){
    alone_Rotate_MateTray(x,isOSTime);
    istime(100, isOSTime);
    moveServo(1, Claw_p2, 300);//抓取位置
    istime(300, isOSTime);  
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//物料在转盘抓取
    moveServo(2, CP_p2, 800);//
    istime(820, isOSTime);
    // moveServo(1, Claw_p2, 300);//物料松开
    ManStepUartCtrl(0,0x400, 0xEE, 800, 1);//物料放盘
    istime(400, isOSTime);
    moveServo(1, Claw_p1, 200);//物料抓取
    istime(220, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);
    istime(500, isOSTime);
    moveServo(2, CP_p1, 800);//
    istime(820, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 3050, 1);//物料放地
    istime(1000, isOSTime);
    moveServo(1, Claw_p3, 200); //爪子张开
    istime(220, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);

}
//叠放第二层
void PutMate_to_M(uint8_t x, bool isOSTime){
     alone_Rotate_MateTray(x,isOSTime);
    istime(100, isOSTime);
    moveServo(1, Claw_p2, 300);//抓取位置
    istime(300, isOSTime);  
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);//物料在转盘抓取
    moveServo(2, CP_p2, 800);//
    istime(820, isOSTime);
    // moveServo(1, Claw_p2, 300);//物料松开
    ManStepUartCtrl(0,0x400, 0xEE, 800, 1);//物料放盘
    istime(400, isOSTime);
    moveServo(1, Claw_p1, 200);//物料抓取
    istime(220, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);
    istime(500, isOSTime);
    moveServo(2, CP_p1, 800);//
    istime(820, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 1630, 1);//物料放地
    istime(1000, isOSTime);
    moveServo(1, Claw_p3, 200); //爪子张开
    istime(220, isOSTime);
    ManStepUartCtrl(0,0x400, 0xEE, 0, 1);
}

void alone_Rotate_MateTray(uint8_t x, bool isOSTime){
    switch (x){
    case 1:
        moveServo(3, MT_p3, 500);//红
        break;
    case 2:
        moveServo(3, MT_p2, 500);//绿
        break;
    case 3:
        moveServo(3, MT_p1, 500);//蓝
        break;
    }
}

void identify_color_circle_postion(uint8_t dir)
{
    if(dir == 1)
    {
        ManStepUartCtrl(0,0x400, 0xEE, 2000, 1);//物料色环识别高度--15cm
    }
    else if(dir == 0)
    {
        ManStepUartCtrl(0,0x250, 0xEE, 0, 1);
    }
}



void move_MechArm_to_MTBottom(bool isOSTime){
    
}

#endif //TOP_LEVEL