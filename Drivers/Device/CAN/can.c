#include "can.h" 
#include "stm32f10x.h"

//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//则波特率为:36M/((8+9+1)*4)=500Kbps
void CAN_bound(CAN_TypeDef * CANx, u8 brp)
{
	CAN_InitTypeDef        CAN_InitStructure;
	
	//设置波特率		
	CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 

    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常发送模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 		//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;		//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        //分频系数(Fdiv)为brp+1
	CAN_Init(CANx, &CAN_InitStructure);	
}
/************************************************************************************
*函数名：CAN_Config()
作用域：
*功能：配置 can
*参数：
*返回值：
*************************************************************************************/

void CAN1_Config(u8 brp, u32 filterIds[], u8 idLen)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif
   	
  	/*外设时钟设置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	CAN_bound(CAN1, brp);
//	CAN_setAllfit(CAN1, 0);	//无过滤ID
	CAN_Config32BitIdMaskFilter(CAN1, 0, (void *)filterIds, idLen);
}
//---------------------------------------------------------------------------------
void CAN_setAllfit(CAN_TypeDef * CANx, u8 Number)	//无过滤ID
{
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = Number;	//过滤器
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;; 	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//激活过滤器0
	
	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
}
//---------------------------------------------------------------------------------
//*           CAN1_Config16BitFilter()
//*
//* 功能   ： 设置CAN滤波器，过两个16位标准帧ID
//*
//* 参数   ： id1 ：要过的一个16位标准帧ID
//*
//*           id2 ：要过的另一个16位标准帧ID
//*
//* 返回值 ： 无
//*
//* 注释   ： 无
//---------------------------------------------------------------------------------
void CAN_Config16BitFilter(CAN_TypeDef * CANx, u8 Number, u16 id1, u16 id2)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
    CAN_FilterInitStructure.CAN_FilterNumber = Number;          //指定初始化的过滤器1~13
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//标示符屏蔽位模式 可以过滤一组标示符
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;//2个16位过滤器
    CAN_FilterInitStructure.CAN_FilterIdHigh = id1;   //0x0000~0xffff  FC00>>7E0 
    CAN_FilterInitStructure.CAN_FilterIdLow = id2;    //0x0000~0xffff
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xffff;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xffff;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
    CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}
//---------------------------------------------------------------------------------
//*           CAN1_Config32BitFilter()
//*
//* 功能   ： 设置CAN滤波器，过一个32位扩展帧ID
//*
//* 参数   ： id ：要过的32位为扩展帧ID
//*
//* 返回值 ： 无
//*
//* 注释   ： 无
//---------------------------------------------------------------------------------
void CAN_Config32BitFilter(CAN_TypeDef * CANx, u8 Number, u32 id)
{
    u32 j = 0xffffffff;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
    CAN_FilterInitStructure.CAN_FilterNumber = Number;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (id >> 13);
    CAN_FilterInitStructure.CAN_FilterIdLow = ((id << 3) | 4);
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (j >> 13);
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = ((j << 3) | 4);
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
 
    CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}
//------------- ----------------------------------------------------------------
//---------------------------------------------------------------------------------
//*           CAN_Config32BitIdMaskFilter()
//*
//* 功能   ： 设置CAN滤波器（掩码模式），针对32位扩展帧ID
//*
//* 参数   ： ids ：要过的32位为扩展帧ID数组
//*			  len : id数组长度
//*
//* 返回值 ： 无
//*
//* 注释   ： 无
//---------------------------------------------------------------------------------
void CAN_Config32BitIdMaskFilter(CAN_TypeDef * CANx, u8 Number, u32 ids[], u8 len)
{
	u32 tmp, mask;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
    CAN_FilterInitStructure.CAN_FilterNumber = Number;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = ((ids[0] << 3) >> 16) & 0xffff;
    CAN_FilterInitStructure.CAN_FilterIdLow =((ids[0] << 3) & 0xffff) | CAN_ID_EXT;  
	
	mask = 0x1fffffff;  

	for(u8 i = 0; i < len; i++)           
	{  
	tmp = ids[i] ^ (~ids[0]); 
	mask &=tmp;  
	}  
	mask <<=3;                        
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (mask >> 16) & 0xffff;  
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (mask & 0xffff) | 0x02;  
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
 
    CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}
