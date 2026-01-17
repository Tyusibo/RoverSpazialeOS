/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "scheduling_constants.h"

#include "Board2.h"
#include "pad_receiver.h"
#include "mpu6050.h"
#include "sonar_init.h"

/* Utility */
#include "DWT.h"
#include "print.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define REAL_TASK 0 // 1: Esegue il codice reale, 0: Simula il carico con HAL_Delay

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t cycles;
/* USER CODE END Variables */
/* Definitions for ReadController */
osThreadId_t ReadControllerHandle;
uint32_t ReadControllerBuffer[ 1024 ];
osStaticThreadDef_t ReadControllerControlBlock;
const osThreadAttr_t ReadController_attributes = {
  .name = "ReadController",
  .stack_mem = &ReadControllerBuffer[0],
  .stack_size = sizeof(ReadControllerBuffer),
  .cb_mem = &ReadControllerControlBlock,
  .cb_size = sizeof(ReadControllerControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ReadGyroscope */
osThreadId_t ReadGyroscopeHandle;
uint32_t ReadGyroscopeBuffer[ 1024 ];
osStaticThreadDef_t ReadGyroscopeControlBlock;
const osThreadAttr_t ReadGyroscope_attributes = {
  .name = "ReadGyroscope",
  .stack_mem = &ReadGyroscopeBuffer[0],
  .stack_size = sizeof(ReadGyroscopeBuffer),
  .cb_mem = &ReadGyroscopeControlBlock,
  .cb_size = sizeof(ReadGyroscopeControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for Supervisor */
osThreadId_t SupervisorHandle;
uint32_t SupervisorBuffer[ 1024 ];
osStaticThreadDef_t SupervisorControlBlock;
const osThreadAttr_t Supervisor_attributes = {
  .name = "Supervisor",
  .stack_mem = &SupervisorBuffer[0],
  .stack_size = sizeof(SupervisorBuffer),
  .cb_mem = &SupervisorControlBlock,
  .cb_size = sizeof(SupervisorControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ReadSonars */
osThreadId_t ReadSonarsHandle;
uint32_t ReadSonarsBuffer[ 1024 ];
osStaticThreadDef_t ReadSonarsControlBlock;
const osThreadAttr_t ReadSonars_attributes = {
  .name = "ReadSonars",
  .stack_mem = &ReadSonarsBuffer[0],
  .stack_size = sizeof(ReadSonarsBuffer),
  .cb_mem = &ReadSonarsControlBlock,
  .cb_size = sizeof(ReadSonarsControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for StartSegger */
osThreadId_t StartSeggerHandle;
uint32_t StartSeggerBuffer[ 128 ];
osStaticThreadDef_t StartSeggerControlBlock;
const osThreadAttr_t StartSegger_attributes = {
  .name = "StartSegger",
  .stack_mem = &StartSeggerBuffer[0],
  .stack_size = sizeof(StartSeggerBuffer),
  .cb_mem = &StartSeggerControlBlock,
  .cb_size = sizeof(StartSeggerControlBlock),
  .priority = (osPriority_t) osPriorityHigh1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static uint32_t ms_to_ticks(uint32_t ms);
static uint8_t periodic_wait(uint32_t *next_release, uint32_t period_ticks);
static uint8_t periodic_wait_no_led(uint32_t *next_release,
		uint32_t period_ticks);
/* USER CODE END FunctionPrototypes */

void StartReadController(void *argument);
void StartReadGyroscope(void *argument);
void StartSupervisor(void *argument);
void StartReadSonars(void *argument);
void StartSeggerTask(void *argument);

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
  /* creation of ReadController */
  ReadControllerHandle = osThreadNew(StartReadController, NULL, &ReadController_attributes);

  /* creation of ReadGyroscope */
  ReadGyroscopeHandle = osThreadNew(StartReadGyroscope, NULL, &ReadGyroscope_attributes);

  /* creation of Supervisor */
  SupervisorHandle = osThreadNew(StartSupervisor, NULL, &Supervisor_attributes);

  /* creation of ReadSonars */
  ReadSonarsHandle = osThreadNew(StartReadSonars, NULL, &ReadSonars_attributes);

  /* creation of StartSegger */
  StartSeggerHandle = osThreadNew(StartSeggerTask, NULL, &StartSegger_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartReadController */
/**
 * @brief  Function implementing the ReadController thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadController */
void StartReadController(void *argument)
{
  /* USER CODE BEGIN StartReadController */
	const uint32_t T = ms_to_ticks(T_REMOTE_CONTROLLER);
	uint32_t next = osKernelGetTickCount();
	/* Infinite loop */
	for (;;) {
#if REAL_TASK
        uint8_t status = PadReceiver_Request();

        if (status == 0) {
//			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
//			break;         // Riprova
        }

        while (!PadReceiver_IsDone()) {
            // attesa attiva
        };

		// Trasferisce i dati dal buffer del driver alla struttura locale
		// La struttura locale del modello Simulink è copiata all'interno di uno stato locale
		// e mai più usata altrove, quindi non ci sono problemi di concorrenza
		PadReceiver_Read(&Board2_U.remoteController);

#else
		HAL_Delay(C_CONTROLLER);
#endif

		periodic_wait_no_led(&next, T);
	}

	osThreadTerminate(osThreadGetId());
  /* USER CODE END StartReadController */
}

/* USER CODE BEGIN Header_StartReadGyroscope */
extern MPU6050_Yaw_t MPU6050_Yaw;
/**
 * @brief Function implementing the ReadGyroscope thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadGyroscope */
void StartReadGyroscope(void *argument)
{
  /* USER CODE BEGIN StartReadGyroscope */
	const uint32_t T = ms_to_ticks(T_GYROSCOPE); // T_GYROSCOPE
	uint32_t next = osKernelGetTickCount();
	/* Infinite loop */
	for (;;) {

#if REAL_TASK
		uint8_t status = MPU6050_Read_Yaw_IT(&hi2c3, &MPU6050_Yaw);

		if (status == 0) {
//			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
//			break;         // Riprova
		}

		while (!MPU6050_IsDone()) {
			// Attesa attiva
		}
		MPU6050_Process_Yaw_IT_Data();

		// rivedere la struttua, magari fare una get come per il padreceiver
		Board2_U.gyroscope = (double) MPU6050_Yaw.KalmanAngleZ;

		//printGyroscope(Board2_U.gyroscope);

#else
		HAL_Delay(C_GYROSCOPE);

#endif

		periodic_wait_no_led(&next, T);
	}

	osThreadTerminate(osThreadGetId());

  /* USER CODE END StartReadGyroscope */
}

/* USER CODE BEGIN Header_StartSupervisor */
#define Board2_IN_Execution            ((uint8_T)2U)
/**
 * @brief Function implementing the Supervisor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSupervisor */
void StartSupervisor(void *argument)
{
  /* USER CODE BEGIN StartSupervisor */
	const uint32_t T = ms_to_ticks(T_SUPERVISOR);
	uint32_t next = osKernelGetTickCount();
	/* Infinite loop */
	for (;;) {

		do {
			Board2_step();
		} while (Board2_DW.is_ExchangeDecision != Board2_IN_Execution);

		// Per permettere al modello di ripartire
		Board2_U.continua = (Board2_U.continua == 0) ? 1 : 0;
		HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);

		periodic_wait(&next, T);

	}

	osThreadTerminate(osThreadGetId());

  /* USER CODE END StartSupervisor */
}

/* USER CODE BEGIN Header_StartReadSonars */
/**
 * @brief Function implementing the ReadSonars thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadSonars */
void StartReadSonars(void *argument)
{
  /* USER CODE BEGIN StartReadSonars */
	const uint32_t T = ms_to_ticks(T_SONAR);
	uint32_t next = osKernelGetTickCount();
	/* Infinite loop */
	for (;;) {
#if REAL_TASK
// 1. Triggera i sensori (questo resetta rx_done a 0)
        hcsr04_trigger(&sonarLeft);
        hcsr04_trigger(&sonarFront);
        hcsr04_trigger(&sonarRight);

        // 2. Attesa attiva: il task non si sospende, ma cicla finché i dati non sono pronti.
        // Aggiungo un timeout di sicurezza usando il tick count per evitare blocchi infiniti
        // se si stacca un cavo (max 40ms, un sonar ne impiega max ~25ms).
        uint32_t start_wait = osKernelGetTickCount();
        
        while (1) {
            uint8_t d1 = hcsr04_is_done(&sonarLeft);
            uint8_t d2 = hcsr04_is_done(&sonarFront);
            uint8_t d3 = hcsr04_is_done(&sonarRight);

            if (d1 && d2 && d3) {
                break; // Tutti pronti
            }

            // Timeout safety (es. 40ms)
            if ((osKernelGetTickCount() - start_wait) > ms_to_ticks(40)) {
                break; 
            }
            
            // Non mettiamo osDelay qui perché hai chiesto "non voglio che il task si sospenda".
            // Il task occuperà la CPU in questo loop (utile se è a bassa priorità e viene preempted dagli altri).
        };

        // 3. Elaborazione condizionale: Aggiorna SOLO se la lettura è conclusa.
        // Se "is_done" è falso (timeout), saltiamo l'aggiornamento e manteniamo il valore vecchio (hold last value).
        if (hcsr04_is_done(&sonarLeft)) {
             hcsr04_process_distance(&sonarLeft);
        }

        if (hcsr04_is_done(&sonarFront)) {
             hcsr04_process_distance(&sonarFront);
        }

        if (hcsr04_is_done(&sonarRight)) {
             hcsr04_process_distance(&sonarRight);
        }

        // 4. Copia dati
        Board2_U.sonar = (BUS_Sonar ) { sonarLeft.distance, sonarFront.distance,
                        sonarRight.distance };
        
        printSonar(&Board2_U.sonar);
#else
		HAL_Delay(C_SONAR);
#endif
		periodic_wait_no_led(&next, T);
	}

	osThreadTerminate(osThreadGetId());
  /* USER CODE END StartReadSonars */
}

/* USER CODE BEGIN Header_StartSeggerTask */
/**
 * @brief Function implementing the StartSegger thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSeggerTask */
void StartSeggerTask(void *argument)
{
  /* USER CODE BEGIN StartSeggerTask */
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	/* Infinite loop */
	for (;;) {
		break;
	}
	osThreadTerminate(osThreadGetId());

  /* USER CODE END StartSeggerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static uint32_t ms_to_ticks(uint32_t ms) {
	uint32_t hz = osKernelGetTickFreq();
	return (ms * hz + 999u) / 1000u;
}

static uint8_t periodic_wait(uint32_t *next_release, uint32_t period_ticks) {
	uint32_t now = osKernelGetTickCount();

	/* Calcola il prossimo rilascio */
	*next_release += period_ticks;

	/* Controllo deadline miss (safe con wrap-around) */
	if ((int32_t) (now - *next_release) > 0) {
		/* Deadline miss: siamo già oltre il rilascio */
		//HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
		return 1;
	}

	/* Sleep assoluta fino al prossimo periodo */
	osDelayUntil(*next_release);
	//HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET); // Accendo LED di errore
	return 0;
}

static uint8_t periodic_wait_no_led(uint32_t *next_release,
		uint32_t period_ticks) {
	uint32_t now = osKernelGetTickCount();

	/* Calcola il prossimo rilascio */
	*next_release += period_ticks;

	/* Controllo deadline miss (safe con wrap-around) */
	if ((int32_t) (now - *next_release) > 0) {
		/* Deadline miss: siamo già oltre il rilascio */
		//HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
		return 1;
	}

	/* Sleep assoluta fino al prossimo periodo */
	osDelayUntil(*next_release);
	return 0;
}
/* USER CODE END Application */

