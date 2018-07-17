#include <stdio.h>
#include "delay.h"
#include "common_utils.h"
#include "uart_manager.h"
#include "msg_parser.h"
#include "font_manager.h"
#include "24cxx.h"
#include "string.h"
#include "led.h"


/**
 * LED: 	A0-A7 B0-B7 B8-B15
 * COLOR:	A11 A12
 * BLE: 	A9 A10
 * EEPROM:	C13 C14
 * IR LED:	A8
 */

int main(void)
{			
 	delay_init();	    	 // 延时函数初始化
	
	TIM3_Int_Init(99, 7199);     //10ms中断	
	USART1_Config(115200);    // 蓝牙串口设置		
	
	AT24CXX_Init();
	
	led_init();
	delay_ms(1000);
	
	while(!AT24CXX_Check()) {
		delay_ms(100);
	}
	
	read_font();

	while(1)
	{
		parser_uart_msg();
		led_display();
		//led_test();
	}
}


/*********************************************END OF FILE**********************/

