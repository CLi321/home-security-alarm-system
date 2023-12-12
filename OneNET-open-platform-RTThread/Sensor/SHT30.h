#ifndef _SHT30_H
#define _SHT30_H


#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"


#define write 0 //IIC�豸��ַһ����7λ��Ҳ��10λ��������ʹ��7λIIC�豸��ַ����8λ��д����
#define read  1 //IIC�豸��ַһ����7λ��Ҳ��10λ��������ʹ��7λIIC�豸��ַ����8λ�Ƕ�����

//IIC���߽ӿڶ���
#define SCL PBout(6)//����ʱ�Ӷ˿�
//SHT30����SDA����ڣ������Ҳ�����룬����������������������
#define SDA_OUT PBout(7)//����������ݶ˿�
#define SDA_IN PBin(7)//�����������ݶ˿�
//���ö˿ڸ�8λ�Ĺ���ģʽ��������Բ��ն˿�λ���ñ�����I/O��������������ã������루&����PB7���ŵ��ĸ�λ��0�����û�|����1
////#define IIC_INPUT_MODE_SET()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}//�ı�PB7��ӦλΪ1000��CNF[1:0]MODE[1:0]�������ó�����������������
////#define IIC_OUTPUT_MODE_SET() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}//�ı�PB7��ӦλΪ0011�����ó�ͨ���������
//#define IIC_INPUT_MODE_SET()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}//�ı�PB7��ӦλΪ1000��CNF[1:0]MODE[1:0]�������ó�����������������
//#define IIC_OUTPUT_MODE_SET() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}//�ı�PB7��ӦλΪ0011�����ó�ͨ���������

#define IIC_INPUT_MODE_SET()  SHT30_In()  //���ó�����������������
#define IIC_OUTPUT_MODE_SET() SHT30_Out()  //���ó�ͨ���������

extern u8 humiture_buff1[20];


void SHT30_Out(void);
void SHT30_In(void);

void SHT30_Init(void);
void IIC_ACK(void);//ȷ���ַ�����ʾ�����������Ѿ�ȷ�Ͻ�������
void IIC_NACK(void);//��ȷ���ַ�����ʾ���ݽ���������߻�δ�������
u8 IIC_wait_ACK(void);//�ȴ�����ȷ���ַ�
void IIC_Start(void);//�ȴ�IIC�������ݿ�ʼ
void IIC_Stop(void);//IIC��������ֹͣ
void IIC_SendByte(u8 byte);//IIC��������
u8 IIC_RcvByte(void);//IIC��������

void SHT30_read_result( u8 addr, u16 *tem, u16 *hum );//SHT30��ʪ�ȴ�������ʼ��ȡ����







#endif

