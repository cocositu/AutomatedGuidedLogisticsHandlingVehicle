#include"task_schedule.h"
#include "string.h"
#include"openmv.h"
uint8_t RingMovCount_total =0;
uint8_t RingMovCount = 1;
uint8_t RingMovZone = 0;
uint8_t RingMovSeq = 0;
uint8_t Arr_CarryColorSeq[2][4] ={0};
uint8_t Arr_ZoneColorSeq[2][4] ={0};
uint8_t Arr_ABS_ZoneMove[2][2][7] ={0};
int8_t Arr_REL_ZoneMove[2][2][7] ={0};
bool xy_flag = 0;
int task_index = 0;
uint8_t taskSta[0xFF]={0};

uint8_t taskList[] = {
    TASK_taskSchedule,
    //1从启停区到二维码区
    TASK_moveSzoneToQRzone,
    //2识别二维码
    TASK_identifyQRcode,
    //3从二维码区到物料区
    TASK_moveQRzoneToMzone,
    //4识别抓取物料
    TASK_identify_grab_Material,
    //5物料区到粗放区
    TASK_moveMzoneToEzone,
    //6识别粗放区第一个色环颜色
    TASK_indetifyCrileColor,
    //7移动到粗放中心色环位置
    TASK_moveContorCricle,
    //8识别粗放区中心色环颜色
    TASK_indetifyCrileColor,
    //9移动到粗放搬运的第一个位置
    TASK_moveBetweenCricle,
    //10调整位置
    TASK_moveXYPosition,
    //11放置物料
    TASK_putMaterial,
    //12移动到粗放搬运的第二个位置
    TASK_moveBetweenCricle,
    //13调整位置
    TASK_moveXYPosition,
    //14放置物料
    TASK_putMaterial,
    //15移动到粗放搬运的第三个位置
    TASK_moveBetweenCricle,
    //16调整位置
    TASK_moveXYPosition,
    //17放置物料
    TASK_putMaterial,
    //18移动到粗放搬运的第一个位置
    TASK_moveBetweenCricle,
    //19抓取物料
    TASK_grabMaterial,
    //20移动到粗放搬运的第二个位置
    TASK_moveBetweenCricle,
    //21抓取物料
    TASK_grabMaterial,
    //22移动到粗放搬运的第三个位置
    TASK_moveBetweenCricle,
    //23抓取物料
    TASK_grabMaterial,
    //24从粗放区到暂存区
    TASK_moveEzoneToTzone,
    //25识别暂存区第一个色环颜色
    TASK_indetifyCrileColor,
    //26移动到暂存第中心色环位置
    TASK_moveContorCricle,
    //27识别暂存区中心色环颜色
    TASK_indetifyCrileColor,
    //28移动到暂存搬运的第一个位置
    TASK_moveBetweenCricle,
    //29调整位置
    TASK_moveXYPosition,
    //30放置物料
    TASK_putMaterial,
    //31移动到暂存搬运的第二个位置
    TASK_moveBetweenCricle,
    //32调整位置
    TASK_moveXYPosition,
    //33放置物料
    TASK_putMaterial,
    //34移动到暂存搬运的第三个位置
    TASK_moveBetweenCricle,
    //35调整位置
    TASK_moveXYPosition,
    //36放置物料
    TASK_putMaterial,
    //37从暂存区到物料区
    TASK_moveTzoneToMzone,
    //38识别抓取物料
     TASK_identify_grab_Material,
    //39物料区到粗放区
    TASK_moveMzoneToEzone,
    //40移动到粗放搬运的第一个位置
    TASK_moveBetweenCricle,
    //41调整位置
    TASK_moveXYPosition,
    //42放置物料
    TASK_putMaterial,
    //43移动到粗放搬运的第二个位置
    TASK_moveBetweenCricle,
    //44调整位置
    TASK_moveXYPosition,
    //45放置物料
    TASK_putMaterial,
    //46移动到粗放搬运的第三个位置
    TASK_moveBetweenCricle,
    //47调整位置
    TASK_moveXYPosition,
    //48放置物料
    TASK_putMaterial,
    //49移动到粗放搬运的第一个位置
    TASK_moveBetweenCricle,
    //50抓取物料
    TASK_grabMaterial,
    //51移动到粗放搬运的第二个位置
    TASK_moveBetweenCricle,
    //52抓取物料
    TASK_grabMaterial,
    //53移动到粗放搬运的第三个位置
    TASK_moveBetweenCricle,
    //54抓取物料
    TASK_grabMaterial,
    //55从粗放区到暂存区
    TASK_moveEzoneToTzone,
    //56移动到暂存搬运的第一个位置
    TASK_moveBetweenCricle,
    //57调整位置
    TASK_moveXYPosition,
    //58放置物料
    TASK_putMaterial,
    //59移动到暂存搬运的第二个位置
    TASK_moveBetweenCricle,
    //60调整位置
    TASK_moveXYPosition,
    //61放置物料
    TASK_putMaterial,
    //62移动到暂存搬运的第三个位置
    TASK_moveBetweenCricle,
    //63调整位置
    TASK_moveXYPosition,
    //64放置物料
    TASK_putMaterial,
    //65从暂存区到启停区
    TASK_moveTzoneToSzone 
};


