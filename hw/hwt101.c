#define _HWT101_C_
#include"hwt101.h"
#ifdef BOTTOM_LEVEL
#include"string.h"

IMU_MSG_Sturct HWT101_Struct={0};

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

	IMU_UART_FUN_DMA_CLK(ENABLE);
    DMA_DeInit(IMU_UART_DMA_STREAM);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_Channel            = IMU_UART_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&IMU_UART_SEND_ADRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)HWT101_Struct.MSG_Rx_Buff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = IMU_MSG_RX_LEN;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_Init(IMU_UART_DMA_STREAM, &DMA_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = IMU_UART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 2;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

	USART_ClearFlag(IMU_UART, USART_FLAG_TC); //清除发送完成标志
	//警告：增加这条在蓝牙的硬件框架下会导致死机，原因是前面只使能了USART_Mode_Rx，而没有使能USART_Mode_Tx
	//while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);	//等待空闲帧发送完成后再清零发送完成标志
	USART_ClearFlag(IMU_UART, USART_FLAG_TC); //清除发送完成标志
 
    USART_ITConfig(IMU_UART, USART_IT_RXNE, DISABLE);			//禁止USART4接收不为空中断
	USART_ITConfig(IMU_UART, USART_IT_TXE, DISABLE);			//禁止USART4发送空中断
	USART_ITConfig(IMU_UART, USART_IT_IDLE, ENABLE);			//开启USART4空闲中断
	USART_ITConfig(IMU_UART, USART_IT_TC, DISABLE);				//禁止USART4传输完成中断
	
	USART_DMACmd(IMU_UART, USART_DMAReq_Tx, DISABLE);  			//禁止串口的DMA发送
	USART_DMACmd(IMU_UART, USART_DMAReq_Rx, ENABLE);  			//使能串口的DMA接收
    DMA_Cmd(IMU_UART_DMA_STREAM, ENABLE);
    USART_Cmd(IMU_UART, ENABLE);
}

//读取数据	角度输出
//0x55	0x53	0x00	0x00	0x00	0x00	YawL	YawH	VL	VH	SUM
/*			   无意义	无意义	无意义	无意义
YawL	偏航角Z低8位	偏航角Z=((YawH<<8)|YawL)/32768*180(°)
YawH	偏航角Z高8位
VL		版本号低8位		版本号计算公式：版本号=(VH<<8)|VL
VH		版本号高8位
SUM		校验和			SUM=0x55+0x53+YawH+YawL+VL+VH
*/

//写数据
/*	
	协议头	协议头	 寄存器	  数据低8位	  数据高8位
	0xFF	0xAA	ADDR	DATAL[7:0]	DATAH[15:8]

	CALIYAW（Z轴角度归零） 寄存器地址: 118(0x76)	默认值: 0x0000
	示例：FF AA 76 00 00（Z轴角度归零）

	WORKMODE（Z轴运行模式）寄存器地址: 72 (0x48)	默认值: 0x0000
	Bit					NAME  				FUNCTION  
	15:4			
											设置Z轴运行模式：
	3:0				WORKMODE[3:0]			0000(0x00): 正常数据模式
											0001(0x01): 求峰峰值模式
											0010(0x02): 求零偏模式
											0011(0x03):求标度因素模式
	示例：发送：FF AA  48 01 00（自动获取零偏）

*/
void IMU_UART_IRQHandler(void){
	if(USART_GetITStatus(IMU_UART,USART_IT_IDLE)!=RESET){		
		DMA_Cmd(IMU_UART_DMA_STREAM, DISABLE); /* 关闭DMA ，防止干扰 */ 
		uint16_t reDataLenth = DMA_GetCurrDataCounter(IMU_UART_DMA_STREAM);
		uint8_t ParityByte  =  												    \
		(uint8_t)(HWT101_Struct.MSG_Rx_Buff[11] + HWT101_Struct.MSG_Rx_Buff[12] + \
				  HWT101_Struct.MSG_Rx_Buff[17] + HWT101_Struct.MSG_Rx_Buff[18] + \
				  HWT101_Struct.MSG_Rx_Buff[19] + HWT101_Struct.MSG_Rx_Buff[20]);
		if(reDataLenth == 0 					 && \
		   HWT101_Struct.MSG_Rx_Buff[11] == 0x55 && \
		   HWT101_Struct.MSG_Rx_Buff[12] == 0x53 && \
		   HWT101_Struct.MSG_Rx_Buff[21] == ParityByte 
		   ){
			 HWT101_Struct.YawAngle = 
			(HWT101_Struct.MSG_Rx_Buff[7]<<8 + HWT101_Struct.MSG_Rx_Buff[6]) / 32768.0 * 180;
			 HWT101_Struct.GetITSta = 1;
		}else{
			HWT101_Struct.GetITSta = -1;
			memset(HWT101_Struct.MSG_Rx_Buff,0,sizeof(HWT101_Struct.MSG_Rx_Buff));
		}
		IMU_UART_DMA_STREAM->NDTR = IMU_MSG_RX_LEN;
		DMA_ClearFlag(IMU_UART_DMA_STREAM, IMU_UART_DMA_FLAG_TCIF);  /* 清DMA标志位 */
		DMA_Cmd(IMU_UART_DMA_STREAM, ENABLE);      
		USART_ClearFlag(IMU_UART, USART_FLAG_TC); //清除发送完成标志
		USART_ReceiveData(IMU_UART);// 清除空闲中断标志位
		//int temp = IMU_UART->SR;   
		//    temp = IMU_UART->DR; //清USART_IT_IDLE标志
	}
}


#endif //BOTTOM