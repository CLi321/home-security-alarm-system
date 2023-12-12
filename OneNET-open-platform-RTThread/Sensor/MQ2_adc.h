#ifndef __MQ2_H
#define __MQ2_H
#include "sys.h"
#include "delay.h"
#include "timer_MQ2.h"
#include "math.h"
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADC ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

void MQ2_ADC_Init(void);
uint16_t  Get_MQ2_ADC(uint8_t ch);
uint16_t  Get_MQ2_ADC_Average(uint8_t ch,uint8_t times);


void  MQ2_cumlate(float RS);
float MQ2_GetPPM(void);


#endif
