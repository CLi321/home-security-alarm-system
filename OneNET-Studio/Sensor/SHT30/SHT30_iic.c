
#include "SHT30_iic.h"

/*
�趨�¶���ֵ��Χ��-20�桪��125��
�趨���ʪ�ȷ�Χ��0%����100%
*/



//������ʪ�ȴ洢����
u8 humiture_buff1[20];


void SHT30_Init(void)
{
    IIC_Init();
}



//�������մ������ɼ����ϳ���ʪ��
void SHT30_read_result( u8 addr, u16 *tem, u16 *hum )
{
    //SHT30�����ֶ�ȡ��ֵ�ķ������ֱ���״̬��ѯ����ֵ��ѯ��������ʹ�õ�����ֵ��ѯ������ָ��Ϊ0x2C06
////    u16 tem,hum;//��������ʪ�ȼ��㹫ʽ�ı���
    u16 buff[6];//����6���ֽڵ����飬����¶ȸߡ��Ͱ�λ��ʪ�ȸߡ��Ͱ�λ�������ֽڵ�CRCУ��λ
////    float Temperature=0;//�������¶�ƴ�ӵı���Temperature����ʼ���¶�Ϊ0
////    float Humidity=0;//������ʪ��ƴ�ӵı���Humidity����ʼ��ʪ��Ϊ0
    //����ָ��Ϊ0x2C06��Ĭ�ϣ�
    IIC_Start();
    IIC_SendByte(addr<<1 | SHT30_write);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊдȡλ
    IIC_wait_ACK();
//	delay_us(1);
    IIC_SendByte(0x2C);//ǰ��η���ָ��Ϊ0x2C
    IIC_wait_ACK();
//	delay_us(1);
    IIC_SendByte(0x06);//���η���ָ��Ϊ0x06
    IIC_wait_ACK();
//	delay_us(1);
    IIC_Stop();
    delay_ms(50);
    IIC_Start();
    IIC_SendByte(addr<<1 | SHT30_read);//д7λI2C�豸��ַ��1��Ϊ��ȡλ,1Ϊ��ȡλ
//SHT30���ص���ֵ��6��Ԫ�ص�����
    if(IIC_wait_ACK()==0)
    {
//  	delay_us(1);
        buff[0]=IIC_RcvByte();//�����¶ȸ�8λ
        IIC_ACK();
        buff[1]=IIC_RcvByte();//�����¶ȵ�8λ
        IIC_ACK();
        buff[2]=IIC_RcvByte();//�¶�crcУ��λ
        IIC_ACK();
        buff[3]=IIC_RcvByte();//����ʪ�ȸ�8λ
        IIC_ACK();
        buff[4]=IIC_RcvByte();//����ʪ�ȵ�8λ
        IIC_ACK();
        buff[5]=IIC_RcvByte();//ʪ��crcУ��λ
        IIC_NACK();
        IIC_Stop();
    }

    *tem = ((buff[0]<<8) | buff[1]);//��buff[0]�ɼ������¶�8λ������buff[1]�ɼ����ĵ�8λ�������ʵ���¶�ƴ��
    *hum = ((buff[3]<<8) | buff[4]);//��buff[3]�ɼ�����ʪ��8λ������buff[4]�ɼ����ĵ�8λ�������ʵ��ʪ��ƴ��



}



/***************************************************************************************************************/

//��ʼ��SHT30��SCL��SDA�ӿ�
static void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	SHT30_IIC_SCL_GPIO_CLK | SHT30_IIC_SDA_GPIO_CLK, ENABLE );

    GPIO_InitStructure.GPIO_Pin = SHT30_IIC_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(SHT30_IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(SHT30_IIC_SCL_GPIO_PORT, SHT30_IIC_SCL_GPIO_PIN); 	//�����

    GPIO_Init(SHT30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(SHT30_IIC_SDA_GPIO_PORT, SHT30_IIC_SDA_GPIO_PIN); 	//�����

//    SHT30_IIC_SCL_1();//����SHT30ʱ��Ҫ��Ĭ��ʱ���Ǹߵ�ƽ��SDA����ʱ��SCL�������ͣ���ʼ���ݵĴ��䣩
//    SHT30_IIC_SDA_1();//�ߵ�ƽ��δ��ʼ���ݵĴ��䣬����ʱ��ʼ���ݵĴ���

}


static void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SHT30_IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SHT30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

static void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SHT30_IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SHT30_IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}



/***************************************************************************************************************/


