
#include "SGP30_iic.h"


/*********************************************������IIC����******************************************************************/

//��ʼ��IIC�ӿ�
void SGP30_Init(void)
{
    IIC_Init();
    SGP30_Write(0x20,0x03);
//	SGP30_ad_write(0x20,0x61);
//	SGP30_ad_write(0x01,0x00);
}

void SGP30_Write(u8 a, u8 b)
{
    IIC_Start();
    IIC_Send_Byte(SGP30_write); //����������ַ+дָ��
    IIC_Wait_Ack();
    IIC_Send_Byte(a);		//���Ϳ����ֽ�
    IIC_Wait_Ack();
    IIC_Send_Byte(b);
    IIC_Wait_Ack();
    IIC_Stop();
    delay_ms(100);
}

uint32_t SGP30_Read(void)
{
    u32 dat;
    u8 crc;
    IIC_Start();
    IIC_Send_Byte(SGP30_read); //����������ַ+��ָ��
    IIC_Wait_Ack();
    dat = IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    crc = IIC_Read_Byte(1); //crc���ݣ���ȥ
    crc = crc;  //Ϊ�˲��ó��ֱ��뾯��
    dat <<= 8;
    dat += IIC_Read_Byte(1);
    dat <<= 8;
    dat += IIC_Read_Byte(0);
    IIC_Stop();
    return(dat);
}




/************************************************IIC���в�������***************************************************************/


//��ʼ��IIC
static void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	SGP30_IIC_SCL_GPIO_CLK | SGP30_IIC_SDA_GPIO_CLK, ENABLE );

    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(SGP30_IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(SGP30_IIC_SCL_GPIO_PORT, SGP30_IIC_SCL_GPIO_PIN); 	//�����

    GPIO_Init(SGP30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(SGP30_IIC_SDA_GPIO_PORT, SGP30_IIC_SDA_GPIO_PIN); 	//�����
}


static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SGP30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SGP30_IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(SGP30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

/***************************************************************************************************************/


//����IIC��ʼ�ź�
static void IIC_Start(void)
{
    SDA_OUT();     	//sda�����
    SGP30_IIC_SDA_1();
    SGP30_IIC_SCL_1();
    delay_us(20);

    SGP30_IIC_SDA_0();	//START:when CLK is high,DATA change form high to low
    delay_us(20);
    SGP30_IIC_SCL_0(); 	//ǯסI2C���ߣ�׼�����ͻ��������
}

//����IICֹͣ�ź�
static void IIC_Stop(void)
{
    SDA_OUT();		//sda�����
    SGP30_IIC_SCL_0();
    SGP30_IIC_SDA_0();	//STOP:when CLK is high DATA change form low to high
    delay_us(20);
    SGP30_IIC_SCL_1();
    SGP30_IIC_SDA_1();	//����I2C���߽����ź�
    delay_us(20);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDA_IN();      		//SDA����Ϊ����
    SGP30_IIC_SDA_1();
    delay_us(10);
    SGP30_IIC_SCL_1();
    delay_us(10);
    while(SGP30_IIC_SDA_READ())
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
//			printf("no ack!\r\n");
            IIC_Stop();
            return 1;
        }
    }
    SGP30_IIC_SCL_0();  		//ʱ�����0
    return 0;
}

//����ACKӦ��
static void IIC_Ack(void)
{
    SGP30_IIC_SCL_0();
    SDA_OUT();
    SGP30_IIC_SDA_0();
    delay_us(20);
    SGP30_IIC_SCL_1();
    delay_us(20);
    SGP30_IIC_SCL_0();
}

//������ACKӦ��
static void IIC_NAck(void)
{
    SGP30_IIC_SCL_0();
    SDA_OUT();
    SGP30_IIC_SDA_1();
    delay_us(20);
    SGP30_IIC_SCL_1();
    delay_us(20);
    SGP30_IIC_SCL_0();
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
static void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    SGP30_IIC_SCL_0(); 	    	//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
            SGP30_IIC_SDA_1();
        else
            SGP30_IIC_SDA_0();
        txd<<=1;
        delay_us(20);
        SGP30_IIC_SCL_1();
        delay_us(20);
        SGP30_IIC_SCL_0();
        delay_us(20);
    }
    delay_us(20);

}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
static u16 IIC_Read_Byte(u8 ack)
{
    u8 i;
    u16 receive=0;
    SDA_IN();//SDA����Ϊ����
    for(i=0; i<8; i++ )
    {
        SGP30_IIC_SCL_0();
        delay_us(20);
        SGP30_IIC_SCL_1();
        receive<<=1;
        if(SGP30_IIC_SDA_READ())
            receive++;
        delay_us(20);
//        printf("%d,%d ",i,receive);
    }
//    printf("\n");
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}


/***************************************************************************************************************/

