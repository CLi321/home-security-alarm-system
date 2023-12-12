#ifndef _GP2Y10_H
#define _GP2Y10_H

#include "stm32f10x.h"
#include "delay.h"

// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    GP2Y_ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    GP2Y_ADC                           ADC2
#define    GP2Y_ADC_CLK                       RCC_APB2Periph_ADC2

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    GP2Y_ADC_GPIO_AHBxClock_FUN        RCC_APB2PeriphClockCmd
#define    GP2Y_ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC
#define    GP2Y_ADC_PORT                      GPIOC
#define    GP2Y_ADC_PIN                       GPIO_Pin_1
// ADC 通道宏定义
#define    GP2Y_ADC_CHANNEL                   ADC_Channel_11


// ILED GPIO宏定义
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