//����IIC���߽���ͨѶ,����IICͨѶ����ʼ����
static void IIC_Start(void)//����IICͨѶ
{
    SDA_OUT();//����PB7��SDAΪ�������
    SHT30_IIC_SDA_1();//����SHT30ʱ������֮ǰ��SDA����Ϊ�ߵ�ƽ
    SHT30_IIC_SCL_1();//����SHT30ʱ������֮ǰ��SCL����Ϊ�ߵ�ƽ
    delay_us(4);//��ʱ4us����Ӳ��һ����Ӧʱ��
    SHT30_IIC_SDA_0();//SDA�������ͣ���ʼ���ݵĴ���
    delay_us(4);
    SHT30_IIC_SCL_0();//SCL���ͣ���SDA��Ӧ����ӦSDA���ݵ����ͣ���ʾ��ʽ��ʼ���ݵĴ���
}

//����IIC����ͨѶ������IICͨѶ�Ľ�������
static void IIC_Stop(void)
{
    SDA_OUT();
    SHT30_IIC_SCL_0();//����IICͨѶ֮ǰ���鿴SCL�Ƿ�������״̬
    SHT30_IIC_SDA_0();//����IICͨѶ֮ǰ���鿴SDA�Ƿ�������״̬
    delay_us(4);
    SHT30_IIC_SCL_1();//��ʱ�����ߣ���ʾ�Ѿ�����IICͨѶ
    delay_us(4);
    SHT30_IIC_SDA_1();//�����ݴ����������ߣ���ʾ��ʽ�������ݵĴ���
    delay_us(4);
}

//�����ȴ��ӻ���ȷ���ַ�ACK
static uint8_t IIC_wait_ACK(void)//�����ȴ��ӻ�����ACK���Ӷ��ж������Ƿ�������
{
    u8 t = 200;
    SDA_OUT();
    SHT30_IIC_SDA_1();//8λ��������ͷ������ߣ�׼������Ӧ��λ
    delay_us(1);
    SHT30_IIC_SCL_0();
    delay_us(1);
    SDA_IN();
    delay_us(1);
    while(SHT30_IIC_SDA_READ())//�ȴ�SHT30Ӧ��
    {
        t--;
        delay_us(1);
        if(t==0)
        {
            SHT30_IIC_SCL_0();
            return 1;
        }
        delay_us(1);
    }
    delay_us(1);
    SHT30_IIC_SCL_1();
    delay_us(1);
    SHT30_IIC_SCL_0();
    delay_us(1);
    return 0;
}


//��������ȷ���ַ�ACK
static void IIC_ACK(void)//IICЭ�飬��Ҫ��������ȷ���ַ�ACK���ж������Ƿ��������
{
    SDA_OUT();//����PB7��SDA���Ϊ�������ģʽ
    SHT30_IIC_SCL_0();
    delay_us(2);
    SHT30_IIC_SDA_0();
    delay_us(2);
    SHT30_IIC_SCL_1();//���ݽ������֮�󣬽�SCL���ߣ��Ա��������������ACK���ӻ�
    delay_us(2);
    SHT30_IIC_SCL_0();//���ݽ�����ɣ���������ACK
    delay_us(1);
}
//����������ȷ���ַ�ACK
static void IIC_NACK(void)//IICЭ�飬���ݴ���δ��ɻ������ݴ�����������������ȷ���ַ�ACK
{
    SDA_OUT();//����PB7��SDA���Ϊ�������ģʽ
    SHT30_IIC_SCL_0();
    delay_us(2);
    SHT30_IIC_SDA_1();
    delay_us(2);
    SHT30_IIC_SCL_1();
    delay_us(2);
    SHT30_IIC_SCL_0();
    delay_us(1);
}

//��byte���ݷ��ͳ�ȥ
static void  IIC_SendByte(u8 byte)
{
    u8  Count;
    SDA_OUT();
    SHT30_IIC_SCL_0();//��ʱ�����ͣ���ʼ���ݵĴ���
    for(Count=0; Count<8; Count++) //Ҫ���͵����ݳ���Ϊ8λ
    {
        if(byte&0x80) SHT30_IIC_SDA_1();//�жϷ���λ������λΪ1����δ��������
        else SHT30_IIC_SDA_0(); //�жϷ���λΪ0����ʼ���ݵķ���
        byte<<=1;
        delay_us(2);
        SHT30_IIC_SCL_1();
        delay_us(2);
        SHT30_IIC_SCL_0();
        delay_us(2);
    }
}

// �������մ���������������
static uint8_t IIC_RcvByte(void)
{
    u8 retc;
    u8 Count;
    retc=0;
    SDA_IN();//����������Ϊ���뷽ʽ
    delay_us(1);
    for(Count=0; Count<8; Count++)
    {
        SHT30_IIC_SCL_0();//����ʱ����Ϊ�ͣ�׼����������λ
        delay_us(2);
        SHT30_IIC_SCL_1();//����ʱ����Ϊ��ʹ��������������Ч
        retc=retc<<1;
        if(SHT30_IIC_SDA_READ()) retc |=1;//������λ,���յ�����λ����retc��
        delay_us(1);
    }
    SHT30_IIC_SCL_0();
    return(retc);
}



/***************************************************************************************************************/

