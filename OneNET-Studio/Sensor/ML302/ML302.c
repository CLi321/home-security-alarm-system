
#include "ML302.h"
/***********************************************************************************************************/

char *strx_MQTT, *extstrx_MQTT;
char *strx_call=0,*extstrx_call; 	//返回值指针判断
char *strx_text,*extstrx_text;
char *strx_LBS,*extstrx_LBS;
unsigned char socketnum=0;//当前的socket号码

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



//需要用户改的
#define ProductID  	"UhMNWpwAbK"  //产品ID
#define DeviceID  	"ML302_MQTT"  //设备名称
#define Token				"version=2018-10-31&res=products%2FUhMNWpwAbK%2Fdevices%2FML302_MQTT&et=1712214981&method=md5&sign=rhHMaRNvKZ3bU7K9hvjrqg%3D%3D"  //token值

void ML302_Init(void)
{

//控制ML302发送数据到MQTT服务器，可以使用mqtt.fx软件进行订阅
//控制ML302打电话
//控制ML302发英文短信
//控制ML302发送TCP数据到服务器，支持收

//    OLED_CLS();
//    OLED_ShowString(1,1,(u8 *)"ML302 is being initialized",16);

    ML302_Init_MQTT();//连接MQTT初始化
    delay_ms(300);

//    ML302_Init_LBS();//基站定位初始化
//    delay_ms(300);

    ML302_Init_call(); //打电话初始化
    delay_ms(500);

    ML302_Init_text();//发短信初始化
    delay_ms(500);

    ML302_MQTTInit();//初始化MQTT参数
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
    Uart1_SendStr("本次测试是发数据到MQTT服务器功能\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
//    printf("AT+MQTTDISC\r\n");		//断开 MQTT 连接。
//    delay_ms(300);
//    printf("AT+MQTTDEL\r\n");		//删除 MQTT 客户端实例，并释放资源。
//    delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
    printf("AT+CGDCONT=2,\"IP\",\"CMNET\"\r\n");//设置APN参数
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,1\r\n");//激活网络，PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//开启网络
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//启动网络接入
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
    Clear_Buffer();
    while(strx_MQTT==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
    }
    printf("AT+CSQ\r\n");//查询信号
    delay_ms(300);
    Clear_Buffer();

}


//初始化MQTT
void ML302_MQTTInit(void)
{
    printf("AT+MQTTDISC\r\n");		//断开 MQTT 连接。
    delay_ms(300);
    printf("AT+MQTTDEL\r\n");		//删除 MQTT 客户端实例，并释放资源。
    delay_ms(300);

    printf("AT+MQTTCFG=\"studio-mqtt.heclouds.com\",1883,\"%s\",60,\"UhMNWpwAbK\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//设置设备ID和Token值
    //printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"mmm\",60,\"399901\",\"version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D\",1,\"\",0\r\n");//设置登录参数
    delay_ms(300);
    Clear_Buffer();
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//连接到ONENET平台
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//返登录成功
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//登录到ONENET成功 此时设备会显示在线
    }
    Clear_Buffer();

}

void ML302_Dis_Re_Connect(void)
{
    printf("AT+MQTTDISC\r\n");		//断开 MQTT 连接。
    delay_ms(300);
    printf("AT+MQTTDEL\r\n");		//删除 MQTT 客户端实例，并释放资源。
    delay_ms(300);

    printf("AT+MQTTCFG=\"studio-mqtt.heclouds.com\",1883,\"%s\",60,\"UhMNWpwAbK\",\"%s\",1,\"\",0\r\n",DeviceID,Token);//设置设备ID和Token值
    //printf("AT+MQTTCFG=\"183.230.40.96\",1883,\"mmm\",60,\"399901\",\"version=2018-10-31&res=products%2F399901&et=1642561719&method=md5&sign=KxOIfudO9DCevSbUqOQ3Bg%3D%3D\",1,\"\",0\r\n");//设置登录参数
    delay_ms(300);
    Clear_Buffer();
    printf("AT+MQTTOPEN=1,1,0,0,0,\"\",\"\"\r\n");//连接到ONENET平台
    delay_ms(300);
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//返登录成功
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTOPEN:OK");//登录到ONENET成功 此时设备会显示在线
    }
    Clear_Buffer();
}
//订阅
void ML302_MQTTSUB( void )
{
	  //设备属性上报
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/post/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    }
    Clear_Buffer();

		
	  //设备获取属性期望值
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/desired/get/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    }
    Clear_Buffer();
		
		
		//清除属性期望值
    printf("AT+MQTTSUB=\"$sys/%s/%s/thing/property/desired/delete/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTSUBACK:");//订阅成功
    }
    Clear_Buffer();
		
		
    printf("AT+MQTTSUB?\r\n");
    delay_ms(300);
    Clear_Buffer();
}


