#ifndef __CAN_H
#define __CAN_H	 
#include "stm32f10x.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.1 
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
//********************************************************************************
//V1.1修改说明 20150528
//修正了CAN初始化函数的相关注释，更正了波特率计算公式
////////////////////////////////////////////////////////////////////////////////// 	 
 
//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    			
   
#define  CAN_500kBps  4 
#define  CAN_250kBps  8  	
#define  CAN_125kBps  16  	
  		

void CAN1_Config(u8 brp, u32 filterIds[], u8 idLen);
void CAN_bound(CAN_TypeDef * CANx, u8 brp);
void CAN_setAllfit(CAN_TypeDef * CANx, u8 Number);	//无过滤ID
void CAN_Config16BitFilter(CAN_TypeDef * CANx, u8 Number, u16 id1, u16 id2);//设置CAN滤波器，过两个16位标准帧ID
void CAN_Config32BitFilter(CAN_TypeDef * CANx, u8 Number, u32 id);//过一个32位扩展帧ID
void CAN_Config32BitIdMaskFilter(CAN_TypeDef * CANx, u8 Number, u32 ids[], u8 len);
#endif

















