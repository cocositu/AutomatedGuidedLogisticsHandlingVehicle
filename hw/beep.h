#ifndef _BEEP_H_
#define _BEEP_H_

#include"config.h"
 
typedef enum{
    SET_EN_LOW_LEVEL   = 0,
    SET_EN_HIGH_LEVEL,
} MSG_BEEP_SET_EN_LEVE;

typedef enum{
    LOW_LEVEL = 0,
    HIGH_LEVEL,
} MSG_BEEP_LEVE;


struct _beep_struct{
    MSG_BEEP_SET_EN_LEVE SetEnLevel;
    MSG_BEEP_LEVE ElecLevel;
    void(*pin_level)(bool);
    void(*on)(struct _beep_struct*);
    void(*off)(struct _beep_struct*);
    // void(*reverse)(struct _led_struct*);
};

typedef struct _beep_struct   BeepStruct;
typedef struct _beep_struct*  pBeepStruct;


pBeepStruct Create_LedStruct(void);

void beep_on(pLedStruct _it_self);
void beep_off(pLedStruct _it_self);
void beep_reverse(pLedStruct _it_self);



/****************************修改区域_begin*******************************/
void led_gpio_init(pLedStruct _it_self);
void beep_pin(bool level);
/****************************修改区域_end*******************************/

#endif //_BEEP_H_