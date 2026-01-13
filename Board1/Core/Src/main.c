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
#include "crc.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Board1.h"

// UART Handlers for communication inter board and debugging
#include "uart_functions.h"
#include "print.h"

// Test
#include "what2test.h"

// Driver lights
#include "lights_init.h"   // #include "a4wd3_led.h"
#include "lights_test.h"

// Driver motors
#include "encoders_init.h"   // #include "encoders.h"
#include "motors_init.h"     // #include "motors_control.h"
// both #include "motor_constants.h"
#include "motors_test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define Board1_IN_Execution            ((uint8_T)3U)

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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t rx_debug_byte;      // Buffer ricezione
volatile uint8_t flow_control_flag = 0; // 1 = Comando ricevuto
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_LPUART1_UART_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	// --- 1. CONFIGURAZIONE PERIFERICHE ---
	setComunicationHandler(&hlpuart1);

#if VERBOSE_DEBUG

	setPrinterHandler(&huart2); // Imposta UART per debug/print
	clearScreen();

#endif
	PRINT_DBG("BEGIN B1 INIT...\r\n");

	// Init Modello Simulink
	Board1_U.continua = 0;
	Board1_initialize();

	led_init();

	// Init Encoder
	Encoders_InitAll();
	Encoders_StartAll();

	// Init Motori (Include anche lo stop iniziale via UART)
    Motors_InitAll();

    //HAL_TIM_Base_Start_IT(&htim6);  // Per la control law

    // --- 2. VARIABILI DI CONFIGURAZIONE TEST ---
	uint8_t use_real_sensors = 0;

	// 0 = Continuo (gestito con Interrupt per Stop), 1 = Step-by-Step (Bloccante)
	uint8_t blocking_step_mode = 0;

	// Reset Flag interrupt
	flow_control_flag = 0;

	// --- 3. SELEZIONE TEST ---
	switch (WHAT_TO_TEST) {
	case TEST_LUCI:
		led_test_set();
		//led_test_toggle();
		//led_test_all();
		break;

	case TEST_MOTORI:
		float setPoint_test = -20.0f;
		test_open_loop(setPoint_test);
		//test_closed_loop(setPoint_test);
		break;

		// --- WITH SENSORS ---
	case TEST_COMMUNICATION_BYTE_RESTART_WITH_SENSORS:
		use_real_sensors = 1;
		blocking_step_mode = 1;
		PRINT_DBG("MODE: RESTART (SENSORS)\r\n");
		break;

	case TEST_COMMUNICATION_BYTE_STOP_WITH_SENSORS:
		use_real_sensors = 1;
		blocking_step_mode = 0;
		PRINT_DBG("MODE: RUN/STOP (SENSORS)\r\n");
#if VERBOSE_DEBUG

		// AVVIA RICEZIONE INTERRUPT per gestire lo Stop asincrono su UART2 (Printer)
		HAL_UART_Receive_IT(getPrinterHandler(), (uint8_t*) &rx_debug_byte, 1);

#endif
		break;

		// --- WITHOUT SENSORS ---
	case TEST_COMMUNICATION_BYTE_RESTART_WITHOUT_SENSORS:
		use_real_sensors = 0;
		blocking_step_mode = 1;
		PRINT_DBG("MODE: RESTART (NO SENS)\r\n");
		break;

	case TEST_COMMUNICATION_BYTE_STOP_WITHOUT_SENSORS:
		use_real_sensors = 0;
		blocking_step_mode = 0;
		PRINT_DBG("MODE: RUN/STOP (NO SENS)\r\n");
		// AVVIA RICEZIONE INTERRUPT per gestire lo Stop asincrono su UART2 (Printer)
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
			float current_speed[4];
			for (int i = 0; i < 4; i++) {
				Encoder_Update(&encoders[i]);
				current_speed[i] = Encoder_GetSpeedRPM(&encoders[i]);
			}
			Board1_U.speed = (BUS_Speed ) { current_speed[0], current_speed[1],
							current_speed[2], current_speed[3] };
			Board1_U.temperature = (Temperature) 35.5f;
			Board1_U.batteryLevel = (BatteryLevel) 12.0f;
		} else {
			Board1_U.speed = (BUS_Speed ) { 32.3f, 32.3, 32.3, 32.3 };
			Board1_U.temperature = (Temperature) 32.3;
			Board1_U.batteryLevel = (BatteryLevel) 32.3;
		}

		// 2. LOGICA DI CONTROLLO FLUSSO
		// Per "Byte Stop", controlliamo se dobbiamo FERMARCI
		if (blocking_step_mode == 0) { //se sono nella mod in cui devo premere per bloccarmi
			//HAL_Delay(TIME_TO_WAIT_BEFORE_RESTART);
			if (flow_control_flag == 1) { // se ho ricevuto un byte mi metto in pausa
				// Eravamo in corsa, è arrivato un byte -> PAUSA
				PRINT_DBG("--- PAUSED (Send Byte to Resume) ---\r\n");

				// Ferma Motori per sicurezza visuale. WTF???
				// for(int i=0; i<4; i++) MotorControl_HardStop(&motors[i]);

				flow_control_flag = 0; // Reset flag

				// Aspetta prossimo byte per ripartire (busy wait sul flag aggiornato dalla ISR)
				while (flow_control_flag == 0) {
					//HAL_Delay(50); // Piccolo delay per non saturare CPU inutilmente
				}

				PRINT_DBG("--- RESUMED ---\r\n");
				flow_control_flag = 0; // Reset flag per tornare a correre finché non ne arriva un altro
			}
		}

		PRINT_DBG("INIZIO COMUNICAZIONE B1 \r\n");

		// 3. MODEL STEP
		do {
			Board1_step();
		} while (Board1_DW.is_ExchangeDecision != Board1_IN_Execution);

		// 4. ATTUAZIONE MOTORI
		for (int i = 0; i < 4; i++) {
			float ref =
					(i == 0 || i == 3) ?
							Board1_Y.setPoint.leftAxis :
							Board1_Y.setPoint.rightAxis;
			// ref = 30;
			if (ref != 0) {
				MotorControl_SetReferenceRPM(&motors[i], ref);
				MotorControl_OpenLoopActuate(&motors[i]);
			} else {
				MotorControl_SetReferenceRPM(&motors[i], 0);

			}

		}

		// 5. LED
		A4WD3_White_Set(&led_left, Board1_DW.board1Decision.leds.white.left);
		A4WD3_White_Set(&led_right, Board1_DW.board1Decision.leds.white.right);

		// 6. Gestione LED debug per emergenze

		// Se la SA non è NONE sta rilevando qualcosa che impedisce di eseguire l'azione
		if (Board1_DW.board1Decision.safeAction != SA_NONE) {
			// Subito accendo il led
			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);
			// Avvio il timer che fa il toggle con la ISR
			if (htim7.State == HAL_TIM_STATE_READY) {
				HAL_TIM_Base_Start_IT(&htim7);
			}
		}

		// Se la SA non è NONE non sta rilevando qualcosa che impedisce di eseguire l'azione
		// Però può comunque star eseguendo un azione di emergenza iniziata
		if (Board1_DW.board1Decision.safeAction == SA_NONE) {
			// Subito spengo il led
			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET);
			// Fermo il timer che fa il toggle con la ISR
			if (htim7.Instance->CR1 & TIM_CR1_CEN) {
				HAL_TIM_Base_Stop_IT(&htim7);
			}
		}

		// 7. Per permettere al modello di ripartire
		Board1_U.continua = (Board1_U.continua == 0) ? 1 : 0;

#if VERBOSE_DEBUG
		printGlobalState(&(Board1_B.board1GlobalState));
		printDecision(&(Board1_DW.board1Decision));
#else
		/* Senza stampe il ciclo è troppo veloce e potrebbe causare:
		 1. Timeout prematuri nel protocollo se basati su contatori di step
		 2. Flooding della UART verso l'altra board
		 Inseriamo un delay per simulare il tempo di stampa. */
		//HAL_Delay(TIME_TO_WAIT_BEFORE_RESTART);
#endif

		// 5. ATTESA (Solo per modalità RESTART)
		if (blocking_step_mode == 1) {
			// Modalità Passo-Passo: il byte serve per AVANZARE

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

// Funzione di utilità per cambiare baudrate ST al volo
void Reinit_UART_Baudrate(UART_HandleTypeDef *huart, uint32_t baudrate) {
    huart->Init.BaudRate = baudrate;
    if (HAL_UART_Init(huart) != HAL_OK) {
        Error_Handler();
    }
}

void Motors_SetPermanentBaudRate_38400(UART_HandleTypeDef *huart, uint8_t address) {
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
