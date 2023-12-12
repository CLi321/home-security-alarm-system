#ifndef _GP2Y10_H
#define _GP2Y10_H

#include "stm32f10x.h"
#include "delay.h"

// ADC ���ѡ��
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    GP2Y_ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    GP2Y_ADC                           ADC2
#define    GP2Y_ADC_CLK                       RCC_APB2Periph_ADC2

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    GP2Y_ADC_GPIO_AHBxClock_FUN        RCC_APB2PeriphClockCmd
#define    GP2Y_ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC
#define    GP2Y_ADC_PORT                      GPIOC
#define    GP2Y_ADC_PIN                       GPIO_Pin_1
// ADC ͨ���궨��
#define    GP2Y_ADC_CHANNEL                   ADC_Channel_11


// ILED GPIO�궨��
#define    GP2Y_ILED_PORT                      GPIOC
#define    GP2Y_ILED_PIN                       GPIO_Pin_0
#define    GP2Y_ILED_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    GP2Y_ILED_GPIO_CLK                  RCC_APB2Periph_GPIOC





void GP2Y_ADC_Init(void);
void GP2Y_GetDate(uint16_t  valve);
void Collect_data(void);
float GP2Y_Convert(void);
float GP2Y_GetDens(void);


uint16_t GP2Y_Get_ADC(u8 ch);
uint16_t GP2Y_Get_ADC_Aveurage(u8 ch, uint8_t times);


#endif /* __DSP_GP2Y_H */


