
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



char *REC_data = NULL;
char *Data_cache = NULL;
char REC_data_parse[1321] = {'\0'};
uint32_t i_parse = 0;



//��Ҫ�û��ĵ�
#define ProductID  	"UhMNWpwAbK"  //��ƷID
#define DeviceID  	"ML302_MQTT"  //�豸����
#define Token				"version=2018-10-31&res=products%2FUhMNWpwAbK%2Fdevices%2FML302_MQTT&et=1712214981&method=md5&sign=rhHMaRNvKZ3bU7K9hvjrqg%3D%3D"  //tokenֵ

void ML302_Init(void)
{

//����ML302�������ݵ�MQTT������������ʹ��mqtt.fx������ж���
//����ML302��绰
//����ML302��Ӣ�Ķ���
//����ML302����TCP���ݵ���������֧����

//    OLED_CLS();
//    OLED_ShowString(1,1,(u8 *)"ML302 is being initialized",16);

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

//    OLED_CLS();
//    OLED_ShowString(1,1,(u8 *)"ML302 initialization is complete",16);
//    delay_ms(100);
//		OLED_CLS();
}

//MQTT
#if 1
void ML302_Init_MQTT(void)
{
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
    printf("AT+MQTTDISC\r\n");		//�Ͽ� MQTT ���ӡ�
    delay_ms(300);
    printf("AT+MQTTDEL\r\n");		//ɾ�� MQTT �ͻ���ʵ�������ͷ���Դ��
    delay_ms(300);

    printf("AT+MQTTCFG=\"studio-mqtt.heclouds.com\",1883,\"%s\",60,\"UhMNWpwAbK\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//�����豸ID��Tokenֵ
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

    printf("AT+MQTTCFG=\"studio-mqtt.heclouds.com\",1883,\"%s\",60,\"UhMNWpwAbK\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//�����豸ID��Tokenֵ
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
	  //�豸�����ϱ�
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/post/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();

		
	  //�豸��ȡ��������ֵ
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/desired/get/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();
		
		
		//�����������ֵ
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/desired/delete/reply\",0\r\n",ProductID,DeviceID);
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
	
	  //�豸�����ϱ�
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/post/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();

		
	  //�豸��ȡ��������ֵ
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/desired/get/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();
		
		
		//�����������ֵ
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/desired/delete/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//���ĳɹ�
    }
    Clear_Buffer();
		

    printf("AT+MQTTSUB?\r\n");
    delay_ms(300);
    Clear_Buffer();
}


//�������ݵ�������,�豸�����ϱ�
void ML302_MQTTPUB_POST( float temp, float hum, float dust, uint32_t CO2, uint32_t TVOC,\
												 char people_state, uint32_t motion_distance, uint32_t static_distance,\
												 char if_people_flag, char Trespass_flag, char Fire_flag, char Smoke_Gas_flag, uint64_t phone,\
												 char LED, char relay,\
												 char ASR_ifwork, char SLA_ifwork, char ML302_ifwork, char OLED_ifwork)
{
    printf("AT+MQTTPUB=\"$sys/UhMNWpwAbK/ML302_MQTT/thing/property/post\",1,0,0,\"{\\\"id\\\":\\\"123\\\",\\\"params\\\":{\
									\\\"temp\\\":{\\\"value\\\":%.2f},\
									\\\"hum\\\":{\\\"value\\\":%.2f},\
									\\\"dust\\\":{\\\"value\\\":%.2f},\
									\\\"COtwo\\\":{\\\"value\\\":%d},\
									\\\"TVOC\\\":{\\\"value\\\":%d},\
									\\\"state\\\":{\\\"value\\\":%d},\
									\\\"motion\\\":{\\\"value\\\":%d},\
									\\\"static\\\":{\\\"value\\\":%d},\
									\\\"ifp\\\":{\\\"value\\\":%d},\
									\\\"Tres\\\":{\\\"value\\\":%d},\
									\\\"Fire\\\":{\\\"value\\\":%d},\
									\\\"Gas\\\":{\\\"value\\\":%d},\
									\\\"phone\\\":{\\\"value\\\":%lld},\
									\\\"LED\\\":{\\\"value\\\":%d},\
									\\\"relay\\\":{\\\"value\\\":%d},\
									\\\"ASR\\\":{\\\"value\\\":%d},\
									\\\"SLA\\\":{\\\"value\\\":%d},\
									\\\"MLCAT\\\":{\\\"value\\\":%d},\
									\\\"OLED\\\":{\\\"value\\\":%d}\
									}}\"\r\n",temp, hum, dust, CO2, TVOC,\
															 people_state, motion_distance, static_distance,\
															 if_people_flag, Trespass_flag, Fire_flag, Smoke_Gas_flag, phone,\
															 LED, relay,\
															 ASR_ifwork, SLA_ifwork, ML302_ifwork, OLED_ifwork);

    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
    }
    Clear_Buffer();

}


