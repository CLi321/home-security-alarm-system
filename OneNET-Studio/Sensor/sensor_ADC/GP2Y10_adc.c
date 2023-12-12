
#include "GP2Y10_adc.h"


#define   MAX_NUM     20
#define   RATIO       0.2            /*灰尘颗粒浓度与电压值比例系数*/
#define   NO_DUST_VOLTAGE      100   /*灰尘浓度为0时的输出电压值*/
#define   TYP					0.5

static  uint16_t  buffer[MAX_NUM];    /*缓冲数组全局变量*/


#if 1
/**
  * @brief  数组内缓冲ADC电压采集数据
  * @param  无
  * @retval 无
  */
void Collect_data(void)
{
    uint8_t  i;

    for(i=0; i<MAX_NUM; i++)
    {
        GPIO_SetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );		//置1  开启内部LED
        /*等待模块输出电压稳定*/
        delay_us(280);		// 开启LED后的280us的等待时间
			
			
        /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
        ADC_SoftwareStartConvCmd(GP2Y_ADC, ENABLE);
        while(ADC_GetFlagStatus(GP2Y_ADC, ADC_FLAG_EOC) != SET);

        /* 数组缓冲ADC转化数值*/
        GP2Y_GetDate(ADC_GetConversionValue(GP2Y_ADC));
        /* 清空ADC悬起标志位*/
        ADC_ClearFlag(GP2Y_ADC, ADC_FLAG_EOC);

			
				delay_us(19);			  //延时19us，因为这里AD采样的周期为239.5，所以AD转换一次需耗时21us，19加21再加280刚好是320us
        GPIO_ResetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );	 /*关闭ILED灯*/	//置0	
				delay_us(9680);			//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED
			
    }
}


/**
  * @brief  数组缓冲ADC数值
  * @param  valve电压编码值
  * @retval 无
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
  * @brief  电压值转化为灰尘浓度值,单位μg/m3
  * @param  dens  灰尘浓度值
  * @retval ADC_ConvertFlag浓度转化标志位
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
    Voltage=(sum/(float)MAX_NUM)/4096.0*3300;   /*实际平均电压值*/

//    if(Voltage <= NO_DUST_VOLTAGE)
//    {
//        return 0;
//    }
//    else
//    {
//        dustVal=(Voltage-NO_DUST_VOLTAGE)*RATIO;   /*电压值换算成灰尘浓度值*/
//        return  dustVal;
//    }
//		dustVal = (Voltage/1000.0-NO_DUST_VOLTAGE/1000.0)/TYP*10*1000;  //乘以1000单位换成ug/m3
		dustVal = ((Voltage/1000.0)*0.167-0.0)*1000;  //乘以1000单位换成ug/m3
		return dustVal;
}

/**
  * @brief  灰尘模块获取浓度值
  * @param  无
  * @retval 无
  */
float  GP2Y_GetDens(void)
{
    float  dens;
    /*将采集到的AD转化数值缓存到数组内*/
    Collect_data();
    dens=GP2Y_Convert();
    return dens;

}
#endif

/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void GP2Y_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*打开 ADC IO端口时钟*/
    GP2Y_ADC_GPIO_AHBxClock_FUN ( GP2Y_ADC_GPIO_CLK, ENABLE );

    /*配置 ADC IO 引脚模式,必须为模拟输入*/
    GPIO_InitStructure.GPIO_Pin = GP2Y_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    /*初始化 ADC IO*/
    GPIO_Init(GP2Y_ADC_PORT, &GPIO_InitStructure);

    /*配置 ILED IO 引脚模式*/
    GPIO_InitStructure.GPIO_Pin = GP2Y_ILED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*初始化 ILED IO*/
    GPIO_Init(GP2Y_ILED_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(GP2Y_ILED_PORT,GP2Y_ILED_PIN );
}


/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void GP2Y_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /*打开ADC时钟*/
    GP2Y_ADC_APBxClock_FUN ( GP2Y_ADC_CLK, ENABLE );

    /* ADC 模式配置*/
    /* 只使用一个ADC，属于独立模式*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;

    /*禁止扫描模式，多通道才要，单通道不需要*/
    ADC_InitStructure.ADC_ScanConvMode = DISABLE ;

    /*连续转换模式*/
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

    /*不用外部触发转换，软件开启即可*/
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

    /* 转换结果右对齐*/
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    /*转换通道1个*/
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    /*初始化ADC*/
    ADC_Init(GP2Y_ADC, &ADC_InitStructure);

    /*配置ADC时钟为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /*配置 ADC 通道转换顺序和采样时间*/
    ADC_RegularChannelConfig(GP2Y_ADC, GP2Y_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    /*开启ADC ，并开始转换*/
    ADC_Cmd(GP2Y_ADC, ENABLE);

    /*初始化ADC 校准寄存器*/
    ADC_ResetCalibration(GP2Y_ADC);
    /*等待校准寄存器初始化完成*/
    while(ADC_GetResetCalibrationStatus(GP2Y_ADC));

    /*ADC开始校准*/
    ADC_StartCalibration(GP2Y_ADC);
    /*等待校准完成*/
    while(ADC_GetCalibrationStatus(GP2Y_ADC));

}


/**
  * @brief  GP2Y初始化
  * @param  无
  * @retval 无
  */
void GP2Y_ADC_Init(void)
{
    GP2Y_GPIO_Config();
    GP2Y_Mode_Config();
}


#if 0
//获得ADC值
//ch:通道值 0~3
uint16_t GP2Y_Get_ADC(u8 ch)
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(GP2Y_ADC,ch, 1, ADC_SampleTime_239Cycles5 );  //ADC1,ADC通道3,第一个转换,采样时间为239.5周期                               

	ADC_SoftwareStartConvCmd(GP2Y_ADC,ENABLE);             //使能指定的ADC1的软件转换启动功能

	while(!ADC_GetFlagStatus(GP2Y_ADC,ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(GP2Y_ADC);      //返回最近一次ADC1规则组的转换结果
}


//获取通道ch的转换值
//取times次,然后平均
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
	
		GPIO_SetBits(GP2Y_ILED_PORT, GP2Y_ILED_PIN);//置1  开启内部LED
		delay_us(280); 			 // 开启LED后的280us的等待时间
		ADCVal = GP2Y_Get_ADC(GP2Y_ADC_CHANNEL);  //PA1 采样，读取AD值
		delay_us(19);			  //延时19us，因为这里AD采样的周期为239.5，所以AD转换一次需耗时21us，19加21再加280刚好是320us
		GPIO_ResetBits(GP2Y_ILED_PORT, GP2Y_ILED_PIN);//置0
		delay_us(9680);			//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED

		Voltage=ADCVal/4096.0*3300;   /*实际平均电压值*/
		dustVal = ((Voltage/1000.0)*0.167-0.0)*1000;  //乘以1000单位换成ug/m3
		return dustVal;
}
#endif

