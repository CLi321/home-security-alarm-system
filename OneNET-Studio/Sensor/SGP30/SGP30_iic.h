#ifndef __SGP30_IIC_H__
#define __SGP30_IIC_H__

#include "stm32f10x.h"
#include "delay.h"


/***************************************************************************************************************/

// IIC GPIO ���ź궨��
#define  SGP30_IIC_SCL_GPIO_CLK           	RCC_APB2Periph_GPIOC
#define  SGP30_IIC_SCL_GPIO_PORT        		GPIOC
#define  SGP30_IIC_SCL_GPIO_PIN          		GPIO_Pin_6

#define  SGP30_IIC_SDA_GPIO_CLK           	RCC_APB2Periph_GPIOC
#define  SGP30_IIC_SDA_GPIO_PORT       			GPIOC
#define  SGP30_IIC_SDA_GPIO_PIN        			GPIO_Pin_7


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */

// IIC���߽ӿڶ���
//����ʱ�Ӷ˿�
#define SGP30_IIC_SCL_1() 		GPIO_SetBits( SGP30_IIC_SCL_GPIO_PORT, SGP30_IIC_SCL_GPIO_PIN )		/* SCL = 1 */
#define SGP30_IIC_SCL_0() 		GPIO_ResetBits( SGP30_IIC_SCL_GPIO_PORT, SGP30_IIC_SCL_GPIO_PIN )		/* SCL = 0 */


//SHT30����SDA����ڣ������Ҳ�����룬����������������������					
//����������ݶ˿�
#define SGP30_IIC_SDA_1()			GPIO_SetBits( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )		/* SDA = 1 */
#define SGP30_IIC_SDA_0()			GPIO_ResetBits( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )		/* SDA = 0 */
//�����������ݶ˿�
#define SGP30_IIC_SDA_READ()  	GPIO_ReadInputDataBit( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )	/* ��SDA����״̬ */


#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define SGP30_IIC_SCL_1()  SGP30_IIC_SCL_GPIO_PORT->BSRR = SGP30_IIC_SCL_GPIO_PIN				/* SCL = 1 */
#define SGP30_IIC_SCL_0()  SGP30_IIC_SCL_GPIO_PORT->BRR = SGP30_IIC_SCL_GPIO_PIN				/* SCL = 0 */

#define SGP30_IIC_SDA_1()  SGP30_IIC_SDA_GPIO_PORT->BSRR = SGP30_IIC_SDA_GPIO_PIN				/* SDA = 1 */
#define SGP30_IIC_SDA_0()  SGP30_IIC_SDA_GPIO_PORT->BRR = SGP30_IIC_SDA_GPIO_PIN				/* SDA = 0 */

#define SGP30_IIC_SDA_READ()  ((SGP30_IIC_SDA_GPIO_PORT->IDR & SGP30_IIC_SDA_GPIO_PIN) != 0)	/* ��SDA����״̬ */
#endif



/***************************************************************************************************************/

//��������IIC����
#define SGP30_read  0xb1
#define SGP30_write 0xb0
							

//��������IIC����
void 			SGP30_Init(void);
void 			SGP30_Write(u8 a, u8 b);
uint32_t 	SGP30_Read(void);


/**********************************************IIC���в�������*****************************************************************/


static void IIC_Init(void);                //��ʼ��IIC��IO��
static void SDA_OUT(void);
static void SDA_IN(void);

static void IIC_Start(void);				//����IIC��ʼ�ź�
static void IIC_Stop(void);	  			//����IICֹͣ�ź�
static u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
static void IIC_Ack(void);					//IIC����ACK�ź�
static void IIC_NAck(void);				//IIC������ACK�ź�
static void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
static u16 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�


static void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
static u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

/***************************************************************************************************************/


#endif


