#ifndef _LED_H_
#define _LED_H_

#include"config.h"
 

 /**********************************用法介绍*********************************/
 /**
  * 请在led.h, led.c文件相关参数未知进行修改,修改后即可开始学习相关用法。
  * 
  * 1、创建LED结构体
  * 首先使用pLedStruct或者Arr_pLedStruct，创建结构体或者结构体数组
  * (1).如果创建的是单个led结构体则使用pLedStruct以及构造函数Create_LedStruct(void)
  * e.g.
  *     pLedStruct LED01 = NULL;
  *     LED01 = Create_LedStruct();
  *(2).如果创建的是数组led结构体则使用Arr_pLedStruct以及构造函数Create_Arr_LedStruct(int arr_n)     
  * e.g.(创建两个led结构体)
  *     Arr_pLedStruct LED = NULL;
  *     LED = Create_Arr_LedStruct(2)
  * 
  * 2、设置led参数，默认led低电平使能,初始化后默认led关闭,如果发现初始化后led亮起，
  * 则需要修改为高电平使能(ps:可以直接在led_gpio_init以及arr_led_gpio_init函数中修改
  * 但是不建议这样做)
  * (1)e.g.
  *     LED01->SetEnLevel = SET_EN_LOW_LEVEL;  //低电平使能
  *     LED01->SetEnLevel = SET_EN_HIGH_LEVEL; //高电平使能
  * (2)e.g.
  *     LED[i]->SetEnLevel = SET_EN_LOW_LEVEL;  //低电平使能
  *     LED[i]->SetEnLevel = SET_EN_HIGH_LEVEL; //高电平使能
  * 
  * 3.led点亮，熄灭，反转函数
  * (1)e.g.
  *     LED01->on(LED01);         //点亮led
  *     LED01->off(LED01);        //熄灭led
  *     LED01->reverse(LED01);    //反转led状态
  * (2)e.g.
  *     LED[i]->on(LED[i]);         //点亮led
  *     LED[i]->off(LED[i]);        //熄灭led
  *     LED[i]->reverse(LED[i]);    //反转led状态
  *
  **/
typedef enum{
    SET_EN_LOW_LEVEL   = 0,
    SET_EN_HIGH_LEVEL,
} MSG_LED_SET_EN_LEVE;

typedef enum{
    LOW_LEVEL = 0,
    HIGH_LEVEL,
} MSG_LED_LEVE;


struct _led_struct{
    MSG_LED_SET_EN_LEVE SetEnLevel;
    MSG_LED_LEVE ElecLevel;
    void(*pin_level)(bool);
    void(*on)(struct _led_struct*);
    void(*off)(struct _led_struct*);
    void(*reverse)(struct _led_struct*);
};

typedef struct _led_struct   LedStruct;
typedef struct _led_struct*  pLedStruct;
typedef struct _led_struct** Arr_pLedStruct;

pLedStruct Create_LedStruct(void);
Arr_pLedStruct Create_Arr_LedStruct(int arr_n);

void led_on(pLedStruct _it_self);
void led_off(pLedStruct _it_self);
void led_reverse(pLedStruct _it_self);


void led_gpio_init(pLedStruct _it_self);
void arr_led_gpio_init(Arr_pLedStruct _arr_self, int arr_n);

/****************************修改区域_begin*******************************/
void led1_pin(bool level);
void led2_pin(bool level);
void led3_pin(bool level);
// void led4_pin(bool level);
/****************************修改区域_end*******************************/

#endif //_LED_H_