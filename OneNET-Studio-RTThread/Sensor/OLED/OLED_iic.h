#ifndef _OLED_IIC_H
#define _OLED_IIC_H


//#include <inttypes.h>
#include "stm32f10x.h"
#include "delay.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define OLED_GPIO_PORT_I2C			GPIOA			/* GPIO�˿� */
#define OLED_RCC_IIC_PORT 			RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define OLED_IIC_SCL_PIN				GPIO_Pin_5			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define OLED_IIC_SDA_PIN				GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
#define OLED_IIC_SCL_1()  		GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SCL_PIN)		/* SCL = 1 */
#define OLED_IIC_SCL_0() 			GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SCL_PIN)		/* SCL = 0 */

#define OLED_IIC_SDA_1()  		GPIO_SetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)		/* SDA = 1 */
#define OLED_IIC_SDA_0()  		GPIO_ResetBits(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)		/* SDA = 0 */

#define OLED_IIC_SDA_READ()  	GPIO_ReadInputDataBit(OLED_GPIO_PORT_I2C, OLED_IIC_SDA_PIN)	/* ��SDA����״̬ */

#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define OLED_IIC_SCL_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_IIC_SCL_PIN				/* SCL = 1 */
#define OLED_IIC_SCL_0()  OLED_GPIO_PORT_I2C->BRR = OLED_IIC_SCL_PIN				/* SCL = 0 */

#define OLED_IIC_SDA_1()  OLED_GPIO_PORT_I2C->BSRR = OLED_IIC_SDA_PIN				/* SDA = 1 */
#define OLED_IIC_SDA_0()  OLED_GPIO_PORT_I2C->BRR = OLED_IIC_SDA_PIN				/* SDA = 0 */

#define OLED_IIC_SDA_READ()  ((OLED_GPIO_PORT_I2C->IDR & OLED_IIC_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78


#define OLED_IIC_WR	0		/* д����bit */
#define OLED_IIC_RD	1		/* ������bit */


//#define OLED_MODE 0
//#define SIZE 8
//#define XLevelL		0x00
//#define XLevelH		0x10
#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xFF
//#define X_WIDTH 	128
//#define Y_WIDTH 	64



uint8_t OLED_CheckDevice(uint8_t _Address);//���I2C�����豸OLED
void OLED_IIC_WriteByte(uint8_t addr,uint8_t data);
void OLED_WriteCmd(unsigned char IIC_Command);
void OLED_WriteDat(unsigned char IIC_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2);


uint8_t OLED_Test(void) ;//OLED������

//void OLED_DrawPoint(u8 x,u8 y,u8 t);
//void Picture(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OLED_IIC_GPIO_Init(void);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void IIC_Start(void);
static void IIC_Stop(void);
static void IIC_SendByte(uint8_t _ucByte);
static uint8_t IIC_ReadByte(void);
static uint8_t IIC_WaitAck(void);
static void IIC_Ack(void);
static void IIC_NAck(void);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif

