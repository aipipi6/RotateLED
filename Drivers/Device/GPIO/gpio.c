#include "gpio.h"

/********************************************************
* 失能JTAG								 
********************************************************/
void JTAGDisable(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
}
/********************************************************
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LEDx
  * @retval None
********************************************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                   // LED1	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/********************************************************
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  * @retval None
********************************************************/
void IO_SetBit(IO_PinTypeDef pin)
{
	switch (pin)
	{
		case LED0: 
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			break;
		
		case MC20PKEY:
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			break;
		
		case CAN_STB:
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			break;
		
		default :
			break;
	}	     
}
/********************************************************
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  * @retval None
********************************************************/
void IO_ResetBit(IO_PinTypeDef pin)
{
	switch (pin)
	{
		case LED0: 
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			break;
		
		case MC20PKEY:
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			break;
		
		case CAN_STB:
			GPIO_ResetBits(GPIOB, GPIO_Pin_14);
			break;
		
		default :
			break;
	}	     
}
/********************************************************
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  * @retval 
********************************************************/
u8 IO_ReadBit(IO_PinTypeDef pin)
{
	uint8_t bitstatus = 0x00;
	
//	bitstatus = GPIO_ReadInputDataBit(IO_ConfTypeDef[pin].GPIO_PORT, IO_ConfTypeDef[pin].GPIO_Pin);
	
	return (u8)bitstatus;

}

