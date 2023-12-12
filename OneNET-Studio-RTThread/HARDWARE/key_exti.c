/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */

#include "key_exti.h"
#include "delay.h"

/**
 * @brief  ����Ƕ�������жϿ�����NVIC
 * @param  ��
 * @retval ��
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����NVICΪ���ȼ���1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* �����ж�Դ������1 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
    /* ������ռ���ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* ���������ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* ʹ���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* �����ж�Դ������2������ʹ������������� */
    NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * @param  ��
 * @retval ��
 */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*��������GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);

    /* ���� NVIC �ж�*/
    NVIC_Configuration();

    /*--------------------------KEY1����-----------------------------*/
    /* ѡ�񰴼��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �������ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY2����-----------------------------*/
    /* ѡ�񰴼��õ���GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    /* ����Ϊ�������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}



/////*
//// * ��������Key_Scan
//// * ����  ������Ƿ��а�������
//// * ����  ��GPIOx��x ������ A��B��C��D���� E
//// *		     GPIO_Pin������ȡ�Ķ˿�λ
//// * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
//// */
////uint8_t Key_Scan_myself(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
////{
////    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )
////    {
////        delay_ms(50);
////        /*����Ƿ��а������� */
////        if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )
////        {
////            /*�ȴ������ͷ� */
////            while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);
////            return 	KEY_ON;
////        }
//////        else
//////            return KEY_OFF;
////    }
////}


/*********************************************END OF FILE**********************/

