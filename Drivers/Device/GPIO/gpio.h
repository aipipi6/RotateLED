#ifndef _GPIO_H
#define _GPIO_H	 
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


typedef enum 
{
	LED0 = 0,	
	MC20PKEY,
	CAN_STB,
	
} IO_PinTypeDef;

void JTAGDisable(void);
void GPIO_Config(void);
void IO_SetBit(IO_PinTypeDef pin);
void IO_ResetBit(IO_PinTypeDef pin);

#endif
