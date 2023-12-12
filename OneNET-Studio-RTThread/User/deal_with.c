
#include "deal_with.h"


_SENSOR 								SENSOR     = {0.0, 0.0, 0.0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0};
_ALARM									ALARM			 = {0, 0, 0, 0, 0, 0, 0, 0, 13244621508, 0, 0, 0, 0};
_IFWORK									IFWORK	   = {1, 1, 1, 1, 1};


_DATA_Interaction 			STM32      = {0.0, 0.0, 0.0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0,\
                                      0, 0, 0, 0, 0, 0, 0, 0, 13244621508, 0, 0, 0, 0,\
                                      1, 1, 1, 1, 1};
_DATA_Interaction   		REC_cache  = {0.0, 0.0, 0.0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0,\
                                      0, 0, 0, 0, 0, 0, 0, 0, 13244621508, 0, 0, 0, 0,\
                                      1, 1, 1, 1, 1};
_DATA_Interaction 			Desired    = {0.0, 0.0, 0.0, 0, 0, 0.0, 0.0, 0, 0, 0, 0, 0, 0,\
                                      0, 0, 0, 0, 0, 0, 0, 0, 13244621508, 0, 0, 0, 0,\
                                      1, 1, 1, 1, 1};

																			
_ONENET_IF							ONENET_IF	 = {1, 1, 1, 1, 1};
_ASR_IF									ASR_IF		 = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};


uint8_t phone_num[12] = "13244621508";
uint8_t call_num[20]  = "ATD13244621508;\r\n";

STRUCT_NVICPriority NVICPriority_Structure;


extern _Detection_Target_LD2410C		Detection_Target_LD2410C;

uint8_t KEY1_flag = 0, KEY2_flag = 0, KEY3_flag = 0, KEY4_flag = 0;


extern rt_sem_t 	ALARM_sem;
extern rt_sem_t 	SENSOR_sem;
extern rt_sem_t 	Interactive_sem;



void First_function(void)
{

    ML302_Init();

    OLED_CLS();
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,22);
    OLED_ShowCN(35,4,23);
    OLED_ShowCN(55,4,24);
    OLED_ShowCN(75,4,25);
    OLED_ShowCN(95,4,26);
    delay_s(1);
    OLED_CLS();

    IWDG_Feed();//ι��

    rt_sem_release(ALARM_sem);
}


void ALARM_function(void)
{

//		GET_Data_ALARM_myself();
//		IWDG_Feed();//ι��
//		ALARM_Handle_myself();
//		IWDG_Feed();//ι��

    GET_Data_ALARM_myself();
    IWDG_Feed();//ι��

    ALARM_Handle_myself();
    IWDG_Feed();//ι��

	

    if ( ALARM.Fire == 0 && ALARM.Smoke_Gas == 0 && ALARM.Trespass == 0 )
        rt_sem_release(SENSOR_sem);
    else
        rt_sem_release(ALARM_sem);

}


void SENSOR_function(void)
{

    GET_Data_SENSOR_myself();
    IWDG_Feed();//ι��

		GET_Data_ALARM_myself();
		IWDG_Feed();//ι��
		ALARM_Handle_myself();
		IWDG_Feed();//ι��

	
	
    printf_myself();
		IWDG_Feed();//ι��	
	
		GET_Data_ALARM_myself();
		IWDG_Feed();//ι��
		ALARM_Handle_myself();
		IWDG_Feed();//ι��

	

		OSD_myself();
    IWDG_Feed();//ι��



    if ( ALARM.Fire == 0 && ALARM.Smoke_Gas == 0 && ALARM.Trespass == 0 )
        rt_sem_release(Interactive_sem);
    else
        rt_sem_release(ALARM_sem);
}

