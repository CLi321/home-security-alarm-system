/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64�����OLED��ʾ�������ļ�����������SD1306����IICͨ�ŷ�ʽ��ʾ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com

	* Function List:
	*	1. uint8_t OLED_CheckDevice(uint8_t _Address) -- ���I2C�����豸OLED
	* 2. void IIC_WriteByte(uint8_t addr,uint8_t data) -- ��Ĵ�����ַдһ��byte������
	* 3. void WriteCmd(unsigned char IIC_Command) -- д����
	* 4. void WriteDat(unsigned char IIC_Data) -- д����
	* 5. void OLED_Init(void) -- OLED����ʼ��
	* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- ������ʼ������
	* 7. void OLED_Fill(unsigned char fill_Data) -- ȫ�����
	* 8. void OLED_CLS(void) -- ����
	* 9. void OLED_ON(void) -- ����
	* 10. void OLED_OFF(void) -- ˯��
	* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- ��ʾ�ַ���(�����С��6*8��8*16����)
	* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- ��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
	* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMPͼƬ
	* 14. uint8_t OLED_Test(void) --OLED������
  *
  ******************************************************************************
  */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
  ******************************************************************************
  * @file    bsp_IIC_gpio.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ��gpioģ��i2c����, ������STM32ϵ��CPU����ģ�鲻����Ӧ�ò�����֡��������I2C���߻�������������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� IIC_CheckDevice() ���I2C�豸�Ƿ�����

*/


#include "OLED_iic.h"
#include "codetab.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*************************************************************************
*	�� �� ��: IIC_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*************************************************************************
*/
uint8_t OLED_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_Start();		/* ���������ź� */

    IIC_SendByte(_Address|OLED_IIC_WR);/* �����豸��ַ */
    ucAck = IIC_WaitAck();	/* ����豸��ACKӦ�� */

    IIC_Stop();			/* ����ֹͣ�ź� */

    return ucAck;
}


/**
 * @brief  IIC_WriteByte����OLED�Ĵ�����ַдһ��byte������
 * @param  addr���Ĵ�����ַ
*					data��Ҫд�������
 * @retval ��
 */
void OLED_IIC_WriteByte(uint8_t addr,uint8_t data) {

    IIC_Start();//����I2C����

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    IIC_SendByte(OLED_ADDRESS|OLED_IIC_WR);

    /*�ȴ�ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED������Ӧ�� */
    }

    IIC_SendByte(addr);//���ͼĴ�����ַ

    /*�ȴ�ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED������Ӧ�� */
    }

    IIC_SendByte(data);//��������

    /*�ȴ�ACK */
    if (IIC_WaitAck() != 0)
    {
        goto cmd_fail;	/* OLED������Ӧ�� */
    }

    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();

}


/**
 * @brief  WriteCmd����OLEDд������
 * @param  IIC_Command���������
 * @retval ��
 */
void OLED_WriteCmd(unsigned char IIC_Command)//д����
{
    OLED_IIC_WriteByte(0x00, IIC_Command);
}


/**
 * @brief  WriteDat����OLEDд������
 * @param  IIC_Data������
 * @retval ��
 */
void OLED_WriteDat(unsigned char IIC_Data)//д����
{
    OLED_IIC_WriteByte(0x40, IIC_Data);
}


/**
 * @brief  OLED_Init����ʼ��OLED
 * @param  ��
 * @retval ��
 */
void OLED_Init(void)
{

    OLED_IIC_GPIO_Init();

    delay_s(1);		// 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������

    OLED_WriteCmd(0xAE); //display off
    OLED_WriteCmd(0x20);	//Set Memory Addressing Mode
    OLED_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    OLED_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    OLED_WriteCmd(0xc8);	//Set COM Output Scan Direction
    OLED_WriteCmd(0x00); //---set low column address
    OLED_WriteCmd(0x10); //---set high column address
    OLED_WriteCmd(0x40); //--set start line address
    OLED_WriteCmd(0x81); //--set contrast control register
    OLED_WriteCmd(0xff); //���ȵ��� 0x00~0xff
    OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
    OLED_WriteCmd(0xa6); //--set normal display
    OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3F); //
    OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    OLED_WriteCmd(0xd3); //-set display offset
    OLED_WriteCmd(0x00); //-not offset
    OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    OLED_WriteCmd(0xf0); //--set divide ratio
    OLED_WriteCmd(0xd9); //--set pre-charge period
    OLED_WriteCmd(0x22); //
    OLED_WriteCmd(0xda); //--set com pins hardware configuration
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0xdb); //--set vcomh
    OLED_WriteCmd(0x20); //0x20,0.77xVcc
    OLED_WriteCmd(0x8d); //--set DC-DC enable
    OLED_WriteCmd(0x14); //
    OLED_WriteCmd(0xaf); //--turn on oled panel
}