//取消订阅
void ML302_MQTTUNSUB( void )
{
	
	  //设备属性上报
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/post/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    }
    Clear_Buffer();

		
	  //设备获取属性期望值
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/desired/get/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    }
    Clear_Buffer();
		
		
		//清除属性期望值
    printf("AT+MQTTUNSUB=\"$sys/%s/%s/thing/property/desired/delete/reply\",0\r\n",ProductID,DeviceID);
    strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    while(strx_MQTTSUB==NULL)
    {
        strx_MQTTSUB=strstr((const char*)RxBuffer,(const char*)"+MQTTUNSUBACK:");//订阅成功
    }
    Clear_Buffer();
		

    printf("AT+MQTTSUB?\r\n");
    delay_ms(300);
    Clear_Buffer();
}


//发送数据到服务器,设备属性上报
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
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
    }
    Clear_Buffer();

}


//设备获取属性期望值
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
//    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
//    while(strx_MQTT==NULL)
//    {
//        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
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

//    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
//    while(strx_MQTT==NULL)
//    {
//        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
//    }
//    Clear_Buffer();
	  Uart1_SendStr(RxBuffer);
		
}

//清除属性期望值
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
    strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//返回发布成功
    while(strx_MQTT==NULL)
    {
        strx_MQTT=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBACK:0");//发布数据成功
    }
    Clear_Buffer();

}



void ML302_MQTT_ParseData(void)
{
	
    strx_MQTT_REC_cache=strstr((const char*)RxBuffer,(const char*)"+MQTTPUBLISH:");//返回+QMTRECV:，表明接收到onenet平台
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
        IWDG_Feed();//喂狗

        REC_data = strstr((const char*)strx_MQTT_REC,(const char*)"\"data\"");		//指向了"data"

//				Uart1_SendStr(REC_data);
		
				i_parse = 0;
				while(*REC_data != '\0')
				{
						REC_data_parse[i_parse] = *REC_data;
						i_parse++;
						REC_data++;
				}	
				REC_data_parse[i_parse] = '\0';   //数组的最后一个字符设置为'\0'
				
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

    // 查找第一个 value 数字的起始位置
    char *value_start = strstr(mqtt_msg_cache, "\"value\":");
    if (value_start == NULL) 
		{
        Uart1_SendStr("There is no \"value\" field in JSON string\n");
//        return -1;
    }
    value_start += strlen("\"value\":");

    // 查找第一个 value 数字的结束位置
    char *value_end = strchr(value_start, '}');
    if (value_end == NULL) 
		{
        Uart1_SendStr("Unexpected end of JSON string\n");
//        return -1;
    }

    // 将“value”字段的值转换成数值并输出
    char value_str[32] = {'\0'};
    strncpy(value_str, value_start, value_end - value_start);
		

		float value_num = atof(value_str);
		//		Data_parse = value_str;
		return value_num;


}


