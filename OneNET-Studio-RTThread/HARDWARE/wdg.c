
#include "wdg.h"


//  *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4					0
//  *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8					1
//  *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16				2
//  *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32				3
//  *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64				4
//  *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128			5
//  *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256			6

//		aȡֵΪ0-6����Ӧ�����ע�ͣ�b(max 4095)
//    IWDG_Init( IWDGԤ��Ƶֵprer, IWDG��װ��ֵrlr );  IWDGԤ��Ƶֵprer /40 *IWDG��װ��ֵrlr = ʱ�� ms


void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����

    IWDG_SetPrescaler(prer);  //����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ64

    IWDG_SetReload(rlr);  //����IWDG��װ��ֵ

    IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������

    IWDG_Enable();  //ʹ��IWDG
}
//ι�������Ź�
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();//reload
}


