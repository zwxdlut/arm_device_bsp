#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "board.h"
#include "uart.h"
#include "can.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/*-----------------------------------------------------------------------------
 Priorities at which the tasks are created
-----------------------------------------------------------------------------*/
//#define TEST_LOW_PWR_MODE
#define BG_TASK_PRIORITY		            ( tskIDLE_PRIORITY + 2 )
#define	GEN_TASK_PRIORITY		            ( tskIDLE_PRIORITY + 1 )

static TimerHandle_t  g_timer; /**< Uart tx timer handle */

/******************************************************************************
* Function prototypes
******************************************************************************/
/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/*-----------------------------------------------------------------------------
 Tasks
------------------------------------------------------------------------------*/
static void prv_task( void *pvParameters );
static void prv_on_timer( TimerHandle_t xTimer );
extern void main_imp(void);
/*-----------------------------------------------------------------------------
 Callbacks
------------------------------------------------------------------------------*/
/******************************************************************************
 * Functions
 ******************************************************************************/
void rtos_start( void )
{	
	sys_init();
	gpio_init();
	uart_init(UART0_INDEX, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	uart_init(UART1_INDEX, 115200, UART_DATA_BITS_8, UART_STOP_BITS_1, UART_PARITY_MODE_NONE);
	can_init(CAN0_INDEX, NULL, 0);
	can_init(CAN1_INDEX, NULL, 0);	
	
	/* Start the four tasks as described in the comments at the top of this file */
	xTaskCreate( prv_task, "prv_task", configMINIMAL_STACK_SIZE, NULL, BG_TASK_PRIORITY, NULL );

	/* Create the software timer that is responsible for uart send */
	g_timer
		= xTimerCreate( "timer",             /* A text name, purely to help debugging. */
		    pdMS_TO_TICKS(500),              /* The timer period, in this case 500ms. */
			pdTRUE,						     /* This is a repeatedly timer, so xAutoReload is set to pdTRUE. */
			( void * ) 0,					 /* The ID is not used, so can be set to anything. */
			prv_on_timer                     /* The callback function . */
		);
				
	/* Start the tasks and timer running. */
	xTimerReset(g_timer, 0);
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

/******************************************************************************
 * Local Functions
 ******************************************************************************/
static void prv_task( void *pvParameters )
{
	(void)pvParameters;
	
	uint8_t  buf[100];
	uint16_t size = 0;
	uint32_t id = 0;
	
	while(1)
	{		
		for(uint8_t i = UART0_INDEX; i <= UART1_INDEX; i++)
		{
			if(0 != (size = uart_receive(i, buf, sizeof(buf))))
				uart_transmit(i, buf, size);
		}
		
		for(uint8_t i = CAN0_INDEX; i <= CAN1_INDEX; i++)
		{
			if(0 != (size = can_receive(i, &id, buf, sizeof(buf))))
			{
				can_transmit(i, id, buf, size);
			}
		}
	}
}

static void prv_on_timer( TimerHandle_t xTimer )
{
	configASSERT( xTimer );
	
	if(g_timer == xTimer)
	{
		GPIO_TOGGLE_PIN(LED0_GPIO, LED0_PIN);
#if defined TEST_LOW_PWR_MODE
		static uint8_t count = 0;
		count++;
		if(10 < count)
		{
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_ON);
			can_pwr_mode_trans(CAN0_INDEX, CAN_PWR_MODE_SLEEP);
			can_pwr_mode_trans(CAN1_INDEX, CAN_PWR_MODE_SLEEP);
			pwr_mode_trans(PWR_MODE_DEEPSLEEP);
			can_pwr_mode_trans(CAN0_INDEX, CAN_PWR_MODE_RUN);
			can_pwr_mode_trans(CAN1_INDEX, CAN_PWR_MODE_RUN);
			count = 0;
			GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_OFF);
		}
#endif
	}
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; )
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		GPIO_TOGGLE_PIN(LED1_GPIO, LED1_PIN);
	}
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; )
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		GPIO_TOGGLE_PIN(LED1_GPIO, LED1_PIN);
	}
}

void vApplicationIdleHook( void )
{
    volatile size_t xFreeHeapSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeHeapSpace = xPortGetFreeHeapSize();

	if( xFreeHeapSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}

/* The Blinky build configuration does not include run time stats gathering,
however, the Full and Blinky build configurations share a FreeRTOSConfig.h
file.  Therefore, dummy run time stats functions need to be defined to keep the
linker happy. */
void vMainConfigureTimerForRunTimeStats( void ) {}
unsigned long ulMainGetRunTimeCounterValue( void ) { return 0UL; }

/* A tick hook is used by the "Full" build configuration.  The Full and blinky
build configurations share a FreeRTOSConfig.h header file, so this simple build
configuration also has to define a tick hook - even though it does not actually
use it for anything. */
void vApplicationTickHook( void ) {}
