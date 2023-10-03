#include"comm_uart.h"
#include"string.h"

Comm_Uart_MSG_Sturct Comm_Uart_Sturct = {0};

void Comm_uart_init(uint32_t bound){
 
	COMM_RX_FUN_GPIO_CLK(ENABLE);
	COMM_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(COMM_RX_GPIO,COMM_RX_GPIO_PINSOURCE, COMM_GPIO_AF_UART); 
	GPIO_PinAFConfig(COMM_TX_GPIO,COMM_TX_GPIO_PINSOURCE, COMM_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = COMM_RX_GPIO_PIN; 
	GPIO_Init(COMM_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = COMM_TX_GPIO_PIN; 
	GPIO_Init(COMM_TX_GPIO,&GPIO_InitStructure); 
	
	COMM_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(COMM_UART, &USART_InitStructure); 
    USART_Cmd(COMM_UART, ENABLE);
	
    USART_ITConfig(COMM_UART, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = COMM_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}

//有效数据8byte
// #define COMM_MSG_LEN    10
// #define COMM_uSOH       0x66
// #define COMM_uEOT       0x88

//方案1
/**
 * 1.      方位   0x0 ~ 0xA     方位图     0x9逆时针  0xA顺时针
 *                              1 2 3
 *                              4 0 5
 *                              6 7 8
 *2.      速度   0x000   -   0xFFFF
 * 3.    距离/角度  0x000 - 0xFFFF   /  0   -   90  
 */

//方案2
/**
 * 0.       命令   0x01  //单纯运动控制   0x02  任务调用
 *
 * 1.      方位   0x0 ~ 0xA     方位图     0x9逆时针  0xA顺时针
 *                              1 2 3
 *                              4 0 5
 *                              6 7 8
 *2.      速度   0x000   -   0xFFFF
 *3.      距离/角度  0x000 - 0xFFFF   /  0   -   90  

任务
 * 1.  任务1：  0x01  :出库
 *     任务2：  0x02  :跑到二维码位置
 *     任务3:   0x03  :从二维码区域跑到转盘位置
 *     任务4:   0x04  :从转盘位置跑到粗加工区域最前端
 *     任务5:   0x05  :
 * 
 * 
 * 
 * 
 */


/**
 * 底板方案1
 * 底板只管运动控制, 顶板负责任务安排，数据处理
 * 底板方案2
 * 底板负责有关运动安排任务
 */


void COMM_IRQHandler(void){
	if(USART_GetITStatus(COMM_UART, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(COMM_UART,USART_IT_RXNE);
		char Res = COMM_UART->DR;//USART_ReceiveData(COMM_UART);//读取接收到的数据
        
        if(Comm_Uart_Sturct.MSG_CNT == COMM_MSG_LEN){
            if( Comm_Uart_Sturct.MSG_Buff[0] == 0x66 && Comm_Uart_Sturct.MSG_Buff[9] == 0x88){
                    //接收成功
                    Comm_Uart_Sturct.GetITSta = 1;
            } 
        }else if(Comm_Uart_Sturct.MSG_CNT > COMM_MSG_LEN){
            //接收失败
             Comm_Uart_Sturct.GetITSta = -1;
        }
        else{
            Comm_Uart_Sturct.MSG_Buff[Comm_Uart_Sturct.MSG_CNT++] = Res;
            Comm_Uart_Sturct.GetITSta = 0;
        }
  	} 
}
