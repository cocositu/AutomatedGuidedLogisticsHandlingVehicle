#ifndef _HWT101_H_
#define _HWT101_H_
#include"config.h"

#define IMU_MSG_RX_LEN	22
#define IMU_MSG_TX_LEN	5

typedef struct
{
	float angle[3]; //x,y,z
}Angle;

//UART4
typedef struct _IMU_struct{
	// uint8_t Frame_Head;					//帧头
	char MSG_Rx_Buff[IMU_MSG_RX_LEN];
	char MSG_Tx_Buff[IMU_MSG_TX_LEN];
	float YawAngle;			
	uint8_t MSG_CNT;
    int GetITSta;
} IMU_MSG_Sturct;

extern IMU_MSG_Sturct HWT101_Struct;
void IMU_uart_init(uint32_t bound);


#endif //_HWT101_H_