/**
 * @brief  OLED_SetPos�����ù��
 * @param  x,���xλ��
*					y�����yλ��
 * @retval ��
 */
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{
    OLED_WriteCmd(0xb0+y);
    OLED_WriteCmd(((x&0xf0)>>4)|0x10);
    OLED_WriteCmd((x&0x0f)|0x01);
}

/**
 * @brief  OLED_Fill�����������Ļ
 * @param  fill_Data:Ҫ��������
* @retval ��
 */
void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
    unsigned char m,n;
    for(m=0; m<8; m++)
    {
        OLED_WriteCmd(0xb0+m);		//page0-page1
        OLED_WriteCmd(0x00);		//low column start address
        OLED_WriteCmd(0x10);		//high column start address
        for(n=0; n<128; n++)
        {
            OLED_WriteDat(fill_Data);
        }
    }
}

/**
 * @brief  OLED_CLS������
 * @param  ��
* @retval ��
 */
void OLED_CLS(void)//����
{
    OLED_Fill(0x00);
}


/**
 * @brief  OLED_ON����OLED�������л���
 * @param  ��
* @retval ��
 */
void OLED_ON(void)
{
    OLED_WriteCmd(0X8D);  //���õ�ɱ�
    OLED_WriteCmd(0X14);  //������ɱ�
    OLED_WriteCmd(0XAF);  //OLED����
}


/**
 * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
 * @param  ��
* @retval ��
 */
void OLED_OFF(void)
{
    OLED_WriteCmd(0X8D);  //���õ�ɱ�
    OLED_WriteCmd(0X10);  //�رյ�ɱ�
    OLED_WriteCmd(0XAE);  //OLED����
}


/**
 * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
 * @param  x,y : ��ʼ������(x:0~127, y:0~7);
*					ch[] :- Ҫ��ʾ���ַ���;
*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
* @retval ��
 */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
    switch(TextSize)
    {
    case 1:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 126)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x,y);
            for(i=0; i<6; i++)
                OLED_WriteDat(F6x8[c][i]);
            x += 6;
            j++;
        }
    }
    break;
    case 2:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 120)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x,y);
            for(i=0; i<8; i++)
                OLED_WriteDat(F8X16[c*16+i]);
            OLED_SetPos(x,y+1);
            for(i=0; i<8; i++)
                OLED_WriteDat(F8X16[c*16+i+8]);
            x += 8;
            j++;
        }
    }
    break;
    }
}

/**
 * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
 * @param  x,y: ��ʼ������(x:0~127, y:0~7);
*					N:������codetab.h�е�����
* @retval ��
 */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x, y);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0; wm < 16; wm++)
    {
        OLED_WriteDat(F16x16[adder]);
        adder += 1;
    }
}



/**
 * @brief  OLED_DrawBMP����ʾBMPλͼ
 * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
*					x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
* @retval ��
 */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
    for(y=y0; y<y1; y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0; x<x1; x++)
        {
            OLED_WriteDat(BMP[j++]);
        }
    }
}




//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
    c=chr-' ';//�õ�ƫ�ƺ��ֵ
    if(x>Max_Column-1) {
        x=0;
        y=y+2;
    }
    if(Char_Size ==16)
    {
        OLED_SetPos(x,y);
        for(i=0; i<8; i++)
            OLED_WriteDat(F8X16[c*16+i]);
        OLED_SetPos(x,y+1);
        for(i=0; i<8; i++)
            OLED_WriteDat(F8X16[c*16+i+8]);
    }
    else {
        OLED_SetPos(x,y);
        for(i=0; i<6; i++)
            OLED_WriteDat(F6x8[c][i]);

    }
}
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {   OLED_ShowChar(x,y,chr[j],Char_Size);
        x+=8;
        if(x>120) {
            x=0;
            y+=2;
        }
        j++;
    }
}

//m^n����
u32 oled_pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0; t<len; t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            } else enshow=1;

        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}


