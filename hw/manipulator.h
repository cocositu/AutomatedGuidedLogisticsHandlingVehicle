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


void init_MechArm_and_MateTray(bool isOSTime);

//materiel tray
void GrabMate_to_MT(uint8_t x,bool isOSTime);

//ground
void GrabMate_to_G(uint8_t x,bool isOSTime);
void PutMate_to_G(uint8_t x,bool isOSTime);

void PutMate_to_M(uint8_t x,bool isOSTime);

void alone_Rotate_MateTray(uint8_t x,bool isOSTime);
void move_MechArm_to_MTBottom(bool isOSTime);
void identify_color_circle_postion(uint8_t dir);
// void ManServo_StepMoter_Init(void);
// void Item_tray_Turn(int x);
// void Catch_Itemtray_Action(void);
// void RoughingArea_Action(void);
// void Catch_StagingArea_Action_(void);
// void Put_ExtensiveArea_Action_(void); 

#endif  //TOP_LEVEL
#endif  //_MANIPULATOR_H_
