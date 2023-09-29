#ifndef _QRCODE_H_
#define _QRCODE_H_
#include"config.h"

//UART5
typedef struct _QRcode_struct{
	char MSG_Buff[8];			
	uint8_t MSG_CNT;
    int GetITSta;
} QRCode_MSG_Sturct;

extern QRCode_MSG_Sturct QRCode;
void QR_uart_init(uint32_t bound);


#endif //_QRCODE_H_