//z_lenΪ������ʾλ����f_lenΪС����ʾλ����size2Ϊ�����С
void OLED_Showdecimal(u8 x,u8 y,float num,u8 z_len,u8 f_len,u8 size2)
{
    u8 t;
    u8 temp = 0;
    u8 enshow = 0;
    int z_temp = 0, f_temp = 0;
    z_temp=(int)num;
    //��������
    for(t=0; t<z_len; t++)
    {
        temp=(z_temp/oled_pow(10,z_len-t-1))%10;
        if(enshow==0 && t<(z_len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }
            else
                enshow=1;
        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
    //С����
    OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2);

    f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
    //С������
    for(t=0; t<f_len; t++)
    {
        temp=(f_temp/oled_pow(10,f_len-t-1))%10;
        OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2);
    }
}




/*
*************************************************************************
*	�� �� ��: OLED������
*	����˵��: ���I2C�����豸��ʵ���Ƕ�OLED_CheckDevice()�ķ�װ
*	��    �Σ�
*	�� �� ֵ: ����ֵ 0 ��ʾû�м�⵽OLED������1��ʾ��⵽OLED
*************************************************************************
*/

uint8_t OLED_Test(void)
{
    if (OLED_CheckDevice(OLED_ADDRESS) == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
*************************************************************************
*	�� �� ��: IIC_CfgGpio
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
void OLED_IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(OLED_RCC_IIC_PORT, ENABLE);	/* ��GPIOʱ�� */

    GPIO_InitStructure.GPIO_Pin = OLED_IIC_SCL_PIN | OLED_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
    GPIO_Init(OLED_GPIO_PORT_I2C, &GPIO_InitStructure);

    /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
    IIC_Stop();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
/*
*************************************************************************
*	�� �� ��: IIC_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_Delay(void)
{
    uint8_t i;

    /*��
     	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�

    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us
    */
    for (i = 0; i < 10; i++);
}

/*
*************************************************************************
*	�� �� ��: IIC_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    OLED_IIC_SDA_1();
    OLED_IIC_SCL_1();
    IIC_Delay();
    OLED_IIC_SDA_0();
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
}

/*
*************************************************************************
*	�� �� ��: IIC_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
    OLED_IIC_SDA_0();
    OLED_IIC_SCL_1();
    IIC_Delay();
    OLED_IIC_SDA_1();
}

/*
*************************************************************************
*	�� �� ��: IIC_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            OLED_IIC_SDA_1();
        }
        else
        {
            OLED_IIC_SDA_0();
        }
        IIC_Delay();
        OLED_IIC_SCL_1();
        IIC_Delay();
        OLED_IIC_SCL_0();
        if (i == 7)
        {
            OLED_IIC_SDA_1(); // �ͷ�����
        }
        _ucByte <<= 1;	/* ����һ��bit */
        IIC_Delay();
    }
}

/*
*************************************************************************
*	�� �� ��: IIC_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*************************************************************************
*/
static uint8_t IIC_ReadByte(void)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        OLED_IIC_SCL_1();
        IIC_Delay();
        if (OLED_IIC_SDA_READ())
        {
            value++;
        }
        OLED_IIC_SCL_0();
        IIC_Delay();
    }
    return value;
}

/*
*************************************************************************
*	�� �� ��: IIC_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*************************************************************************
*/
static uint8_t IIC_WaitAck(void)
{
    uint8_t re;

    OLED_IIC_SDA_1();	/* CPU�ͷ�SDA���� */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC_Delay();
    if (OLED_IIC_SDA_READ())	/* CPU��ȡSDA����״̬ */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    OLED_IIC_SCL_0();
    IIC_Delay();
    return re;
}

/*
*************************************************************************
*	�� �� ��: IIC_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_Ack(void)
{
    OLED_IIC_SDA_0();	/* CPU����SDA = 0 */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU����1��ʱ�� */
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
    OLED_IIC_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*************************************************************************
*	�� �� ��: IIC_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*************************************************************************
*/
static void IIC_NAck(void)
{
    OLED_IIC_SDA_1();	/* CPU����SDA = 1 */
    IIC_Delay();
    OLED_IIC_SCL_1();	/* CPU����1��ʱ�� */
    IIC_Delay();
    OLED_IIC_SCL_0();
    IIC_Delay();
}

#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


