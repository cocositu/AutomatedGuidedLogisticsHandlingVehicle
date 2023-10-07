#ifndef _CARTASKCONFIG_H_
#define _CARTASKCONFIG_H_

#include "config.h"
#include "sys.h"

#define CARTASK_MSG_LEN 10
/*
通信采用 交互模式
发送任务之后    有任务接受ACK和任务完成ACK
接收到任务完成ACK之后启动下一项任务
*/

/*
顶板和底板 通信协议帧
0：帧头 0x66
1：任务号
2：任务状态 顶板 0x11开始任务  底板 0x22接收任务  0x33完成任务
3：0x00  ~  0xFF        //二维码 1
4：0x00  ~  0xFF        //二维码 2
5：无效                 //二维码 3
6：无效                 //二维码 4
7: 无效                 //二维码 5
8. 无效                 //二维码 6
9：帧尾 0x88
*/

#define StartTask_Sta   0x11
#define ReceTask_Sta    0x22
#define FinishTask_Sta  0x33

//vehicle move task
#define Lo_micro_move            0x00  //后续 参数：坐标
//任务顺序执行
#define Lo_zero_QRcode           0x01  //出库 + 移动到二维码位置
#define Lo_QRcode_Supply         0x02  //二维码位置到物料台 + 前移车身靠近物料盘（后续加微调）

//粗加工 1
#define Lo_Supply_RoughL_1       0x03  //物料 到 粗加工左 
#define Lo_RoughL_RoughM_1       0x04  //粗加工 左到中
#define Lo_RoughM_RoughR_1       0x05  //粗加工 中到右
#define Lo_RoughR_RoughM_1       0x06  //粗加工 右到中
#define Lo_RoughM_RoughL_1       0x07  //粗加工 中到左
#define Lo_RoughL_StoreL_1       0x08  //粗加工左 到 存放右

//存放 1
#define Lo_StoreL_StoreM_1       0x09  //存放  左到中
#define Lo_StoreM_StoreR_1       0x0A  //存放   中到右
#define Lo_StoreR_Supply_1       0x0B  //存放右 到 物料盘

//粗加工 2
#define Lo_Supply_RoughL_2       0x0C  //物料 到 粗加工左 
#define Lo_RoughL_RoughM_2       0x0D  //粗加工 左到中
#define Lo_RoughM_RoughR_2       0x0E  //粗加工 中到右
#define Lo_RoughR_RoughM_2       0x0F  //粗加工 右到中
#define Lo_RoughM_RoughL_2       0x10  //粗加工 中到左
#define Lo_RoughL_StoreL_2       0x11  //粗加工左 到 存放右

//存放 2
#define Lo_StoreL_StoreM_2       0x12  //存放  左到中
#define Lo_StoreM_StoreR_2       0x13  //存放   中到右
#define Lo_StoreR_zero           0x14  //存放右 到 初始位置


#ifdef TOP_LEVEL
//定长数据 
typedef struct {
    uint8_t RxBuff[CARTASK_MSG_LEN];
    uint8_t TxBuff[CARTASK_MSG_LEN];
    uint32_t RxCnt;

    uint8_t TaskNum;
    uint8_t TaskState;

}TopDataType;

extern TopDataType TopData;

void CarTopComUartInit(uint32_t bound);
void CarTopSendData(uint8_t  DataBuff[]);
void TopSendTask(uint8_t TaskID, uint8_t TaskState);
void TopSendQRMsg(char QR_MSG[]);

#endif // TOP_LEVEL



#ifdef BOTTOM_LEVEL

typedef struct 
{
    uint8_t RxBuff[CARTASK_MSG_LEN];
    uint8_t TxBuff[CARTASK_MSG_LEN];
    uint32_t RxCnt;

    uint8_t QR1_MSG[3];
    uint8_t QR2_MSG[3];

    uint8_t TaskNum;
    uint8_t TaskState;
    uint8_t X_Lo;
    uint8_t Y_Lo;

}BottomDataType;

extern BottomDataType BottomData;

void CarBottomComUartInit(uint32_t bound);
void CarBottomSendData(uint8_t DataBuff[]);
void BottomReturnData(uint8_t TaskID, uint8_t TaskState);

#endif // BOTTOM_LEVEL

#endif  //_CARTASKCONFIG_H_


