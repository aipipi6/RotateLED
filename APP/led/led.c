#include "led.h"
#include "font_manager.h"
#include "timer.h"

void led_color(u8 color)
{
	switch(color) {
		case 0:
			GPIO_SetBits(LED_COLOR_PORT, LED_COLOR_GPIO_PIN1);
			GPIO_ResetBits(LED_COLOR_PORT, LED_COLOR_GPIO_PIN2);
			break;
		
		case 1:
			GPIO_SetBits(LED_COLOR_PORT, LED_COLOR_GPIO_PIN2);
			GPIO_ResetBits(LED_COLOR_PORT, LED_COLOR_GPIO_PIN1);
			break;
		
		default:break;
	}
}

void led_port_set(u8 data1, u8 data2, u8 data3)
{
	LED1_PORT->ODR &= ~LED1_GPIO_PINs;
	LED1_PORT->ODR |= data1;
	
	LED2_PORT->ODR &= ~LED2_GPIO_PINs;
	LED2_PORT->ODR |= data2;
	
	LED3_PORT->ODR &= ~LED3_GPIO_PINs;
	LED3_PORT->ODR |= (data3 << 8);
}

void led_init()
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //使能PORTA,B,C时钟和AFIO时钟

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，禁用JTAG

	GPIO_InitTypeDef GPIO_InitStructure;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PINs;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PINs;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PINs;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LED_COLOR_GPIO_PIN1 | LED_COLOR_GPIO_PIN2;
	GPIO_Init(LED_COLOR_PORT, &GPIO_InitStructure);

	// 红外接收二极管，中断初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                 //选择GPIO_Pin_15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //选择上拉输入模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);                    //初始化以上参数
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//选择中断通道11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//响应优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断
	NVIC_Init(&NVIC_InitStructure);//完成初始化
	

	EXTI_InitTypeDef EXTI_InitStructure;
	//清空中断标志
	EXTI_ClearITPendingBit(EXTI_Line15);
	//连接中断管脚PA11
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//选择中断线路11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//设置中断触发方式为下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//外部中断使能
	EXTI_Init(&EXTI_InitStructure);
	
	led_port_set(0xFF, 0xFF, 0xFF);
}

u8 ledShowFlag = 0;
void EXTI15_10_IRQHandler(void)
{
	ledShowFlag = 1;
	//清空中断标志位，防止持续进入中断
	EXTI_ClearITPendingBit(EXTI_Line15);
}

u16 fontOffset = 0;
void led_display()
{
//	if(!fontPrepareFlag) {
//		return ;
//	}
//	
//	if(ledShowFlag) {
//		ledShowFlag = 0;
//		for(u8 i = 0; i < 24 * 4; i++) {
//			
//			led_port_set(fontBuffer[fontOffset],
//						fontBuffer[fontOffset + 1],
//						fontBuffer[fontOffset + 2]);
//			fontOffset += 3;
//			
//			if(fontOffset >= fontLen) {
//				fontOffset = 0;
//			}
//		}
//	}
	static u32 count = 0;
	static u32 ledData = 0;
	if(is_time500ms()) {
		count++;
		ledData = ~(1 << (count % 24));
		led_port_set(ledData & 0xFF, (ledData >> 8) & 0xFF, (ledData >> 16) & 0xFF);
//		led_color(0);
//		if(count % 2) {
//			led_port_set(0xAA, 0xAA, 0xAA);
//		} else {
//			led_port_set(0x55, 0x55, 0x55);
//		}
	}
	
}

