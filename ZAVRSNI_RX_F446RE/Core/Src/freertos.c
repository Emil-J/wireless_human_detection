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
#include "LCD1602.h"
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
char serialStatus[50];
/* USER CODE END Variables */
/* Definitions for CC1101 */
osThreadId_t CC1101Handle;
const osThreadAttr_t CC1101_attributes = {
  .name = "CC1101",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for LCD */
osThreadId_t LCDHandle;
const osThreadAttr_t LCD_attributes = {
  .name = "LCD",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for BUZZER */
osThreadId_t BUZZERHandle;
const osThreadAttr_t BUZZER_attributes = {
  .name = "BUZZER",
  .stack_size = 200 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myQueue02 */
osMessageQueueId_t myQueue02Handle;
const osMessageQueueAttr_t myQueue02_attributes = {
  .name = "myQueue02"
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartCC1101(void *argument);
void StartLCD(void *argument);
void StartBUZZER(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
int _write(int file, char *ptr, int len)
{
   int i=0;

   (void)file;

   for(i=0 ; i<len ; i++)
   {
      ITM_SendChar((uint32_t)(*ptr++));
   }

   return len;
}

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
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

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

  /* creation of myQueue02 */
  myQueue02Handle = osMessageQueueNew (1, sizeof(uint8_t), &myQueue02_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of CC1101 */
  CC1101Handle = osThreadNew(StartCC1101, NULL, &CC1101_attributes);

  /* creation of LCD */
  LCDHandle = osThreadNew(StartLCD, NULL, &LCD_attributes);

  /* creation of BUZZER */
  BUZZERHandle = osThreadNew(StartBUZZER, NULL, &BUZZER_attributes);

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
	uint8_t rxData[5] = {0x00,0x00,0x00,0x00,0x00};
  /* Infinite loop */
	for(;;)
	{
	  osMutexAcquire(myMutex01Handle, osWaitForever);
	  if(receive_burst_byte_433(rxData))
	  {
		  if(rxData[2] == 0xFF)
		  {
			  msg = rxData[2];
			  osMessageQueuePut(myQueue01Handle, &msg, 0, 0);
			  osMessageQueuePut(myQueue02Handle, &msg, 0, 0);
		  }
	  }
	  else
	  {
		  msg = 0x00;
		  osMessageQueuePut(myQueue01Handle, &msg, 0, 0);
		  osMessageQueuePut(myQueue02Handle, &msg, 0, 0);
	  }
	  osMutexRelease(myMutex01Handle);
	  osDelay(10);
	}
  /* USER CODE END StartCC1101 */
}

/* USER CODE BEGIN Header_StartLCD */
/**
* @brief Function implementing the LCD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLCD */
void StartLCD(void *argument)
{
  /* USER CODE BEGIN StartLCD */
	uint8_t msg;
	osStatus_t status;

  /* Infinite loop */
  for(;;)
  {
	  osMutexAcquire(myMutex01Handle, osWaitForever);
	  if(osMessageQueueGetCount(myQueue01Handle))
	  {
		  status = osMessageQueueGet(myQueue01Handle, &msg, 0, 0);
		  if((status == osOK) && (msg == 0xFF))
		  {
			  LCD1602_Backlight();
			  LCD1602_Display();
		  }
	  }
	  osMutexRelease(myMutex01Handle);
	  osDelay(10);
  }
  /* USER CODE END StartLCD */
}

/* USER CODE BEGIN Header_StartBUZZER */
/**
* @brief Function implementing the BUZZER thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBUZZER */
void StartBUZZER(void *argument)
{
  /* USER CODE BEGIN StartBUZZER */
	uint8_t msg;
	osStatus_t status;
  /* Infinite loop */
  for(;;)
  {
	  osMutexAcquire(myMutex01Handle, osWaitForever);
	  if(osMessageQueueGetCount(myQueue02Handle))
	  {
		  status = osMessageQueueGet(myQueue02Handle, &msg, 0, 0);
		  if((status == osOK) && (msg == 0xFF))
		  {
			  for(int i = 0; i < 8; i++)
			  {
				  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
				  HAL_Delay(125);
				  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
				  HAL_Delay(125);
			  }
		  }
		  else if((status == osOK) && (msg == 0x00))
		  {
			  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
			  LCD1602_NoBacklight();
			  LCD1602_NoDisplay();
		  }
	  }
	  osMutexRelease(myMutex01Handle);
	  osDelay(10);
  }
  /* USER CODE END StartBUZZER */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

