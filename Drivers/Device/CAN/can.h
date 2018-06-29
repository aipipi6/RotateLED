#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f10x.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.1 
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.1�޸�˵�� 20150528
//������CAN��ʼ�����������ע�ͣ������˲����ʼ��㹫ʽ
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    			
   
#define  CAN_500kBps  4 
#define  CAN_250kBps  8  	
#define  CAN_125kBps  16  	
  		

void CAN1_Config(u8 brp, u32 filterIds[], u8 idLen);
void CAN_bound(CAN_TypeDef * CANx, u8 brp);
void CAN_setAllfit(CAN_TypeDef * CANx, u8 Number);	//�޹���ID
void CAN_Config16BitFilter(CAN_TypeDef * CANx, u8 Number, u16 id1, u16 id2);//����CAN�˲�����������16λ��׼֡ID
void CAN_Config32BitFilter(CAN_TypeDef * CANx, u8 Number, u32 id);//��һ��32λ��չ֡ID
void CAN_Config32BitIdMaskFilter(CAN_TypeDef * CANx, u8 Number, u32 ids[], u8 len);
#endif

















