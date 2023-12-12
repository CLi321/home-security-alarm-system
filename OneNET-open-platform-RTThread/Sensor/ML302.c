
#include "ML302.h"


/***********************************************************************************************************/

char *strx_MQTT, *extstrx_MQTT;
char *strx_call=0,*extstrx_call; 	//����ֵָ���ж�
char *strx_text,*extstrx_text;
char *strx_LBS,*extstrx_LBS;
unsigned char socketnum=0;//��ǰ��socket����

extern char  RxBuffer[USART_REC_LEN_cache];
extern uint16_t RxCounter;


char *strx_MQTTSUB;


char *strx_MQTT_REC = NULL;
char *strx_MQTT_REC_cache = NULL;
char judge_rec[10];
uint32_t time_rec = 0; // ms ��ʱ����


char *MQTT_REC_datastreams = NULL;
char *id_rec_cache = NULL, *data_rec_cache = NULL;
char id_rec[6], data_rec[12];
u8 i_rec = 0, k_rec = 0;


uint8_t i_switch = 0;


//��Ҫ�û��ĵ�
#define ProductID  "578927"  //��ƷID
#define DeviceID  "STM32_ML302"  //�豸����
#define Token		"version=2018-10-31&res=products%2F578927&et=1710410243&method=md5&sign=G2pO4kgiYhqfg%2FL4ReI%2FbA%3D%3D"  //tokenֵ

void ML302_Init(void)
{

//����ML302�������ݵ�MQTT������������ʹ��mqtt.fx������ж���
//����ML302��绰
//����ML302��Ӣ�Ķ���
//����ML302����TCP���ݵ���������֧����

    OLED_Clear();
    OLED_ShowString(1,1,(u8 *)"ML302 is being initialized",16);

    ML302_Init_MQTT();//����MQTT��ʼ��
    delay_ms(300);

//    ML302_Init_LBS();//��վ��λ��ʼ��
//    delay_ms(300);

    ML302_Init_call(); //��绰��ʼ��
    delay_ms(500);

    ML302_Init_text();//�����ų�ʼ��
    delay_ms(500);

    ML302_MQTTInit();//��ʼ��MQTT����
    delay_ms(500);

    ML302_MQTTSUB();
    delay_ms(500);

    OLED_Clear();
    OLED_ShowString(1,1,(u8 *)"ML302 initialization is complete",16);
    delay_ms(100);
		OLED_Clear();
}

//MQTT
#if 1
void ML302_Init_MQTT(void)
{		
		Clear_Buffer_separate();
	
    Uart1_SendStr("���β����Ƿ����ݵ�MQTT����������\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
//    printf("AT+MQTTDISC\r\n");		//�Ͽ� MQTT ���ӡ�
//    delay_ms(300);
//    printf("AT+MQTTDEL\r\n");		//ɾ�� MQTT �ͻ���ʵ�������ͷ���Դ��
//    delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
    printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//����APN����
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,1\r\n");//�������磬PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//��������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//�����������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
    }
    printf("AT+CSQ\r\n");//��ѯ�ź�
    delay_ms(300);
    Clear_Buffer();

}


//��ʼ��MQTT
void ML302_MQTTInit(void)
{
		Clear_Buffer_separate();
	
    printf("AT+MQTTDISC\r\n");		//�Ͽ� MQTT ���ӡ�
    delay_ms(300);
    printf("AT+MQTTDEL\r\n");		//ɾ�� MQTT �ͻ���ʵ�������ͷ���Դ��
    delay_ms(300);

    printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"%s\",60,\"578927\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//�����豸ID��Tokenֵ
    //printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"mmm\",60,\"399901\",\"version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D\",1,\"\",0\r\n");//���õ�¼����
    delay_ms(300);
    Clear_Buffer();
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//���ӵ�ONENETƽ̨
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//����¼�ɹ�

    while(strx_MQTT==NULL)
    {		
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//��¼��ONENET�ɹ� ��ʱ�豸����ʾ����
    }
    Clear_Buffer();


}