uint64_t Parse_JSON_phone(char *mqtt_msg_cache)
{

    // 查找第一个 value 数字的起始位置
    char *value_start = strstr(mqtt_msg_cache, "\"value\":");
    if (value_start == NULL) 
		{
        Uart1_SendStr("There is no \"value\" field in JSON string\n");
//        return -1;
    }
    value_start += strlen("\"value\":");

    // 查找第一个 value 数字的结束位置
    char *value_end = strchr(value_start, '}');
    if (value_end == NULL) 
		{
        Uart1_SendStr("Unexpected end of JSON string\n");
//        return -1;
    }

    // 将“value”字段的值转换成数值并输出
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
    Uart1_SendStr("本次测试是打电话功能\r\n");
    printf("AT\r\n");
    delay_ms(500);
    printf("AT\r\n");
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    while(strx_call==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0\r\n"); //关闭回显
    delay_ms(500);
    Clear_Buffer();
    printf("AT+CSQ\r\n"); //检查CSQ
    delay_ms(500);
    /////////////////////////////////
    printf("AT+CPIN?\r\n");//检查SIM卡是否在位
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//查看是否返回ready
    while(strx_call==NULL)
    {
        Clear_Buffer();
        printf("AT+CPIN?\r\n");
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
    }
    Clear_Buffer();
    ///////////////////////////////////
    printf("AT+QICLOSE=0\r\n");//断开连接
    delay_ms(500);
    ///////////////////////////////////
    printf("AT+CREG?\r\n");//查看是否注册GSM网络
    delay_ms(500);
    strx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,1");//返回正常
    extstrx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,5");//返回正常，漫游
    while(strx_call==NULL&&extstrx_call==NULL)
    {
        Clear_Buffer();
        printf("AT+CREG?\r\n");//查看是否注册GSM网络
        delay_ms(500);
        strx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,1");//返回正常
        extstrx_call=strstr((const char*)RxBuffer,(const char*)"+CREG: 0,5");//返回正常，漫游
    }
    Clear_Buffer();

}
#endif



//text
#if 1
void ML302_Init_text(void)
{
    Uart1_SendStr("本次测试是发送英文短信功能\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
    printf("AT+MIPCLOSE=1\r\n");//关闭上一次连接
    delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
    printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//设置APN参数
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,2\r\n");//激活网络，PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//开启网络
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//启动网络接入
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx_text=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
    Clear_Buffer();
    while(strx_text==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx_text=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
    }
    printf("AT+CSQ\r\n");//查询信号
    delay_ms(300);
    Clear_Buffer();

}


//发送短信配置
void ML302_SendMessage(uint8_t *phone,uint8_t *data)
{
    printf("AT+CMGF=1\r\n");//设置文本模式
    delay_ms(300);
    printf("AT+CMGS=\"%s\"\r\n",phone);//发送手机号码
    delay_ms(300);
    printf("%s",data);
    delay_ms(300);
    while((USART2->SR&0X40)==0) {} //循环发送,直到发送完毕
    USART2->DR=(u8)0x1A;
    strx_text=strstr((const char*)RxBuffer,(const char*)"+CMGS:");//返回+CMGS: ,表明短信发送成功
    while(strx_text==NULL)
    {
        strx_text=strstr((const char*)RxBuffer,(const char*)"+CMGS:");//返回+CMGS: ,表明短信发送成功
    }
    Clear_Buffer();
}

#endif



//LBS
#if 1
void ML302_Init_LBS(void)
{
    Uart1_SendStr("本次测试是TCP功能\r\n");
    delay_ms(300);
    printf("AT\r\n");
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300);
    printf("AT+MIPCLOSE=1\r\n");//关闭上一次连接
    delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
    printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n");//设置APN参数
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1,1\r\n");//激活网络，PDP
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CSTT=\"CMNET\",\"\",\"\" \r\n");//开启网络
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CIICR\r\n");//启动网络接入
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返1
    Clear_Buffer();
    while(strx_LBS==NULL)
    {
        Clear_Buffer();
        printf("AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"+CGATT:1");//返回1,表明注网成功
    }
    printf("AT+CSQ\r\n");//查询信号
    delay_ms(300);
    Clear_Buffer();
    printf("AT+CGACT=1\r\n");//激活基站定位获取
    delay_ms(300);
    Clear_Buffer();

    IWDG_Feed();//喂狗
}




void ML302_CreateTCPSokcet(void)//创建sokcet
{

    printf("AT+MIPOPEN=1,\"TCP\",\"47.92.146.210\",8888,100,0,1,2,11002\r\n");//连接服务器
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,CONNECT OK");//返回CONNECT OK,TCP可以判断出是否有服务器存在
    while(strx_LBS==NULL)
    {
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,CONNECT OK");//返回CONNECTOK
    }

    Clear_Buffer();

    IWDG_Feed();//喂狗
}

//数据发送函数
void ML302_TCPSend(uint8_t *data,uint8_t len)
{
    printf("AT+MIPSEND=1,%d\r\n",len);//发送0socketIP和端口后面跟对应数据长度以及数据,
    delay_ms(300);
    printf("%s",data);
    delay_ms(300);
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,SEND OK");//返回	1,SEND OK,TCP可以判断出是否有服务器存在
    while(strx_LBS==NULL)
    {
        strx_LBS=strstr((const char*)RxBuffer,(const char*)"1,SEND OK");//返回	1,SEND OK
    }
    Clear_Buffer();
}




void ML302_RECData(void)
{
    strx_LBS=strstr((const char*)RxBuffer,(const char*)"+MIPURC:");//返回+MIPURC:，表明接收到TCP服务器发回的数据
    if(strx_LBS)
    {
        Clear_Buffer();
    }
}


#endif
