/******************************************************************************/

#include "main.h"
 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

		Init_myself();

    while(1)
    {
			
				GET_Data_Desired_myself();	
				IWDG_Feed();//ι��
			
				GET_Data_SENSOR_myself();		
				IWDG_Feed();//ι��
	
				GET_Data_ALARM_myself();		
				IWDG_Feed();//ι��
			
				ALARM_Handle_myself();
				IWDG_Feed();//ι��
						
				printf_myself();		
				OSD_myself();	
				IWDG_Feed();//ι��
				
				deal_with();
				POST_Data_myself();
				IWDG_Feed();//ι��

			
				delay_s(3);
				IWDG_Feed();//ι��
//				OLED_CLS();	
    }
}


/*********************************************END OF FILE**********************/
