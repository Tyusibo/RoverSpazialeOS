/* USER CODE BEGIN Header */
/* PROVA COD GEN B2 */
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
#include "crc.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Board2.h"

// UART Handlers for communication inter board and debugging
#include "uart_functions.h"
#include "print.h"


// Sonar
#include "sonar_init.h"  // #include "HCSR04.h"
#include "sonar_test.h"

// Gyroscope
#include "gyroscope_init.h"  // #include "mpu6050.h"
#include "gyroscope_test.h"

// RemoteController
#include "joystick_test.h"
#include "driver/pad_receiver.h"

// Driver motors
#include "motors_init.h"  // #include "motors_control.h" e #include "motor_constants.h"

// DWT
#include "DWT.h"

/* debug configurations include */
#include "debug.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#if SEGGER_BUILD
/* DWT Registers for Cortex-M4 (STM32G4) */
#define DEMCR           (*((volatile uint32_t *)0xE000EDFC))
#define DWT_CTRL        (*((volatile uint32_t *)0xE0001000))
#define DWT_CYCCNT      (*((volatile uint32_t *)0xE0001004))

/* Bitmask */
#define TRCENA          (1 << 24)
#define DWT_CYCCNTENA   (1 << 0)
#endif

#define Board2_IN_Execution ((uint8_T)2U)

/* --- CONFIGURAZIONE DEBUG --- */

#if VERBOSE_DEBUG_MAIN == 1
#define PRINT_DBG(msg) printMsg(msg)
#else
#define PRINT_DBG(msg) ((void)0)
#endif
/* ---------------------------- */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// ROBA DI SONAR
// Variabili globali rimosse: ora il flag è dentro la struct hcsr04_t
// Verifica se tutti i sonar hanno completato la lettura
uint8_t all_sonar_done(void) {
	return (hcsr04_is_done(&sonarLeft) && hcsr04_is_done(&sonarFront)
			&& hcsr04_is_done(&sonarRight));
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t rx_debug_byte;
volatile uint8_t flow_control_flag = 0;

// REMOTE CONTROLLER: Gestito ora da driver/pad_receiver

// GYROSCOPE
MPU6050_Yaw_t MPU6050_Yaw;
volatile uint8_t gyro_done = 0; // Flag di completamento lettura
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
	/* 1. Enable Trace Registers access (TRCENA) */
	  DEMCR |= TRCENA;

	  /* 2. Reset the cycle counter to 0 */
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_I2C3_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	//init DWT
	DWT_Init();

	setComunicationHandler(&hlpuart1);
#if VERBOSE_DEBUG_MAIN
	setPrinterHandler(&huart2); // Imposta UART per debug
	clearScreen();
#endif
	PRINT_DBG("BEGIN B2 INIT...\r\n");

	// Init Modello Simulink
	Board2_initialize();
	Board2_U.remoteController = (BUS_RemoteController ) { 0, 0, 0 };
	Board2_U.gyroscope = 0.0f;
	Board2_U.sonar = (BUS_Sonar ) { 0, 0, 0 };

	// Init Sonar
	if (Sonar_InitAll() == 1) {
		PRINT_DBG("ERROR SONAR INIT\r\n");
	}
	StartSonarTimer();

	// Init Gyroscope (IMU)
	if (MPU6050_Init(&hi2c3) == 1) {
		PRINT_DBG("ERROR GYRO INIT\r\n");
	}

	// Init Motori
	//Motors_InitAll();
	//Motors_StartAllPwm();
	//Motors_SetDefaultCcr((uint32_t) 946);




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
