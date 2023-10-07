#include "CarTaskConfig.h"
#include "string.h"

#ifdef TOP_LEVEL
TopDataType TopData = {0};

void CarTopComUartInit(uint32_t bound){
  
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
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream7);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)TopData.TxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = CARTASK_MSG_LEN;
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
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);

    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA2_Stream7, DISABLE);
   
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
}

void CarTopSendData(uint8_t  DataBuff[]){
    DMA2_Stream7->CR  &= ~DMA_SxCR_EN; // 清除EN位
    DMA2_Stream7->NDTR = CARTASK_MSG_LEN;
    DMA2_Stream7->M0AR = (uint32_t)DataBuff; 
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
    DMA2_Stream7->CR  |= DMA_SxCR_EN; // 设置EN位  
}	 

void TopSendTask(uint8_t TaskID, uint8_t TaskState){
    memset(TopData.TxBuff, 0, sizeof(TopData.TxBuff));
    TopData.TxBuff[0] =  0x66;
    TopData.TxBuff[1] =  TaskID;
    TopData.TxBuff[2] =  TaskState;
    TopData.TxBuff[CARTASK_MSG_LEN-1] =  0x88;
    CarTopSendData(TopData.TxBuff);
}

//存进二维码信息
void TopSendQRMsg(char QR_MSG[]){
    TopData.TxBuff[3] =  QR_MSG[0] - '0';
    TopData.TxBuff[4] =  QR_MSG[1] - '0';
    TopData.TxBuff[5] =  QR_MSG[2] - '0';
    TopData.TxBuff[6] =  QR_MSG[4] - '0';
    TopData.TxBuff[7] =  QR_MSG[5] - '0';
    TopData.TxBuff[8] =  QR_MSG[6] - '0';
}

//存进微调XY信息
void  TopSendXYMsg(uint8_t X_MSG, uint8_t Y_MSG){
    TopData.TxBuff[3] =  X_MSG;
    TopData.TxBuff[4] =  Y_MSG;
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        TopData.RxBuff[TopData.RxCnt++] = USART1->DR;
        if((TopData.RxBuff[0] == 0x66)&&(TopData.RxBuff[CARTASK_MSG_LEN-1] == 0x88)){
            TopData.TaskNum   = TopData.RxBuff[1];
            TopData.TaskState = TopData.RxBuff[2];
        }
        if (TopData.RxCnt == 8){
            TopData.RxCnt = 0;
            memset(TopData.RxBuff,0,sizeof(TopData.RxBuff));
        }
	}
}

#endif // TOP_LEVEL

#ifdef BOTTOM_LEVEL

BottomDataType BottomData = {0};

void  CarBottomComUartInit(uint32_t bound){
  
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6, GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7, GPIO_AF_USART6); 
	
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
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream6);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)BottomData.TxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = CARTASK_MSG_LEN;
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
    DMA_Init(DMA2_Stream6, &DMA_InitStructure);

    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA2_Stream6, DISABLE);
    USART_Cmd(USART6, ENABLE);
}

 void CarBottomSendData(uint8_t   DataBuff[]){
    DMA2_Stream6->CR &= ~DMA_SxCR_EN; // 清除EN位
    DMA2_Stream6->NDTR = CARTASK_MSG_LEN;
    DMA2_Stream6->M0AR = (uint32_t)DataBuff; 
    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);
    DMA2_Stream6->CR |= DMA_SxCR_EN; // 设置EN位  
}	

void BottomReturnData(uint8_t TaskID, uint8_t TaskState){
    memset(BottomData.TxBuff, 0, sizeof(BottomData.TxBuff));
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  TaskID;
    BottomData.TxBuff[2] =  TaskState;
    BottomData.TxBuff[CARTASK_MSG_LEN-1] =  0x88;
    CarBottomSendData(BottomData.TxBuff);
}

void USART6_IRQHandler(void){
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(USART6,USART_IT_RXNE);

        uint8_t tmp = USART6->DR;
        if (tmp == 0x66 || BottomData.RxBuff[0] == 0x66){
            BottomData.RxBuff[BottomData.RxCnt++] = tmp;
        }       
        
        if((BottomData.RxBuff[0] == 0x66)&&(BottomData.RxBuff[CARTASK_MSG_LEN-1] == 0x88))
        {
            if(BottomData.RxBuff[1] == Lo_QRcode_Supply){
                BottomData.QR1_MSG[0] =  BottomData.RxBuff[3];
                BottomData.QR1_MSG[1] =  BottomData.RxBuff[4];
                BottomData.QR1_MSG[2] =  BottomData.RxBuff[5];
                BottomData.QR2_MSG[0] =  BottomData.RxBuff[6];
                BottomData.QR2_MSG[1] =  BottomData.RxBuff[7];
                BottomData.QR2_MSG[2] =  BottomData.RxBuff[8];
            }else if (BottomData.RxBuff[1] == Lo_micro_move){
                BottomData.X_Lo      = BottomData.RxBuff[3];
                BottomData.Y_Lo      = BottomData.RxBuff[4];
            }
            BottomData.TaskNum   = BottomData.RxBuff[1];
            BottomData.TaskState = BottomData.RxBuff[2];

        }
        if (BottomData.RxCnt == 8){
            BottomData.RxCnt = 0;
            memset(BottomData.RxBuff,0,sizeof(BottomData.RxBuff));
        }
	}
}


#endif // BOTTOM_LEVEL

