#include "timer.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//BIT_Union8  _TimeFlag;
u32 timer_10ms = 0;
/********************************************************
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
********************************************************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存 0Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设					 
}
/********************************************************
********************************************************/
void TIM3_IRQHandler(void)   //TIM3中断
{	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 		
		timer_10ms++;	
	}	
}

u8 is_time10ms() {
	static u32 _timer = 1;
	if(_timer < timer_10ms) {
		_timer = timer_10ms;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time20ms()
{
	static u32 _timer = 2;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 2;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time50ms()
{
	static u32 _timer = 5;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 5;
		return 1;
	} else {
		return 0;
	}
}


u8 is_time100ms() 
{
	static u32 _timer = 10;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 10;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time200ms()
{
	static u32 _timer = 20;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 20;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time500ms()
{
	static u32 _timer = 50;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 50;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time1000ms()
{
	static u32 _timer = 100;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 100;
		return 1;
	} else {
		return 0;
	}
}

u8 is_time5000ms()
{
	static u32 _timer = 300;
	if(_timer < timer_10ms) {
		_timer = timer_10ms + 300;
		return 1;
	} else {
		return 0;
	}
}
