#ifndef _TIMER_H
#define _TIMER_H
#include "type.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern BIT_Union8  _TimeFlag;

#define  TIMEFLAG        _TimeFlag.WORD
#define  TimeFlag_10ms   _TimeFlag.Bits.BIT0
#define  TimeFlag_20ms   _TimeFlag.Bits.BIT1
#define  TimeFlag_50ms   _TimeFlag.Bits.BIT2
#define  TimeFlag_100ms  _TimeFlag.Bits.BIT3
#define  TimeFlag_200ms  _TimeFlag.Bits.BIT4
#define  TimeFlag_500ms  _TimeFlag.Bits.BIT5
#define  TimeFlag_1s     _TimeFlag.Bits.BIT6

extern u32 timer_10ms;

void TIM3_Int_Init(uint16_t arr,uint16_t psc);
u8 is_time10ms(void);
u8 is_time20ms(void);
u8 is_time50ms(void);
u8 is_time100ms(void);
u8 is_time200ms(void);
u8 is_time500ms(void);
u8 is_time1000ms(void);
u8 is_time5000ms(void);

#endif
