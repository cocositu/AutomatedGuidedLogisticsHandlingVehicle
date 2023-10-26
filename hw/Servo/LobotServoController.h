#ifndef LOBOTSERVOCONTROLLER_H_
#define LOBOTSERVOCONTROLLER_H_

#include "config.h"
#ifdef TOP_LEVEL
#include "manipulator.h"

#define FRAME_HEADER 0x55             //帧头
#define CMD_SERVO_MOVE 0x03           //舵机移动指令
#define CMD_ACTION_GROUP_RUN 0x06     //运行动作组指令
#define CMD_ACTION_GROUP_STOP 0x07    //停止动作组指令
#define CMD_ACTION_GROUP_SPEED 0x0B   //设置动作组运行速度
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //获取电池电压指令


//舵机位置宏
//爪子ID1
#define  Claw_p1   1335  //抓取位置
#define  Claw_p2   1515  //松开位置
#define  Claw_p3   1675  //识别位置
//云台ID2
#define  CP_p1     1163  //云台转到识别物料位置
#define  CP_p2     2070  //云台转到物料盘抓取位置
//物料盘ID3
#define  MT_p1     690   //蓝 (3)
#define  MT_p2     1580  //绿 (2)
#define  MT_p3     2440  //红 (1)

extern bool isUartRxCompleted;
extern uint8_t LobotRxBuf[16];
extern uint8_t LobotTxBuf[128];  //发送缓存
extern uint16_t batteryVolt;
extern void receiveHandle(void);

typedef struct _lobot_servo_ {  //舵机ID,舵机目标位置
	uint8_t ID;
	uint16_t Position;
} LobotServo;


void moveServo(uint8_t servoID, uint16_t Position, uint16_t Time);
void moveServosByArray(LobotServo servos[], uint8_t Num, uint16_t Time);
void moveServos(uint8_t Num, uint16_t Time, ...);
void runActionGroup(uint8_t numOfAction, uint16_t Times);
void stopActionGroup(void);
void setActionGroupSpeed(uint8_t numOfAction, uint16_t Speed);
void setAllActionGroupSpeed(uint16_t Speed);
void getBatteryVoltage(void);



#endif
#endif //TOP_LEVEL