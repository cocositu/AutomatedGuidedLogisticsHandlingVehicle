#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"
#include "stm32f4xx.h"

// #define led_delay_ms(ms) for(int i=0;i<ms;++i){                    \
//                         for(int j=0;j<1000;++j){                   \
//                             for(int z=0;z<10;++z);                  \
//                         }                                           \
//                     }                                               \
    
#define led_delay_ms(ms) delay_xms(ms)

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif



//-----------------LCD端口定义---------------- 

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOF,GPIO_Pin_0)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOF,GPIO_Pin_0)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOF,GPIO_Pin_1)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOF,GPIO_Pin_1)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOF,GPIO_Pin_2)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOF,GPIO_Pin_2)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOF,GPIO_Pin_3)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOF,GPIO_Pin_3)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOF,GPIO_Pin_4)//CS1
#define LCD_CS_Set()   GPIO_SetBits(GPIOF,GPIO_Pin_4)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOF,GPIO_Pin_5)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOF,GPIO_Pin_5)


void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(uint8_t dat);//模拟SPI时序
void LCD_WR_DATA8(uint8_t dat);//写入一个字节
void LCD_WR_DATA(uint16_t dat);//写入两个字节
void LCD_WR_REG(uint8_t dat);//写入一个指令
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