// uint8_t Arr_CarryColorSeq[2][3];
// uint8_t Arr_ZoneColorSeq[2][4];
// uint8_t Arr_ABS_ZoneMove[2][2][6];
// uint8_t Arr_REL_ZoneMove[2][2][6];

void CalMovBetweenRings(int zone, int seq, int init_pos){
   
    for(int i = 1;i <=3; i++){
        Arr_ABS_ZoneMove[zone][seq][i+3]=Arr_ABS_ZoneMove[zone][seq][i]= \
        Arr_ZoneColorSeq[zone][Arr_CarryColorSeq[seq][i]];
    }
    Arr_ABS_ZoneMove[zone][seq][0] = (int8_t)init_pos;
    for(int i =0;i<6; i++){
        Arr_REL_ZoneMove[zone][seq][i]=\
        (int8_t)Arr_ABS_ZoneMove[zone][seq][i+1] - (int8_t)Arr_ABS_ZoneMove[zone][seq][i];
    }
}


uint8_t get_task_status(uint8_t task_name){
    #ifdef TOP_LEVEL
    if(task_name > 0xA0/* && (TopData.xy_pos_sta == 0 || task_name != TASK_moveXYPosition)*/){
        //串口发送数据获取任务运行状态,装入taskSta[task_name]中
        //sendInquireTaskSta(task_name);
        //改为读取引脚电平返回状态
        taskSta[task_name] = getTaskSta_GPIO_READ();
    }
    #endif //TOP_LEVEL
    return taskSta[task_name];
}


#ifdef TOP_LEVEL
TopDataType TopData = {0};

void XYPos_GPIO_init(void){
    XYADJUST_FUN_GPIO_CLK(ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = XYADJUST_GPIO_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(XYADJUST_GPIO, &GPIO_InitStructure);
}

void getTaskSta_GPIO_Init(void){
    getTaskSta_FUN_GPIO_CLK(ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin   = getTaskSta_GPIO_PIN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(getTaskSta_GPIO, &GPIO_InitStructure);
}

bool XY_GPIO_READ(void){
    return GPIO_ReadInputDataBit(XYADJUST_GPIO, XYADJUST_GPIO_PIN);
}

bool getTaskSta_GPIO_READ(void){
 return GPIO_ReadInputDataBit(getTaskSta_GPIO, getTaskSta_GPIO_PIN);
}

void TopComUartInit(uint32_t bound){
  
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
    // USART_Cmd(USART1, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    // USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    // USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);	
 
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream7); 
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)TopData.TxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = COM_MSG_LEN;
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
   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream5);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)TopData.RxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = COM_MSG_LEN;
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
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);

    USART_ClearFlag(USART1, USART_FLAG_TC); //清除发送完成标志
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//禁止USART6接收不为空中断
	// USART_ITConfig(USART1, USART_IT_TXE, DISABLE);			//禁止USART6发送空中断
	// USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);			//开启USART6空闲中断
	// USART_ITConfig(USART1, USART_IT_TC, DISABLE);			//禁止USART6传输完成中断
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  		
	// USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  	    //使能串口的DMA
    DMA_Cmd(DMA2_Stream7, DISABLE);
    // DMA_Cmd(DMA2_Stream5, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void TopSendData(uint8_t  DataBuff[], uint8_t DataLenth){
    while((USART1->SR & USART_SR_TC) == 0);
    USART_Cmd(USART1, DISABLE);
    DMA2_Stream7->CR  &= ~DMA_SxCR_EN; // 清除EN位
    DMA2_Stream7->NDTR = DataLenth;
    DMA2_Stream7->M0AR = (uint32_t)DataBuff; 
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
    DMA2_Stream7->CR  |= DMA_SxCR_EN; // 设置EN位  
    USART_Cmd(USART1, ENABLE);
}	 

void sendStartTask(uint8_t task_name){  
    uint8_t cmd  = 0xB2;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] = task_name;
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;  
    TopSendData(TopData.TxBuff, COM_MSG_LEN);
}

void sendInquireTaskSta(uint8_t task_name){
    uint8_t cmd  = 0xC3;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] = task_name;
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;
    TopSendData(TopData.TxBuff, COM_MSG_LEN);  
}

void sendCarrySeq(void){
    uint8_t cmd  = 0xD4;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] =  Arr_CarryColorSeq[FIRST_SEQ][1];
    TopData.TxBuff[3] =  Arr_CarryColorSeq[FIRST_SEQ][2];
    TopData.TxBuff[4] =  Arr_CarryColorSeq[FIRST_SEQ][3];
    TopData.TxBuff[5] =  Arr_CarryColorSeq[SECOND_SEQ][1];
    TopData.TxBuff[6] =  Arr_CarryColorSeq[SECOND_SEQ][2];
    TopData.TxBuff[7] =  Arr_CarryColorSeq[SECOND_SEQ][3];
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88; 
    TopSendData(TopData.TxBuff, COM_MSG_LEN); 
}


void sendEzoneSeq(void){
  uint8_t cmd  = 0xE5;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] =  Arr_ZoneColorSeq[EZONE_SEQ][1];
    TopData.TxBuff[3] =  Arr_ZoneColorSeq[EZONE_SEQ][2];
    TopData.TxBuff[4] =  Arr_ZoneColorSeq[EZONE_SEQ][3];
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;
    TopSendData(TopData.TxBuff, COM_MSG_LEN); 
}

void sendTzoneSeq(void){
    uint8_t cmd  = 0xF6;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] =  Arr_ZoneColorSeq[TZONE_SEQ][1];
    TopData.TxBuff[3] =  Arr_ZoneColorSeq[TZONE_SEQ][2];
    TopData.TxBuff[4] =  Arr_ZoneColorSeq[TZONE_SEQ][3];
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;
    TopSendData(TopData.TxBuff, COM_MSG_LEN); 
}

void replyXYPos(uint8_t dx[], uint8_t dy[]){
    uint8_t cmd  = 0x11;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[2] = dx[0];
    TopData.TxBuff[3] = dx[1];
    TopData.TxBuff[4] = dx[2];
    TopData.TxBuff[5] = dy[0];
    TopData.TxBuff[6] = dy[1];
    TopData.TxBuff[7] = dy[2];
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;
    TopSendData(TopData.TxBuff, COM_MSG_LEN);
}

void sendInqSta(uint8_t inq_cmd){
    uint8_t cmd  = 0x11;
    memset(TopData.TxBuff, 0, COM_MSG_LEN);
    TopData.TxBuff[0] = 0x66;
    TopData.TxBuff[1] = cmd;
    TopData.TxBuff[1] = inq_cmd;
    TopData.TxBuff[COM_MSG_LEN-1] = 0x88;
    TopSendData(TopData.TxBuff, COM_MSG_LEN);
}



#endif // TOP_LEVEL

#ifdef BOTTOM_LEVEL
BottomDataType BottomData = {0};


void XYPos_GPIO_init(void){
  	GPIO_InitTypeDef  GPIO_InitStructure;
    XYADJUST_FUN_GPIO_CLK(ENABLE);
  	GPIO_InitStructure.GPIO_Pin = XYADJUST_GPIO_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
  	GPIO_Init(XYADJUST_GPIO, &GPIO_InitStructure);//初始化
}

void getTaskSta_GPIO_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
	getTaskSta_FUN_GPIO_CLK(ENABLE);
  	GPIO_InitStructure.GPIO_Pin  = getTaskSta_GPIO_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  	GPIO_Init(getTaskSta_GPIO, &GPIO_InitStructure);
}

void XY_GPIO_CTRL(bool level){
    level?GPIO_SetBits(XYADJUST_GPIO, XYADJUST_GPIO_PIN):GPIO_ResetBits(XYADJUST_GPIO, XYADJUST_GPIO_PIN);
    
}

void getTaskSta_GPIO_CTRL(bool level){
    level?GPIO_SetBits(getTaskSta_GPIO , getTaskSta_GPIO_PIN):GPIO_ResetBits(getTaskSta_GPIO, getTaskSta_GPIO_PIN);
}

