#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "uart_manager.h"
#include "msg_parser.h"


#define UART_RING_SIZE  512

u16     uart_i_get = 0;
u16     uart_i_put = 0;
u8   uart_ring_buf[UART_RING_SIZE];

/**********************************************************************
* UART接收
**********************************************************************/
u16 UART_INC(u16 val)
{
    return ((val + 1) == UART_RING_SIZE) ? 0 : (val + 1);
}

void put_uart_ring_buf(u8 data)
{
    if (uart_i_get == UART_INC(uart_i_put)) {
        // FULL
        LOG("uart ring full\r\n");
        return;
    }


    uart_ring_buf[uart_i_put] = data;
    uart_i_put = UART_INC(uart_i_put);
	
	return;
}

u16 get_uart_ring_buf(u8 *buf)
{ 
    u16 start = uart_i_get;
    u16 end = uart_i_put;
    u16 num = 0;
	

    if (start == end) {
        ; // EMPTY
    } else if (start < end) {
        num = end - start;
        memcpy(buf, &uart_ring_buf[start], num);
        uart_i_get = end;
    } else {
        num = UART_RING_SIZE - start;
        memcpy(buf, &uart_ring_buf[start], num);
        memcpy(buf + num, &uart_ring_buf[0], end);
        num += end;
        uart_i_get = end;
    }
	
	#if 0
		for(u16 i = 0; i < num; i++) {
			LOG("%02x ", buf[i]);
		}
	#endif
	
    return num;
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		u8 data =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据	
 		put_uart_ring_buf(data);
    } 
} 

/*
 * 蓝牙发送 串口发送数据
 */
u8 sendBuff[128];
void ble_send(u8 ack, u8 len, u8 *data)
{
	sendBuff[0] = (MSG_START_TAG >> 8) & 0xFF;
	sendBuff[1] = (MSG_START_TAG     ) & 0xFF;
	sendBuff[2] = ack;
	sendBuff[3] = len;
	memcpy(&sendBuff[4], data, len);
	sendBuff[4 + len] = calc_crc(sendBuff, 4 + len);
	
	USART_Send(BLE_UART, sendBuff, 4 + len + 1);
}

