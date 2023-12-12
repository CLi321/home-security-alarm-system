
#include "GP2Y10_adc.h"


#define   MAX_NUM     20
#define   RATIO       0.2            /*�ҳ�����Ũ�����ѹֵ����ϵ��*/
#define   NO_DUST_VOLTAGE      100   /*�ҳ�Ũ��Ϊ0ʱ�������ѹֵ*/
#define   TYP					0.5

static  uint16_t  buffer[MAX_NUM];    /*��������ȫ�ֱ���*/


#if 1
/**
  * @brief  �����ڻ���ADC��ѹ�ɼ�����
  * @param  ��
  * @retval ��
  */
void Collect_data(void)
{
    uint8_t  i;

    for(i=0; i<MAX_NUM; i++)
    {
        GPIO_SetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );		//��1  �����ڲ�LED
        /*�ȴ�ģ�������ѹ�ȶ�*/
        delay_us(280);		// ����LED���280us�ĵȴ�ʱ��
			
			
        /* ����û�в����ⲿ����������ʹ���������ADCת�� */
        ADC_SoftwareStartConvCmd(GP2Y_ADC, ENABLE);
        while(ADC_GetFlagStatus(GP2Y_ADC, ADC_FLAG_EOC) != SET);

        /* ���黺��ADCת����ֵ*/
        GP2Y_GetDate(ADC_GetConversionValue(GP2Y_ADC));
        /* ���ADC�����־λ*/
        ADC_ClearFlag(GP2Y_ADC, ADC_FLAG_EOC);

			
				delay_us(19);			  //��ʱ19us����Ϊ����AD����������Ϊ239.5������ADת��һ�����ʱ21us��19��21�ټ�280�պ���320us
        GPIO_ResetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );	 /*�ر�ILED��*/	//��0	
				delay_us(9680);			//��Ҫ�����0.32ms/10ms��PWM�ź������������е�LED
			
    }
}


/**
  * @brief  ���黺��ADC��ֵ
  * @param  valve��ѹ����ֵ
  * @retval ��
  */
void GP2Y_GetDate(uint16_t  valve)
{
    static  uint8_t   k=0;

    buffer[k]=valve;
    k++;
    if(k==MAX_NUM)
    {
        k=0;
    }
}


/**
  * @brief  ��ѹֵת��Ϊ�ҳ�Ũ��ֵ,��λ��g/m3
  * @param  dens  �ҳ�Ũ��ֵ
  * @retval ADC_ConvertFlagŨ��ת����־λ
  */
float  GP2Y_Convert(void)
{
    uint8_t   i;
    uint32_t sum=0;
    float    Voltage,dustVal;

    for(i=0; i<MAX_NUM; i++)
    {
        sum+=buffer[i];
    }
    Voltage=(sum/(float)MAX_NUM)/4096.0*3300;   /*ʵ��ƽ����ѹֵ*/

//    if(Voltage <= NO_DUST_VOLTAGE)
//    {
//        return 0;
//    }
//    else
//    {
//        dustVal=(Voltage-NO_DUST_VOLTAGE)*RATIO;   /*��ѹֵ����ɻҳ�Ũ��ֵ*/
//        return  dustVal;
//    }
//		dustVal = (Voltage/1000.0-NO_DUST_VOLTAGE/1000.0)/TYP*10*1000;  //����1000��λ����ug/m3
		dustVal = ((Voltage/1000.0)*0.167-0.0)*1000;  //����1000��λ����ug/m3
		return dustVal;
}

/**
  * @brief  �ҳ�ģ���ȡŨ��ֵ
  * @param  ��
  * @retval ��
  */
