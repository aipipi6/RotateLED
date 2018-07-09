#ifndef _TIMER_H
#define _TIMER_H
#include "type.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
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
