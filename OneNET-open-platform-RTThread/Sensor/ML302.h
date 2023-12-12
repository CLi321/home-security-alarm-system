#ifndef __ML302_H
#define __ML302_H

#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
#include "usart2.h"
#include "usart.h"
#include "wdg.h"
#include "led_test.h"
#include "oled.h"
#include "GeneralTim.h"
#include "main.h"

#include "board.h"
#include "rtthread.h"

void ML302_Init(void);


void ML302_Init_MQTT(void);
void ML302_MQTTInit(void);

void ML302_MQTTPUBMessage_STM32( float temp_STM32, float hum_STM32, float dust_STM32, float MQ2_STM32,\
                                 uint32_t TVOC_STM32, uint32_t CO2_STM32, char relay_STM32,\
                                 char Fire_STM32, char people_state_STM32, uint32_t motion_distance_STM32, uint32_t static_distance_STM32,\
                                 char LED_STM32, char BEEP_STM32);

void ML302_MQTTPUBMessage_WeChat( uint32_t temp_WeChat, uint32_t hum_WeChat, uint32_t dust_WeChat, uint32_t MQ2_WeChat,\
                                  uint32_t TVOC_WeChat, uint32_t CO2_WeChat, char relay_WeChat,\
                                  uint64_t phone_WeChat,\
                                  char ASR_ifwork, char LED_ifwork, char BEEP_ifwork, char ML302_ifwork, char OLED_ifwork);


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





