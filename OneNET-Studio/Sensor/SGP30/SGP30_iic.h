#ifndef __SGP30_IIC_H__
#define __SGP30_IIC_H__

#include "stm32f10x.h"
#include "delay.h"


/***************************************************************************************************************/

// IIC GPIO 引脚宏定义
#define  SGP30_IIC_SCL_GPIO_CLK           	RCC_APB2Periph_GPIOC
#define  SGP30_IIC_SCL_GPIO_PORT        		GPIOC
#define  SGP30_IIC_SCL_GPIO_PIN          		GPIO_Pin_6

#define  SGP30_IIC_SDA_GPIO_CLK           	RCC_APB2Periph_GPIOC
#define  SGP30_IIC_SDA_GPIO_PORT       			GPIOC
#define  SGP30_IIC_SDA_GPIO_PIN        			GPIO_Pin_7


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */

// IIC总线接口定义
//设置时钟端口
#define SGP30_IIC_SCL_1() 		GPIO_SetBits( SGP30_IIC_SCL_GPIO_PORT, SGP30_IIC_SCL_GPIO_PIN )		/* SCL = 1 */
#define SGP30_IIC_SCL_0() 		GPIO_ResetBits( SGP30_IIC_SCL_GPIO_PORT, SGP30_IIC_SCL_GPIO_PIN )		/* SCL = 0 */


//SHT30数据SDA传输口，有输出也有输入，所以需配置它的输入和输出					
//设置输出数据端口
#define SGP30_IIC_SDA_1()			GPIO_SetBits( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )		/* SDA = 1 */
#define SGP30_IIC_SDA_0()			GPIO_ResetBits( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )		/* SDA = 0 */
//设置输入数据端口
#define SGP30_IIC_SDA_READ()  	GPIO_ReadInputDataBit( SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN )	/* 读SDA口线状态 */


#else	/* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define SGP30_IIC_SCL_1()  SGP30_IIC_SCL_GPIO_PORT->BSRR = SGP30_IIC_SCL_GPIO_PIN				/* SCL = 1 */
#define SGP30_IIC_SCL_0()  SGP30_IIC_SCL_GPIO_PORT->BRR = SGP30_IIC_SCL_GPIO_PIN				/* SCL = 0 */

#define SGP30_IIC_SDA_1()  SGP30_IIC_SDA_GPIO_PORT->BSRR = SGP30_IIC_SDA_GPIO_PIN				/* SDA = 1 */
#define SGP30_IIC_SDA_0()  SGP30_IIC_SDA_GPIO_PORT->BRR = SGP30_IIC_SDA_GPIO_PIN				/* SDA = 0 */

#define SGP30_IIC_SDA_READ()  ((SGP30_IIC_SDA_GPIO_PORT->IDR & SGP30_IIC_SDA_GPIO_PIN) != 0)	/* 读SDA口线状态 */
#endif



/***************************************************************************************************************/

//传感器的IIC配置
#define SGP30_read  0xb1
#define SGP30_write 0xb0
							

//传感器的IIC函数
void 			SGP30_Init(void);
void 			SGP30_Write(u8 a, u8 b);
uint32_t 	SGP30_Read(void);


/**********************************************IIC所有操作函数*****************************************************************/


static void IIC_Init(void);                //初始化IIC的IO口
static void SDA_OUT(void);
static void SDA_IN(void);

static void IIC_Start(void);				//发送IIC开始信号
static void IIC_Stop(void);	  			//发送IIC停止信号
static u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
static void IIC_Ack(void);					//IIC发送ACK信号
static void IIC_NAck(void);				//IIC不发送ACK信号
static void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
static u16 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节


static void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
static u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

/***************************************************************************************************************/


#endif