void ML302_Dis_Re_Connect(void)
{
    printf("AT+MQTTDISC\r\n");		//�Ͽ� MQTT ���ӡ�
    delay_ms(300);
    printf("AT+MQTTDEL\r\n");		//ɾ�� MQTT �ͻ���ʵ�������ͷ���Դ��
    delay_ms(300);

    printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"%s\",60,\"578927\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//�����豸ID��Tokenֵ
    //printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"mmm\",60,\"399901\",\"version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D\",1,\"\",0\r\n");//���õ�¼����
    delay_ms(300);
    Clear_Buffer();
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//���ӵ�ONENETƽ̨
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//����¼�ɹ�
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//��¼��ONENET�ɹ� ��ʱ�豸����ʾ����
    }
    Clear_Buffer();
}
//����
void ML302_MQTTSUB( void )
{
		Clear_Buffer_separate();
//    printf("AT+MQTTSUB=\"$sys/%s/%s/dp/post/json/accepted\",0\r\n",ProductID,DeviceID);
//		printf("AT+MQTTSUB=\"$sys/%s/%s/#\",0\r\n",ProductID,DeviceID);
//    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
//		while(strx_MQTTSUB==NULL)
//		{
//				strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
//		}
//    Clear_Buffer();


    printf("AT+MQTTSUB=\"$sys/%s/%s/cmd/request/+\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();


    printf("AT+MQTTSUB?\r\n");
    delay_ms(300);
    Clear_Buffer();
}


//ȡ������
void ML302_MQTTUNSUB( void )
{
//		printf("AT+MQTTUNSUB=\"$sys/%s/%s/#\"\r\n",ProductID,DeviceID);
//		strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTT UNSUBACK:");//
//		while(strx_MQTTSUB==NULL)
//		{
//				strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTT UNSUBACK:");//
//		}
//		Clear_Buffer();


    printf("AT+MQTTUNSUB=\"$sys/%s/%s/cmd/request/+\"\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTT UNSUBACK:");//
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTT UNSUBACK:");//
    }
    Clear_Buffer();


    printf("AT+MQTTSUB?\r\n");
    delay_ms(300);
    Clear_Buffer();
}

//�������ݵ�������
void ML302_MQTTPUBMessage_STM32( float temp_STM32, float hum_STM32, float dust_STM32, float MQ2_STM32,\
                                 uint32_t TVOC_STM32, uint32_t CO2_STM32, char relay_STM32,\
                                 char Fire_STM32, char people_state_STM32, uint32_t motion_distance_STM32, uint32_t static_distance_STM32,\
                                 char LED_STM32, char BEEP_STM32)
{
    printf("AT+MQTTPUB=\"$sys/578927/STM32_ML302/dp/post/json\",1,0,0,\"{\\\"id\\\":123,\\\"dp\\\":{\
									\\\"STM32_temp\\\":[{\\\"v\\\":%.2f}],\
									\\\"STM32_hum\\\":[{\\\"v\\\":%.2f}],\
									\\\"STM32_dust\\\":[{\\\"v\\\":%.2f}],\
									\\\"STM32_MQ2\\\":[{\\\"v\\\":%.2f}],\
									\\\"STM32_TVOC\\\":[{\\\"v\\\":%d}],\
									\\\"STM32_CO2\\\":[{\\\"v\\\":%d}],\
									\\\"STM32_relay\\\":[{\\\"v\\\":%d}],\
									\\\"STM32_Fire\\\":[{\\\"v\\\":%d}],\
									\\\"people_state\\\":[{\\\"v\\\":%d}],\
									\\\"motion_distance\\\":[{\\\"v\\\":%d}],\
									\\\"static_distance\\\":[{\\\"v\\\":%d}],\
									\\\"STM32_LED\\\":[{\\\"v\\\":%d}],\
									\\\"STM32_BEEP\\\":[{\\\"v\\\":%d}]\
									}}\"\r\n",temp_STM32, hum_STM32, dust_STM32, MQ2_STM32, TVOC_STM32, CO2_STM32, relay_STM32,\
           Fire_STM32, people_state_STM32, motion_distance_STM32, static_distance_STM32,\
           LED_STM32, BEEP_STM32);
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
    }
    Clear_Buffer();

}

//�������ݵ�������
void ML302_MQTTPUBMessage_WeChat( uint32_t temp_WeChat, uint32_t hum_WeChat, uint32_t dust_WeChat, uint32_t MQ2_WeChat,\
                                  uint32_t TVOC_WeChat, uint32_t CO2_WeChat, char relay_WeChat,\
                                  uint64_t phone_WeChat,\
                                  char ASR_ifwork, char LED_ifwork, char BEEP_ifwork, char ML302_ifwork, char OLED_ifwork)
{

    printf("AT+MQTTPUB=\"$sys/578927/STM32_ML302/dp/post/json\",1,0,0,\"{\\\"id\\\":123,\\\"dp\\\":{\
									\\\"WeChat_temp\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_hum\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_dust\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_MQ2\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_TVOC\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_CO2\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_relay\\\":[{\\\"v\\\":%d}],\
									\\\"WeChat_phone\\\":[{\\\"v\\\":%lld}],\
									\\\"ASR_ifwork\\\":[{\\\"v\\\":%d}],\
									\\\"LED_ifwork\\\":[{\\\"v\\\":%d}],\
									\\\"BEEP_ifwork\\\":[{\\\"v\\\":%d}],\
									\\\"ML302_ifwork\\\":[{\\\"v\\\":%d}],\
									\\\"OLED_ifwork\\\":[{\\\"v\\\":%d}]\
									}}\"\r\n",temp_WeChat, hum_WeChat, dust_WeChat, MQ2_WeChat, TVOC_WeChat, CO2_WeChat, relay_WeChat,\
           phone_WeChat,\
           ASR_ifwork, LED_ifwork, BEEP_ifwork, ML302_ifwork, OLED_ifwork);
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
    }
    Clear_Buffer();

}



