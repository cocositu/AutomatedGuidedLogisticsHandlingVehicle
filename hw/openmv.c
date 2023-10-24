#include"openmv.h"
#include "string.h"
#include"lcd.h"
#ifdef TOP_LEVEL

OV_MSG_Sturct OV_Struct = {0};

void ToOpenMV_uart_init(uint32_t bound){
 
	ToOPENMV_UART_RX_FUN_GPIO_CLK(ENABLE);
	ToOPENMV_UART_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(ToOPENMV_UART_RX_GPIO,ToOPENMV_UART_RX_GPIO_PINSOURCE, ToOPENMV_UART_GPIO_AF_UART); 
	GPIO_PinAFConfig(ToOPENMV_UART_TX_GPIO,ToOPENMV_UART_TX_GPIO_PINSOURCE, ToOPENMV_UART_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = ToOPENMV_UART_RX_GPIO_PIN; 
	GPIO_Init(ToOPENMV_UART_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = ToOPENMV_UART_TX_GPIO_PIN; 
	GPIO_Init(ToOPENMV_UART_TX_GPIO,&GPIO_InitStructure); 
	
	ToOPENMV_UART_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(ToOPENMV_UART, &USART_InitStructure); 

	USART_ITConfig(ToOPENMV_UART, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = ToOPENMV_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    USART_Cmd(ToOPENMV_UART, ENABLE);
}

void OV_SendData(char DataByte){
	OV_Struct.TaskNum = DataByte;
	USART_ClearFlag(ToOPENMV_UART, USART_FLAG_TC);	
	USART_SendData(ToOPENMV_UART, DataByte);
    while(USART_GetFlagStatus(ToOPENMV_UART, USART_FLAG_TC) == 0);
}											

void ToOPENMV_UART_IRQHandler(void){
	
	if(USART_GetITStatus(ToOPENMV_UART, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(ToOPENMV_UART,USART_IT_RXNE);
		OV_Struct.RxBuff[OV_Struct.RxCnt++] = ToOPENMV_UART->DR;
		if ((OV_Struct.RxBuff[0] == '[')&&(OV_Struct.RxBuff[9] == ']')){
			if(OV_Struct.RxBuff[1] == OV_Struct.TaskNum + '0')	//任务匹配（可不要）相等前提是先给了OpenMV数据
			{
				if((OV_Struct.TaskNum == OV_RED_COLOR)||(OV_Struct.TaskNum == OV_GREEN_COLOR)||(OV_Struct.TaskNum == OV_BLUE_COLOR)){
					OV_Struct.TaskState = OV_Struct.RxBuff[2];	//2~7数据相同 判断是否抓取物块 'y' 如果是y则开始抓取
				}
				else if((OV_Struct.TaskNum == OV_RED_CIRCL)||(OV_Struct.TaskNum == OV_GREEN_CIRCL)||(OV_Struct.TaskNum == OV_BLUE_CIRCL)){
					// OV_Struct.Px = (OV_Struct.RxBuff[2]-'0')*100 + (OV_Struct.RxBuff[3]-'0')*10 + (OV_Struct.RxBuff[4]-'0');
					// OV_Struct.Px = (OV_Struct.RxBuff[5]-'0')*100 + (OV_Struct.RxBuff[6]-'0')*10 + (OV_Struct.RxBuff[7]-'0');
					OV_Struct.xy_sta = 1;
					OV_Struct.sPx[0] = OV_Struct.RxBuff[2];
					OV_Struct.sPx[1] = OV_Struct.RxBuff[3];
					OV_Struct.sPx[2] = OV_Struct.RxBuff[4];
					OV_Struct.sPy[0] = OV_Struct.RxBuff[5];
					OV_Struct.sPy[1] = OV_Struct.RxBuff[6];
					OV_Struct.sPy[2] = OV_Struct.RxBuff[7];
				}else if(OV_Struct.TaskNum == OV_CIRCL_COLOR){
					OV_Struct.circleColor = OV_Struct.RxBuff[2]-'0'; //1,2,3
				}
			}
		}
		if (OV_Struct.RxCnt == 10){
			OV_Struct.RxCnt = 0;
			memset(OV_Struct.RxBuff,0,sizeof(OV_Struct.RxBuff));
		}
  	} 
}


#endif