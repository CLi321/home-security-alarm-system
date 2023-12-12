#include "relay.h"

/**
 * @brief  ��ʼ������LED��IO
 * @param  ��
 * @retval ��
 */
void RELAY_GPIO_Config(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����LED��ص�GPIO����ʱ��*/
    RCC_APB2PeriphClockCmd( RELAY_GPIO_CLK, ENABLE);
    /*ѡ��Ҫ���Ƶ�GPIO����*/
    GPIO_InitStructure.GPIO_Pin = RELAY_GPIO_PIN;

    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*���ÿ⺯������ʼ��GPIO*/
    GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStructure);


    /* �ر�����led��	*/
    GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_GPIO_PIN);

}

/*********************************************END OF FILE**********************/
