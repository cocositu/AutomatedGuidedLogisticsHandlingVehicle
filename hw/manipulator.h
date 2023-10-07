#ifndef _MANIPULATOR_H_
#define _MANIPULATOR_H_

#include"config.h"
#ifdef  TOP_LEVEL

#define uartWriteBuf(...)    ManServoSend(__VA_ARGS__)

#define MAN_SEP_MSG_LEN 13
#define MAN_SERVO_MSG_LEN 8

extern char MAN_SEP_MSG_Buff[MAN_SEP_MSG_LEN];
extern char MAN_SERVO_MSG_Buff[MAN_SERVO_MSG_LEN];

void MAN_STEP_UART_Init(uint32_t bound);
void MAN_SERVO_UART_Init(uint32_t bound);

void ManServoSend(uint8_t *buf, uint8_t len);
void ManStepUartCtrl(uint8_t Motor_dir, uint16_t Motor_vel, uint8_t Motor_acc, \
                     uint32_t Motor_clk, uint8_t Abs_or_Rel_Flag);
                     
#endif  //TOP_LEVEL
#endif  //_MANIPULATOR_H_
