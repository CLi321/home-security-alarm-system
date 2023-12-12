#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"
#include "sys.h"
#include "wdg.h"
#include "delay.h"

#define USART_REC_LEN_cache 185		//101到57，158恰好接收全部(数据长度为57时)


void uart2_init(u32 bound);

void UART2_send_byte(char data);

void Uart2_SendStr(char*SendBuf);


void Clear_Buffer(void);//清空缓存
void Clear_Buffer_separate(void);


#endif


