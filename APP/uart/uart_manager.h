#ifndef __MSG_MANAGER_H__
#define __MSG_MANAGER_H__

#include "gpio.h"
#include "usart.h"
#include "can.h"
#include "timer.h"
#include "common_utils.h"


#define BLE_UART USART1

u16 get_uart_ring_buf(u8 *buf);
void put_uart_ring_buf(u8 data);

void ble_send(u8 ack, u8 len, u8 *data);


#endif
