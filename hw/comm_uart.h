#ifndef  _COMM_UART_H_
#define _COMM_UART_H_

#include"config.h"

#define COMM_MSG_LEN    10
#define COMM_uSOH       0x66
#define COMM_uEOT       0x88

//UART1
typedef struct _COMM_Uart_struct{
	char MSG_Buff[COMM_MSG_LEN];			
	uint8_t MSG_CNT;
    int GetITSta;
} Comm_Uart_MSG_Sturct;

extern Comm_Uart_MSG_Sturct Comm_Uart_Sturct;
void Comm_uart_init(uint32_t bound);


#endif //_COMM_UART_H_