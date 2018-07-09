#include <stdarg.h>	 	 	 	 
#include <string.h>	
#include <stdio.h>
#include "sys.h"
#include "usart.h"	  
 

uint8_t USART_TX_BUF[1024];

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
//int fputc(int ch, FILE *f)
//{      
//	while((USART3->SR & 0x40) == 0){}//循环发送,直到发送完毕   
//    USART3->DR = (u8) ch;      
//	return ch;
//}
int fputc(int ch, FILE *f)
{      
	while((USART1->SR & 0x40) == 0){}//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 



/******************************************************************************
******
*函数名：USART_bound()
作用域：
*功能：配置 串口波特率
*参数：
*返回值：
*******************************************************************************
******/ 
void USART_bound(USART_TypeDef* USARTx, u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

	USART_Init(USARTx, &USART_InitStructure); ; //初始化串口
}
/******************************************************************************
******
*函数名：USART1_Config()
作用域：
*功能：配置 串口1
*参数：
*返回值：
*******************************************************************************
******/  
void USART1_Config(u32 bound)
{  
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	
#if EN_USART1_RX   //如果使能了接收
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	
//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
	//USART 初始化设置

#if EN_USART1_RX   //如果使能了接收
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口1接收中断 
#endif

	USART_Cmd(USART1, ENABLE);                    //使能串口1	
	
	USART_bound(USART1, bound);
}
/******************************************************************************
******
*函数名：USART2_Config()
作用域：
*功能：配置 串口2
*参数：
*返回值：
*******************************************************************************
******/
void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	USART_Cmd(USART2, ENABLE);                    //使能串口 
	
	USART_bound(USART2, bound);
}
/******************************************************************************
******
*函数名：USART2_Config()
作用域：
*功能：配置 串口2
*参数：
*返回值：
*******************************************************************************
******/
void USART3_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10

	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11 

	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	USART_bound(USART3, bound);
}
/******************************************************************************
******
*函数名：USART_printf()
作用域：
*功能：串口,printf 函数
*参数：
*返回值：
*******************************************************************************
******/
void USART_printf(USART_TypeDef* USARTx, char* fmt,...)  
{  
	vu16 i; 
	vu16 length;
	va_list ap; 
	
	va_start(ap, fmt);
	vsprintf((char*)USART_TX_BUF, fmt, ap);
	va_end(ap);
	length = strlen((const char*)USART_TX_BUF);		//此次发送数据的长度
	
	for (i = 0; i < length; i++)				//循环发送数据
	{
		USARTx->DR = USART_TX_BUF[i];
		while ((USARTx->SR & 0X40) == 0);//循环发送,直到发送完毕   
	} 
}
/******************************************************************************
******
*函数名：USART_Send()
作用域：
*功能：串口,发送函数
*参数：
*返回值：
*******************************************************************************
******/
void USART_Send(USART_TypeDef* USARTx, uint8_t* buf, u16 length)  
{  
	u16 i;
	
	memcpy((uint8_t *)USART_TX_BUF, (const uint8_t *)buf, length);
	
	for (i = 0; i < length; i++)							//循环发送数据
	{
		USART_SendData(USARTx, (uint8_t) USART_TX_BUF[i]);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	} 
}
//----------------------------------------------END--------------------------
