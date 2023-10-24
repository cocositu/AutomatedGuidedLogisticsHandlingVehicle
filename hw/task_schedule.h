#ifndef _TASK_SCHEDULE_H_
#define _TASK_SCHEDULE_H_

#include"config.h"

#define EZONE_SEQ   0
#define TZONE_SEQ   1
#define FIRST_SEQ   0
#define SECOND_SEQ  1

#define TASK_IDLE_STATE  0
#define TASK_BUSY_STATE  1

#define COM_MSG_LEN 10

//0.上任务调度
#define TASK_taskSchedule               0x00
//**1.从启停区到二维码区**
#define TASK_moveSzoneToQRzone          0x01   +  0xA0
//**2.识别二维码**
#define TASK_identifyQRcode             0x02   
//**3.从二维码区到物料区**
#define TASK_moveQRzoneToMzone          0x03   +  0xA0
//**4.识别抓取物料**
#define TASK_identify_grab_Material     0x04
//**5.物料区到粗放区**
#define TASK_moveMzoneToEzone           0x05   +  0xA0
//**6.识别区域色环颜色**
#define TASK_indetifyCrileColor         0x06
//**7.识别物料**
#define TASK_identifyMaterial           0x07
//**8.抓取物料**
#define TASK_grabMaterial               0x08
//**9.放置物料**
#define TASK_putMaterial                0x09
//**10.从粗放区到暂存区**
#define TASK_moveEzoneToTzone           0x0A   +  0xA0
//**11.从暂存区到物料区**
#define TASK_moveTzoneToMzone           0x0B   +  0xA0
//**12.从暂存区到启停区**
#define TASK_moveTzoneToSzone           0x0C   +  0xA0
//**13.色环间移动**
#define TASK_moveBetweenCricle          0x0D   +  0xA0
//**14.调整XY任务**
#define TASK_moveXYPosition             0x0E   +  0xA0
//**15.移动到色环中间**
#define  TASK_moveContorCricle          0x0F   +  0xA0

extern int task_index;
extern uint8_t taskSta[];
extern uint8_t taskList[];

extern uint8_t Arr_CarryColorSeq[2][4];
extern uint8_t Arr_ZoneColorSeq[2][4];
extern uint8_t Arr_ABS_ZoneMove[2][2][7];
extern int8_t Arr_REL_ZoneMove[2][2][7];

extern uint8_t RingMovCount_total;
extern uint8_t RingMovCount;
extern uint8_t RingMovZone;
extern uint8_t RingMovSeq;


#ifdef TOP_LEVEL
//定长数据 
typedef struct {
    uint8_t RxBuff[COM_MSG_LEN];
    uint8_t TxBuff[COM_MSG_LEN];
    uint32_t RxCnt;

    uint8_t tmp_taskName;
    uint8_t tmp_taskSta;
    uint8_t sta_CarrySeq;
    uint8_t sta_EzoneSeq;
    uint8_t sta_TzoneSeq;

    uint8_t xy_pos_sta;


}TopDataType;

extern TopDataType TopData;

void TopComUartInit(uint32_t bound);
void TopSendData(uint8_t  DataBuff[], uint8_t DataLenth);
void sendStartTask(uint8_t task_name);
void sendInquireTaskSta(uint8_t task_name);
void sendCarrySeq(void);
void sendEzoneSeq(void);
void sendTzoneSeq(void);
void sendInqSta(uint8_t inq_cmd);
void replyXYPos(uint8_t dx[], uint8_t dy[]);

#endif // TOP_LEVEL

#ifdef BOTTOM_LEVEL

typedef struct {
    uint8_t RxBuff[COM_MSG_LEN];
    uint8_t TxBuff[COM_MSG_LEN];
    uint32_t RxCnt;

    uint8_t needRelyTask;
    uint8_t needStartTask;
    // uint8_t arr_CarrySeq[6];
    // uint8_t arr_EzoneSeq[3];
    // uint8_t arr_TzoneSeq[3];

   // uint8_t sta_rely;
    uint8_t sta_CarrySeq;
    uint8_t sta_EzoneSeq;
    uint8_t sta_TzoneSeq;
    
    uint8_t sta_xy;
    int px;
    int py;

}BottomDataType;

extern BottomDataType BottomData;

void BottomComUartInit(uint32_t bound);
void sendData_Bottom(uint8_t   DataBuff[], uint32_t DataLenth);
void replyCurTaskStatus(uint8_t task_name);
void replyRecEzoneRingSta(void);
void replyRecTzoneRingSta(void);
void replyRecCarrySta(void);
void inqCurXYPos(void);

#endif //BOTTOM_LEVEL


void CalMovBetweenRings(int zone, int seq, int init_pos);
uint8_t get_task_status(uint8_t task_name);

#endif //_TASK_SCHEDULE_H_ 

