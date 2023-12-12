/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               ����
*************************************************************************
*/



/*
* �����嵥���ź�����������ͬ��
*/

/* ָ���߳̿��ƿ��ָ�� *//* �����߳̿��ƿ� */
static rt_thread_t 		communication_thread 	= RT_NULL;
static rt_thread_t 		sensor_thread 				= RT_NULL;
static rt_thread_t 		recognition_thread 		= RT_NULL;


///* �����ź������ƿ� */
//struct rt_semaphore sem;

/* �����ź������ƿ� */
rt_sem_t 	communication_sem 	= RT_NULL;
rt_sem_t 	recognition_sem 		= RT_NULL;
rt_sem_t 	sensor_sem 					= RT_NULL;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void communication_thread_entry(void* parameter);
static void sensor_thread_entry(void* parameter);
static void recognition_thread_entry(void* parameter);



/*
*************************************************************************
*                             main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
	
/**************************************************************************/		
	/* ����һ���ź��� */
	communication_sem = rt_sem_create("communication_sem",/* �ź������� */
                     0,     /* �ź�����ʼֵ0 */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
  if (communication_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");
	
/**************************************************************************/		
	
	/* ����һ���ź��� */
	recognition_sem = rt_sem_create("recognition_sem",/* �ź������� */
                     0,     /* �ź�����ʼֵ0 */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
  if (recognition_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");	

/**************************************************************************/		

	/* ����һ���ź��� */
	sensor_sem = rt_sem_create("sensor_sem",/* �ź������� */
                     0,     /* �ź�����ʼֵ0 */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
  if (sensor_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");	

	

/**************************************************************************/

		/* �����߳�1 */
		communication_thread =                          	/* �߳̿��ƿ�ָ�� */
			rt_thread_create( "communication",              /* �߳����� */
												communication_thread_entry,   /* �߳���ں��� */
												RT_NULL,             					/* �߳���ں������� */
												512,                 					/* �߳�ջ��С */
												4,                  					/* �̵߳����ȼ� */
												20);                					/* �߳�ʱ��Ƭ */
								
		/* �����̣߳��������� */
		if (communication_thread != RT_NULL)
			rt_thread_startup(communication_thread);
		
/**************************************************************************/
		
		/* �����߳�2 */
		sensor_thread =                        					 /* �߳̿��ƿ�ָ�� */
			rt_thread_create( "sensor",             			 /* �߳����� */
												sensor_thread_entry,  			 /* �߳���ں��� */
												RT_NULL,             				 /* �߳���ں������� */
												512,                				 /* �߳�ջ��С */
												4,                  				 /* �̵߳����ȼ� */
												20);                 				 /* �߳�ʱ��Ƭ */
		
		/* �����̣߳��������� */		
		if (sensor_thread != RT_NULL)
			rt_thread_startup(sensor_thread);
		
/**************************************************************************/
		
		/* �����߳�3 */
		recognition_thread =                          /* �߳̿��ƿ�ָ�� */
			rt_thread_create( "recognition",              /* �߳����� */
												recognition_thread_entry,   /* �߳���ں��� */
												RT_NULL,             /* �߳���ں������� */
												512,                 /* �߳�ջ��С */
												4,                   /* �̵߳����ȼ� */
												20);                 /* �߳�ʱ��Ƭ */
		
		/* �����̣߳��������� */	
		if (recognition_thread != RT_NULL)
			rt_thread_startup(recognition_thread);
		
/**************************************************************************/
		return 0;		
}

/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/


static void recognition_thread_entry(void* parameter)
{	
		rt_err_t err = RT_EOK;
	
		while (1)
		{
				rt_sem_take(recognition_sem,	/* ��ȡ�ź��� */
										RT_WAITING_FOREVER); /* �ȴ�ʱ�䣺һֱ�� */	
				if(RT_EOK == err)
				{
						Face_recognition_myself();	
				}
		}
}

static void communication_thread_entry(void* parameter)
{
		rt_err_t err = RT_EOK;
	
		ML302_Init();
		rt_sem_release(communication_sem);
		while(1)
		{
				rt_sem_take(communication_sem,	/* ��ȡ�ź��� */
										RT_WAITING_FOREVER); /* �ȴ�ʱ�䣺һֱ�� */	
				if(RT_EOK == err)
				{
						Upload_Receive_Data_STM32();
						Upload_Receive_Data_wechat();		
				}

		}
}

static void sensor_thread_entry(void* parameter)
{
		rt_err_t err = RT_EOK;

		while(1)
		{
				rt_sem_take(sensor_sem,	/* ��ȡ�ź��� */
										RT_WAITING_FOREVER); /* �ȴ�ʱ�䣺һֱ�� */	
				if(RT_EOK == err)
				{
						Getting_Data_myself();
						OSD_myself();	
						printf_myself();					
				}

		}
}


/********************************END OF FILE****************************/
