#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"
#include "sys.h"
#include "wdg.h"
#include "delay.h"

#define USART_REC_LEN_cache			1321 		//


void uart2_init(u32 bound);

void UART2_send_byte(char data);

void Uart2_SendStr(char*SendBuf);


void Clear_Buffer(void);//Çå¿Õ»º´æ
void Clear_Buffer_separate(void);


#endif


