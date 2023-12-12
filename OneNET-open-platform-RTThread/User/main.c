/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 

/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               变量
*************************************************************************
*/



/*
* 程序清单：信号量用于任务同步
*/

/* 指向线程控制块的指针 *//* 定义线程控制块 */
static rt_thread_t 		communication_thread 	= RT_NULL;
static rt_thread_t 		sensor_thread 				= RT_NULL;
static rt_thread_t 		recognition_thread 		= RT_NULL;


///* 定义信号量控制块 */
//struct rt_semaphore sem;

/* 定义信号量控制块 */
rt_sem_t 	communication_sem 	= RT_NULL;
rt_sem_t 	recognition_sem 		= RT_NULL;
rt_sem_t 	sensor_sem 					= RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void communication_thread_entry(void* parameter);
static void sensor_thread_entry(void* parameter);
static void recognition_thread_entry(void* parameter);



/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
	
/**************************************************************************/		
	/* 创建一个信号量 */
	communication_sem = rt_sem_create("communication_sem",/* 信号量名字 */
                     0,     /* 信号量初始值0 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
  if (communication_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");
	
/**************************************************************************/		
	
	/* 创建一个信号量 */
	recognition_sem = rt_sem_create("recognition_sem",/* 信号量名字 */
                     0,     /* 信号量初始值0 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
  if (recognition_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");	

/**************************************************************************/		

	/* 创建一个信号量 */
	sensor_sem = rt_sem_create("sensor_sem",/* 信号量名字 */
                     0,     /* 信号量初始值0 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
  if (sensor_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");	

	

/**************************************************************************/

		/* 创建线程1 */
		communication_thread =                          	/* 线程控制块指针 */
			rt_thread_create( "communication",              /* 线程名字 */
												communication_thread_entry,   /* 线程入口函数 */
												RT_NULL,             					/* 线程入口函数参数 */
												512,                 					/* 线程栈大小 */
												4,                  					/* 线程的优先级 */
												20);                					/* 线程时间片 */
								
		/* 启动线程，开启调度 */
		if (communication_thread != RT_NULL)
			rt_thread_startup(communication_thread);
		
/**************************************************************************/
		
		/* 创建线程2 */
		sensor_thread =                        					 /* 线程控制块指针 */
			rt_thread_create( "sensor",             			 /* 线程名字 */
												sensor_thread_entry,  			 /* 线程入口函数 */
												RT_NULL,             				 /* 线程入口函数参数 */
												512,                				 /* 线程栈大小 */
												4,                  				 /* 线程的优先级 */
												20);                 				 /* 线程时间片 */
		
		/* 启动线程，开启调度 */		
		if (sensor_thread != RT_NULL)
			rt_thread_startup(sensor_thread);
		
/**************************************************************************/
		
		/* 创建线程3 */
		recognition_thread =                          /* 线程控制块指针 */
			rt_thread_create( "recognition",              /* 线程名字 */
												recognition_thread_entry,   /* 线程入口函数 */
												RT_NULL,             /* 线程入口函数参数 */
												512,                 /* 线程栈大小 */
												4,                   /* 线程的优先级 */
												20);                 /* 线程时间片 */
		
		/* 启动线程，开启调度 */	
		if (recognition_thread != RT_NULL)
			rt_thread_startup(recognition_thread);
		
/**************************************************************************/
		return 0;		
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/


static void recognition_thread_entry(void* parameter)
{	
		rt_err_t err = RT_EOK;
	
		while (1)
		{
				rt_sem_take(recognition_sem,	/* 获取信号量 */
										RT_WAITING_FOREVER); /* 等待时间：一直等 */	
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
				rt_sem_take(communication_sem,	/* 获取信号量 */
										RT_WAITING_FOREVER); /* 等待时间：一直等 */	
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
				rt_sem_take(sensor_sem,	/* 获取信号量 */
										RT_WAITING_FOREVER); /* 等待时间：一直等 */	
				if(RT_EOK == err)
				{
						Getting_Data_myself();
						OSD_myself();	
						printf_myself();					
				}

		}
}


/********************************END OF FILE****************************/
