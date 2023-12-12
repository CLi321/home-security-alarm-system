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
static rt_thread_t 		ALARM_thread 		  		   = RT_NULL;
static rt_thread_t 		SENSOR_thread 		       = RT_NULL;
static rt_thread_t 		Interactive_thread 		   = RT_NULL;



///* �����ź������ƿ� */
//struct rt_semaphore sem;

/* �����ź������ƿ� */
rt_sem_t 	ALARM_sem 					= RT_NULL;
rt_sem_t 	SENSOR_sem 					= RT_NULL;
rt_sem_t 	Interactive_sem 		= RT_NULL;

uint32_t time_rec = 0; // ms ��ʱ����

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void ALARM_thread_entry(void* parameter);
static void SENSOR_thread_entry(void* parameter);
static void Interactive_thread_entry(void* parameter);



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
    ALARM_sem = rt_sem_create("ALARM_sem",		              /* �ź������� */
                              0,     								 			  /* �ź�����ʼֵ0 */
                              RT_IPC_FLAG_FIFO); 		 			  /* �ź���ģʽ FIFO(0x00)*/
    if (ALARM_sem != RT_NULL)
        rt_kprintf("�ź��������ɹ���\n\n");

    /**************************************************************************/

    /* ����һ���ź��� */
    SENSOR_sem = rt_sem_create("SENSOR_sem",						    /* �ź������� */
                               0,     									    /* �ź�����ʼֵ0 */
                               RT_IPC_FLAG_FIFO); 				  /* �ź���ģʽ FIFO(0x00)*/
    if (SENSOR_sem != RT_NULL)
        rt_kprintf("�ź��������ɹ���\n\n");

    /**************************************************************************/

    /* ����һ���ź��� */
    Interactive_sem = rt_sem_create("Interactive_sem",    	/* �ź������� */
                                    0,    								  /* �ź�����ʼֵ0 */
                                    RT_IPC_FLAG_FIFO); 		  /* �ź���ģʽ FIFO(0x00)*/
    if (Interactive_sem != RT_NULL)
        rt_kprintf("�ź��������ɹ���\n\n");

    /**************************************************************************/
    /**************************************************************************/

    /* �����߳�1 */
    ALARM_thread =                          	 				 /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "ALARM",            				 /* �߳����� */
                          ALARM_thread_entry,   			 /* �߳���ں��� */
                          RT_NULL,             				 /* �߳���ں������� */
                          512,                 				 /* �߳�ջ��С */
                          4,                  				 /* �̵߳����ȼ� */
                          20);                				 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (ALARM_thread != RT_NULL)
        rt_thread_startup(ALARM_thread);

    /**************************************************************************/

    /* �����߳�2 */
    SENSOR_thread =                        					   /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "SENSOR",             			 /* �߳����� */
                          SENSOR_thread_entry,  			 /* �߳���ں��� */
                          RT_NULL,             				 /* �߳���ں������� */
                          512,                				 /* �߳�ջ��С */
                          5,                  				 /* �̵߳����ȼ� */
                          20);                 				 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (SENSOR_thread != RT_NULL)
        rt_thread_startup(SENSOR_thread);

    /**************************************************************************/

    /* �����߳�3 */
    Interactive_thread =                               /* �߳̿��ƿ�ָ�� */
        rt_thread_create( "Interactive",               /* �߳����� */
                          Interactive_thread_entry,    /* �߳���ں��� */
                          RT_NULL,           				   /* �߳���ں������� */
                          512,                				 /* �߳�ջ��С */
                          5,                   				 /* �̵߳����ȼ� */
                          20);                				 /* �߳�ʱ��Ƭ */

    /* �����̣߳��������� */
    if (Interactive_thread != RT_NULL)
        rt_thread_startup(Interactive_thread);

    /**************************************************************************/
    return 0;
}

/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/


static void ALARM_thread_entry(void* parameter)
{
    rt_err_t err = RT_EOK;

    First_function();

    while (1)
    {
        rt_sem_take(ALARM_sem,	               /* ��ȡ�ź��� */
                    RT_WAITING_FOREVER);       /* �ȴ�ʱ�䣺һֱ�� */
        if(RT_EOK == err)
        {
            ALARM_function();
        }
    }
}

static void SENSOR_thread_entry(void* parameter)
{
    rt_err_t err = RT_EOK;

    while(1)
    {
        rt_sem_take(SENSOR_sem,	               /* ��ȡ�ź��� */
                    RT_WAITING_FOREVER);       /* �ȴ�ʱ�䣺һֱ�� */
        if(RT_EOK == err)
        {
            SENSOR_function();
        }

    }
}

static void Interactive_thread_entry(void* parameter)
{
    rt_err_t err = RT_EOK;

    while(1)
    {
        rt_sem_take(Interactive_sem,	        /* ��ȡ�ź��� */
                    RT_WAITING_FOREVER);      /* �ȴ�ʱ�䣺һֱ�� */
        if(RT_EOK == err)
        {
            Interactive_function();
        }

    }
}


/********************************END OF FILE****************************/