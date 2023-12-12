#ifndef _SHT30_IIC_H
#define _SHT30_IIC_H


#include "stm32f10x.h"
#include "delay.h"


/***************************************************************************************************************/

// IIC GPIO ���ź궨��
#define  SHT30_IIC_SCL_GPIO_CLK           	RCC_APB2Periph_GPIOB
#define  SHT30_IIC_SCL_GPIO_PORT        		GPIOB
#define  SHT30_IIC_SCL_GPIO_PIN        	  	GPIO_Pin_6

#define  SHT30_IIC_SDA_GPIO_CLK           	RCC_APB2Periph_GPIOB
#define  SHT30_IIC_SDA_GPIO_PORT       			GPIOB
#define  SHT30_IIC_SDA_GPIO_PIN        			GPIO_Pin_7


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */

// IIC���߽ӿڶ���
//����ʱ�Ӷ˿�
#define SHT30_IIC_SCL_1() 		GPIO_SetBits( SHT30_IIC_SCL_GPIO_PORT, SHT30_IIC_SCL_GPIO_PIN )		/* SCL = 1 */
#define SHT30_IIC_SCL_0() 		GPIO_ResetBits( SHT30_IIC_SCL_GPIO_PORT, SHT30_IIC_SCL_GPIO_PIN )		/* SCL = 0 */


//SHT30����SDA����ڣ������Ҳ�����룬����������������������					
//����������ݶ˿�
#define SHT30_IIC_SDA_1()			GPIO_SetBits( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )		/* SDA = 1 */
#define SHT30_IIC_SDA_0()			GPIO_ResetBits( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )		/* SDA = 0 */
//�����������ݶ˿�
#define SHT30_IIC_SDA_READ()  	GPIO_ReadInputDataBit( SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN )	/* ��SDA����״̬ */


#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define SHT30_IIC_SCL_1()  SHT30_IIC_SCL_GPIO_PORT->BSRR = SHT30_IIC_SCL_GPIO_PIN				/* SCL = 1 */
#define SHT30_IIC_SCL_0()  SHT30_IIC_SCL_GPIO_PORT->BRR = SHT30_IIC_SCL_GPIO_PIN				/* SCL = 0 */

#define SHT30_IIC_SDA_1()  SHT30_IIC_SDA_GPIO_PORT->BSRR = SHT30_IIC_SDA_GPIO_PIN				/* SDA = 1 */
#define SHT30_IIC_SDA_0()  SHT30_IIC_SDA_GPIO_PORT->BRR = SHT30_IIC_SDA_GPIO_PIN				/* SDA = 0 */

#define SHT30_IIC_SDA_READ()  ((SHT30_IIC_SDA_GPIO_PORT->IDR & SHT30_IIC_SDA_GPIO_PIN) != 0)	/* ��SDA����״̬ */
#endif




//#define IIC_INPUT_MODE_SET()  SDA_IN()  //���ó�����������������
//#define IIC_OUTPUT_MODE_SET() SDA_OUT()  //���ó�ͨ���������


/***************************************************************************************************************/

							
#define SHT30_write 0 //IIC�豸��ַһ����7λ��Ҳ��10λ��������ʹ��7λIIC�豸��ַ����8λ��д����
#define SHT30_read  1 //IIC�豸��ַһ����7λ��Ҳ��10λ��������ʹ��7λIIC�豸��ַ����8λ�Ƕ�����

extern u8 humiture_buff1[20];


void SHT30_Init(void);
void SHT30_read_result( u8 addr, u16 *tem, u16 *hum );//SHT30��ʪ�ȴ�������ʼ��ȡ����


/***************************************************************************************************************/
							
							
static void IIC_Init(void);            //��ʼ��IIC��IO��
static void SDA_OUT(void);
static void SDA_IN(void);


static void IIC_Start(void);//�ȴ�IIC�������ݿ�ʼ
static void IIC_Stop(void);//IIC��������ֹͣ
static uint8_t IIC_wait_ACK(void);//�ȴ�����ȷ���ַ�
static void IIC_ACK(void);//ȷ���ַ�����ʾ�����������Ѿ�ȷ�Ͻ�������
static void IIC_NACK(void);//��ȷ���ַ�����ʾ���ݽ���������߻�δ�������
static void IIC_SendByte(u8 byte);//IIC��������
static uint8_t IIC_RcvByte(void);//IIC��������


/***************************************************************************************************************/




#endif