void BottomComUartInit(uint32_t bound){
  
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
	
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	

    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream6); 
    DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)BottomData.TxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = COM_MSG_LEN;
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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_DeInit(DMA2_Stream1);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART6->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)BottomData.RxBuff;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = COM_MSG_LEN;
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
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);

	USART_ClearFlag(USART6, USART_FLAG_TC); //清除发送完成标志
 
    USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);			//禁止USART6接收不为空中断
	USART_ITConfig(USART6, USART_IT_TXE, DISABLE);			//禁止USART6发送空中断
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);			//开启USART6空闲中断
	USART_ITConfig(USART6, USART_IT_TC, DISABLE);			//禁止USART6传输完成中断
	
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);  		
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);  	    //使能串口的DMA
    DMA_Cmd(DMA2_Stream6, DISABLE);
    DMA_Cmd(DMA2_Stream1, ENABLE);
    USART_Cmd(USART6, ENABLE);
}

void sendData_Bottom(uint8_t DataBuff[], uint32_t DataLenth){
   // while((USART6->SR & USART_SR_TC) == 0);
    //USART_Cmd(USART6, DISABLE);
    DMA2_Stream6->CR &= ~DMA_SxCR_EN; // 清除EN位
    DMA2_Stream6->NDTR = COM_MSG_LEN;
    DMA2_Stream6->M0AR = (uint32_t)DataBuff; 
    DMA_ClearFlag(DMA2_Stream6, DMA_FLAG_TCIF6);
    DMA2_Stream6->CR |= DMA_SxCR_EN; // 设置EN位
    //USART_Cmd(USART6, ENABLE);  
}	

void replyCurTaskStatus(uint8_t task_name){
    uint8_t cmd = 0xC3;
    memset(BottomData.TxBuff,0,COM_MSG_LEN);
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  cmd;
    BottomData.TxBuff[2] =  task_name;
    BottomData.TxBuff[3] =  get_task_status(task_name);
    BottomData.TxBuff[COM_MSG_LEN-1] =  0x88;
    sendData_Bottom(BottomData.TxBuff, COM_MSG_LEN);
}

void replyRecCarrySta(void){
    uint8_t cmd = 0xD4;
    memset(BottomData.TxBuff,0,COM_MSG_LEN);
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  cmd;
    BottomData.TxBuff[2] =  BottomData.sta_CarrySeq;
    // BottomData.TxBuff[3] = Arr_CarryColorSeq[FIRST_SEQ][1];
    // BottomData.TxBuff[4] = Arr_CarryColorSeq[FIRST_SEQ][2];
    // BottomData.TxBuff[5] = Arr_CarryColorSeq[FIRST_SEQ][3];
    // BottomData.TxBuff[6] = Arr_CarryColorSeq[1][1];
    // BottomData.TxBuff[7] = Arr_CarryColorSeq[1][2];
    // BottomData.TxBuff[8] = Arr_CarryColorSeq[1][3];
    BottomData.TxBuff[COM_MSG_LEN-1] =  0x88;

    
    sendData_Bottom(BottomData.TxBuff, COM_MSG_LEN);
}

void replyRecEzoneRingSta(void){
    uint8_t cmd = 0xE5;
    memset(BottomData.TxBuff,0,COM_MSG_LEN);
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  cmd;
    BottomData.TxBuff[2] =  BottomData.sta_EzoneSeq;
    // BottomData.TxBuff[3] = Arr_ZoneColorSeq[0][1];
    // BottomData.TxBuff[4] = Arr_ZoneColorSeq[0][2];
    // BottomData.TxBuff[5] = Arr_ZoneColorSeq[0][3];
    BottomData.TxBuff[COM_MSG_LEN-1] =  0x88;
    sendData_Bottom(BottomData.TxBuff, COM_MSG_LEN);
}


void replyRecTzoneRingSta(void){
    uint8_t cmd = 0xF6;
    memset(BottomData.TxBuff,0,COM_MSG_LEN);
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  cmd;
    // BottomData.TxBuff[3] = Arr_ZoneColorSeq[1][1];
    // BottomData.TxBuff[4] = Arr_ZoneColorSeq[1][2];
    // BottomData.TxBuff[5] = Arr_ZoneColorSeq[1][3];
    BottomData.TxBuff[2] =  BottomData.sta_TzoneSeq;
    BottomData.TxBuff[COM_MSG_LEN-1] =  0x88;
    sendData_Bottom(BottomData.TxBuff, COM_MSG_LEN);
}

void inqCurXYPos(void){
    uint8_t cmd = 0x11;
    memset(BottomData.TxBuff,0,COM_MSG_LEN);
    BottomData.TxBuff[0] =  0x66;
    BottomData.TxBuff[1] =  cmd;
    BottomData.TxBuff[COM_MSG_LEN-1] =  0x88;
    sendData_Bottom(BottomData.TxBuff, COM_MSG_LEN);
}

#endif // BOTTOM_LEVEL

