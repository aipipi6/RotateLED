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
 	delay_init();	    	 // ��ʱ������ʼ��
	
	TIM3_Int_Init(99, 7199);     //10ms�ж�	
	
	USART1_Config(115200);    // ������������
//	USART3_Config(115200);    // ��־��������		
	
	AT24CXX_Init();
	
	led_init();
	delay_ms(1000);
	
//	LOG("AT24CXX_Check start\r\n");
//	while(!AT24CXX_Check()) {
//		delay_ms(100);
//	}
//	LOG("AT24CXX_Check over\r\n");
//	
//	LOG("read font start\r\n");
//	read_font();
//	LOG("read %d bytes over\r\n", fontLen);

//	log_array(fontBuffer, fontLen);
//	;
	LOG("RotateLED start\r\n");
	
	
	while(1)
	{
		parser_uart_msg();
		led_display();
	}
}


/*********************************************END OF FILE**********************/

