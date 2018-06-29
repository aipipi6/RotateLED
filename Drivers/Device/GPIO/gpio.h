#ifndef _GPIO_H
#define _GPIO_H	 
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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
