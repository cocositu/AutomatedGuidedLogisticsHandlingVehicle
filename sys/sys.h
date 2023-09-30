#ifndef _SYS_H_
#define _SYS_H_


#define SYSTEM_SUPPORT_OS 1
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;

void delay_init();
//延时nus
//nus为要延时的us数.
void delay_us(uint32_t nus);

void delay_ms(uint16_t nms);
//延时nms,不会引起任务调度
//nms:要延时的ms 数
void delay_xms(uint32_t nms);

int _abs(int num);
int _sign(int num);
int _sign_f(float num);

#endif //_SYS_H_