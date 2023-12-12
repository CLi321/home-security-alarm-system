
#include "Fire_adc.h"


/**
  * @brief  �ҳ�ģ���ȡŨ��ֵ
  * @param  ��
  * @retval ��
  */
float  Fire_GetData(void)
{
	
	  float    ADC_data;
    float    Voltage;

		ADC_data = Fire_Get_ADC_Aveurage(Fire_ADC_CHANNEL,20);
    Voltage = ADC_data/4096.0*3300;   /*ʵ��ƽ����ѹֵ*/
	
		return Voltage;
}


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void Fire_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*�� ADC IO�˿�ʱ��*/
    Fire_ADC_GPIO_AHBxClock_FUN ( Fire_ADC_GPIO_CLK, ENABLE );

    /*���� ADC IO ����ģʽ,����Ϊģ������*/
    GPIO_InitStructure.GPIO_Pin = Fire_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*��ʼ�� ADC IO*/
    GPIO_Init(Fire_ADC_PORT, &GPIO_InitStructure);
	

}


/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void Fire_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /*��ADCʱ��*/
    Fire_ADC_APBxClock_FUN ( Fire_ADC_CLK, ENABLE );

    /* ADC ģʽ����*/
    /* ֻʹ��һ��ADC�����ڶ���ģʽ*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    /*��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ*/
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ;

    /*����ת��ģʽ*/
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

    /*�����ⲿ����ת���������������*/
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    /* ת������Ҷ���*/
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    /*ת��ͨ��1��*/
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    /*��ʼ��ADC*/
    ADC_Init(Fire_ADC, &ADC_InitStructure);

    /*����ADCʱ��ΪPCLK2��8��Ƶ����9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*���� ADC ͨ��ת��˳��Ͳ���ʱ��*/
    ADC_RegularChannelConfig(Fire_ADC, Fire_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    /*����ADC ������ʼת��*/
    ADC_Cmd(Fire_ADC, ENABLE);

    /*��ʼ��ADC У׼�Ĵ���*/
    ADC_ResetCalibration(Fire_ADC);
    /*�ȴ�У׼�Ĵ�����ʼ�����*/
    while(ADC_GetResetCalibrationStatus(Fire_ADC));

    /*ADC��ʼУ׼*/
    ADC_StartCalibration(Fire_ADC);
    /*�ȴ�У׼���*/
    while(ADC_GetCalibrationStatus(Fire_ADC));

//    ADC_DeInit(Fire_ADC);  //��λADC1                 ��bug

}


/**
  * @brief  Fire��ʼ��
  * @param  ��
  * @retval ��
  */
void Fire_ADC_Init(void)
{
    Fire_GPIO_Config();
    Fire_Mode_Config();
}






//���ADCֵ
//ch:ͨ��ֵ 0~3
uint16_t Fire_Get_ADC(uint8_t ch)
{
	

    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    ADC_RegularChannelConfig(Fire_ADC, ch, 1, ADC_SampleTime_239Cycles5 );	//Fire_ADC,ADCͨ��,����ʱ��Ϊ239.5����

    ADC_SoftwareStartConvCmd(Fire_ADC, ENABLE);		//ʹ��ָ����Fire_ADC�����ת����������

    while(!ADC_GetFlagStatus(Fire_ADC, ADC_FLAG_EOC ));//�ȴ�ת������

    return ADC_GetConversionValue(Fire_ADC);	//�������һ��Fire_ADC�������ת�����
}

uint16_t Fire_Get_ADC_Aveurage(uint8_t ch,uint8_t times)
{
    uint32_t temp_val=0;
    uint8_t t;
    for(t=0; t<times; t++)
    {
        temp_val+=Fire_Get_ADC(ch);
        delay_ms(5);
    }
    return temp_val/times;
}



