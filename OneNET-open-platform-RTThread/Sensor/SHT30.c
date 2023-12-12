#include "SHT30.h"

/*
�趨�¶���ֵ��Χ��-20�桪��125��
�趨���ʪ�ȷ�Χ��0%����100%
*/



//������ʪ�ȴ洢����
u8 humiture_buff1[20];


void SHT30_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void SHT30_In(void)
{
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SHT30_Init(void)//��ʼ��SHT30��SCL��SDA�ӿ�
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//����SCL���ӵ�STM32��PB6�����ϣ�����SDA_OUT/SDA_IN���ӵ�STM32��PB7������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SCL=1;//����SHT30ʱ��Ҫ��Ĭ��ʱ���Ǹߵ�ƽ��SDA����ʱ��SCL�������ͣ���ʼ���ݵĴ��䣩
    SDA_OUT=1;//�ߵ�ƽ��δ��ʼ���ݵĴ��䣬����ʱ��ʼ���ݵĴ���
}
//��������ȷ���ַ�ACK
void IIC_ACK(void)//IICЭ�飬��Ҫ��������ȷ���ַ�ACK���ж������Ƿ��������
{
    IIC_OUTPUT_MODE_SET();//����PB7��SDA���Ϊ�������ģʽ
    SCL=0;
    delay_us(2);
    SDA_OUT=0;
    delay_us(2);
    SCL=1;//���ݽ������֮�󣬽�SCL���ߣ��Ա��������������ACK���ӻ�
    delay_us(2);
    SCL=0;//���ݽ�����ɣ���������ACK
    delay_us(1);
}
//����������ȷ���ַ�ACK
void IIC_NACK(void)//IICЭ�飬���ݴ���δ��ɻ������ݴ�����������������ȷ���ַ�ACK
{
    IIC_OUTPUT_MODE_SET();//����PB7��SDA���Ϊ�������ģʽ
    SCL=0;
    delay_us(2);
    SDA_OUT=1;
    delay_us(2);
    SCL=1;
    delay_us(2);
    SCL=0;
    delay_us(1);
}
//�����ȴ��ӻ���ȷ���ַ�ACK
u8 IIC_wait_ACK(void)//�����ȴ��ӻ�����ACK���Ӷ��ж������Ƿ�������
{
    u8 t = 200;
    IIC_OUTPUT_MODE_SET();
    SDA_OUT=1;//8λ��������ͷ������ߣ�׼������Ӧ��λ
    delay_us(1);
    SCL=0;
    delay_us(1);
    IIC_INPUT_MODE_SET();
    delay_us(1);
    while(SDA_IN)//�ȴ�SHT30Ӧ��
    {
        t--;
        delay_us(1);
        if(t==0)
        {
            SCL=0;
            return 1;
        }
        delay_us(1);
    }
    delay_us(1);
    SCL=1;
    delay_us(1);
    SCL=0;
    delay_us(1);
    return 0;
}
//����IIC���߽���ͨѶ,����IICͨѶ����ʼ����
void IIC_Start(void)//����IICͨѶ
{
    IIC_OUTPUT_MODE_SET();//����PB7��SDAΪ�������
    SDA_OUT=1;//����SHT30ʱ������֮ǰ��SDA����Ϊ�ߵ�ƽ
    SCL=1;//����SHT30ʱ������֮ǰ��SCL����Ϊ�ߵ�ƽ
    delay_us(4);//��ʱ4us����Ӳ��һ����Ӧʱ��
    SDA_OUT=0;//SDA�������ͣ���ʼ���ݵĴ���
    delay_us(4);
    SCL=0;//SCL���ͣ���SDA��Ӧ����ӦSDA���ݵ����ͣ���ʾ��ʽ��ʼ���ݵĴ���
}

//����IIC����ͨѶ������IICͨѶ�Ľ�������
void IIC_Stop(void)
{
    IIC_OUTPUT_MODE_SET();
    SCL=0;//����IICͨѶ֮ǰ���鿴SCL�Ƿ�������״̬
    SDA_OUT=0;//����IICͨѶ֮ǰ���鿴SDA�Ƿ�������״̬
    delay_us(4);
    SCL=1;//��ʱ�����ߣ���ʾ�Ѿ�����IICͨѶ
    delay_us(4);
    SDA_OUT=1;//�����ݴ����������ߣ���ʾ��ʽ�������ݵĴ���
    delay_us(4);
}

//��byte���ݷ��ͳ�ȥ
void  IIC_SendByte(u8 byte)
{
    u8  Count;
    IIC_OUTPUT_MODE_SET();
    SCL=0;//��ʱ�����ͣ���ʼ���ݵĴ���
    for(Count=0; Count<8; Count++) //Ҫ���͵����ݳ���Ϊ8λ
    {
        if(byte&0x80) SDA_OUT=1;//�жϷ���λ������λΪ1����δ��������
        else SDA_OUT=0; //�жϷ���λΪ0����ʼ���ݵķ���
        byte<<=1;
        delay_us(2);
        SCL=1;
        delay_us(2);
        SCL=0;
        delay_us(2);
    }
}

// �������մ���������������
u8 IIC_RcvByte(void)
{
    u8 retc;
    u8 Count;
    retc=0;
    IIC_INPUT_MODE_SET();//����������Ϊ���뷽ʽ
    delay_us(1);
    for(Count=0; Count<8; Count++)
    {
        SCL=0;//����ʱ����Ϊ�ͣ�׼����������λ
        delay_us(2);
        SCL=1;//����ʱ����Ϊ��ʹ��������������Ч
        retc=retc<<1;
        if(SDA_IN) retc |=1;//������λ,���յ�����λ����retc��
        delay_us(1);
    }
    SCL=0;
    return(retc);
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
    IIC_SendByte(addr<<1 | write);//д7λI2C�豸��ַ��0��Ϊдȡλ,1Ϊдȡλ
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
    IIC_SendByte(addr<<1 | read);//д7λI2C�豸��ַ��1��Ϊ��ȡλ,1Ϊ��ȡλ
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




