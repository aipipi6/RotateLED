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
 * IR LED:	C15
 */

int main(void)
{			
	u8 buff[512];
	u8 buff1[512];
 	delay_init();	    	 // 延时函数初始化
	
	TIM3_Int_Init(99, 7199);     //10ms中断	
	
	USART1_Config(115200);    // 蓝牙串口设置
//	USART3_Config(115200);    // 日志串口设置		
	
	AT24CXX_Init();
	
	led_init();
	delay_ms(1000);
	
	LOG("AT24CXX_Check start\r\n");
	while(!AT24CXX_Check()) {
		delay_ms(100);
	}
	LOG("AT24CXX_Check over\r\n");
	
//	LOG("read font start\r\n");
//	read_font();
//	LOG("read %d bytes over\r\n", fontLen);
	for(int i = 0; i < sizeof(buff); i++) {
		buff[i] = i & 0xff;
	}
	AT24CXX_WriteBytes(FONT_START_ADDR, buff, sizeof(buff));
	
	AT24CXX_ReadBytes(FONT_START_ADDR, buff1, sizeof(buff1));
	log_array(buff1, sizeof(buff1));

	LOG("RotateLED start\r\n");
	
	led_color(2);
	while(1)
	{
		parser_uart_msg();
		led_display();
	}
}


/*********************************************END OF FILE**********************/