float  GP2Y_GetDens(void)
{
    float  dens;
    /*���ɼ�����ADת����ֵ���浽������*/
    Collect_data();
    dens=GP2Y_Convert();
    return dens;

}
#endif

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void GP2Y_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*�� ADC IO�˿�ʱ��*/
    GP2Y_ADC_GPIO_AHBxClock_FUN ( GP2Y_ADC_GPIO_CLK, ENABLE );

    /*���� ADC IO ����ģʽ,����Ϊģ������*/
    GPIO_InitStructure.GPIO_Pin = GP2Y_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*��ʼ�� ADC IO*/
    GPIO_Init(GP2Y_ADC_PORT, &GPIO_InitStructure);

    /*���� ILED IO ����ģʽ*/
    GPIO_InitStructure.GPIO_Pin = GP2Y_ILED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*��ʼ�� ILED IO*/
    GPIO_Init(GP2Y_ILED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );
}


/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void GP2Y_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /*��ADCʱ��*/
    GP2Y_ADC_APBxClock_FUN ( GP2Y_ADC_CLK, ENABLE );

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
    ADC_Init(GP2Y_ADC, &ADC_InitStructure);

    /*����ADCʱ��ΪPCLK2��8��Ƶ����9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*���� ADC ͨ��ת��˳��Ͳ���ʱ��*/
    ADC_RegularChannelConfig(GP2Y_ADC, GP2Y_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    /*����ADC ������ʼת��*/
    ADC_Cmd(GP2Y_ADC, ENABLE);

    /*��ʼ��ADC У׼�Ĵ���*/
    ADC_ResetCalibration(GP2Y_ADC);
    /*�ȴ�У׼�Ĵ�����ʼ�����*/
    while(ADC_GetResetCalibrationStatus(GP2Y_ADC));

    /*ADC��ʼУ׼*/
    ADC_StartCalibration(GP2Y_ADC);
    /*�ȴ�У׼���*/
    while(ADC_GetCalibrationStatus(GP2Y_ADC));

}


/**
  * @brief  GP2Y��ʼ��
  * @param  ��
  * @retval ��
  */
void GP2Y_ADC_Init(void)
{
    GP2Y_GPIO_Config();
    GP2Y_Mode_Config();
}


#if 0
//���ADCֵ
//ch:ͨ��ֵ 0~3
uint16_t GP2Y_Get_ADC(u8 ch)
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(GP2Y_ADC,ch, 1, ADC_SampleTime_239Cycles5 );  //ADC1,ADCͨ��3,��һ��ת��,����ʱ��Ϊ239.5����                               

	ADC_SoftwareStartConvCmd(GP2Y_ADC,ENABLE);             //ʹ��ָ����ADC1�����ת����������

	while(!ADC_GetFlagStatus(GP2Y_ADC,ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(GP2Y_ADC);      //�������һ��ADC1�������ת�����
}


//��ȡͨ��ch��ת��ֵ
//ȡtimes��,Ȼ��ƽ��
uint16_t GP2Y_Get_ADC_Aveurage(u8 ch, uint8_t times)
{
	u32 pm_val=0;
	u8 t;

	for(t=0;t<times;t++)
	{
		pm_val+=GP2Y_Get_ADC(ch);
		delay_ms(5);
	}
	
	return pm_val/times;
}         

float  GP2Y_GetDens(void)
{
		float ADCVal,Voltage,dustVal;
	
		GPIO_SetBits(GP2Y_ILED_PORT, GP2Y_ILED_PIN);//��1  �����ڲ�LED
		delay_us(280); 			 // ����LED���280us�ĵȴ�ʱ��
		ADCVal = GP2Y_Get_ADC(GP2Y_ADC_CHANNEL);  //PA1 ��������ȡADֵ
		delay_us(19);			  //��ʱ19us����Ϊ����AD����������Ϊ239.5������ADת��һ�����ʱ21us��19��21�ټ�280�պ���320us
		GPIO_ResetBits(GP2Y_ILED_PORT, GP2Y_ILED_PIN);//��0
		delay_us(9680);			//��Ҫ�����0.32ms/10ms��PWM�ź������������е�LED

		Voltage=ADCVal/4096.0*3300;   /*ʵ��ƽ����ѹֵ*/
		dustVal = ((Voltage/1000.0)*0.167-0.0)*1000;  //����1000��λ����ug/m3
		return dustVal;
}
#endif

