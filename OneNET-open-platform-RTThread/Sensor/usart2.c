
#include "usart2.h"
#include "usart.h"





//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while((USART2->SR&0X40)==0) {}; //循环发送,直到发送完毕
		USART2->DR = (u8) ch;
    return ch;
}
#endif


void UART2_send_byte(char data)
{
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, data);
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误

uint16_t RxCounter;
char RxBuffer[USART_REC_LEN_cache];     //接收缓冲,最大USART_REC_LEN个字节.



void Clear_Buffer(void)//清空缓存
{
    u16 i;
    Uart1_SendStr(RxBuffer);
    for(i=0; i<USART_REC_LEN_cache; i++)
        RxBuffer[i]=0;//缓存
    RxCounter=0;
    IWDG_Feed();//喂狗

}


void Clear_Buffer_separate(void)//清空缓存
{
    u16 i;
    for(i=0; i<USART_REC_LEN_cache; i++)
        RxBuffer[i]=0;//缓存
    RxCounter=0;
    IWDG_Feed();//喂狗
}



//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记


//初始化IO 串2
//bound:波特率
void uart2_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能，GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//USART2
    USART_DeInit(USART2);  //复位串口2
    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2

    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART2, ENABLE);                    //使能串口

}


void Uart2_SendStr(char*SendBuf)//串口2打印数据
{
    while(*SendBuf)
    {
        while((USART2->SR&0X40)==0);//等待发送完成
        USART2->DR = (u8) *SendBuf;
        SendBuf++;
    }
}




void USART2_IRQHandler(void)                	//串口2中断服务程序
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收模块返回的数据
    {
        RxBuffer[RxCounter++] =USART_ReceiveData(USART2);//接收模块的数据
        if(RxCounter > USART_REC_LEN_cache)//长度自行设定
            RxCounter=0;
    }
}


