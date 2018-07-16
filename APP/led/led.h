#ifndef __LED_H__
#define __LED_H__
#include "sys.h"

#define GPIO_PINs(x, y)		(uint16_t)((0xFFFF >> (15 - ((y) - (x)))) << (x))

#define LED1_PORT	GPIOA
#define LED2_PORT	GPIOB
#define LED3_PORT	GPIOB

#define LED1_GPIO_PINs 		GPIO_PINs(0, 7)
#define LED2_GPIO_PINs		GPIO_PINs(0, 7)
#define LED3_GPIO_PINs		GPIO_PINs(8, 15)


#define LED_COLOR_PORT 			GPIOA
#define LED_COLOR_GPIO_PIN1		GPIO_Pin_11
#define LED_COLOR_GPIO_PIN2		GPIO_Pin_12


void led_init(void);
void led_display(void);
void led_port_set(u8 data1, u8 data2, u8 data3);
void led_color_set(u8 color);
void led_color_change(void);
void led_test(void);

#endif
