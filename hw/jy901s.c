#define _JY901S_C_
#include"jy901s.h"
#ifdef BOTTOM_LEVEL
#include"string.h"


IMU_MSG_Sturct GY901S_Struct={0};

void IMU_uart_init(uint32_t bound){
 
	IMU_RX_FUN_GPIO_CLK(ENABLE);
	IMU_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(IMU_RX_GPIO,IMU_RX_GPIO_PINSOURCE, IMU_GPIO_AF_UART); 
	GPIO_PinAFConfig(IMU_TX_GPIO,IMU_TX_GPIO_PINSOURCE, IMU_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = IMU_RX_GPIO_PIN; 
	GPIO_Init(IMU_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = IMU_TX_GPIO_PIN; 
	GPIO_Init(IMU_TX_GPIO,&GPIO_InitStructure); 
	
	IMU_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(IMU_UART, &USART_InitStructure); 
    USART_Cmd(IMU_UART, ENABLE);

    USART_ITConfig(IMU_UART, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = IMU_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}


void IMU_IRQHandler(void){
	if(USART_GetITStatus(IMU_UART, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(IMU_UART,USART_IT_RXNE);
		if(GY901S_Struct.GetITSta != 0) return;
		char Res = IMU_UART->DR;//USART_ReceiveData(IMU_UART);//读取接收到的数据
		if(Res == 0x0D){
			if(GY901S_Struct.MSG_CNT == IMU_MSG_LEN){
				//接收成功
				GY901S_Struct.MSG_Buff[GY901S_Struct.MSG_CNT] = '\0';
				GY901S_Struct.GetITSta = 1;
			}else{
				//接收失败
				GY901S_Struct.GetITSta = -1;
				GY901S_Struct.MSG_CNT  = 0;
				memset(GY901S_Struct.MSG_Buff, 0, sizeof(GY901S_Struct.MSG_Buff));
			}	
		}else if(GY901S_Struct.MSG_CNT > IMU_MSG_LEN){
			//接收失败
				GY901S_Struct.GetITSta = -1;
				GY901S_Struct.MSG_CNT  = 0;
				memset(GY901S_Struct.MSG_Buff, 0, sizeof(GY901S_Struct.MSG_Buff));
		}else{
			GY901S_Struct.MSG_Buff[GY901S_Struct.MSG_CNT++] = Res;
		}	 				 
  	} 
}

#endif //BOTTOM