void Interactive_function(void)
{
	
//    delay_s(2);
    GET_Data_Desired_myself();
    IWDG_Feed();//ι��

	
		GET_Data_ALARM_myself();
		IWDG_Feed();//ι��
		ALARM_Handle_myself();
		IWDG_Feed();//ι��
		OSD_myself();
			
	
	

    deal_with();


		GET_Data_ALARM_myself();
		IWDG_Feed();//ι��
		ALARM_Handle_myself();
		IWDG_Feed();//ι��
		OSD_myself();


	
	
//    delay_s(2);
    POST_Data_myself();
    IWDG_Feed();//ι��




    rt_sem_release(ALARM_sem);
}



void Init_myself(void)
{
    uart_init(115200); //���ڳ�ʼ��Ϊ115200
    delay_init(72);	   //��ʱ������ʼ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    NVICPriority_Structure.Usart3 = 2;


    LED_GPIO_Config();
    BEEP_GPIO_Config();		//������ GPIO ��ʼ��
    RELAY_GPIO_Config();	//�̵��� GPIO ��ʼ��

    OLED_Init();		//��ʼ��OLED
    OLED_CLS();
    OLED_ShowCN(45,2,18);
    OLED_ShowCN(65,2,19);
    OLED_ShowCN(15,4,20);
    OLED_ShowCN(35,4,21);
    OLED_ShowCN(55,4,22);
    OLED_ShowCN(75,4,23);
    OLED_ShowCN(95,4,24);


    SHT30_Init();		//��ʼ��SHT30
    SGP30_Init();   //��ʼ��SGP30

    GP2Y_ADC_Init();			//GP2Y��ADC��ʼ��
    MQ2_ADC_Init();		 		//MQ135��ADC��ʼ��
    TIM7_Init(1000,71);
    Fire_ADC_Init();		  //��ʼ�����洫������AD��ȡ�������������


    USART3_Init();				//�Խ�����ʶ��TX510
    UART5_Config();				//�Խ�����ʶ��ASR
    uart4_init(256000);		//�Խ��״�LD2410C
    uart2_init(115200);   //�Խ�4Gģ��ML302

    delay_ms(500);

//    EXTI_Key_Config(); 		//��ʼ��EXTI�жϣ����°����ᴥ���жϣ������жϻ����stm32f1xx_it.c�ļ��еĺ���
//		Key_GPIO_Config();
    IWDG_Init(6,1250);    //8Sһ��

//    ML302_Init();

//    OLED_CLS();
//    OLED_ShowCN(45,2,18);
//    OLED_ShowCN(65,2,19);
//    OLED_ShowCN(15,4,22);
//    OLED_ShowCN(35,4,23);
//    OLED_ShowCN(55,4,24);
//    OLED_ShowCN(75,4,25);
//    OLED_ShowCN(95,4,26);
//    delay_s(1);
//    OLED_CLS();
    IWDG_Feed();//ι��
}

