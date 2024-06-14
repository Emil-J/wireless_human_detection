/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "CC1101_E07_M1101D_SMA.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for CC1101 */
osThreadId_t CC1101Handle;
const osThreadAttr_t CC1101_attributes = {
  .name = "CC1101",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for HD */
osThreadId_t HDHandle;
const osThreadAttr_t HD_attributes = {
  .name = "HD",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartCC1101(void *argument);
void StartHD(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= 1;
	DWT->CYCCNT = 0;
}

__weak unsigned long getRunTimeCounterValue(void)
{
	return DWT->CYCCNT;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (1, sizeof(uint8_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of CC1101 */
  CC1101Handle = osThreadNew(StartCC1101, NULL, &CC1101_attributes);

  /* creation of HD */
  HDHandle = osThreadNew(StartHD, NULL, &HD_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartCC1101 */
/**
  * @brief  Function implementing the CC1101 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartCC1101 */
void StartCC1101(void *argument)
{
  /* USER CODE BEGIN StartCC1101 */
  uint8_t msg;
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(myQueue01Handle, &msg, 0, 0);
	  if(msg == 0xFF)
	  {
		  transmit_burst_byte_433();
	  }
	  osDelay(500 * configTICK_RATE_HZ / 500);
  }
  /* USER CODE END StartCC1101 */
}

/* USER CODE BEGIN Header_StartHD */
/**
* @brief Function implementing the HD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHD */
void StartHD(void *argument)
{
  /* USER CODE BEGIN StartHD */
  uint8_t msg;
  /* Infinite loop */
  for(;;)
  {
	  if(HAL_GPIO_ReadPin(HD_Out_GPIO_Port, HD_Out_Pin))
	  {
		  msg = 0xFF;
		  osMessageQueuePut(myQueue01Handle, &msg, 0, 0);
	  }
	  else
	  {
		  msg = 0x00;
		  osMessageQueuePut(myQueue01Handle, &msg, 0, 0);
	  }
	  osDelay(100 * configTICK_RATE_HZ / 100);
  }
  /* USER CODE END StartHD */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

