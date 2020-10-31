/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "ibus.h"
#include "rc_handler.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
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
/* Definitions for statusLedTask */
osThreadId_t statusLedTaskHandle;
uint32_t statusLedTaskBuffer[ 128 ];
osStaticThreadDef_t statusLedTaskControlBlock;
const osThreadAttr_t statusLedTask_attributes = {
  .name = "statusLedTask",
  .stack_mem = &statusLedTaskBuffer[0],
  .stack_size = sizeof(statusLedTaskBuffer),
  .cb_mem = &statusLedTaskControlBlock,
  .cb_size = sizeof(statusLedTaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for rcHandlerTask */
osThreadId_t rcHandlerTaskHandle;
uint32_t rcHandlerTaskBuffer[ 128 ];
osStaticThreadDef_t rcHandlerTaskControlBlock;
const osThreadAttr_t rcHandlerTask_attributes = {
  .name = "rcHandlerTask",
  .stack_mem = &rcHandlerTaskBuffer[0],
  .stack_size = sizeof(rcHandlerTaskBuffer),
  .cb_mem = &rcHandlerTaskControlBlock,
  .cb_size = sizeof(rcHandlerTaskControlBlock),
  .priority = (osPriority_t) osPriorityRealtime,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StatusLedTask(void *argument);
void RcHandlerTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of statusLedTask */
  statusLedTaskHandle = osThreadNew(StatusLedTask, NULL, &statusLedTask_attributes);

  /* creation of rcHandlerTask */
  rcHandlerTaskHandle = osThreadNew(RcHandlerTask, NULL, &rcHandlerTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StatusLedTask */
/**
  * @brief  Function implementing the statusLedTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StatusLedTask */
void StatusLedTask(void *argument)
{
  /* USER CODE BEGIN StatusLedTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(status_led_GPIO_Port, status_led_Pin);
    osDelay(600);
  }
  /* USER CODE END StatusLedTask */
}

/* USER CODE BEGIN Header_RcHandlerTask */
/**
* @brief Function implementing the rcHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RcHandlerTask */
void RcHandlerTask(void *argument)
{
  /* USER CODE BEGIN RcHandlerTask */
  uint8_t ibusData[IBUS_SERIAL_RX_PACKET_LENGTH];
  uint16_t checkSum = 0;
  uint16_t *checkSumPos = (uint16_t*)(ibusData+IBUS_SERIAL_RX_PACKET_LENGTH-2);
  /* Infinite loop */

  // Only call once
  initRcActionHandler();
  HAL_UART_Receive_IT(&huart1, ibusData, sizeof(ibusData));
  for(;;)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // Process only when the message changes
    if (*checkSumPos == checkSum) {
      continue;
    }

    checkSum = *checkSumPos;
    ibusUpdateChannel(ibusData);
    RcActionHandler();
  }

  /* USER CODE END RcHandlerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
