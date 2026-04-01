/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    EVT_LEFT,
    EVT_RIGHT,
    EVT_BOTH,
    EVT_NONE
} Event_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Definitions for IndicatorTask */
osThreadId_t IndicatorTaskHandle;
const osThreadAttr_t IndicatorTask_attributes = {
  .name = "IndicatorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ButtonTask */
osThreadId_t ButtonTaskHandle;
const osThreadAttr_t ButtonTask_attributes = {
  .name = "ButtonTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

typedef enum {
    STATE_IDLE,
    STATE_LEFT,
    STATE_RIGHT,
    STATE_HAZARD
} State_t;

State_t currentState = STATE_IDLE;

osMessageQueueId_t eventQueue;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartIndicatorTask(void *argument);
void StartButtonTask(void *argument);
void StartLEDTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  eventQueue = osMessageQueueNew(10, sizeof(Event_t), NULL);
  if(eventQueue == NULL)
  {
      Error_Handler();
  }

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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
  /* creation of IndicatorTask */
  IndicatorTaskHandle = osThreadNew(StartIndicatorTask, NULL, &IndicatorTask_attributes);

  /* creation of ButtonTask */
  ButtonTaskHandle = osThreadNew(StartButtonTask, NULL, &ButtonTask_attributes);

  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(StartLEDTask, NULL, &LEDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartIndicatorTask */
/**
  * @brief  Function implementing the IndicatorTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartIndicatorTask */
void StartIndicatorTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	Event_t evt;
  /* Infinite loop */
  for(;;)
  {
	  if(osMessageQueueGet(eventQueue, &evt, NULL, 100) == osOK)
	          {
		  	  switch(currentState)
		  	  {
		      case STATE_IDLE:
		          if(evt == EVT_LEFT) currentState = STATE_LEFT;
		          else if(evt == EVT_RIGHT) currentState = STATE_RIGHT;
		          else if(evt == EVT_BOTH) currentState = STATE_HAZARD;
		          break;

		      case STATE_LEFT:
		          if(evt == EVT_LEFT) currentState = STATE_IDLE;
		          else if(evt == EVT_RIGHT) currentState = STATE_RIGHT;
		          else if(evt == EVT_BOTH) currentState = STATE_HAZARD;
		          break;

		      case STATE_RIGHT:
		          if(evt == EVT_RIGHT) currentState = STATE_IDLE;
		          else if(evt == EVT_LEFT) currentState = STATE_LEFT;
		          else if(evt == EVT_BOTH) currentState = STATE_HAZARD;
		          break;

		      case STATE_HAZARD:
		          if(evt == EVT_LEFT || evt == EVT_RIGHT)
		              currentState = STATE_IDLE;
		          break;
		  	  }
	          }

	          osDelay(50);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartButtonTask */
/**
* @brief Function implementing the ButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartButtonTask */
void StartButtonTask(void *argument)
{
  /* USER CODE BEGIN StartButtonTask */
	Event_t evt;
	uint32_t left_press_time = 0;
	uint32_t right_press_time = 0;

	// persists across loop iterations
	static uint8_t hazard_sent = 0;
  /* Infinite loop */
  for(;;)
  {
	  int left = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
	      int right = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);

	      // HAZARD FIRST (highest priority)
	      if(left == GPIO_PIN_RESET && right == GPIO_PIN_RESET)
	      {
	        if(!hazard_sent)
	        {
	          evt = EVT_BOTH;
	          osMessageQueuePut(eventQueue, &evt, 0, 0);
	          hazard_sent = 1;
	        }

	        // reset individual timers
	        left_press_time = 0;
	        right_press_time = 0;
	      }
	      else
	      {
	        hazard_sent = 0;

	        // LEFT BUTTON
	        if(left == GPIO_PIN_RESET)
	        {
	          left_press_time += 100;
	          if(left_press_time >= 1000)
	          {
	            evt = EVT_LEFT;
	            osMessageQueuePut(eventQueue, &evt, 0, 0);
	            left_press_time = 0;
	          }
	        }
	        else
	        {
	          left_press_time = 0;
	        }

	        // RIGHT BUTTON
	        if(right == GPIO_PIN_RESET)
	        {
	          right_press_time += 100;
	          if(right_press_time >= 1000)
	          {
	            evt = EVT_RIGHT;
	            osMessageQueuePut(eventQueue, &evt, 0, 0);
	            right_press_time = 0;
	          }
	        }
	        else
	        {
	          right_press_time = 0;
	        }
	      }

    osDelay(100);
  }
  /* USER CODE END StartButtonTask */
}

/* USER CODE BEGIN Header_StartLEDTask */
/**
* @brief Function implementing the LEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void *argument)
{
  /* USER CODE BEGIN StartLEDTask */
  /* Infinite loop */
  for(;;)
  {
	  switch(currentState)
	      {
	          case STATE_LEFT:
	              HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	              osDelay(500);   // slower blink
	              break;

	          case STATE_RIGHT:
	              HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	              osDelay(500);   // same as left
	              break;

	          case STATE_HAZARD:
	              HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	              osDelay(200);   // faster blink
	              break;

	          default:
	              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	              osDelay(100);
	              break;
	      }

  }
  /* USER CODE END StartLEDTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
