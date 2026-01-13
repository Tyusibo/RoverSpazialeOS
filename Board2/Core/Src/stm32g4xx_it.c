/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32g4xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32g4xx_it.h"
#include "driver/mpu6050.h"    // Includi i tuoi driver
#include "driver/pad_receiver.h"

// Fai extern delle handle se sono definite in main.c o i2c.c
extern I2C_HandleTypeDef hi2c1; 
// Se l'MPU è su un'altra I2C, ad esempio hi2c2, aggiungila qui
// extern I2C_HandleTypeDef hi2c2; 

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hcsr04.h"
#include "print.h"
#include "uart_functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* --- CONFIGURAZIONE DEBUG --- */
// 1 per abilitare le stampe, 0 per disabilitarle
#define VERBOSE_DEBUG 0

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1) {
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event interrupt / I2C1 wake-up interrupt through EXTI line 23.
  */
void I2C1_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_EV_IRQn 0 */

  /* USER CODE END I2C1_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_EV_IRQn 1 */

  /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_ER_IRQn 0 */

  /* USER CODE END I2C1_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt, DAC2 and DAC4 channel underrun error interrupts.
  */
void TIM7_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_DAC_IRQn 0 */

  /* USER CODE END TIM7_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_DAC_IRQn 1 */

  /* USER CODE END TIM7_DAC_IRQn 1 */
}

/**
  * @brief This function handles LPUART1 global interrupt.
  */
void LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN LPUART1_IRQn 0 */

  /* USER CODE END LPUART1_IRQn 0 */
  HAL_UART_IRQHandler(&hlpuart1);
  /* USER CODE BEGIN LPUART1_IRQn 1 */

  /* USER CODE END LPUART1_IRQn 1 */
}

/**
  * @brief This function handles I2C3 event interrupt / I2C3 wake-up interrupt through EXTI line 27.
  */
void I2C3_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C3_EV_IRQn 0 */

  /* USER CODE END I2C3_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c3);
  /* USER CODE BEGIN I2C3_EV_IRQn 1 */

  /* USER CODE END I2C3_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C3 error interrupt.
  */
void I2C3_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C3_ER_IRQn 0 */

  /* USER CODE END I2C3_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c3);
  /* USER CODE BEGIN I2C3_ER_IRQn 1 */

  /* USER CODE END I2C3_ER_IRQn 1 */
}

/* USER CODE BEGIN 1 */

extern hcsr04_t sonarLeft;
extern hcsr04_t sonarFront;
extern hcsr04_t sonarRight;

/* INTERRUPT CALLBACK FROM SONAR WITHOUT DEBUGGING */
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
//	if (htim->Instance != TIM2)
//		return;
//
//	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
//		(void) hcsr04_read_distance(&sonarLeft);
//	} else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
//		(void) hcsr04_read_distance(&sonarFront);
//	} else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
//		(void) hcsr04_read_distance(&sonarRight);
//	}
//}
/* Variabili definite in main.c */
extern volatile uint8_t sonarLeft_done;
extern volatile uint8_t sonarFront_done;
extern volatile uint8_t sonarRight_done;

/* INTERRUPT CALLBACK FROM SONAR FOR DEBUGGING */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance != TIM2)
		return;

	// Logica esistente: se il driver finisce la cattura (falling edge), alziamo il flag
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
		uint8_t before = sonarLeft.capture_flag;
		(void) hcsr04_read_distance(&sonarLeft); // La macchina a stati avanza
		if (before == 1 && sonarLeft.capture_flag == 0) // Falling edge rilevato e calcolo finito
			sonarLeft_done = 1;

	} else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		uint8_t before = sonarFront.capture_flag;
		(void) hcsr04_read_distance(&sonarFront);
		if (before == 1 && sonarFront.capture_flag == 0)
			sonarFront_done = 1;

	} else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		uint8_t before = sonarRight.capture_flag;
		(void) hcsr04_read_distance(&sonarRight);
		if (before == 1 && sonarRight.capture_flag == 0)
			sonarRight_done = 1;
	}
}
#include "sonar_test.h"

extern volatile uint8_t rx_debug_byte;
extern volatile uint8_t flow_control_flag;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	UART_HandleTypeDef *h = getComunicationHandler();
	if (h != NULL && huart->Instance == h->Instance) {
		PRINT_DBG("B2 Received\n\r");
		if (receivedFlag == 0) {
			receivedFlag = 1;
		}
		return;
	}

	// Verifica che l'interrupt arrivi dalla UART di debug (Printer)
	h = getPrinterHandler();
	if (h != NULL && huart->Instance == h->Instance) {
		if (flow_control_flag == 0) {
			flow_control_flag = 1;
		}

		// Ri-arma l'interrupt per il prossimo byte
		HAL_UART_Receive_IT(h, (uint8_t*) &rx_debug_byte, 1);

		return;
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);

	if (huart->Instance == LPUART1) {

		uint32_t err = huart->ErrorCode;

		PRINT_DBG("UART ERROR: ");

		if (err & HAL_UART_ERROR_ORE) {
			PRINT_DBG("ORE ");
		}
		if (err & HAL_UART_ERROR_FE) {
			PRINT_DBG("FE ");
		}
		if (err & HAL_UART_ERROR_NE) {
			PRINT_DBG("NE ");
		}
		if (err & HAL_UART_ERROR_PE) {
			PRINT_DBG("PE ");
		}

		PRINT_DBG("\r\n");

		// --- recovery minimo indispensabile ---
		__HAL_UART_CLEAR_OREFLAG(huart);
		__HAL_UART_CLEAR_FEFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);
		__HAL_UART_CLEAR_PEFLAG(huart);

		HAL_UART_AbortReceive_IT(huart);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM7) {
		HAL_GPIO_TogglePin(LedDebug_GPIO_Port, LedDebug_Pin);
	}

}

//nuov
/**
  * @brief  Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  // Gestione Pad Receiver (Master Receive) su I2C1 (esempio)
  if (hi2c->Instance == I2C1) {
    PadReceiver_RxCpltCallback();
  }
}

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  // Gestione MPU6050 (Mem Read) su I2C_MPU (sostituire con l'istanza corretta, es. I2C2 o I2C1)
  // Nota: MPU6050 usa I2C Mem Read, quindi va qui, NON in MasterRxCpltCallback
  
  // Esempio: se MPU è su I2C1 insieme al Pad, devi distinguere o usare handle diverse
  // Se MPU è su I2C2:
  // if (hi2c->Instance == I2C2) { ... }
  
  // Supponendo sia l'istanza corretta per l'MPU:
  MPU6050_Process_Yaw_IT_Data();
}

/**
  * @brief  I2C Error callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1) {
    PadReceiver_ErrorCallback();
  }
}

/* USER CODE END 1 */
