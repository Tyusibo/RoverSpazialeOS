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

// Test
#include "what2test.h"

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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Board2_IN_Execution ((uint8_T)2U)

/* --- CONFIGURAZIONE DEBUG --- */
// 1 per abilitare le stampe, 0 per disabilitarle
#define VERBOSE_DEBUG 1

#if VERBOSE_DEBUG == 1
#define PRINT_DBG(msg) printMsg(msg)
#else
#define PRINT_DBG(msg) ((void)0)
#endif
/* ---------------------------- */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// ROBA DI SONAR
volatile uint8_t sonarLeft_done = 0;
volatile uint8_t sonarFront_done = 0;
volatile uint8_t sonarRight_done = 0;

// Rimuovo static inline per renderla visibile agli altri file (come sonar_test.c)
uint8_t all_sonar_done(void) {
	return (sonarLeft_done && sonarFront_done && sonarRight_done);
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Callback per gestire il completamento della ricezione I2C
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // Verifica che l'interrupt provenga dal bus del giroscopio (I2C3)
    if (hi2c->Instance == I2C3)
    {
        // Chiama la funzione di elaborazione dati del driver
        MPU6050_Yaw_RxCpltCallback(hi2c);
        
        // Segnala al main loop che i dati sono pronti
        gyro_done = 1;
    }
}

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
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_I2C3_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

	// --- 1. CONFIGURAZIONE PERIFERICHE ---
	setComunicationHandler(&hlpuart1);
#if VERBOSE_DEBUG
	setPrinterHandler(&huart2); // Imposta UART per debug
	clearScreen();
#endif
	PRINT_DBG("BEGIN B2 INIT...\r\n");

	// Init Modello Simulink
	Board2_U.continua = 0;
	Board2_initialize();

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

