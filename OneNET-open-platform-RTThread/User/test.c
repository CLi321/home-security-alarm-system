/******************************************************************************/
#if 0
#include "main.h"
#include "bmp.h"


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

				Upload_Receive_Data_STM32();

        Face_recognition_myself();

        Getting_Data_myself();

        printf_myself();

        OSD_myself();

        IWDG_Feed();//ι��

				Warning_contorl_myself();

				Upload_Receive_Data_wechat();

        IWDG_Feed();//ι��
        OLED_Clear();

    }
}

#endif
/*********************************************END OF FILE**********************/
