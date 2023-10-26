#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_

#include"config.h"
#include"pid.h"

#ifdef STEPPER_MOTOR_DRIVER

#define STEPS_LOOP_MAXNUM           400
#define STEPS_UART_BUFFER_LENTH     13   
#define sendMotorTim_Once(...)      Fun_En_DMA_Motor(__VA_ARGS__)


#define SZONE_TO_TMPZONE_DIS    0.6272f
#define TMPZONE_TO_QRCODE_DIS   2.468f
#define QRZONE_TO_MZONE_DIS     2.7715f
#define MZONE_TO_FIRCOR_DIS     1.7195f
#define FIRCOR_TO_EZONE_DIS     2.9535f
#define RINGS_BETWEEN_DIS       0.607f
#define EZONE_TO_SECCOR_DIS     2.1645f
#define SECCOR_TO_TZONE_DIS     2.730f
#define TZONE_TO_SECCOR_DIS     2.730f
#define SECCOR_TO_FIRCOR_DIS    6.3925f
#define FIRCOR_TO_MZONE_DIS     1.7195f
#define TZONE_TO_THIRCOR_DIS    3.3378f
#define THIRCOR_DIS_TO_TMPZONE_DIS  5.4821f
#define TMPZONE_TO_SZONE_X_DIS  1.5575f
#define TMPZONE_TO_SZONE_Y_DIS  0.324f

typedef enum _MOTOR_UART_ADDR_ENUM{
    MOTOR_ALL_ADDR = 0x00,
    MOTOR_LF_ADDR = 0x01,
    MOTOR_LR_ADDR = 0x02,
    MOTOR_RR_ADDR = 0x03,
    MOTOR_RF_ADDR = 0x04,
}MOTOR_UART_ADDR_ENUM;

typedef enum _ABS_OR_REL_FLAG{
    REL_FLAG = 0x00,
    ABS_FLAG = 0x01,
}ABS_OR_REL_FLAG;

typedef enum _MOTOR_UART_SEND_ENUM{
    MOTOR_UART_SEND_DISEN = 0x00,
    MOTOR_UART_SEND_EN    = 0x01,
} MOTOR_UART_SEND_ENUM;

typedef enum _MOTOR_DIR_ENUM{
    MOTOR_REVERSE   = 0x00,
    MOTOR_FORWARD   = 0x01,
} MOTOR_DIR_ENUM;


typedef enum _MOTOR_TYPE_ENUM{
    TIM_CTRL  = 0x00,
    UART_CTRL = 0x01,
} MOTOR_TYPE_ENUM;

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

extern Arr_pStruct_Pos_PID xy_pid;

void MOTOR_LF_TIM_Init(void);
void MOTOR_LR_TIM_Init(void);
void MOTOR_RF_TIM_Init(void);
void MOTOR_RR_TIM_Init(void);
    
void  MOTOR_LF_UART_Init(uint32_t bound);
void  MOTOR_LR_UART_Init(uint32_t bound);
void  MOTOR_RF_UART_Init(uint32_t bound);
void  MOTOR_RR_UART_Init(uint32_t bound);


void MotorUartCtrl(                                                                       \
    MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, uint16_t Motor_vel,   \
    uint8_t Motor_acc, uint32_t Motor_clk, ABS_OR_REL_FLAG Abs_or_Rel_Flag, bool isSend   \
);

void MotorTIMCtrl(MOTOR_UART_ADDR_ENUM Motor_addr, MOTOR_DIR_ENUM Motor_dir, 
    uint32_t Motor_vel,uint32_t Motor_acc, uint32_t Motor_clk, bool isGearShift, bool isEn);

void sendMotorUart_Once(MOTOR_UART_ADDR_ENUM Motor_addr, int Msg_Lenth);
void Fun_En_DMA_Motor(MOTOR_UART_ADDR_ENUM Motor_addr);
void stop_all_motor(void);

void AntiClockwise_90Angle(MOTOR_TYPE_ENUM Motor_ctrl);
void Clockwise_90Angle(MOTOR_TYPE_ENUM Motor_ctrl);
void AntiClockwise_180Angle(MOTOR_TYPE_ENUM Motor_ctrl);
void Clockwise_180Angle(MOTOR_TYPE_ENUM Motor_ctrl);
void pid_init(void);
void MoveInLine_PID(double vel, double dsitance, bool isOSTime);
void TranslationMove(MOTOR_TYPE_ENUM Motor_ctrl, double V, double Dx, double Dy, bool isOSTime);
void TranslationMove_PID(double V, double Dx, double Dy, bool isOSTime);
void AdjustXYPostion(int t_x, int t_y, bool isOSTime);
void MoveXy_PID(int cx ,int cy, int tx, int ty);

#endif //STEPPER_MOTOR_DRIVER
#endif //_MOTOR_DRIVER_H_


		