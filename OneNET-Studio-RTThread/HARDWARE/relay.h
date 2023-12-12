#ifndef __RELAY_H
#define	__RELAY_H


#include "stm32f10x.h"


/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define RELAY_GPIO_PORT    	GPIOC	             			 /* GPIO�˿� */
#define RELAY_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO�˿�ʱ�� */
#define RELAY_GPIO_PIN		  GPIO_Pin_14			        /* ���ӵ�SCLʱ���ߵ�GPIO */



/** the macro definition to trigger the led on or off
  * 1 - off
  *0 - on
  */
#define RELAY_ON  0
#define RELAY_OFF 1
//GPIO_SetBits    1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define RELAY(a)	if (a)	\
					GPIO_SetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN);\
					else		\
					GPIO_ResetBits(RELAY_GPIO_PORT,RELAY_GPIO_PIN)





void RELAY_GPIO_Config(void);

#endif /* __LED_H */
