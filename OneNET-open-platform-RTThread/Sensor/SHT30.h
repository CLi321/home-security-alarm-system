#ifndef _SHT30_H
#define _SHT30_H


#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"


#define write 0 //IIC设备地址一般是7位，也有10位，本程序使用7位IIC设备地址，第8位是写命令
#define read  1 //IIC设备地址一般是7位，也有10位，本程序使用7位IIC设备地址，第8位是读命令

//IIC总线接口定义
#define SCL PBout(6)//设置时钟端口
//SHT30数据SDA传输口，有输出也有输入，所以需配置它的输入和输出
#define SDA_OUT PBout(7)//设置输出数据端口
#define SDA_IN PBin(7)//设置输入数据端口
//设置端口高8位的工作模式（具体可以参照端口位配置表），即I/O输入输出方向设置，先用与（&）对PB7引脚的四个位清0，再用或（|）置1
////#define IIC_INPUT_MODE_SET()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}//改变PB7对应位为1000（CNF[1:0]MODE[1:0]），设置成上拉或者下拉输入
////#define IIC_OUTPUT_MODE_SET() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}//改变PB7对应位为0011，设置成通用推挽输出
//#define IIC_INPUT_MODE_SET()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}//改变PB7对应位为1000（CNF[1:0]MODE[1:0]），设置成上拉或者下拉输入
//#define IIC_OUTPUT_MODE_SET() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}//改变PB7对应位为0011，设置成通用推挽输出

#define IIC_INPUT_MODE_SET()  SHT30_In()  //设置成上拉或者下拉输入
#define IIC_OUTPUT_MODE_SET() SHT30_Out()  //设置成通用推挽输出

extern u8 humiture_buff1[20];


void SHT30_Out(void);
void SHT30_In(void);

void SHT30_Init(void);
void IIC_ACK(void);//确认字符，表示发来的数据已经确认接收无误
void IIC_NACK(void);//无确认字符，表示数据接收有误或者还未接收完成
u8 IIC_wait_ACK(void);//等待接收确认字符
void IIC_Start(void);//等待IIC传输数据开始
void IIC_Stop(void);//IIC传输数据停止
void IIC_SendByte(u8 byte);//IIC发送数据
u8 IIC_RcvByte(void);//IIC接收数据

void SHT30_read_result( u8 addr, u16 *tem, u16 *hum );//SHT30温湿度传感器开始读取数据







#endif

