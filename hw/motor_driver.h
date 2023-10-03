#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include"config.h"

#ifdef STEPPER_MOTOR_DRIVER

#define STEPS_LOOP_MAXNUM           400
#define STEPS_UART_BUFFER_LENTH     13   

typedef enum{
    MOTOR_ALL_ADDR = 0x00,
    MOTOR_LF_ADDR = 0x01,
    MOTOR_LR_ADDR = 0x02,
    MOTOR_RR_ADDR = 0x03,
    MOTOR_RF_ADDR = 0x04,
}MOTOR_UART_ADDR_ENUM;
typedef enum{
    REL_FLAG = 0x00,
    ABS_FLAG = 0x01,
}ABS_OR_REL_FLAG;

typedef enum{
    MOTOR_UART_SEND_DISEN = 0x00,
    MOTOR_UART_SEND_EN    = 0x01,
} MOTOR_UART_SEND_ENUM;

typedef enum{
    MOTOR_REVERSE   = 0x00,
    MOTOR_FORWARD   = 0x01,
} MOTOR_DIR_ENUM;

typedef uint32_t StepBufferType;
typedef uint8_t  MotorUartBufferType;

extern StepBufferType StepMotorBuffer[4][STEPS_LOOP_MAXNUM+1];
extern StepBufferType *StepMotorLFBuffer;
extern StepBufferType *StepMotorLRBuffer;
extern StepBufferType *StepMotorRFBuffer;
extern StepBufferType *StepMotorRRBuffer;

extern MotorUartBufferType MotorUartBuffer[5][STEPS_UART_BUFFER_LENTH];
extern MotorUartBufferType *MotorLFUartBuffer;
extern MotorUartBufferType *MotorLRUartBuffer;
extern MotorUartBufferType *MotorRFUartBuffer;
extern MotorUartBufferType *MotorRRUartBuffer;

void MOTOR_LF_TIM_Init(void);
void MOTOR_LR_TIM_Init(void);
void MOTOR_RF_TIM_Init(void);
void MOTOR_RR_TIM_Init(void);
    
void  MOTOR_LF_UART_Init(uint32_t bound);
void  MOTOR_LR_UART_Init(uint32_t bound);
void  MOTOR_RF_UART_Init(uint32_t bound);
void  MOTOR_RR_UART_Init(uint32_t bound);

void sendMotorUart_Once(MOTOR_UART_ADDR_ENUM Motor_addr, int Msg_Lenth);
void MotorUartCtrl(                                                                       \
    MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, uint16_t Motor_vel,   \
    uint8_t Motor_acc, uint32_t Motor_clk, ABS_OR_REL_FLAG Abs_or_Rel_Flag, bool isSend   \
);

void MotorTIMCtrl(MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, 
    uint32_t Motor_vel,uint32_t Motor_acc, uint32_t Motor_clk, bool isGearShift, bool isEn);
void Fun_En_DMA_Motor(MOTOR_UART_ADDR_ENUM Motor_addr);
#endif //STEPPER_MOTOR_DRIVER
#endif //_MOTOR_DRIVER_H_


		