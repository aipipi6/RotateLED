#include "led.h"
#include "font_manager.h"
#include "timer.h"
#include "delay.h"
void led_color_change() 
{
	static u8 count = 0;
	count++;
	led_color_set(count % 2);
}

void led_color_set(u8 color)
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTA,B,Cʱ�Ӻ�AFIOʱ��

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD������JTAG

	// LED�������ų�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PINs;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PINs;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PINs;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	
	// LED��ɫ���ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = LED_COLOR_GPIO_PIN1 | LED_COLOR_GPIO_PIN2;
	GPIO_Init(LED_COLOR_PORT, &GPIO_InitStructure);
	led_color_change();

	// ������ն����ܣ��жϳ�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                 //ѡ��GPIO_Pin_15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //ѡ����������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ�����ϲ���
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	//ѡ���ж�ͨ��8
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);//��ɳ�ʼ��
	

	EXTI_InitTypeDef EXTI_InitStructure;
	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line8);
	//�����жϹܽ�PA11
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);

	EXTI_InitStructure.EXTI_Line = EXTI_Line8;//ѡ���ж���·11
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����Ϊ�ж����󣬷��¼�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�����жϴ�����ʽΪ�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ⲿ�ж�ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	led_port_set(0xFF, 0xFF, 0xFF);
}

u8 ledShowFlag = 0;
void EXTI9_5_IRQHandler(void)
{
	ledShowFlag = 1;
	//����жϱ�־λ����ֹ���������ж�
	EXTI_ClearITPendingBit(EXTI_Line8);
}

void led_display()
{
	static u16 fontOffset = 0;
	
	if(fontPrepareFlag == 0) {
		return ;
	} else if(fontPrepareFlag == 1) {
		fontOffset = 0;
		fontPrepareFlag = 2;
	}
	
	if(ledShowFlag) {
		for(u16 i = 0; i < 24 * 4; i++) {
			led_port_set(~fontBuffer[i * 3 + fontOffset],
					~fontBuffer[i * 3 + 1 + fontOffset],
					~fontBuffer[i * 3 + 2 + fontOffset]);
		
			delay_us(250);
			led_port_set(0xFF, 0xFF, 0xFF);	//��Ӱ
		}	
		fontOffset += 3;
		if(fontOffset >= fontLen) {
			fontOffset = 0;
			//led_color_change();  // ������ɫ�л�
		}
		ledShowFlag = 0;
	}
	

}

void led_test() 
{
	static u32 count = 0;
	u32 ledData = 0;
	if(is_time500ms()) {
		if(count % 24 == 0) {
			led_color_change();
		}
		ledData = ~(1 << (count % 24));
		led_port_set(ledData & 0xFF, (ledData >> 8) & 0xFF, (ledData >> 16) & 0xFF);
		
		count++;
	}
	
}

