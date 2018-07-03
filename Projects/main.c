#include <stdio.h>
#include "delay.h"
#include "common_utils.h"
#include "uart_manager.h"
#include "msg_parser.h"
#include "font_manager.h"
#include "24cxx.h"
#include "string.h"
#include "led.h"

extern u32 timer_10ms;

int main(void)
{			
 	delay_init();	    	 // 延时函数初始化
	GPIO_Config();
	
	TIM3_Int_Init(99, 719);     //10ms中断	
	
	USART1_Config(115200);    // 蓝牙串口设置
	USART3_Config(115200);    // 日志串口设置		
	
	AT24CXX_Init();
	
	led_init();
	delay_ms(1000);
	
	LOG("AT24CXX_Check start\r\n");
	while(!AT24CXX_Check()) {
		delay_ms(100);
	}
	LOG("AT24CXX_Check over\r\n");
	
	LOG("read font start\r\n");
	read_font();
	LOG("read %d bytes over\r\n", fontLen);

	log_array(fontBuffer, fontLen);
	
	LOG("RotateLED start\r\n");
	while(1)
	{
		parser_uart_msg();
		led_display();
	}
}


/*********************************************END OF FILE**********************/