void GET_Data_ALARM_myself(void)
{
    SENSOR.MQ2_cache = MQ2_GetPPM();				//MQ2��������õ�Ũ��
    SENSOR.Voltage_cache = Fire_GetData();  //��ȡ���洫��������������Ķ�Ӧ��ѹֵ

    SENSOR.people_state_cache = Detection_Target_LD2410C.STATE_target;
//		SENSOR.Face_cache =




    //����
    if( SENSOR.Voltage_cache > 200)
    {
        ALARM.Fire = 1;
        ALARM.Fire_flag = 1;
    }
    else
    {
        ALARM.Fire = 0;
        ALARM.Fire_flag = 0;
    }

    //����orȼ��
    if( SENSOR.MQ2_cache > 100)
    {
        ALARM.Smoke_Gas = 1;
        ALARM.Smoke_Gas_flag = 1;
    }
    else
    {
        ALARM.Smoke_Gas = 0;
        ALARM.Smoke_Gas_flag = 0;
    }

    //�״��Ƿ��⵽����			//		0 ���ˣ�	1 �˶���	2 ��ֹ��	3 �˶�&��ֹ
    if( SENSOR.people_state_cache == 1 ||SENSOR.people_state_cache == 2 ||SENSOR.people_state_cache == 3 )
    {
        ALARM.if_people = 1;
        ALARM.if_people_flag = 1;
    }
    else if( SENSOR.people_state_cache == 0)
    {
        ALARM.if_people = 0;
        ALARM.if_people_flag = 0;
    }


//		if( KEY1_flag == 1  )
//		{
//				LED2(LED_ON);
//		}
//		if( KEY2_flag == 1  )
//		{
//				LED2(LED_OFF);
//		}
//		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
//		{
//				LED2(LED_ON);
//		}
//		if( Key_Scan(KEY2_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
//		{
//				LED2(LED_OFF);
//		}

    if ( ASR_IF.Enroll == 1)
    {
        HLK_TX510_Enroll_FaceID();
        Dealwith_HLK_TX510();
        ASR_IF.Enroll = 0;
			  ASR_IF.Enroll_flag ++;
    }
    else if ( ASR_IF.DeleteAll == 1)
    {
        HLK_TX510_DeleteAll_FaceID();
        Dealwith_HLK_TX510();
        ASR_IF.DeleteAll = 0;
			  ASR_IF.DeleteAll_flag ++;
    }
    else if ( ASR_IF.Identify == 1 )
    {
        HLK_TX510_Identify_FaceID();
        Dealwith_HLK_TX510();
        ASR_IF.Identify  = 0;
			  ASR_IF.Identify_flag ++;
    }

//    else if ( ASR_IF.Identify == 1 || ALARM.if_people == 1)

////    if (ALARM.if_people == 0)
////    {
//////				HLK_TX510_OFF();
//////				Dealwith_HLK_TX510();
////    }
////    else if (ALARM.if_people == 1)
////    {
//////				HLK_TX510_ON();
//////				Dealwith_HLK_TX510();

//////				HLK_TX510_Identify_FaceID();
//////				Dealwith_HLK_TX510();
////    }


}


void GET_Data_SENSOR_myself(void)
{

    Calculate_SGP30( &SENSOR.CO2_cache, &SENSOR.TVOC_cache);
    Calculate_SHT30( &SENSOR.temp_cache, &SENSOR.hum_cache);
    SENSOR.dust_cache= GP2Y_GetDens();				//��ȡGP2Y�۳��������Ļҳ�Ũ��ֵ

//		SENSOR.MQ2_cache = MQ2_GetPPM();				//MQ2��������õ�Ũ��
//		SENSOR.Voltage_cache = Fire_GetData();  //��ȡ���洫��������������Ķ�Ӧ��ѹֵ

//		SENSOR.people_state_cache = Detection_Target_LD2410C.STATE_target;
    SENSOR.motion_distance_cache = Detection_Target_LD2410C.MOTION_target_distance;
    SENSOR.static_distance_cache = Detection_Target_LD2410C.STATIC_target_distance;

//		SENSOR.Face_cache =

#if 0
//		//����
//		if( SENSOR.Voltage_cache > 100)
//		{
//				ALARM.Fire = 1;
//		}
//		else
//		{
//				ALARM.Fire = 0;
//		}
//
//		//����orȼ��
//		if( SENSOR.MQ2_cache > 50)
//		{
//				ALARM.Smoke_Gas = 1;
//		}
//		else
//		{
//				ALARM.Smoke_Gas = 0;
//		}

//		//�״��Ƿ��⵽����			//		0 ���ˣ�	1 �˶���	2 ��ֹ��	3 �˶�&��ֹ
//		if( SENSOR.people_state_cache == 1 ||SENSOR.people_state_cache == 2 ||SENSOR.people_state_cache == 3 )
//		{
//				ALARM.if_people = 1;
//		}
//		else if( SENSOR.people_state_cache == 0)
//		{
//				ALARM.if_people = 0;
//		}
//

//
//		if (ALARM.if_people == 0)
//    {
////				HLK_TX510_OFF();
////				Dealwith_HLK_TX510();
//    }
//    else if (ALARM.if_people == 1)
//    {
////				HLK_TX510_ON();
////				Dealwith_HLK_TX510();

////				HLK_TX510_Identify_FaceID();
////				Dealwith_HLK_TX510();
//    }
#endif

}

