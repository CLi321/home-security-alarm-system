#include "uart5.h"	  
#include "led_test.h"  
#include "main.h"  
/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 F103-ָ���� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void UART5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);		

  // ���������ɱ�־
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}


/* UART5�жϺ��� */
void UART5_IRQHandler(void)
{
    uint8_t ucTemp;  //��������
    if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)
    {      
        ucTemp = USART_ReceiveData(UART5);
        USART_SendData(UART5,ucTemp);
/////////////////////////////////////////////
        if(ucTemp == 0x31)
        {
						LED1(LED_ON);
        }
        if(ucTemp == 0x32)
        {
            LED1(LED_OFF);
        }
/////////////////////////////////////////////
        if(ucTemp == 0x33)
        {
						ASR_IF.ASR = 1;
        }
        if(ucTemp == 0x34)
        {
            ASR_IF.ASR = 0;
        }
/////////////////////////////////////////////				
        if(ucTemp == 0x35)
        {
						ASR_IF.SLA = 1;
        }
        if(ucTemp == 0x36)
        {
            ASR_IF.SLA = 0;
        }
/////////////////////////////////////////////				
        if(ucTemp == 0x37)
        {
						ASR_IF.ML302 = 1;
        }
        if(ucTemp == 0x38)
        {
            ASR_IF.ML302 = 0;
        }
/////////////////////////////////////////////				
        if(ucTemp == 0x39)
        {
            ASR_IF.OLED = 1;
        }
        if(ucTemp == 0x40)
        {
						ASR_IF.OLED = 0;
        }
/////////////////////////////////////////////				
        if(ucTemp == 0x41)
        {
            ASR_IF.relay = 1;
        }
        if(ucTemp == 0x42)
        {
            ASR_IF.relay = 0;
        }
    }
}



/*****************  ����һ���ַ� **********************/
void Uart5_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Uart5_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Uart5_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
