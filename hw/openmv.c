#include"openmv.h"
#include "string.h"
#include"lcd.h"
#include"FreeRTOS.h"
#include"task.h"
#ifdef TOP_LEVEL

OV_MSG_Sturct OV_Struct = {0};

void Fill_inLight_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能PORTA~E,PORTG时钟
  	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
} 

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

	DMA_InitTypeDef DMA_InitStructure;
    ToOPENMV_UART_TX_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(ToOPENMV_UART_TX_DMA_STREAM); 
    DMA_InitStructure.DMA_Channel            = ToOPENMV_UART_TX_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ToOPENMV_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)OV_Struct.TxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = 1;
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
    DMA_Init(ToOPENMV_UART_TX_DMA_STREAM, &DMA_InitStructure);
   
   	USART_DMACmd(ToOPENMV_UART, USART_DMAReq_Tx, ENABLE);  		
    DMA_Cmd(ToOPENMV_UART_TX_DMA_STREAM, DISABLE);
    USART_Cmd(ToOPENMV_UART, ENABLE);
}

void OV_SendData(char DataByte){
	OV_Struct.TaskNum = OV_Struct.TxBuff[0] = DataByte;
	// USART_ClearFlag(ToOPENMV_UART, USART_FLAG_TC);	
	// USART_SendData(ToOPENMV_UART, DataByte);
    // while(USART_GetFlagStatus(ToOPENMV_UART, USART_FLAG_TC) == 0);
    ToOPENMV_UART_TX_DMA_STREAM->CR  &= ~DMA_SxCR_EN; // 清除EN位
    ToOPENMV_UART_TX_DMA_STREAM->NDTR =1;
    ToOPENMV_UART_TX_DMA_STREAM->M0AR = (uint32_t)OV_Struct.TxBuff; 
    DMA_ClearFlag(ToOPENMV_UART_TX_DMA_STREAM, DMA_FLAG_TCIF4);
    ToOPENMV_UART_TX_DMA_STREAM->CR  |= DMA_SxCR_EN; // 设置EN位  
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