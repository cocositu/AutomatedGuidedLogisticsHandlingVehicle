#include"sys.h"
#include"stm32f4xx.h"
#include"FreeRTOS.h"
#include"task.h"

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

extern void xPortSysTickHandler(void);
void SysTick_Handler(void){	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED){
        xPortSysTickHandler();
    }
}

//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(){
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	uint32_t reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	//FreeRTOS选择外部时钟  HCLK					
	fac_us=SystemCoreClock/1000000;	
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	reload=SystemCoreClock/1000000; //每秒钟的计数次数 单位为M
    reload*=1000000/configTICK_RATE_HZ; //根据configTICK_RATE_HZ 设定溢出
    //时间reload 为24 位寄存器,最大值:
    //16777216,在72M 下,约合0.233s 左右
    fac_ms=1000/configTICK_RATE_HZ; //代表OS 可以延时的最少单位
    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk; //开启SYSTICK 中断
    SysTick->LOAD=reload; //每1/configTICK_RATE_HZ 秒中断 一次
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
#endif
}


#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
//延时nus
//nus为要延时的us数.		    								   
void delay_us(uint32_t nus){		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;					//LOAD的值	    	 
	ticks=nus*fac_us; 							//需要的节拍数	
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1){
		tnow=SysTick->VAL;	
		if(tnow!=told){	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//时间超过/等于要延迟的时间,则退出.
		}  
	};						    
}

//延时nms
//nms:要延时的ms数
void delay_ms(uint16_t nms){		
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)   {		 
		if(nms>=fac_ms){ 
			 vTaskDelay(nms/fac_ms); 
		}
		nms%=fac_ms;							//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((uint32_t)(nms*1000));					//普通方式延时  
}

//延时nms,不会引起任务调度
//nms:要延时的ms 数
void delay_xms(uint32_t nms)
{
    uint32_t i;
    for(i=0;i<nms;i++) 
		delay_us(1000);
}

#endif