void ML302_MQTT_RECData(void)
{

    strx_MQTT_REC_cache=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");//����+QMTRECV:���������յ�onenetƽ̨
    strx_MQTT_REC=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
    sscanf(strx_MQTT_REC_cache, "%[^:]", judge_rec );

    time_rec = 0;
    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM, ENABLE);

    while ( strcmp(judge_rec, "+MQTTPUBLISH") )
    {
        strx_MQTT_REC_cache=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
        strx_MQTT_REC=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
        sscanf(strx_MQTT_REC_cache, "%[^:]", judge_rec );

        if ( time_rec == 13000 ) /* 1000 * 1 ms = 1s ʱ�䵽 */
        {
            time_rec = 0;
            break;
        }
    }
    TIM_Cmd(GENERAL_TIM, DISABLE);



//		Clear_Buffer_separate();
//		Clear_Buffer();


    Uart1_SendStr(strx_MQTT_REC);
    if ( !strcmp(judge_rec, "+MQTTPUBLISH") )
    {
        IWDG_Feed();//ι��
        MQTT_REC_datastreams = strx_MQTT_REC+100;		//ָ����datastreamǰ��Ĵ�����
        id_rec_cache = MQTT_REC_datastreams+23;			//ָ���˼�
        data_rec_cache = id_rec_cache + 28;					//ָ����ֵ

//				Uart1_SendStr(MQTT_REC_datastreams);
//				Uart1_SendStr(id_rec_cache);
//				Uart1_SendStr(data_rec_cache);
        memset(id_rec, '\0', sizeof(id_rec));
        memset(data_rec, '\0', sizeof(data_rec));

        for(i_rec = 0; i_rec < strlen(id_rec_cache); i_rec ++)
        {
            if( (id_rec_cache[i_rec] >= 'a' && id_rec_cache[i_rec] <= 'z') || (id_rec_cache[i_rec] >= 'A' && id_rec_cache[i_rec] <= 'Z') )
            {
                id_rec[k_rec] = id_rec_cache[i_rec];
                k_rec ++;
            }
        }
        k_rec = 0;

        for(i_rec = 0; i_rec < strlen(data_rec_cache); i_rec ++)
        {
            if(data_rec_cache[i_rec] >= '0' && data_rec_cache[i_rec] <= '9')
            {
                data_rec[k_rec] = data_rec_cache[i_rec];
                k_rec ++;
            }
        }
        k_rec = 0;


//				Uart1_SendStr(id_rec);
//				Uart1_SendStr(data_rec);
//				OLED_ShowString(2,0,(u8 *)id_rec,16);
//				OLED_ShowString(5,11,(u8 *)data_rec,16);

        if( id_rec[0] == 't' && id_rec[1] == 'e' && id_rec[2] == 'm' && id_rec[3] == 'p')
        {

            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.temp = WeChat_cache.temp*10 + (data_rec[i_rec] - '0');
            }
            WeChat.temp = WeChat_cache.temp;
            WeChat_cache.temp = 0;
        }
        else if( id_rec[0] == 'h' && id_rec[1] == 'u' && id_rec[2] == 'm')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.hum = WeChat_cache.hum*10 + (data_rec[i_rec] - '0');
            }
            WeChat.hum = WeChat_cache.hum;
            WeChat_cache.hum = 0;
        }
        else if( id_rec[0] == 'd' && id_rec[1] == 'u' && id_rec[2] == 's' && id_rec[3] == 't')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.dust = WeChat_cache.dust*10 + (data_rec[i_rec] - '0');
            }
            WeChat.dust = WeChat_cache.dust;
            WeChat_cache.dust = 0;
        }
        else if( id_rec[0] == 'M' && id_rec[1] == 'Q' && id_rec[2] == 's')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.MQ2 = WeChat_cache.MQ2*10 + (data_rec[i_rec] - '0');
            }
            WeChat.MQ2 = WeChat_cache.MQ2;
            WeChat_cache.MQ2 = 0;
        }
        else if( id_rec[0] == 'T' && id_rec[1] == 'V' && id_rec[2] == 'O' && id_rec[3] == 'C')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.TVOC = WeChat_cache.TVOC*10 + (data_rec[i_rec] - '0');
            }
            WeChat.TVOC = WeChat_cache.TVOC;
            WeChat_cache.TVOC = 0;
        }
        else if( id_rec[0] == 'C' && id_rec[1] == 'O' && id_rec[2] == 't' && id_rec[3] == 'w' && id_rec[4] == 'o')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.CO2 = WeChat_cache.CO2*10 + (data_rec[i_rec] - '0');
            }
            WeChat.CO2 = WeChat_cache.CO2;
            WeChat_cache.CO2 = 0;
        }


        else if( id_rec[0] == 'p' && id_rec[1] == 'h' && id_rec[2] == 'o' && id_rec[3] == 'n' && id_rec[4] == 'e')
        {
            for(i_rec = 0; i_rec < strlen(data_rec); i_rec ++)
            {
                WeChat_cache.phone = WeChat_cache.phone*10 + (data_rec[i_rec] - '0');
            }
            WeChat.phone = WeChat_cache.phone;
            WeChat_cache.phone = 0;
        }


        else if( id_rec[0] == 'r' && id_rec[1] == 'e' && id_rec[2] == 'l' && id_rec[3] == 'a' && id_rec[4] == 'y')
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.relay = (data_rec[0] - '0');
            }
            WeChat.relay = WeChat_cache.relay;
            WeChat_cache.relay = 0;
        }
        else if( id_rec[0] == 'A' && id_rec[1] == 'S' && id_rec[2] == 'R')//
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.ASR_ifwork = (data_rec[0] - '0');
            }
            WeChat.ASR_ifwork = WeChat_cache.ASR_ifwork;
            WeChat_cache.ASR_ifwork = 0;
        }
        else if( id_rec[0] == 'L' && id_rec[1] == 'E' && id_rec[2] == 'D')//��������
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.LED_ifwork = (data_rec[0] - '0');
            }
            WeChat.LED_ifwork = WeChat_cache.LED_ifwork;
            WeChat_cache.LED_ifwork = 1;
        }
        else if( id_rec[0] == 'B' && id_rec[1] == 'E' && id_rec[2] == 'E' && id_rec[3] == 'P')
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.BEEP_ifwork = (data_rec[0] - '0');
            }
            WeChat.BEEP_ifwork = WeChat_cache.BEEP_ifwork;
            WeChat_cache.BEEP_ifwork = 0;
        }
        else if( id_rec[0] == 'M' && id_rec[1] == 'L' && id_rec[2] == 'G')
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.ML302_ifwork = (data_rec[0] - '0');
            }
            WeChat.ML302_ifwork = WeChat_cache.ML302_ifwork;
            WeChat_cache.ML302_ifwork = 0;
        }
        else if( id_rec[0] == 'O' && id_rec[1] == 'L' && id_rec[2] == 'E' && id_rec[3] == 'D')
        {
            if ( data_rec[0] == '0' || data_rec[0] == '1')
            {
                WeChat_cache.OLED_ifwork = (data_rec[0] - '0');
            }
            WeChat.OLED_ifwork = WeChat_cache.OLED_ifwork;
            WeChat_cache.OLED_ifwork = 0;
        }
        else
        {}
        memset(id_rec, '\0', sizeof(id_rec));
        memset(data_rec, '\0', sizeof(data_rec));

