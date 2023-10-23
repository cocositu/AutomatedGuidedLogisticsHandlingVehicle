#include"QRcode.h"
#ifdef TOP_LEVEL
#include"lcd.h"
#include"string.h"
#define QRCODE_MSG_LEN	7
QRCode_MSG_Sturct QRCode={0};

void QR_uart_init(uint32_t bound){
 
	QRCODE_RX_FUN_GPIO_CLK(ENABLE);
	QRCODE_TX_FUN_GPIO_CLK(ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinAFConfig(QRCODE_RX_GPIO,QRCODE_RX_GPIO_PINSOURCE, QRCODE_GPIO_AF_UART); 
	GPIO_PinAFConfig(QRCODE_TX_GPIO,QRCODE_TX_GPIO_PINSOURCE, QRCODE_GPIO_AF_UART);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin   = QRCODE_RX_GPIO_PIN; 
	GPIO_Init(QRCODE_RX_GPIO,&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin   = QRCODE_TX_GPIO_PIN; 
	GPIO_Init(QRCODE_TX_GPIO,&GPIO_InitStructure); 
	
	QRCODE_FUN_UART_CLK(ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(QRCODE_UART, &USART_InitStructure); 
    USART_Cmd(QRCODE_UART, ENABLE);

    USART_ITConfig(QRCODE_UART, USART_IT_RXNE, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = QRCODE_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}


void QRCODE_IRQHandler(void){
	if(USART_GetITStatus(QRCODE_UART, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(QRCODE_UART,USART_IT_RXNE);
		if(QRCode.GetITSta != 0) return;
		char Res = QRCODE_UART->DR;//USART_ReceiveData(QRCODE_UART);//读取接收到的数据
		if(Res == 0x0D){
			if(QRCode.MSG_CNT == QRCODE_MSG_LEN){
				//接收成功
				QRCode.MSG_Buff[QRCode.MSG_CNT] = '\0';
				QRCode.GetITSta = 1;
			}else{
				//接收失败
				QRCode.GetITSta = -1;
				QRCode.MSG_CNT  = 0;
				memset(QRCode.MSG_Buff, 0, sizeof(QRCode.MSG_Buff));
			}	
		}else if(QRCode.MSG_CNT > QRCODE_MSG_LEN){
			//接收失败
				QRCode.GetITSta = -1;
				QRCode.MSG_CNT  = 0;
				memset(QRCode.MSG_Buff, 0, sizeof(QRCode.MSG_Buff));
		}else{
			QRCode.MSG_Buff[QRCode.MSG_CNT++] = Res;
		}	 				 
  	} 
}

void showQRCodeMessage(char QRcode[]){
	int med_lcd  = 76;//240/2 - 88/2;
	int septal = 88/2;
	int init_compen = 6; 
	uint16_t FRONT_COLOR = WHITE;
	if(QRcode[7] != '\0' && QRcode[3] == '+'){
		return;
	}
	for (uint8_t i = 0; i < 7; ++i){
		switch (QRcode[i]){
		case '1':
			FRONT_COLOR = RED;
			break;
		case '2':
			FRONT_COLOR = GREEN;
			break;
		case '3':
			FRONT_COLOR = BLUE;
			break;
		default:
			FRONT_COLOR = WHITE;
		}
		LCD_ShowChar(init_compen+i*septal, med_lcd, QRcode[i] , FRONT_COLOR, BLACK, 88, 0);
	}
	memset(QRCode.MSG_Buff, 0, sizeof(QRCode.MSG_Buff));
	QRCode.GetITSta = 0;
	QRCode.MSG_CNT  = 0;
}

#endif //TOP_LEVEL