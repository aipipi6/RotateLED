#ifndef _TYPE_H
#define _TYPE_H
#include "stm32f10x.h"

typedef union 
{
	u8 BYTE;
	struct 
	{
		u8 BIT0    :1;           
		u8 BIT1    :1;           
		u8 BIT2    :1;           
		u8 BIT3    :1;           
		u8 BIT4    :1; 
		u8 BIT5    :1;              
		u8 BIT6    :1;           
		u8 BIT7    :1;           
	} Bits;
} BIT_Union8;

typedef union 
{
	u16 WORD;
	struct 
	{
		u16 BIT0    :1;           
		u16 BIT1    :1;           
		u16 BIT2    :1;           
		u16 BIT3    :1;           
		u16 BIT4    :1; 
		u16 BIT5    :1;              
		u16 BIT6    :1;           
		u16 BIT7    :1;          
		u16 BIT8    :1;           
		u16 BIT9    :1;           
		u16 BIT10   :1;           
		u16 BIT11   :1;           
		u16 BIT12   :1;           
		u16 BIT13   :1;          
		u16 BIT14   :1;           
		u16 BIT15   :1;          
	} Bits;
} BIT_Union16;;

#endif
