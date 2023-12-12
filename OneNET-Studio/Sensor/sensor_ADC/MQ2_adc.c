#include "MQ2_adc.h"


#define CAL_PPM 20  // У׼������PPMֵ
#define RL			5		// RL��ֵ
static float R0; // Ԫ���ڽྻ�����е���ֵ

uint32_t times_MQ2 = 0;
uint8_t TIM7_flag = 0;

//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3
void  MQ2_ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE );	   //ʹ��ADC1ͨ��ʱ��


    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

    //PA1 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

    ADC_DeInit(ADC1);  //��λADC1

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���


    ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

    ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼

    while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����

    ADC_StartCalibration(ADC1);	 //����ADУ׼

    while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����

}


//���ADCֵ
//ch:ͨ��ֵ 0~3
uint16_t Get_MQ2_ADC(uint8_t ch)
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

    return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

uint16_t Get_MQ2_ADC_Average(uint8_t ch,uint8_t times)
{
    uint32_t temp_val=0;
    uint8_t t;
    for(t=0; t<times; t++)
    {
        temp_val+=Get_MQ2_ADC(ch);
        delay_ms(5);
    }
    return temp_val/times;
}



void MQ2_cumlate(float RS)
{
    R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);
}

float MQ2_GetPPM(void)
{

		if (TIM7_flag == 0)
    {
				TIM_Cmd(TIM7,ENABLE);//������ʱ��7
				TIM7_flag = 1;
    }	
    u16 adcx;
    float Vrl;
    float RS;
    float ppm;
    adcx = Get_MQ2_ADC_Average(ADC_Channel_12,20);//ADC1,ȡ30�ε�ƽ��ֵ
    Vrl = 3.3f * adcx / 4096.f;  //3.3v�Ĳο���ѹ��4096��
    Vrl = ( (float)( (int)( (Vrl+0.005)*100 ) ) )/100;
    RS = (3.3f - Vrl) / Vrl * RL;

    if(times_MQ2<3000) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼���õ��˶�ʱ��
    {
        R0 = RS / pow(CAL_PPM / 613.9f, 1 / -2.074f);//У׼R0
    }
    else if(times_MQ2>3000) // ��ȡϵͳִ��ʱ�䣬3sǰ����У׼���õ��˶�ʱ��
    {
        TIM_Cmd(TIM7,DISABLE);
    }		
    ppm = 613.9f * pow(RS/R0, -2.074f);

    return  ppm;
}