#if 0
        if ( !strcmp(id_rec, "LED") )
            i_switch = 1;
        else if ( !strcmp(id_rec, "temp") )
            i_switch = 2;
        else if ( !strcmp(id_rec, "hum") )
            i_switch = 3;


        switch ( i_switch )
        {
        case 1:
        {
            if ( data_rec[0] == '0')
            {
                LED1(LED_ON);
            }
            else if ( data_rec[0] == '1')
            {
                LED1(LED_OFF);
            }
            LED_wxchat = (data_rec[0] - '0');
        }
        break;


        case 2:
        {
            temp_WeChat = (data_rec[0] - '0');
        }
        break;


        case 3:
        {
            hum_WeChat = (data_rec[0] - '0');
        }
        break;


        default:
            break;
        }
#endif




    }
    Clear_Buffer_separate();
}
#endif

//call
#if 1

void  ML302_Init_call(void)
{
    Uart1_SendStr("���β����Ǵ�绰����\r\n");
    printf("AT\r\n");
    delay_ms(500);
    printf("AT\r\n");
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    while(strx_call==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0\r\n"); //�رջ���
    delay_ms(500);
    Clear_Buffer();
    printf("AT+CSQ\r\n"); //���CSQ
    delay_ms(500);
    /////////////////////////////////
    printf("AT+CPIN?\r\n");//���SIM���Ƿ���λ
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//�鿴�Ƿ񷵻�ready
    while(strx_call==NULL)
    {
        Clear_Buffer();
        printf("AT+CPIN?\r\n");
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//���SIM���Ƿ���λ���ȴ�����λ�������ʶ�𲻵���ʣ��Ĺ�����û������
    }
    Clear_Buffer();
    ///////////////////////////////////
    printf("AT+QICLOSE=0\r\n");//�Ͽ�����
    delay_ms(500);
    ///////////////////////////////////
    printf("AT+CREG?\r\n");//�鿴�Ƿ�ע��GSM����
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,1");//��������
    extstrx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,5");//��������������
    while(strx_call==NULL&&extstrx_call==NULL)
    {
        Clear_Buffer();
        printf("AT+CREG?\r\n");//�鿴�Ƿ�ע��GSM����
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,1");//��������
        extstrx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,5");//��������������
    }
    Clear_Buffer();

}
#endif



//text
#if 1
void ML302_Init_text(void)
{
    Uart1_SendStr("���β����Ƿ���Ӣ�Ķ��Ź���\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
    printf("AT+MIPCLOSE=1\r\n");//�ر���һ������
    delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
    printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//����APN����
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,2\r\n");//�������磬PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//��������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//�����������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
    }
    printf("AT+CSQ\r\n");//��ѯ�ź�
    delay_ms(300);
    Clear_Buffer();

}


//���Ͷ�������
void ML302_SendMessage(uint8_t *phone,uint8_t *data)
{
    printf("AT+CMGF=1\r\n");//�����ı�ģʽ
    delay_ms(300);
    printf("AT+CMGS=\"%s\"\r\n",phone);//�����ֻ�����
    delay_ms(300);
    printf("%s",data);
    delay_ms(300);
    while((USART2->SR&0X40)==0) {} //ѭ������,ֱ���������
    USART2->DR=(u8)0x1A;
    strx_text=strstr((const char*)RxBuffer,(const char*)"+CMGS:");//����+CMGS: ,�������ŷ��ͳɹ�
    while(strx_text==NULL)
    {
        strx_text=strstr((const char*)RxBuffer,(const char*)"+CMGS:");//����+CMGS: ,�������ŷ��ͳɹ�
    }
    Clear_Buffer();
}

#endif



//LBS
#if 1
void ML302_Init_LBS(void)
{
    Uart1_SendStr("���β�����TCP����\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300);
    printf("AT+MIPCLOSE=1\r\n");//�ر���һ������
    delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
    printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//����APN����
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,1\r\n");//�������磬PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//��������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//�����������
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//��1
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//����1,����ע���ɹ�
    }
    printf("AT+CSQ\r\n");//��ѯ�ź�
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1\r\n");//�����վ��λ��ȡ
    delay_ms(300);
    Clear_Buffer();

    IWDG_Feed();//ι��
}




void ML302_CreateTCPSokcet(void)//����sokcet
{

    printf("AT+MIPOPEN=1,\"TCP\",\"47.92.146.210\",8888,100,0,1,2,11002\r\n");//���ӷ�����
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,CONNECT OK");//����CONNECT OK,TCP�����жϳ��Ƿ��з���������
    while(strx_LBS==NULL)
    {
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,CONNECT OK");//����CONNECTOK
    }

    Clear_Buffer();

    IWDG_Feed();//ι��
}

//���ݷ��ͺ���
void ML302_TCPSend(uint8_t *data,uint8_t len)
{
    printf("AT+MIPSEND=1,%d\r\n",len);//����0socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����,
    delay_ms(300);
    printf("%s",data);
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,SEND OK");//����	1,SEND OK,TCP�����жϳ��Ƿ��з���������
    while(strx_LBS==NULL)
    {
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,SEND OK");//����	1,SEND OK
    }
    Clear_Buffer();
}




void ML302_RECData(void)
{
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"+MIPURC:");//����+MIPURC:���������յ�TCP���������ص�����
    if(strx_LBS)
    {
        Clear_Buffer();
    }
}


#endif