void ALARM_Handle_myself(void)
{
    char  *strx_call;
    uint8_t i = 0;
    uint32_t  i_call = 6;

    uint64_t phone_cache = Desired.phone;
    for( i = 0; i < 11; i++)
    {
        phone_num[i]   = (phone_cache / pow(10.0,10-i)) + '0';
        phone_cache    = phone_cache - (phone_num[i]-'0')*pow(10.0,10-i);
				call_num[3+i]  = phone_num[i];
    }
		
//	  Uart1_SendStr("hhhhhhhhhhhhhh\r\n");	
    char phone_num_cache[18] = {'\0'};
    sprintf(phone_num_cache, "ATD%s;\r\n",phone_num);
//    Uart1_SendStr( phone_num_cache );		
		
//		char txt_parse[100] = {'\0'};
//		Uart1_SendStr("\r\n");
//		Uart1_SendStr("hhhhhhhhhhhhhhhh\r\n");
//    sprintf(txt_parse, "%s\r\n", phone_num);
//		Uart1_SendStr( txt_parse );


    if ( ALARM.Fire == 1 || ALARM.Smoke_Gas ==1 || ALARM.Trespass == 1 )
    {

#if 1
        if (IFWORK.relay)
        {
            if ( ALARM.Fire == 1 || ALARM.Smoke_Gas ==1 )
            {
                RELAY(RELAY_OFF);		  // �̵�����
                if ( ALARM.Fire == 1 && IFWORK.SLA ==0 && IFWORK.ML302 ==0 )
                {
                    ALARM.Fire = 0;
                }
                else if ( ALARM.Smoke_Gas == 1 && IFWORK.SLA ==0 && IFWORK.ML302 ==0 )
                {
                    ALARM.Smoke_Gas = 0;
                }
            }
        }
#endif

        if (IFWORK.SLA)
        {
            LED1(LED_ON);			    // LED��
            BEEP(BEEP_ON); 			  // ��������
            if ( ALARM.Fire == 1 && IFWORK.ML302 ==0 )
            {
                ALARM.Fire = 0;
            }
            else if ( ALARM.Smoke_Gas == 1  && IFWORK.ML302 ==0)
            {
                ALARM.Smoke_Gas = 0;
            }
            else if ( ALARM.Trespass == 1  && IFWORK.ML302 ==0)
            {
                ALARM.Trespass = 0;
            }
        }


#if 1
        if (IFWORK.ML302)
        {
            if ( ALARM.Fire == 1 )
            {
                ALARM.Fire = 0;
                ML302_SendMessage(phone_num,"There is an open flame in the room");//����Ӣ�Ķ��ţ������ֱ����ֻ������Ӣ���ַ�����
                delay_ms(500);
                Clear_Buffer();
            }
            else if ( ALARM.Smoke_Gas == 1 )
            {
                ALARM.Smoke_Gas = 0;
                ML302_SendMessage(phone_num,"Smoke or gas leaks in the home");//����Ӣ�Ķ��ţ������ֱ����ֻ������Ӣ���ַ�����
                delay_ms(500);
                Clear_Buffer();
            }
            else if ( ALARM.Trespass == 1 )
            {
                ALARM.Trespass = 0;
                ML302_SendMessage(phone_num,"The house was trespassed");//����Ӣ�Ķ��ţ������ֱ����ֻ������Ӣ���ַ�����
                delay_ms(500);
                Clear_Buffer();
            }
#if 1
//            printf("ATD13244621508;\r\n");//����绰,����������֧�ֵ绰
//            while ( i_call -- )
//            {
//                delay_s(1);
//                IWDG_Feed();
//                strx_call=strstr((const char*)RxBuffer,(const char*)"NO CARRIER");//�������˽���
//                if(strx_call)
//                {
//                    Uart1_SendStr("ͨ���Ѿ�����\r\n");
//                    Clear_Buffer();
//                }
//            }
            printf(phone_num_cache);//����绰,����������֧�ֵ绰
            while (1)
            {
								BEEP(BEEP_OFF);
                IWDG_Feed();
                strx_call=strstr((const char*)RxBuffer,(const char*)"CALL");//�������˽���
                //Uart1_SendStr(RxBuffer);
                GET_Data_ALARM_myself();
                if(strx_call)
                {
                    Uart1_SendStr("ͨ���Ѿ�����\r\n");
                    Clear_Buffer();
                    break;
                }

            }
#endif
        }
#endif

    }
    else if ( ALARM.Fire == 0 && ALARM.Smoke_Gas == 0 )
    {
        RELAY(RELAY_ON);			  //�̵�����
        if ( ALARM.Trespass == 0 )
        {
            LED1(LED_OFF);		   // LED��
            BEEP(BEEP_OFF);		  //����������
        }
    }

}

