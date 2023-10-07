#include "CarTaskConfig.h"
#include "string.h"

#ifdef TOP_LEVEL
TopDataType TopData = {0};

void uart1_init(uint32_t bound){
  
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_USART1); 
	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
 
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(USART1, &USART_InitStructure); 
    USART_Cmd(USART1, ENABLE);
	
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}
 

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        TopData.RxBuff[TopData.RxCnt++] = USART1->DR;
        if((TopData.RxBuff[0] == 0x66)&&(TopData.RxBuff[7] == 0x88))
        {
            TopData.TaskNum   = TopData.RxBuff[1];
            TopData.TaskState = TopData.RxBuff[2];
        }
        if (TopData.RxCnt == 8)     //arr full
        {
            TopData.RxCnt = 0;
            memset(TopData.RxBuff,0,sizeof(TopData.RxBuff));
        }
        
	}
}

#endif // TOP_LEVEL

#ifdef BOTTOM_LEVEL

BottomDataType BottomData = {0};

void uart6_init(uint32_t bound){
  
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6, GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); 
	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
 
	USART_InitStructure.USART_BaudRate   = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(USART6, &USART_InitStructure); 
    USART_Cmd(USART6, ENABLE);
	
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}
void USART6_IRQHandler(void){
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(USART6,USART_IT_RXNE);
        BottomData.RxBuff[BottomData.RxCnt++] = USART6->DR;
        if((BottomData.RxBuff[0] == 0x66)&&(BottomData.RxBuff[7] == 0x88))
        {
            BottomData.TaskNum   = BottomData.RxBuff[1];
            BottomData.TaskState = BottomData.RxBuff[2];
            BottomData.X_Lo      = BottomData.RxBuff[3];
            BottomData.Y_Lo      = BottomData.RxBuff[4];
        }
        if (BottomData.RxCnt == 8)     //arr full
        {
            BottomData.RxCnt = 0;
            memset(BottomData.RxBuff,0,sizeof(BottomData.RxBuff));
        }
	}
}


#endif // BOTTOM_LEVEL

