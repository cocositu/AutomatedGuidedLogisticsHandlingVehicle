#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include"config.h"

#ifdef STEPPER_MOTOR_DRIVER

#define STEPS_LOOP_MAXNUM           1000
#define STEPS_UART_BUFFER_LENTH     8   

typedef uint32_t StepBufferType;
typedef uint8_t  MotorUartBufferType;

extern StepBufferType StepMotorBuffer[4][STEPS_LOOP_MAXNUM+1];
extern StepBufferType *StepMotorLFBuffer;
extern StepBufferType *StepMotorLRBuffer;
extern StepBufferType *StepMotorRFBuffer;
extern StepBufferType *StepMotorRRBuffer;

extern MotorUartBufferType MotorUartBuffer[4][STEPS_UART_BUFFER_LENTH];
extern MotorUartBufferType *MotorLFUartBuffer;
extern MotorUartBufferType *MotorLRUartBuffer;
extern MotorUartBufferType *MotorRFUartBuffer;
extern MotorUartBufferType *MotorRRUartBuffer;


void MOTOR_LF_TIM_Init(void);
void MOTOR_LR_TIM_Init(void);
void MOTOR_RF_TIM_Init(void);
void MOTOR_RR_TIM_Init(void);
    
void MOTOR_LF_DMA_Init(void);
void MOTOR_LR_DMA_Init(void);
void MOTOR_RF_DMA_Init(void);
void MOTOR_RR_DMA_Init(void);


void  MOTOR_LF_UART_Init(uint32_t bound);
void  MOTOR_LR_UART_Init(uint32_t bound);
void  MOTOR_RF_UART_Init(uint32_t bound);
void  MOTOR_RR_UART_Init(uint32_t bound);

#endif //STEPPER_MOTOR_DRIVER
#endif //_MOTOR_DRIVER_H_