void printf_myself(void)
{
    char txt_parse[100] = {'\0'};

//		printf("״̬:%d, �˶�����:%dcm, ��ֹ����:%dcm\r\n",STM32.people_state,STM32.motion_distance,STM32.static_distance);
//		printf("�¶�%.2f, ʪ��:%.2f\r\n",STM32.temp,STM32.hum);
//		printf("CO2:%dppm, TVOC:%dppd\r\n",STM32.CO2,STM32.TVOC);
//		printf("�ҳ�Ũ��:%9.4f��g/m3\r\n",STM32.dust);
//		printf("smoke:%.2f ppm\r\n", STM32.MQ2);
//		printf("\r\n");

    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "�Ƿ��������: %d\r\n",ALARM.Fire_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�Ƿ����Ũ�Ƚϴ�������ȼ��: %d\r\n",ALARM.Smoke_Gas_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�Ƿ�����: %d\r\n",ALARM.if_people_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�Ƿ���Ƿ�����: %d\r\n",ALARM.Trespass_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�����ȼ��Ũ��: %.2f ppm\r\n",SENSOR.MQ2_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "���洫������ѹֵ: %9.2f mV\r\n",SENSOR.Voltage_cache);
    Uart1_SendStr( txt_parse );




    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "���ⱨ������״̬: %d\r\n",ASR_IF.SLA);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�绰��������״̬: %d\r\n",ASR_IF.ML302);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��Ļ��ʾ����״̬: %d\r\n",ASR_IF.OLED);
    Uart1_SendStr( txt_parse );

		
	
			
		

    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "����ʶ��-����ʶ����״̬: %d\r\n",ASR_IF.ASR);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-���ⱨ������״̬: %d\r\n",ASR_IF.SLA);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-�绰��������״̬: %d\r\n",ASR_IF.ML302);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-��Ļ��ʾ����״̬: %d\r\n",ASR_IF.OLED);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-�Ƿ����ü̵����� %d\r\n",ASR_IF.relay);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-¼����������: %d\r\n",ASR_IF.Enroll_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-ɾ����������: %d\r\n",ASR_IF.DeleteAll_flag);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "����ʶ��-����ʶ�����: %d\r\n",ASR_IF.Identify_flag);
    Uart1_SendStr( txt_parse );



    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "״̬: %d\r\n",SENSOR.people_state_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�˶�����: %d cm\r\n",SENSOR.motion_distance_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��ֹ����: %d cm\r\n",SENSOR.static_distance_cache);
    Uart1_SendStr( txt_parse );



    Uart1_SendStr("\r\n");
    sprintf(txt_parse, "�¶�: %.2f ��\r\n",SENSOR.temp_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "ʪ��: %.2f %%\r\n",SENSOR.hum_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "CO2Ũ��: %d ppm\r\n",SENSOR.CO2_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "��ȩŨ��: %d ppd\r\n",SENSOR.TVOC_cache);
    Uart1_SendStr( txt_parse );

    sprintf(txt_parse, "�����������Ũ��: %.2f ��g/m3\r\n",SENSOR.dust_cache);
    Uart1_SendStr( txt_parse );
    Uart1_SendStr("\r\n");

