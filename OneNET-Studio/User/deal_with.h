#ifndef __DEALWITH_H
#define	__DEALWITH_H

#include "main.h"



typedef struct
{
	
		float 		  temp_cache;
		float 			hum_cache;
		float 		  dust_cache;
		uint32_t    CO2_cache;
		uint32_t    TVOC_cache;
	
		float 			MQ2_cache;		
		float 			Voltage_cache;
	
		char        people_state_cache;
	  uint32_t    motion_distance_cache;
		uint32_t    static_distance_cache;

		char		  	Face_cache;
		char				LED;		
		char 				relay;	
	
}_SENSOR;


typedef struct
{
		char				if_people;	
		char				Trespass;	
	
		char				Fire;
		char				Smoke_Gas;


		char				if_SLA;
		char 				if_SMS;	
		char 				if_CALL;	
		char 				if_Cloud;	
		uint64_t 		phone;	
	
		char				if_people_flag;	
		char				Trespass_flag;	
		char				Fire_flag;
		char				Smoke_Gas_flag;	
	
}_ALARM;

typedef struct
{
		char				ASR;
		char				SLA;
		char				ML302;
		char				OLED;
		char 				relay;
}_IFWORK;



typedef struct
{

		float 		  temp;
		float 			hum;
		float 		  dust;
		uint32_t    CO2;
		uint32_t    TVOC;
		float 			MQ2;		
		float 			Voltage;	
		char        people_state;
	  uint32_t    motion_distance;
		uint32_t    static_distance;

		char			  Face;
		char				LED;		
		char 				relay;		
	
		char			  if_people;	
		char				Trespass;	
		char				Fire;
		char				Smoke_Gas;

		char				if_SLA;
		char 				if_SMS;	
		char 				if_CALL;	
		char 				if_Cloud;	
		uint64_t 		phone;		
		
		char				if_people_flag;	
		char				Trespass_flag;	
		char				Fire_flag;
		char				Smoke_Gas_flag;	
	
		char				ASR_ifwork;
		char				SLA_ifwork;
		char        ML302_ifwork;
		char        OLED_ifwork;
		char 				relay_ifwork;
		
}_DATA_Interaction;




typedef struct
{
		char				ASR;
		char				SLA;
		char				ML302;
		char				OLED;
		char 				relay;
}_ASR_IF;
typedef struct
{
		char				ASR;
		char				SLA;
		char				ML302;
		char				OLED;
		char 				relay;
}_ONENET_IF;






typedef struct {
    uint8_t Usart1;
    uint8_t Usart2;
    uint8_t Usart3;
    uint8_t Rtc;
    uint8_t Tim2;
    uint8_t Tim3;
    uint8_t Tim4;
} STRUCT_NVICPriority;



extern _SENSOR 							SENSOR;
extern _ALARM 							ALARM;
extern _IFWORK 							IFWORK;

extern _DATA_Interaction 	 	STM32;
extern _DATA_Interaction 		REC_cache;
extern _DATA_Interaction 		Desired;

extern _ONENET_IF 					ONENET_IF;
extern _ASR_IF 							ASR_IF;


extern uint8_t phone_num[12];
extern STRUCT_NVICPriority NVICPriority_Structure;

extern char  RxBuffer[USART_REC_LEN_cache];
extern uint16_t RxCounter;




void deal_with(void);
void Calculate_SGP30( uint32_t *CO2_cache, uint32_t *TVOC_cache);	
void Calculate_SHT30(float *temp_cache, float *hum_cache);


void Init_myself(void);
void Face_recognition_myself(void);

void GET_Data_ALARM_myself(void);
void GET_Data_SENSOR_myself(void);
void GET_Data_Desired_myself(void);
void ALARM_Handle_myself(void);
void printf_myself(void);
void OSD_myself(void);
void POST_Data_myself(void);


void Uart1_SendFloat(float value);
char *Myself_itoa(int value, char *str, int base);
char *Myself_ftoa(float value, char *str, int base, int precision);


#endif 		//__DEALWITH_H

