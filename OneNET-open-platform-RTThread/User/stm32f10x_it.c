/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "board.h"
#include "rtthread.h"

#include "GeneralTim.h"
#include "key_exti.h"
#include "led_test.h"
extern volatile uint32_t time_rec;
extern uint8_t KEY1_flag, KEY2_flag;

extern rt_sem_t 	communication_sem;
extern rt_sem_t 	recognition_sem;
extern rt_sem_t 	sensor_sem;

void NMI_Handler(void)
{
}

//void HardFault_Handler(void)
//{
//  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
//}

void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

//void PendSV_Handler(void)
//{
//}
//
//void SysTick_Handler(void)
//{
//}


void  GENERAL_TIM_IRQHandler (void)
{
    if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET )
    {
        time_rec++;
        TIM_ClearITPendingBit(GENERAL_TIM, TIM_FLAG_Update);
			
        if ( time_rec == 21 || time_rec == 3021 || time_rec == 6021 || time_rec == 9021) /* 1000 * 1 ms = 1s 时间到 */
        {
						rt_sem_release(recognition_sem);
        }	
				if ( time_rec == 1521 || time_rec == 4521 || time_rec == 7521 || time_rec == 10521)
				{
						rt_sem_release(sensor_sem);
				}
        if ( time_rec == 12021 ) /* 1000 * 1 ms = 1s 时间到 */
        {
						rt_sem_release(communication_sem);
        }	

    }
}

void KEY1_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
        KEY1_flag = 1;
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

void KEY2_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
    {
        KEY2_flag = 1;
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
    }
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