////		Myself_ftoa( Desired.temp, txt_parse, 10, 2 );
////    sprintf(txt_parse, "%.2f", Desired.temp);  // ��������ת��Ϊ�ַ�����������λС��
////		Uart1_SendStr( txt_parse );
////		Uart1_SendStr("\r\n");
////		Uart1_SendFloat( Desired.temp );

}

void OSD_myself(void)
{

    if ( IFWORK.OLED == 1)
    {
//        OLED_CLS();
#if 0
//					if ( STM32.CO2 == 400 && STM32.TVOC == 0 )
//					{
//							//OLED_ShowString(2,6,(u8 *)"SGP30 is detecting",16);
//					}
//					else
//					{
//								OLED_ShowNum(0,0,STM32.TVOC,4,16);
//								OLED_ShowNum(0,3,STM32.CO2,4,16);
//					}

//				OLED_ShowCN(0,0,2);
//				OLED_ShowCN(112,0,2);
//				OLED_ShowCN(0,6,2);
//				OLED_ShowCN(112,6,2);
#endif
			
				OLED_CLS();
				//�¶�ʪ��
        OLED_ShowCN(15,1,0);
        OLED_ShowCN(35,1,2);
        OLED_ShowCN(15,4,1);
        OLED_ShowCN(35,4,2);

        OLED_Showdecimal(60,1,SENSOR.temp_cache,2,2,16);
        OLED_ShowCN(100,1,17);
        OLED_Showdecimal(60,4,SENSOR.hum_cache,2,2,16);
        OLED_ShowStr(105,4,(u8*)"%",2);


////				rt_sem_release(ALARM_sem);
				GET_Data_SENSOR_myself();
				IWDG_Feed();//ι��
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��
			
				printf_myself();
				IWDG_Feed();//ι��	
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��




				OLED_CLS();
        //��ȩŨ��
        OLED_ShowCN(30,0,7);
        OLED_ShowCN(50,0,8);
        OLED_ShowCN(70,0,15);
        OLED_ShowCN(90,0,16);
        OLED_ShowNum(30,2,SENSOR.TVOC_cache,4,16);
        OLED_ShowStr(70,2,(u8*)"ppd",2);

        //CO2Ũ��
        OLED_ShowStr(35,4,(u8*)"CO2",2);
        OLED_ShowCN(70,4,15);
        OLED_ShowCN(90,4,16);
        OLED_ShowNum(30,6,SENSOR.CO2_cache,4,16);
        OLED_ShowStr(70,6,(u8*)"ppm",2);


////				rt_sem_release(ALARM_sem);
				GET_Data_SENSOR_myself();
				IWDG_Feed();//ι��
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��
			
				printf_myself();
				IWDG_Feed();//ι��	
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��

//    char txt_parse[100] = {'\0'};
//    sprintf(txt_parse, "LED��: %d\r\n",REC_cache.LED);
//    Uart1_SendStr( txt_parse );		
//		if (REC_cache.LED == '1')
//    {
//				LED2(LED_ON);
//    }
//    else if (REC_cache.LED == '0')
//    {
//				LED2(LED_OFF);
//    }
//		if (Desired.relay == 1)
//    {
//				RELAY(RELAY_ON);
//    }
//    else if (Desired.relay == 0)
//    {
//				RELAY(RELAY_OFF);
//    }	


				OLED_CLS();
        //�����������Ũ��
        OLED_ShowCN(40,0,9);
        OLED_ShowCN(60,0,10);
        OLED_ShowCN(80,0,11);
        OLED_ShowCN(15,2,12);
        OLED_ShowCN(35,2,13);
        OLED_ShowCN(55,2,14);
        OLED_ShowCN(75,2,15);
        OLED_ShowCN(95,2,16);
        OLED_Showdecimal(20,5,SENSOR.dust_cache,3,2,16);
        OLED_ShowStr(70,5,(u8*)"ppm",2);


////				rt_sem_release(ALARM_sem);
				GET_Data_SENSOR_myself();
				IWDG_Feed();//ι��
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��
			
				printf_myself();
				IWDG_Feed();//ι��	
				GET_Data_ALARM_myself();
				IWDG_Feed();//ι��
				ALARM_Handle_myself();
				IWDG_Feed();//ι��





////        OLED_CLS();
    }
    else if( IFWORK.OLED == 0)
    {
        OLED_CLS();
    }

}








