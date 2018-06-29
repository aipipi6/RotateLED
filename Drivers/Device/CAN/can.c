#include "can.h" 
#include "stm32f10x.h"

//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
void CAN_bound(CAN_TypeDef * CANx, u8 brp)
{
	CAN_InitTypeDef        CAN_InitStructure;
	
	//���ò�����		
	CAN_InitStructure.CAN_TTCM=DISABLE;			//��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_ABOM=DISABLE;			//����Զ����߹���	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART=ENABLE;			//��ֹ�����Զ����� 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//���Ĳ�����,�µĸ��Ǿɵ�  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//���ȼ��ɱ��ı�ʶ������ 

    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 		//Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;		//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=brp;        //��Ƶϵ��(Fdiv)Ϊbrp+1
	CAN_Init(CANx, &CAN_InitStructure);	
}
/************************************************************************************
*��������CAN_Config()
������
*���ܣ����� can
*������
*����ֵ��
*************************************************************************************/

void CAN1_Config(u8 brp, u32 filterIds[], u8 idLen)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

#if CAN_RX0_INT_ENABLE 
	NVIC_InitTypeDef  		NVIC_InitStructure;
#endif
   	
  	/*����ʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//��ʼ��IO

#if CAN_RX0_INT_ENABLE 
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0��Ϣ�Һ��ж�����.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	CAN_bound(CAN1, brp);
//	CAN_setAllfit(CAN1, 0);	//�޹���ID
	CAN_Config32BitIdMaskFilter(CAN1, 0, (void *)filterIds, idLen);
}
//---------------------------------------------------------------------------------
void CAN_setAllfit(CAN_TypeDef * CANx, u8 Number)	//�޹���ID
{
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = Number;	//������
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 	//����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;; 	//32λ�� 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;	//32λID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32λMASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//���������0
	
	CAN_FilterInit(&CAN_FilterInitStructure);			//�˲�����ʼ��
}
//---------------------------------------------------------------------------------
//*           CAN1_Config16BitFilter()
//*
//* ����   �� ����CAN�˲�����������16λ��׼֡ID
//*
//* ����   �� id1 ��Ҫ����һ��16λ��׼֡ID
//*
//*           id2 ��Ҫ������һ��16λ��׼֡ID
//*
//* ����ֵ �� ��
//*
//* ע��   �� ��
//---------------------------------------------------------------------------------
void CAN_Config16BitFilter(CAN_TypeDef * CANx, u8 Number, u16 id1, u16 id2)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    
    CAN_FilterInitStructure.CAN_FilterNumber = Number;          //ָ����ʼ���Ĺ�����1~13
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//��ʾ������λģʽ ���Թ���һ���ʾ��
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;//2��16λ������
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
//* ����   �� ����CAN�˲�������һ��32λ��չ֡ID
//*
//* ����   �� id ��Ҫ����32λΪ��չ֡ID
//*
//* ����ֵ �� ��
//*
//* ע��   �� ��
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
//* ����   �� ����CAN�˲���������ģʽ�������32λ��չ֡ID
//*
//* ����   �� ids ��Ҫ����32λΪ��չ֡ID����
//*			  len : id���鳤��
//*
//* ����ֵ �� ��
//*
//* ע��   �� ��
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
