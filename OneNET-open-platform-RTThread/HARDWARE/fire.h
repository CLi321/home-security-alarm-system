#ifndef __FIRE_H
#define __FIRE_H 


#include "stm32f10x.h"


#define GPIO_FIRE_RCC  RCC_APB2Periph_GPIOC
#define GPIO_FIRE_PORT GPIOC//Ñ¡ÔñGPIOA¼Ä´æÆ÷
#define GPIO_FIRE_PIN  GPIO_Pin_8 //Òý½Å5


void FIRE_CONFIG(void	);


#endif