void deal_with(void)
{

//    GET_Data_SENSOR_myself();


    STM32.temp	= SENSOR.temp_cache;
    STM32.hum 	= SENSOR.hum_cache;
    STM32.dust 	= SENSOR.dust_cache;
    STM32.CO2 	= SENSOR.CO2_cache;
    STM32.TVOC	= SENSOR.TVOC_cache;

    STM32.MQ2 	    = SENSOR.MQ2_cache;
    STM32.Voltage 	= SENSOR.Voltage_cache;

    STM32.people_state 		= SENSOR.people_state_cache;
    STM32.motion_distance = SENSOR.motion_distance_cache;
    STM32.static_distance = SENSOR.static_distance_cache;

    STM32.Face       = SENSOR.Face_cache;
    STM32.LED        = SENSOR.LED;
    STM32.relay      = SENSOR.relay;


//		STM32.if_people  = ALARM.if_people;
//		STM32.Trespass   = ALARM.Trespass;
//		STM32.Fire       = ALARM.Fire;
//		STM32.Smoke_Gas  = ALARM.Smoke_Gas;

    STM32.if_SLA     = ALARM.if_SLA;
    STM32.if_SMS     = ALARM.if_SMS;
    STM32.if_CALL    = ALARM.if_CALL;
    STM32.if_Cloud   = ALARM.if_Cloud;
    STM32.phone      = ALARM.phone;

    STM32.if_people_flag      = ALARM.if_people_flag;
    STM32.Trespass_flag       = ALARM.Trespass_flag;
    STM32.Fire_flag           = ALARM.Fire_flag;
    STM32.Smoke_Gas_flag      = ALARM.Smoke_Gas_flag;




//		ONENET_IF.ASR     = Desired.ASR_ifwork;
//		ONENET_IF.SLA     = Desired.SLA_ifwork;
//		ONENET_IF.ML302   = Desired.ML302_ifwork;
//		ONENET_IF.OLED    = Desired.OLED_ifwork;
//		ONENET_IF.relay   = Desired.relay;

//		IFWORK.ASR 		= ONENET_IF.ASR | ASR_IF.ASR;
//		IFWORK.SLA  	= ONENET_IF.SLA | ASR_IF.SLA;
//		IFWORK.ML302 	= ONENET_IF.ML302 | ASR_IF.ML302;
//		IFWORK.OLED 	= ONENET_IF.OLED | ASR_IF.OLED;
//		IFWORK.relay 	= ONENET_IF.relay | ASR_IF.relay;
    IFWORK.ASR 		= ASR_IF.ASR;
    IFWORK.SLA  	= ASR_IF.SLA;
    IFWORK.ML302 	= ASR_IF.ML302;
    IFWORK.OLED 	= ASR_IF.OLED;
    IFWORK.relay 	= ASR_IF.relay;

}


