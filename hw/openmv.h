#ifndef _BOTTOM_TASK_H_
#define _BOTTOM_TASK_H_

#include"config.h"
#ifdef  TOP_LEVEL

#define OV_MSG_RX_LEN 10
#define OV_MSG_TX_LEN 10

#define OV_RED_COLOR		  	0x01
#define OV_GREEN_COLOR			0x02
#define OV_BLUE_COLOR			0x03
#define OV_RED_CIRCL		  	0x04
#define OV_GREEN_CIRCL			0x05
#define OV_BLUE_CIRCL			0x06
#define	OV_CIRCL_COLOR		    0x07
/*数据帧格式*/
/* 10位数据
0  	帧头：[
1  	任务标号
2	数据
3	数据
4	数据
5	数据
6	数据
7	数据
8	颜色
9	帧尾：] 
*/

typedef struct _OV_struct{
	// uint8_t Frame_Head;					//帧头
	uint8_t RxBuff[OV_MSG_RX_LEN];
	uint8_t TxBuff[OV_MSG_TX_LEN];		
	uint8_t RxCnt;

	uint8_t TaskNum;
	uint8_t TaskState;
	uint8_t circleColor;
	// uint8_t Px;
	// uint8_t Py;
	uint8_t xy_sta;
	uint8_t sPx[3];
	uint8_t	sPy[4];

} OV_MSG_Sturct;


extern OV_MSG_Sturct OV_Struct;

void Fill_inLight_init(void);
void ToOpenMV_uart_init(uint32_t bound);
void OV_SendData(char DataByte);


#endif  //TOP_LEVEL


#endif  //_BOTTOM_TASK_H_
