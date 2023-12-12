
#include "deal_with.h"


_DATA_STM32 						STM32 = {0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
_DATA_WeChat 					 	WeChat = {0, 0, 0, 0, 0, 0, 0, 13244621508, 1, 0, 0, 0, 0};
_DATA_WeChat_cache 			WeChat_cache =  {0, 0, 0, 0, 0, 0, 0, 13244621508, 1, 0, 0, 0, 0};

_SENSOR 								SENSOR = {0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 1, 0};

STRUCT_NVICPriority NVICPriority_Structure;


extern _Detection_Target_LD2410C		Detection_Target_LD2410C;

uint8_t KEY1_flag = 0, KEY2_flag = 0;



void deal_with(void)
{


    Calculate_SGP30( &SENSOR.CO2_cache, &SENSOR.TVOC_cache);
    Calculate_SHT30( &SENSOR.temp_cache, &SENSOR.hum_cache);


    SENSOR.dust_cache = GP2Y_GetDens();				//��ȡGP2Y�۳��������Ļҳ�Ũ��ֵ
    SENSOR.MQ2_cache  = MQ2_GetPPM();				//MQ2������

    SENSOR.people_state_cache = Detection_Target_LD2410C.STATE_target;
    SENSOR.motion_distance_cache = Detection_Target_LD2410C.MOTION_target_distance;
    SENSOR.static_distance_cache = Detection_Target_LD2410C.STATIC_target_distance;

    //FIRE���洫����
    if(GPIO_ReadInputDataBit(GPIO_FIRE_PORT,GPIO_FIRE_PIN))
    {
        SENSOR.Fire_cache = 1;
    }
}


void Calculate_SGP30( uint32_t *CO2_cache, uint32_t *TVOC_cache)
{
    uint32_t   CO2Data,TVOCData;//����CO2Ũ�ȱ�����TVOCŨ�ȱ���
    uint32_t   sgp30_dat;//SGP30��ȡ����ֵ


    SGP30_Write(0x20,0x08);
    sgp30_dat = SGP30_Read();//��ȡSGP30��ֵ

    CO2Data = (sgp30_dat & 0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
    TVOCData = sgp30_dat & 0x0000ffff;       //ȡ��TVOCֵ

    *CO2_cache = CO2Data;
    *TVOC_cache = TVOCData;
}


void Calculate_SHT30(float *temp_cache, float *hum_cache)
{
    u16 tem=0, hum=0;


    SHT30_read_result( 0x44, &tem, &hum );//ADDR�Ĵ�������ַѡ�����ţ���GND��ַΪ0x44����VDD��ַΪ0x45

    //��ѯSHT30�����ֲ��֪����ʪ�ȵļ��㷽������
    *temp_cache= 175.0*(float)tem/65535.0-45.0 ;// T = -45 + 175 * tem / (2^16-1)
    *hum_cache= 100.0*(float)hum/65535.0;// RH = hum*100 / (2^16-1)

    if((*temp_cache>=-20)&&(*temp_cache<=125)&&(*hum_cache>=0)&&(*hum_cache<=100))//�趨�¶Ⱥ�ʪ�ȵ���ֵ�����������ֵ�򷵻ش�����ʾ
    {
//        printf("�¶ȣ�%6.2f��\r\n",Temperature)
//        printf("ʪ�ȣ�%6.2f%%\r\n",Humidity);
        *temp_cache = *temp_cache;
        *hum_cache = *hum_cache;
    }
    else
    {
//        printf("��ʪ�ȳ���������ֵ��\r\n");
        *temp_cache = 0.0;
        *hum_cache = 0.0;
    }
    hum=0;
    tem=0;


}


void Init_myself(void)
{
    uart_init(115200); //���ڳ�ʼ��Ϊ115200
    delay_init(72);	   //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�


    NVICPriority_Structure.Usart3 = 2;
    USART3_Init();
    EXTI_Key_Config(); //��ʼ��EXTI�жϣ����°����ᴥ���жϣ������жϻ����stm32f1xx_it.c�ļ��еĺ���



    LED_GPIO_Config();
//		BEEP_GPIO_Config();		//������ GPIO ��ʼ��
//	  RELAY_GPIO_Config();	//�̵��� GPIO ��ʼ��
//		FIRE_CONFIG();		//��ʼ�����洫����
    GP2Y_ADC_Init();			//GP2Y��ADC��ʼ��
    MQ2_ADC_Init();		 //MQ135��ADC��ʼ��
    TIM7_Init(1000,71);

    OLED_Init();			//��ʼ��OLED
    OLED_Clear();

    SHT30_Init();//��ʼ��SHT30

    SGP30_Init();   //��ʼ��SGP30

    GENERAL_TIM_Init();


    UART5_Config();

    uart4_init(256000);		//�Խ��״�

    uart2_init(115200);//�Խ�ML302
    delay_ms(500);

    IWDG_Init(6,2345);    //15.008sһ��

//		ML302_Init();



}

void Face_recognition_myself(void)
{
    if ( KEY1_flag == 1)
    {
        HLK_TX510_Enroll_FaceID();
        Dealwith_HLK_TX510();
        delay_s(2);
        KEY1_flag = 0;
    }
    else if ( KEY2_flag == 1)
    {
        HLK_TX510_DeleteAll_FaceID();
        Dealwith_HLK_TX510();
        delay_s(2);
        KEY2_flag = 0;
    }
    else
    {
        HLK_TX510_Identify_FaceID();
        Dealwith_HLK_TX510();
        delay_s(2);
    }
		IWDG_Feed();//ι��
}

void Getting_Data_myself(void)
{
    deal_with();
    STM32.temp	= SENSOR.temp_cache;
    STM32.hum 	= SENSOR.hum_cache;
    STM32.CO2 	= SENSOR.CO2_cache;
    STM32.TVOC	= SENSOR.TVOC_cache;
    STM32.dust 	= SENSOR.dust_cache;
    STM32.MQ2 	= SENSOR.MQ2_cache;
    STM32.Fire  = SENSOR.Fire_cache;

    STM32.people_state 		= SENSOR.people_state_cache;
    STM32.motion_distance = SENSOR.motion_distance_cache;
    STM32.static_distance = SENSOR.static_distance_cache;
	
		IWDG_Feed();//ι��
}


void printf_myself(void)
{

		rt_kprintf("״̬: %d,  �˶�����: %dcm,  ��ֹ����: %dcm\r\n",STM32.people_state,STM32.motion_distance,STM32.static_distance);
		rt_kprintf("�¶�: %.2f��,  ʪ��:%.2f%%\r\n",STM32.temp,STM32.hum);
		rt_kprintf("CO2: %dppm,  TVOC:%dppd\r\n",STM32.CO2,STM32.TVOC);
		rt_kprintf("�ҳ�Ũ��: %.4f��g/m3\r\n",STM32.dust);
		rt_kprintf("smoke: %.2f ppm\r\n", STM32.MQ2);
		rt_kprintf("\r\n");

}


void OSD_myself(void)
{
//    if ( STM32.CO2 == 400 && STM32.TVOC == 0 )
//    {
//        //OLED_Clear();
//        //OLED_ShowString(2,6,(u8 *)"SGP30 is detecting",16);
//    }
//    else
//    {
//        OLED_Clear();
//        OLED_ShowNum(0,0,STM32.TVOC,4,16);
//        OLED_ShowNum(0,3,STM32.CO2,4,16);
//    }
	
		OLED_Clear();
		OLED_ShowNum(0,0,STM32.TVOC,4,16);
		OLED_ShowNum(0,3,STM32.CO2,4,16);
//    OLED_ShowNum(35,0,STM32.people_state,4,16);
//    OLED_ShowNum(35,3,STM32.motion_distance,4,16);
    OLED_Showdecimal(35,0,STM32.temp,2,2,16);
    OLED_Showdecimal(35,3,STM32.hum,2,2,16);
    OLED_Showdecimal(80,0,STM32.MQ2,3,2,16);
    OLED_Showdecimal(80,3,STM32.dust,3,2,16);
		
		IWDG_Feed();//ι��
//    delay_s(1);
}


void Upload_Receive_Data_STM32(void)
{

    ML302_MQTTPUBMessage_STM32(STM32.temp, STM32.hum, STM32.dust, STM32.MQ2, STM32.TVOC, STM32.CO2, STM32.relay,\
                               STM32.Fire, STM32.people_state, STM32.motion_distance, STM32.static_distance,\
                               STM32.LED, STM32.BEEP);
    ML302_MQTT_RECData();
		IWDG_Feed();//ι��

}

void Upload_Receive_Data_wechat(void)
{

    ML302_MQTTPUBMessage_WeChat( WeChat.temp, WeChat.hum, WeChat.dust, WeChat.MQ2, WeChat.TVOC, WeChat.CO2, WeChat.relay,\
                                 WeChat.phone,\
                                 WeChat.ASR_ifwork, WeChat.LED_ifwork, WeChat.BEEP_ifwork, WeChat.ML302_ifwork, WeChat.OLED_ifwork);
    ML302_MQTT_RECData();
		IWDG_Feed();//ι��
}


void Warning_contorl_myself(void)
{
    char  *strx_call;
    uint32_t  i_call = 60000;

    if (WeChat.ML302_ifwork)
    {
        if ( STM32.dust < 10.0 || STM32.dust > 30.0)
        {
            ML302_SendMessage("13244621508","error");//����Ӣ�Ķ��ţ������ֱ����ֻ������Ӣ���ַ�����
            delay_ms(500);
        }

        if ( STM32.MQ2 < 10.0 || STM32.MQ2 > 80.0 )
        {
            printf("ATD13244621508;\r\n");//����绰,����������֧�ֵ绰
            delay_ms(500);
            Clear_Buffer();

            while ( i_call -- )
            {
                strx_call=strstr((const char*)RxBuffer,(const char*)"NO CARRIER");//�������˽���
                if(strx_call)
                {
                    Uart1_SendStr("ͨ���Ѿ�����\r\n");
                    Clear_Buffer();
                }
            }
        }
    }


    if(WeChat.relay == 1)
    {
        RELAY(RELAY_ON);			  //�̵�����
        delay_s(1);
        RELAY(RELAY_OFF);		   // �̵�����
        delay_s(1);
        RELAY(RELAY_ON);			  //�̵�����
        delay_s(1);
    }

    if (!WeChat.LED_ifwork)
    {
        LED1(LED_ON);			  // LED��
        delay_s(1);
        LED1(LED_OFF);		   // LED��
        delay_s(1);
    }

    if(WeChat.BEEP_ifwork)
    {
        BEEP(BEEP_ON); 			  // ��������
        delay_s(1);
        BEEP(BEEP_OFF);		  //����������
        delay_s(1);
        BEEP(BEEP_ON); 			  // ��������
        delay_s(1);
    }


}



/*********************************************END OF FILE**********************/