void GET_Data_Desired_myself(void)
{
    Clear_Buffer_separate();
    ML302_MQTTPUB_GET_desired();
    ML302_MQTT_ParseData();
    Clear_Buffer_separate();

    Desired = REC_cache;
//		ML302_MQTTPUB_Delete_desired();

}
void POST_Data_myself(void)
{
    Clear_Buffer_separate();
    ML302_MQTTPUB_POST(STM32.temp, STM32.hum, STM32.dust, STM32.CO2, STM32.TVOC,\
                       STM32.people_state, STM32.motion_distance, STM32.static_distance,\
                       STM32.if_people_flag, STM32.Trespass_flag, STM32.Fire_flag, STM32.Smoke_Gas_flag, STM32.phone,\
                       STM32.LED, STM32.relay,\
                       STM32.ASR_ifwork, STM32.SLA_ifwork, STM32.ML302_ifwork, STM32.OLED_ifwork);

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
}

void Uart1_SendFloat(float value)
{
    char str[32];
    sprintf(str, "%.2f", value);  // ��������ת��Ϊ�ַ�����������λС��
    Uart1_SendStr(str);
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



//���磬���� Myself_ftoa(21, str, 10) �������21ת��Ϊ�ַ���
char *Myself_itoa(int value, char *str, int base)
{
    // �������Ƿ�Ϸ�
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    // ���ɷ���λ
    char *p = str;
    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    // �����ְ��ս���ת�����ַ���
    char *q = p;
    do {
        *q++ = "0123456789abcdefghijklmnopqrstuvwxyz"[value % base];
        value /= base;
    } while (value > 0);

    // ����ַ�����������
    *q = '\0';

    // ��ת�ַ���
    char *begin = p;
    char *end = q - 1;
    while (begin < end) {
        char tmp = *begin;
        *begin++ = *end;
        *end-- = tmp;
    }

    return str;
}

//���磬���� Myself_ftoa(3.14159, str, 10, 3) �����С�����ֱ��� 3 λС����ת��Ϊ�ַ���
char *Myself_ftoa(float value, char *str, int base, int precision)
{
    // �����ж���ֵ�Ƿ��������� NaN
    if (isnan(value)) {
        strcpy(str, "nan");
        return str;
    }
    if (isinf(value)) {
        strcpy(str, "inf");
        return str;
    }

    // ������
    int is_negative = 0;
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // �洢�������ֺ�С������
    char int_part[100];
    char frac_part[100];
    int int_size = 0;
    int frac_size = 0;

    int int_value = (int)value;
    double frac_value = value - int_value;

    // ����С������
    if (frac_value > 0) {
        // ��С�������������룬λ��ȡ���ھ���
        double factor = pow(base, precision);
        frac_value = round(frac_value * factor) / factor;

        // ת��С������Ϊ�ַ���
        char *p = frac_part;
        *p++ = '.';
        while (frac_value > 0 && frac_size < precision) {
            frac_value *= base;
            int digit = (int)frac_value;
            frac_value -= digit;
            *p++ = "0123456789abcdefghijklmnopqrstuvwxyz"[digit];
            frac_size++;
        }
        *p = '\0';
    }

    // ������������
    if (int_value == 0) {
        int_part[0] = '0';
        int_part[1] = '\0';
        int_size = 1;
    } else {
        char *p = int_part;
        while (int_value > 0) {
            int digit = int_value % base;
            int_value /= base;
            *p++ = "0123456789abcdefghijklmnopqrstuvwxyz"[digit];
            int_size++;
        }
        *p = '\0';
        // ��ת��������
        p = int_part;
        char *q = p + int_size - 1;
        while (p < q) {
            char tmp = *p;
            *p++ = *q;
            *q-- = tmp;
        }
    }

    // ���������С������
    char *p = str;
    if (is_negative) {
        *p++ = '-';
    }
    strcat(p, int_part);
    p += int_size;
    strcat(p, frac_part);

    return str;
}


/*********************************************END OF FILE**********************/