//�豸��ȡ��������ֵ
#if 0
void ML302_MQTTPUB_GET_desired( void )
{
    printf("AT+MQTTPUB=\"$sys/UhMNWpwAbK/ML302_MQTT/thing/property/desired/get\",1,0,0,\"{\\\"id\\\":\\\"123\\\",\\\"params\\\":[\
									\\\"temp\\\",\
									\\\"hum\\\",\
									\\\"dust\\\",\
									\\\"MQtwo\\\",\
									\\\"TVOC\\\",\
									\\\"COtwo\\\",\
									\\\"relay\\\",\
									\\\"Fire\\\",\
									\\\"state\\\",\
									\\\"motion\\\",\
									\\\"static\\\",\
									\\\"LED\\\",\
									\\\"BEEP\\\",\
									\\\"phone\\\",\
									\\\"ASR\\\",\
									\\\"SLA\\\",\
									\\\"MLCAT\\\",\
									\\\"OLED\\\"\
									]}\"\r\n");
	
    delay_ms(300);
//    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
//    while(strx_MQTT==NULL)
//    {
//        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
//    }
//    Clear_Buffer();

}
#endif
void ML302_MQTTPUB_GET_desired( void )
{
    printf("AT+MQTTPUB=\"$sys/UhMNWpwAbK/ML302_MQTT/thing/property/desired/get\",1,0,0,\"{\\\"id\\\":\\\"123\\\",\\\"params\\\":[\
									\\\"temp\\\",\
									\\\"hum\\\",\
									\\\"dust\\\",\
									\\\"COtwo\\\",\
									\\\"TVOC\\\",\
									\\\"phone\\\",\
									\\\"LED\\\",\
									\\\"relay\\\",\
									\\\"ASR\\\",\
									\\\"SLA\\\",\
									\\\"MLCAT\\\",\
									\\\"OLED\\\"\
									]}\"\r\n");
	
    delay_ms(300);

//    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
//    while(strx_MQTT==NULL)
//    {
//        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
//    }
//    Clear_Buffer();
	  Uart1_SendStr(RxBuffer);
		
}

//�����������ֵ
void ML302_MQTTPUB_Delete_desired( void )
{
    printf("AT+MQTTPUB=\"$sys/UhMNWpwAbK/ML302_MQTT/thing/property/desired/delete\",1,0,0,\"{\\\"id\\\":\\\"123\\\",\\\"params\\\":{\
									\\\"temp\\\":{},\
									\\\"hum\\\":{},\
									\\\"dust\\\":{},\
									\\\"MQtwo\\\":{},\
									\\\"TVOC\\\":{},\
									\\\"COtwo\\\":{},\
									\\\"relay\\\":{},\
									\\\"Fire\\\":{},\
									\\\"state\\\":{},\
									\\\"motion\\\":{},\
									\\\"static\\\":{},\
									\\\"LED\\\":{},\
									\\\"BEEP\\\":{},\
									\\\"phone\\\":{},\
									\\\"ASR\\\":{},\
									\\\"SLA\\\":{},\
									\\\"MLCAT\\\":{},\
									\\\"OLED\\\":{}\
									}}\"\r\n");
	
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//���ط����ɹ�
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//�������ݳɹ�
    }
    Clear_Buffer();

}



void ML302_MQTT_ParseData(void)
{
	
    strx_MQTT_REC_cache=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");//����+QMTRECV:���������յ�onenetƽ̨
    strx_MQTT_REC=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
    sscanf(strx_MQTT_REC_cache, "%[^:]", judge_rec );


    while ( strcmp(judge_rec, "+MQTTPUBLISH") )
    {
        strx_MQTT_REC_cache=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
        strx_MQTT_REC=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");
        sscanf(strx_MQTT_REC_cache, "%[^:]", judge_rec );
    }

//		Clear_Buffer_separate();
//		Clear_Buffer();
//    Uart1_SendStr(strx_MQTT_REC);

		
    if ( !strcmp(judge_rec, "+MQTTPUBLISH") )
    {
        IWDG_Feed();//ι��

        REC_data = strstr((const char*)strx_MQTT_REC,(const char*)"\"data\"");		//ָ����"data"

//				Uart1_SendStr(REC_data);
		
				i_parse = 0;
				while(*REC_data != '\0')
				{
						REC_data_parse[i_parse] = *REC_data;
						i_parse++;
						REC_data++;
				}	
				REC_data_parse[i_parse] = '\0';   //��������һ���ַ�����Ϊ'\0'
				
//				Uart1_SendStr(REC_data_parse);
	
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"temp");
				REC_cache.temp = (float)Parse_JSON(Data_cache);		

				Data_cache = strstr((const char*)REC_data_parse,(const char*)"hum");
				REC_cache.hum = (float)Parse_JSON(Data_cache);		
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"dust");
				REC_cache.dust = (float)Parse_JSON(Data_cache);	
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"COtwo");
				REC_cache.CO2 = (uint32_t)Parse_JSON(Data_cache);	

				Data_cache = strstr((const char*)REC_data_parse,(const char*)"TVOC");
				REC_cache.TVOC = (uint32_t)Parse_JSON(Data_cache);		

				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"LED");
				REC_cache.LED = (char)Parse_JSON(Data_cache);	
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"relay");
				REC_cache.relay = (char)Parse_JSON(Data_cache);		
	
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"phone");
				REC_cache.phone = (uint64_t)Parse_JSON_phone(Data_cache);	


				Data_cache = strstr((const char*)REC_data_parse,(const char*)"ASR");
				REC_cache.ASR_ifwork = (char)Parse_JSON(Data_cache);

				Data_cache = strstr((const char*)REC_data_parse,(const char*)"SLA");
				REC_cache.SLA_ifwork = (char)Parse_JSON(Data_cache);	
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"MLCAT");
				REC_cache.ML302_ifwork = (char)Parse_JSON(Data_cache);		
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"OLED");
				REC_cache.OLED_ifwork = (char)Parse_JSON(Data_cache);

				
#if 0	
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"BEEP");
				REC_cache.BEEP = (char)Parse_JSON(Data_cache);
			
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"Fire");
				REC_cache.Fire = (char)Parse_JSON(Data_cache);	
				
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"state");
				REC_cache.people_state = (uint32_t)Parse_JSON(Data_cache);	
				
				Data_cache = strstr((const char*)REC_data_parse,(const char*)"static");
				REC_cache.static_distance = (char)Parse_JSON(Data_cache);						

				Data_cache = strstr((const char*)REC_data_parse,(const char*)"motion");
				REC_cache.motion_distance = (uint32_t)Parse_JSON(Data_cache);
#endif				
				
    }
}

float Parse_JSON(char *mqtt_msg_cache)
{

    // ���ҵ�һ�� value ���ֵ���ʼλ��
    char *value_start = strstr(mqtt_msg_cache, "\"value\":");
    if (value_start == NULL) 
		{
        Uart1_SendStr("There is no \"value\" field in JSON string\n");
//        return -1;
    }
    value_start += strlen("\"value\":");

    // ���ҵ�һ�� value ���ֵĽ���λ��
    char *value_end = strchr(value_start, '}');
    if (value_end == NULL) 
		{
        Uart1_SendStr("Unexpected end of JSON string\n");
//        return -1;
    }

    // ����value���ֶε�ֵת������ֵ�����
    char value_str[32] = {'\0'};
    strncpy(value_str, value_start, value_end - value_start);
		

		float value_num = atof(value_str);
		//		Data_parse = value_str;
		return value_num;


}


uint64_t Parse_JSON_phone(char *mqtt_msg_cache)
{

    // ���ҵ�һ�� value ���ֵ���ʼλ��
    char *value_start = strstr(mqtt_msg_cache, "\"value\":");
    if (value_start == NULL) 
		{
        Uart1_SendStr("There is no \"value\" field in JSON string\n");
//        return -1;
    }
    value_start += strlen("\"value\":");

    // ���ҵ�һ�� value ���ֵĽ���λ��
    char *value_end = strchr(value_start, '}');
    if (value_end == NULL) 
		{
        Uart1_SendStr("Unexpected end of JSON string\n");
//        return -1;
    }

    // ����value���ֶε�ֵת������ֵ�����
    char value_str[32] = {'\0'};
    strncpy(value_str, value_start, value_end - value_start);
		

		uint8_t i = 0;
		uint64_t phone_cache_parase = 0;
		for( i = 0; i < 11; i++)
		{ 		
				phone_cache_parase = (value_str[i]-'0') + phone_cache_parase*10;
 
		}	
		
		return phone_cache_parase;

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
