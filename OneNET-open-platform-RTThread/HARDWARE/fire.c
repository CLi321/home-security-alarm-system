
#include "fire.h"


void FIRE_CONFIG(void)
{
	
	GPIO_InitTypeDef GPIO_FIRE;
	
	/*≈‰÷√ ±÷”*/
		//RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
	RCC_APB2PeriphClockCmd(GPIO_FIRE_RCC,ENABLE);
	
	GPIO_FIRE.GPIO_Pin  = GPIO_FIRE_PIN;
	GPIO_FIRE.GPIO_Mode =  GPIO_Mode_IN_FLOATING;//∏°ø’ ‰»Îƒ£ Ω
	
	/*≈‰÷√GPIO  ‰»Î*/
		//void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	GPIO_Init(GPIO_FIRE_PORT,&GPIO_FIRE);
	
}

		


