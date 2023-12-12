#ifndef __DEALWITH_H
#define	__DEALWITH_H

#include "main.h"




typedef struct {
    uint8_t Usart1;
    uint8_t Usart2;
    uint8_t Usart3;
    uint8_t Rtc;
    uint8_t Tim2;
    uint8_t Tim3;
    uint8_t Tim4;
} STRUCT_NVICPriority;



typedef struct
{

    float 		  temp;
    float 			hum;
    float 		  dust;
    float 			MQ2;
    uint32_t    CO2;
    uint32_t    TVOC;
    char 				relay;

    char 				Fire;
    char        people_state;
    uint32_t    motion_distance;
    uint32_t    static_distance;

    char				LED;
    char				BEEP;

} _DATA_STM32;


typedef struct
{

    uint32_t		temp;
    uint32_t    hum;
    uint32_t		dust;
    uint32_t    MQ2;
    uint32_t    TVOC;
    uint32_t    CO2;
    char 				relay;

    uint64_t 		phone;

    char				LED_ifwork;
    char				BEEP_ifwork;
    char				ASR_ifwork;
    char        ML302_ifwork;
    char        OLED_ifwork;


} _DATA_WeChat;


typedef struct
{

    uint32_t		temp;
    uint32_t    hum;
    uint32_t		dust;
    uint32_t    MQ2;
    uint32_t    TVOC;
    uint32_t    CO2;
    char 				relay;

    uint64_t 		phone;

    char				LED_ifwork;
    char				BEEP_ifwork;
    char				ASR_ifwork;
    char        ML302_ifwork;
    char        OLED_ifwork;


} _DATA_WeChat_cache;


typedef struct
{

    float 		  temp_cache;
    float 			hum_cache;
    float 		  dust_cache;
    float 			MQ2_cache;
    uint32_t    CO2_cache;
    uint32_t    TVOC_cache;

    char 				relay;

    char 				Fire_cache;

    char        people_state_cache;
    uint32_t    motion_distance_cache;
    uint32_t    static_distance_cache;

    char				LED;
    char				BEEP;

} _SENSOR;


extern _DATA_STM32 	 				STM32;
extern _DATA_WeChat  				WeChat;
extern _DATA_WeChat_cache 	WeChat_cache;

extern _SENSOR SENSOR;
extern STRUCT_NVICPriority NVICPriority_Structure;

extern char  RxBuffer[USART_REC_LEN_cache];
extern uint16_t RxCounter;




void deal_with(void);
void Calculate_SGP30( uint32_t *CO2_cache, uint32_t *TVOC_cache);
void Calculate_SHT30(float *temp_cache, float *hum_cache);


void Init_myself(void);
void Face_recognition_myself(void);
void Getting_Data_myself(void);
void printf_myself(void);
void OSD_myself(void);
void Upload_Receive_Data_STM32(void);
void Upload_Receive_Data_wechat(void);
void Warning_contorl_myself(void);





#endif 		//__DEALWITH_H

