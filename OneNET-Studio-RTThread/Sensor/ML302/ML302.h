#ifndef __ML302_H
#define __ML302_H

#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "ctype.h"
#include "usart2.h"
#include "usart.h"
#include "wdg.h"
#include "led_test.h"
#include "OLED_iic.h"
#include "main.h"


void ML302_Init(void);


void ML302_Init_MQTT(void);
void ML302_MQTTInit(void);

void ML302_MQTTPUB_POST( float temp, float hum, float dust, uint32_t CO2, uint32_t TVOC,\
												 char people_state, uint32_t motion_distance, uint32_t static_distance,\
												 char if_people, char Trespass, char Fire, char Smoke_Gas, uint64_t phone,\
												 char LED, char relay,\
												 char ASR_ifwork, char SLA_ifwork, char ML302_ifwork, char OLED_ifwork);

														 
void ML302_MQTTPUB_GET_desired(void);													 
void ML302_MQTTPUB_Delete_desired(void);
void ML302_MQTT_ParseData(void);
float Parse_JSON(char *mqtt_msg_cache);
uint64_t Parse_JSON_phone(char *mqtt_msg_cache);
												 
void ML302_MQTTSUB( void );
void ML302_MQTTUNSUB( void );
void ML302_MQTT_RECData(void);
void ML302_Dis_Re_Connect(void);


void ML302_Init_call(void);


void ML302_Init_text(void);
void ML302_SendMessage(uint8_t *phone,uint8_t *data);


void ML302_Init_LBS(void);
void ML302_CreateTCPSokcet(void);
void ML302_TCPSend(uint8_t *data,uint8_t len);
void ML302_RECData(void);



#endif





