#include "CarTaskConfig.h"
#include "string.h"

#ifdef TOP_LEVEL
TopDataType TopData = {0};
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

