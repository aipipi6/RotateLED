#include <stdio.h>
#include "delay.h"
#include "common_utils.h"
#include "uart_manager.h"
#include "msg_parser.h"
#include "24cxx.h"

u32 main_task_flag = 0;
int main(void)
{					
 	delay_init();	    	 // ��ʱ������ʼ��
	GPIO_Config();
	
	TIM3_Int_Init(99, 7199);     //10ms�ж�	
	
	USART1_Config(115200 * 4);    // ������������
	USART3_Config(115200);    // ��־��������		
	
//	AT24CXX_Init();

	delay_ms(1000);
	LOG("RotateLED start\r\n");
	
	while(1)
	{
		parser_uart_msg();
	}
}


/*********************************************END OF FILE**********************/

