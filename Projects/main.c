#include <stdio.h>
#include "delay.h"
#include "common_utils.h"
#include "uart_manager.h"
#include "msg_parser.h"

u32 main_task_flag = 0;
int main(void)
{					
 	delay_init();	    	 // 延时函数初始化
	GPIO_Config();
	
	TIM3_Int_Init(99, 7199);     //10ms中断	
	
	USART1_Config(115200 * 4);    // 蓝牙串口设置
	USART3_Config(115200);    // 日志串口设置		

	delay_ms(1000);
	LOG("RotateLED start\r\n");
	
	while(1)
	{
		parser_uart_msg();
	}
}


/*********************************************END OF FILE**********************/

