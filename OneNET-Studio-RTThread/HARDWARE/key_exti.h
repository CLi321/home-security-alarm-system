#ifndef __KEY_EXTI_H
#define	__KEY_EXTI_H


#include "stm32f10x.h"


//引脚定义
//#define KEY1_INT_GPIO_PORT         GPIOA
//#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
//#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
//#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
//#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
//#define KEY1_INT_EXTI_LINE         EXTI_Line0
//#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

//#define KEY1_IRQHandler            EXTI0_IRQHandler




//#define KEY2_INT_GPIO_PORT         GPIOA
//#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
//#define KEY2_INT_GPIO_PIN          GPIO_Pin_1
//#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
//#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource1
//#define KEY2_INT_EXTI_LINE         EXTI_Line1
//#define KEY2_INT_EXTI_IRQ          EXTI1_IRQn

//#define KEY2_IRQHandler            EXTI1_IRQHandler



#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_4
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY1_INT_EXTI_LINE         EXTI_Line4
#define KEY1_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY1_IRQHandler            EXTI4_IRQHandler




#define KEY2_INT_GPIO_PORT         GPIOA
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_6
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource6
#define KEY2_INT_EXTI_LINE         EXTI_Line6
#define KEY2_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY2_IRQHandler            EXTI9_5_IRQHandler


///** 按键按下标置宏
//*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
//*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
//*/
//#define KEY_ON	0
//#define KEY_OFF	1



//uint8_t Key_Scan_myself(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void EXTI_Key_Config(void);


#endif /* __EXTI_H */
