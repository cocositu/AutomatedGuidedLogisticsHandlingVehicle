#ifndef _JY901S_H_
#define _JY901S_H_
#include"config.h"

#define IMU_MSG_LEN	7

typedef struct
{
	float angle[3]; //x,y,z
}Angle;

//UART4
typedef struct _IMU_struct{
	uint8_t Frame_Head;					//帧头
	char MSG_Buff[IMU_MSG_LEN+1];			
	uint8_t MSG_CNT;
    int GetITSta;
} IMU_MSG_Sturct;

extern IMU_MSG_Sturct GY901S_Struct;
void IMU_uart_init(uint32_t bound);


#endif //_GY901S_H_