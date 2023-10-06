#define _LED_C_
#include"led.h"
#include"stdlib.h"

/**
 * @brief  STM32F407标准库相关参数，想要设置多个led请添加修改相关led
 * 引脚擦参数在这里做修改
 */
/****************************修改区域_begin*******************************/
#ifdef STM32F4xx
#ifdef  TOP_LEVEL
    GPIO_TypeDef *LED_GPIO[] = {GPIOC, GPIOC, GPIOC};
    uint16_t LED_GPIO_PIN[]  = {GPIO_Pin_9, GPIO_Pin_8, GPIO_Pin_7};
    uint32_t LED_GPIO_CLK[]  = {RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOC};
    void(*LED_FUN_SET_CLK[])(uint32_t, FunctionalState) = {RCC_AHB1PeriphClockCmd, RCC_AHB1PeriphClockCmd, RCC_AHB1PeriphClockCmd};
#endif //TOP_LEVEL
#ifdef  BOTTOM_LEVEL
    GPIO_TypeDef *LED_GPIO[] = {GPIOE, GPIOE, GPIOE};
    uint16_t LED_GPIO_PIN[]  = {GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4};
    uint32_t LED_GPIO_CLK[]  = {RCC_AHB1Periph_GPIOE, RCC_AHB1Periph_GPIOE, RCC_AHB1Periph_GPIOE};
    void(*LED_FUN_SET_CLK[])(uint32_t, FunctionalState) = {RCC_AHB1PeriphClockCmd, RCC_AHB1PeriphClockCmd, RCC_AHB1PeriphClockCmd};
#endif //BOTTOM_LEVEL
#endif //STM32F4xx

/**
 * @brief  函数数组，这里添加相关设置引脚电平的函数
 */
void (*led_pin[])(bool) ={led1_pin, led2_pin, led3_pin, /*led4_pin*/};
/****************************修改区域_end*******************************/

/**
 *  pLedStruct Create_LedStruct(void)
 *  Arr_pLedStruct Create_Arr_LedStruct(int arr_n)
 *  void led_on(pLedStruct _it_self);
 *  void led_off(pLedStruct _it_self);
 *  void led_reverse(pLedStruct _it_self);
 * 这些函数请不要修改
 **/
pLedStruct Create_LedStruct(void){
    pLedStruct _it_self = NULL;
    _it_self = (pLedStruct)malloc(sizeof(LedStruct));
    _it_self->SetEnLevel = SET_EN_HIGH_LEVEL;
    _it_self->ElecLevel  = (MSG_LED_LEVE)!_it_self->SetEnLevel;
    _it_self->on         = led_on;
    _it_self->off        = led_off;
    _it_self->reverse    = led_reverse;
    led_gpio_init(_it_self);
    return _it_self;
}

Arr_pLedStruct Create_Arr_LedStruct(int arr_n){
    Arr_pLedStruct _arr_self = NULL;
    _arr_self = (Arr_pLedStruct)malloc(arr_n * sizeof(pLedStruct));

    for(int i = 0; i < arr_n; ++i){
        _arr_self[i] = (pLedStruct)malloc(sizeof(LedStruct));
        _arr_self[i]->SetEnLevel = SET_EN_HIGH_LEVEL;
        _arr_self[i]->ElecLevel  = (MSG_LED_LEVE)!_arr_self[i]->SetEnLevel;
        _arr_self[i]->on         = led_on;
        _arr_self[i]->off        = led_off;
        _arr_self[i]->reverse    = led_reverse;
    }
    arr_led_gpio_init(_arr_self , arr_n);
    return _arr_self;
}

void led_on(pLedStruct _it_self){
    _it_self->ElecLevel = (MSG_LED_LEVE)(_it_self->SetEnLevel);
    _it_self->pin_level((bool)_it_self->ElecLevel);
}

void led_off(pLedStruct _it_self){
    _it_self->ElecLevel = (MSG_LED_LEVE)!_it_self->SetEnLevel;
    _it_self->pin_level((bool)_it_self->ElecLevel);
}

void led_reverse(pLedStruct _it_self){
    _it_self->ElecLevel = (MSG_LED_LEVE)!_it_self->ElecLevel;
    _it_self->pin_level((bool)_it_self->ElecLevel);  
}


/**
 * @brief  初始化单个led_gpio函数，非必要请不要修改
 * @param  _it_self:  led结构体指针
 */
void led_gpio_init(pLedStruct _it_self){
#ifdef STM32F4xx
    GPIO_InitTypeDef GPIO_InitStructure;
    LED_FUN_SET_CLK[2](LED_GPIO_CLK[2], ENABLE);
    GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN[2];
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_GPIO[2], &GPIO_InitStructure); 
#endif
    _it_self->pin_level  = led3_pin;
    _it_self->SetEnLevel = SET_EN_LOW_LEVEL;
    _it_self->off(_it_self);  
}

/**
 * @brief  初始化多个led_gpio函数，非必要请不要修改
 * @param  _arr_self: led结构体指针数组
 * @param  arr_n:     数组长度
 */
void arr_led_gpio_init(Arr_pLedStruct _arr_self, int arr_n){
    for(int i = 0; i < arr_n; ++i){
#ifdef STM32F4xx
        GPIO_InitTypeDef GPIO_InitStructure;
        LED_FUN_SET_CLK[i](LED_GPIO_CLK[i], ENABLE);
        GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN[i];
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_Init(LED_GPIO[i], &GPIO_InitStructure); 
#endif
        _arr_self[i]->pin_level =  led_pin[i];
        _arr_self[i]->SetEnLevel = SET_EN_LOW_LEVEL;
        _arr_self[i]->off(_arr_self[i]);
    }
}

/****************************修改区域_begin*******************************/
/**
 * @brief  led1_pin(bool level),led2_pin(bool level)为引脚电平配置函数，
 * 如果想要设置更多led，请参照一下引脚函数，自己写出其他led引脚函数，并在头
 * 文件中去定义，并在led引脚函数数组中去添加，并完善led相关参数配置
 * @param  level: 
 */

void led1_pin(bool level){
#ifdef STM32F4xx
    level > 0 ? GPIO_SetBits(LED_GPIO[0],LED_GPIO_PIN[0])
              : GPIO_ResetBits(LED_GPIO[0], LED_GPIO_PIN[0]);
#endif
}

void led2_pin(bool level){
#ifdef STM32F4xx
    level > 0 ? GPIO_SetBits(LED_GPIO[1],LED_GPIO_PIN[1])
              : GPIO_ResetBits(LED_GPIO[1], LED_GPIO_PIN[1]);
#endif
}

void led3_pin(bool level){
#ifdef STM32F4xx
    level > 0 ? GPIO_SetBits(LED_GPIO[2],LED_GPIO_PIN[2])
              : GPIO_ResetBits(LED_GPIO[2], LED_GPIO_PIN[2]);
#endif
}
// void led4_pin(bool level){

// }
/****************************修改区域_end*******************************/
