#ifndef LOBOTSERVOCONTROLLER_H_
#define LOBOTSERVOCONTROLLER_H_

#include "config.h"
#ifdef TOP_LEVEL
#include "manipulator.h"

#define FRAME_HEADER 0x55             //֡ͷ
#define CMD_SERVO_MOVE 0x03           //����ƶ�ָ��
#define CMD_ACTION_GROUP_RUN 0x06     //���ж�����ָ��
#define CMD_ACTION_GROUP_STOP 0x07    //ֹͣ������ָ��
#define CMD_ACTION_GROUP_SPEED 0x0B   //���ö����������ٶ�
#define CMD_GET_BATTERY_VOLTAGE 0x0F  //��ȡ��ص�ѹָ��


//���λ�ú�
//צ��ID1
#define  Claw_p1   1335  //ץȡλ��
#define  Claw_p2   1515  //�ɿ�λ��
#define  Claw_p3   1675  //ʶ��λ��
//��̨ID2
#define  CP_p1     1163  //��̨ת��ʶ������λ��
#define  CP_p2     2070  //��̨ת��������ץȡλ��
//������ID3
#define  MT_p1     690   //�� (3)
#define  MT_p2     1580  //�� (2)
#define  MT_p3     2440  //�� (1)

extern bool isUartRxCompleted;
extern uint8_t LobotRxBuf[16];
extern uint8_t LobotTxBuf[128];  //���ͻ���
extern uint16_t batteryVolt;
extern void receiveHandle(void);

typedef struct _lobot_servo_ {  //���ID,���Ŀ��λ��
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