//  Prova con un riferimento in open loop
//	for (int i = 0; i < 4; i++) {
//		MotorControl_SetReferenceRPM(&motors[i], 0);
//		MotorControl_OpenLoopActuate(&motors[i]);
//	}

	// --- 2. VARIABILI DI CONFIGURAZIONE TEST ---
	uint8_t use_real_sensors = 0;
	uint8_t blocking_step_mode = 0;
	uint8_t use_sonar = 1; // Default attivo
	flow_control_flag = 0;

	// --- 3. SELEZIONE TEST ---
	switch (WHAT_TO_TEST) {
	case TEST_SONAR:
		SonarTest();
		break;
	case TEST_GYROSCOPE:
		GyroscopeTest();
		break;

	case TEST_REMOTE_CONTROLLER:
		JoystickTest();
		break;

		// --- WITH SENSORS ---
	case TEST_COMMUNICATION_BYTE_RESTART_WITH_SENSORS:
		use_real_sensors = 1;
		blocking_step_mode = 1;
		PRINT_DBG("B2 MODE: RESTART (SENSORS)\r\n");
		break;

	case TEST_COMMUNICATION_BYTE_STOP_WITH_SENSORS:
		use_real_sensors = 1;
		blocking_step_mode = 0;
		PRINT_DBG("B2 MODE: RUN/STOP (SENSORS)\r\n");
		// AVVIA RICEZIONE INTERRUPT
#if VERBOSE_DEBUG
        HAL_UART_Receive_IT(getPrinterHandler(), (uint8_t*) &rx_debug_byte, 1);
#endif
        break;

    case TEST_COMMUNICATION_NOSONAR:
        use_real_sensors = 1;
        blocking_step_mode = 0;
        use_sonar = 0; // Disabilita sonar
        PRINT_DBG("B2 MODE: RUN/STOP (NO SONAR)\r\n");
        // AVVIA RICEZIONE INTERRUPT
#if VERBOSE_DEBUG
        HAL_UART_Receive_IT(getPrinterHandler(), (uint8_t*) &rx_debug_byte, 1);
#endif
        break; 

        // --- WITHOUT SENSORS ---
	case TEST_COMMUNICATION_BYTE_RESTART_WITHOUT_SENSORS:
		use_real_sensors = 0;
		blocking_step_mode = 1;
		PRINT_DBG("B2 MODE: RESTART (NO SENS)\r\n");
		break;

	case TEST_COMMUNICATION_BYTE_STOP_WITHOUT_SENSORS:
		use_real_sensors = 0;
		blocking_step_mode = 0;
		PRINT_DBG("B2 MODE: RUN/STOP (NO SENS)\r\n");
		// AVVIA RICEZIONE INTERRUPT
#if VERBOSE_DEBUG
		HAL_UART_Receive_IT(getPrinterHandler(), (uint8_t*) &rx_debug_byte, 1);
#endif
		break;

	default:
		break;
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        // 1. INPUT (Sensori o Dummy)
        if (use_real_sensors) {
            // --- JOYSTICK ---
            // Avvia ricezione e attendi completamento
            PadReceiver_Request();
            uint32_t startTickPad = HAL_GetTick();
            while (!PadReceiver_IsDone()); // attesa attiva
            PadReceiver_Read(&Board2_U.remoteController);

            // --- GYRO ---
            gyro_done = 0; // Reset flag prima di avviare
            MPU6050_Read_Yaw_IT(&hi2c3, &MPU6050_Yaw);
            
            // Attesa con Timeout di 50ms
            uint32_t startTickGyro = HAL_GetTick();
            while (!gyro_done && (HAL_GetTick() - startTickGyro < 50));

            // Se timer scade, si prende l'ultimo valore valido
            Board2_U.gyroscope = (double)MPU6050_Yaw.KalmanAngleZ;

            // --- SONAR ---
            if (use_sonar) {
                // Timeout per ogni singolo sensore (ms)
                // 30ms è sufficiente per ~5 metri
                const uint32_t SONAR_TIMEOUT = 30; 
                uint32_t startTick;

                // 1. LEFT
                sonarLeft_done = 0;
                hcsr04_trigger(&sonarLeft);
                startTick = HAL_GetTick();
                while (!sonarLeft_done && (HAL_GetTick() - startTick < SONAR_TIMEOUT));

                // 2. FRONT
                sonarFront_done = 0;
                hcsr04_trigger(&sonarFront);
                startTick = HAL_GetTick();
                while (!sonarFront_done && (HAL_GetTick() - startTick < SONAR_TIMEOUT));

                // 3. RIGHT
                sonarRight_done = 0;
                hcsr04_trigger(&sonarRight);
                startTick = HAL_GetTick();
                while (!sonarRight_done && (HAL_GetTick() - startTick < SONAR_TIMEOUT));

                Board2_U.sonar = (BUS_Sonar ) { sonarLeft.distance,
                                sonarFront.distance, sonarRight.distance };
            } else {
                // Dummy Values se sonar disattivato
                Board2_U.sonar = (BUS_Sonar ) { 400, 400, 400 };
            }

        } else {
            // --- DUMMY ---
            // Simuliamo ostacoli o quiete
            Board2_U.sonar = (BUS_Sonar ) { 400, 400, 400 }; // Lontano ostacoli
            Board2_U.gyroscope = (Gyroscope) 32.3f;
            Board2_U.remoteController = (BUS_RemoteController ) { 0, 30, 0 };
        }

		// 2. CHECK STOP/PAUSA
		if (blocking_step_mode == 0) {
			//HAL_Delay(TIME_TO_WAIT_BEFORE_RESTART);
			if (flow_control_flag == 1) {
				PRINT_DBG("--- B2 PAUSED ---\r\n");
				flow_control_flag = 0;
				while (flow_control_flag == 0) {
					//HAL_Delay(50);
				}
				PRINT_DBG("--- B2 RESUMED ---\r\n");
				flow_control_flag = 0;
			}
		}

		PRINT_DBG("INIZIO COMUNICAZIONE B2 \r\n");

		// 3. MODEL STEP
		do {
			Board2_step();
		} while (Board2_DW.is_ExchangeDecision != Board2_IN_Execution);

		// 4. Gestione LED debug per emergenze

		// Se la SA non è NONE sta rilevando qualcosa che impedisce di eseguire l'azione
		if (Board2_DW.board2Decision.safeAction != SA_NONE) {
			// Subito accendo il led
			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);
			// Avvio il timer che fa il toggle con la ISR
			if (htim7.State == HAL_TIM_STATE_READY) {
				HAL_TIM_Base_Start_IT(&htim7);
			}
		}

		// Se la SA non è NONE non sta rilevando qualcosa che impedisce di eseguire l'azione
		// Però può comunque star eseguendo un azione di emergenza iniziata
		if (Board2_DW.board2Decision.safeAction == SA_NONE) {
			// Subito spengo il led
			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET);
			// Fermo il timer che fa il toggle con la ISR
			if (htim7.Instance->CR1 & TIM_CR1_CEN) {
				HAL_TIM_Base_Stop_IT(&htim7);
			}
		}

		// 5. Per permettere al modello di ripartire
		Board2_U.continua = (Board2_U.continua == 0) ? 1 : 0;

#if VERBOSE_DEBUG
		printGlobalState(&(Board2_B.board2GlobalState));
		printDecision(&(Board2_DW.board2Decision));
#else
		/* Senza stampe il ciclo è troppo veloce e potrebbe causare problemi di sincronizzazione.
		 Inseriamo un delay per simulare il tempo di stampa. */
		//HAL_Delay(TIME_TO_WAIT_BEFORE_RESTART);
#endif

		// 6. ATTESA (Solo per RESTART)
		if (blocking_step_mode == 1) {
#if VERBOSE_DEBUG
			uint8_t dummy;
			HAL_UART_Receive(getPrinterHandler(), &dummy, 1, HAL_MAX_DELAY);
#endif
		}
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
