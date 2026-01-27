/* USER CODE BEGIN Header */
/* PROVA COD GEN B1 */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "crc.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* UART Handler for communication inter board */
#include "uart_functions.h"

/* Simulink Model */
#include "Board1.h"

/* Drivers */
#include "lights_init.h"     // #include "a4wd3_led.h"
#include "encoders_init.h"   // #include "encoders.h"
#include "motors_init.h"
/*
#include "motors_control.h"
#include "motor_constants.h"
#include "regulator.h"
*/
#include "battery_init.h"        // #include "batt_level.h"
#include "temperature_init.h"    //#include "temperature_adc.h"

/* Utility */
#include "DWT.h"
#include "print.h"
#include "debug.h"
#include "phase.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* --- DEBUG CONFIGURATIONS --- */
#if VERBOSE_DEBUG_MAIN == 1
#define PRINT_DBG(msg) printMsg(msg)
#else
	#define PRINT_DBG(msg) ((void)0)
#endif
/* ---------------------------- */
#if SEGGER_BUILD
/* DWT registers for Cortex-M4 (STM32G4) */
#define DEMCR           (*((volatile uint32_t *)0xE000EDFC))
#define DWT_CTRL        (*((volatile uint32_t *)0xE0001000))
#define DWT_CYCCNT      (*((volatile uint32_t *)0xE0001004))

/* Bitmask */
#define TRCENA          (1 << 24)
#define DWT_CYCCNTENA   (1 << 0)
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile system_phase_t system_phase = SYNCHRONIZATION_PHASE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
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
	#if SEGGER_BUILD
	/* 1. Enable access to Trace registers (TRCENA) */
	DEMCR |= TRCENA;

	/* 2. Reset counter */
	DWT_CYCCNT = 0;

	/* 3. Enable the cycle counter (CYCCNTENA) */
	DWT_CTRL |= DWT_CYCCNTENA;
	#endif
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
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_ADC2_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

	DWT_Init();

	setComunicationHandler(&hlpuart1);


	setPrinterHandler(&huart2);
	clearScreen();

#if VERBOSE_DEBUG_MAIN

	PRINT_DBG("BEGIN B1 INIT...\r\n");

#endif

	// Init Simulink Model
	Board1_initialize();
	Board1_U.speed = (BUS_Speed ) { 0.0f, 0.0f, 0.0f, 0.0f };
	Board1_U.batteryLevel = 90.0f;
	Board1_U.temperature = 30.0f;
//	Board1_U.batteryLevel = BatteryLevel_UNKNOWN;
//	Board1_U.temperature = Temperature_UNKNOWN;

	/* DRIVER INITIALIZATIONS */
	led_init();

	Encoders_InitAll();
	Encoders_StartAll();

	Motors_InitAll();
	Motors_StartAllPwm();
	Motors_SetDefaultCcr(720);

	temperature_sensor_init();
	battery_sensor_init();


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

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

/* USER CODE BEGIN 4 */

// Funzione di utilità per cambiare baudrate ST al volo
void Reinit_UART_Baudrate(UART_HandleTypeDef *huart, uint32_t baudrate) {
	huart->Init.BaudRate = baudrate;
	if (HAL_UART_Init(huart) != HAL_OK) {
		Error_Handler();
	}
}

void Motors_SetPermanentBaudRate_38400(UART_HandleTypeDef *huart,
		uint8_t address) {
	// 1. Configuriamo STM32 a 9600 (default Sabertooth)
	Reinit_UART_Baudrate(huart, 9600);
	HAL_Delay(100);

	// 2. Inviamo comando: Address, CMD=15, Value=4 (38400), Checksum
	uint8_t command = 15;
	uint8_t value = 4; // 4 = 38400 baud
	uint8_t checksum = (address + command + value) & 0x7F;

	uint8_t packet[4] = { address, command, value, checksum };
	HAL_UART_Transmit(huart, packet, 4, 100);

	HAL_Delay(200); // Tempo alla Sabertooth per scrivere EEPROM e cambiare baud

	// 3. Ora riconfiguriamo STM32 a 38400 per poter parlare
	Reinit_UART_Baudrate(huart, 38400);

	// Opzionale: inviare 0xAA per risincronizzare nel dubbio
	uint8_t sync = 0xAA;
	HAL_UART_Transmit(huart, &sync, 1, 10);
	HAL_Delay(100);
}

extern volatile unsigned long ulHighFrequencyTimerTicks; // Aggiungi se non visibile

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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
	while (1) {
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
