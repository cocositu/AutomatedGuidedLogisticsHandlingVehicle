#ifndef __LCD_H
#define __LCD_H

// #include "FreeRTOS.h"
// #include "task.h"
#include"stm32f4xx.h"		
// #include "sys.h"
// #define delay_ms(ms) vTaskDelay(ms)

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);//指定区域填充颜色
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//在指定位置画一个点
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);//在指定位置画一条线
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);//在指定位置画一个矩形
void Draw_Circle(uint16_t x0,uint16_t y0,u8 r,uint16_t color);//在指定位置画一个圆

void LCD_ShowChinese(uint16_t x,uint16_t y,u8 *s,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,u8 *s,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,u8 *s,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,u8 *s,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,u8 *s,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示单个32x32汉字

void LCD_ShowChar(uint16_t x,uint16_t y,u8 num,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowString(uint16_t x,uint16_t y,const u8 *p,uint16_t fc,uint16_t bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,u8 len,uint16_t fc,uint16_t bc,u8 sizey);//显示整数变量
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,u8 len,uint16_t fc,uint16_t bc,u8 sizey);//显示两位小数变量
void showQRCodeMessage(char QRcode[]);
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const u8 pic[]);//显示图片

//字库函数
void ZK_command(u8 dat);
u8 get_data_from_ROM(void);
void get_n_bytes_data_from_ROM(u8 AddrHigh,u8 AddrMid,u8 AddrLow,u8 *pBuff,u8 DataLen);
void Display_GB2312(uint16_t x,uint16_t y,u8 zk_num,uint16_t fc,uint16_t bc);
void Display_GB2312_String(uint16_t x,uint16_t y,u8 zk_num,u8 text[],uint16_t fc,uint16_t bc);
void Display_Asc(uint16_t x,uint16_t y,u8 zk_num,uint16_t fc,uint16_t bc);
void Display_Asc_String(uint16_t x,uint16_t y,uint16_t zk_num,u8 text[],uint16_t fc,uint16_t bc);
void Display_Arial_TimesNewRoman(uint16_t x,uint16_t y,u8 zk_num,uint16_t fc,uint16_t bc);
void Display_Arial_String(uint16_t x,uint16_t y,uint16_t zk_num,u8 text[],uint16_t fc,uint16_t bc);
void Display_TimesNewRoman_String(uint16_t x,uint16_t y,uint16_t zk_num,u8 text[],uint16_t fc,uint16_t bc);

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif





