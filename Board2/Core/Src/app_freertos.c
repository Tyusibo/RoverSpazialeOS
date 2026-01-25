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

#include "Board2.h" 	    // Simulink Model

#include "pad_receiver.h"   // Remote Controller
#include "gyroscope_init.h" // #include "mpu6050.h"
#include "sonar_init.h"     // #include "HCSR04.h"
#include "motors_init.h"
/*
 #include "motors_control.h"
 #include "motor_constants.h"
 #include "regulator.h"
 */

/* Utility */
#include "DWT.h"
#include "print.h"
#include "debug.h"

#include "scheduling_constants.h"
#include "event_flags_constant.h"

#if SEGGER_BUILD
#include "SEGGER_SYSVIEW_FreeRTOS.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
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

/* MISS COUNTERS */
volatile uint32_t MissReadController = 0;
volatile uint32_t MissReadGyroscope = 0;
volatile uint32_t MissSupervisor = 0;
volatile uint32_t MissReadSonars = 0;
volatile uint32_t MissPollingServer = 0;

/* STATUS FLAGS */
uint8_t pad_receiver_read_failed = 0;
uint8_t gyroscope_read_failed = 0;
uint8_t sonar_read_failed = 0;

/* USER CODE END Variables */
/* Definitions for ReadController */
osThreadId_t ReadControllerHandle;
uint32_t ReadControllerBuffer[2048];
osStaticThreadDef_t ReadControllerControlBlock;
const osThreadAttr_t ReadController_attributes = { .name = "ReadController",
		.stack_mem = &ReadControllerBuffer[0], .stack_size =
				sizeof(ReadControllerBuffer), .cb_mem =
				&ReadControllerControlBlock, .cb_size =
				sizeof(ReadControllerControlBlock), .priority =
				(osPriority_t) osPriorityHigh, };
/* Definitions for ReadGyroscope */
osThreadId_t ReadGyroscopeHandle;
uint32_t ReadGyroscopeBuffer[2048];
osStaticThreadDef_t ReadGyroscopeControlBlock;
const osThreadAttr_t ReadGyroscope_attributes = { .name = "ReadGyroscope",
		.stack_mem = &ReadGyroscopeBuffer[0], .stack_size =
				sizeof(ReadGyroscopeBuffer), .cb_mem =
				&ReadGyroscopeControlBlock, .cb_size =
				sizeof(ReadGyroscopeControlBlock), .priority =
				(osPriority_t) osPriorityAboveNormal, };
/* Definitions for Supervisor */
osThreadId_t SupervisorHandle;
uint32_t SupervisorBuffer[2048];
osStaticThreadDef_t SupervisorControlBlock;
const osThreadAttr_t Supervisor_attributes = { .name = "Supervisor",
		.stack_mem = &SupervisorBuffer[0], .stack_size =
				sizeof(SupervisorBuffer), .cb_mem = &SupervisorControlBlock,
		.cb_size = sizeof(SupervisorControlBlock), .priority =
				(osPriority_t) osPriorityNormal, };
/* Definitions for ReadSonars */
osThreadId_t ReadSonarsHandle;
uint32_t ReadSonarsBuffer[2048];
osStaticThreadDef_t ReadSonarsControlBlock;
const osThreadAttr_t ReadSonars_attributes = { .name = "ReadSonars",
		.stack_mem = &ReadSonarsBuffer[0], .stack_size =
				sizeof(ReadSonarsBuffer), .cb_mem = &ReadSonarsControlBlock,
		.cb_size = sizeof(ReadSonarsControlBlock), .priority =
				(osPriority_t) osPriorityLow, };
/* Definitions for StartSegger */
osThreadId_t StartSeggerHandle;
uint32_t StartSeggerBuffer[128];
osStaticThreadDef_t StartSeggerControlBlock;
const osThreadAttr_t StartSegger_attributes = { .name = "StartSegger",
		.stack_mem = &StartSeggerBuffer[0], .stack_size =
				sizeof(StartSeggerBuffer), .cb_mem = &StartSeggerControlBlock,
		.cb_size = sizeof(StartSeggerControlBlock), .priority =
				(osPriority_t) osPriorityHigh1, };
/* Definitions for Synchronization */
osThreadId_t SynchronizationHandle;
uint32_t SynchronizationBuffer[128];
osStaticThreadDef_t SynchronizationControlBlock;
const osThreadAttr_t Synchronization_attributes = { .name = "Synchronization",
		.stack_mem = &SynchronizationBuffer[0], .stack_size =
				sizeof(SynchronizationBuffer), .cb_mem =
				&SynchronizationControlBlock, .cb_size =
				sizeof(SynchronizationControlBlock), .priority =
				(osPriority_t) osPriorityLow, };
/* Definitions for PollingServer */
osThreadId_t PollingServerHandle;
uint32_t PollingServerBuffer[2048];
osStaticThreadDef_t PollingServerControlBlock;
const osThreadAttr_t PollingServer_attributes = { .name = "PollingServer",
		.stack_mem = &PollingServerBuffer[0], .stack_size =
				sizeof(PollingServerBuffer), .cb_mem =
				&PollingServerControlBlock, .cb_size =
				sizeof(PollingServerControlBlock), .priority =
				(osPriority_t) osPriorityHigh, };
/* Definitions for flagsOS */
osEventFlagsId_t flagsOSHandle;
osStaticEventGroupDef_t flagsOSControlBlock;
const osEventFlagsAttr_t flagsOS_attributes = { .name = "flagsOS", .cb_mem =
		&flagsOSControlBlock, .cb_size = sizeof(flagsOSControlBlock), };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static uint32_t ms_to_ticks(uint32_t ms);
static void periodic_wait(uint32_t *next_release, uint32_t period_ticks,
		volatile uint32_t *miss_counter);
static void waitForSynchonization(void);

/* USER CODE END FunctionPrototypes */

void StartReadController(void *argument);
void StartReadGyroscope(void *argument);
void StartSupervisor(void *argument);
void StartReadSonars(void *argument);
void StartSeggerTask(void *argument);
void StartSynchronization(void *argument);
void StartPollingServer(void *argument);

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
	ReadControllerHandle = osThreadNew(StartReadController, NULL,
			&ReadController_attributes);

	/* creation of ReadGyroscope */
	ReadGyroscopeHandle = osThreadNew(StartReadGyroscope, NULL,
			&ReadGyroscope_attributes);

	/* creation of Supervisor */
	SupervisorHandle = osThreadNew(StartSupervisor, NULL,
			&Supervisor_attributes);

	/* creation of ReadSonars */
	ReadSonarsHandle = osThreadNew(StartReadSonars, NULL,
			&ReadSonars_attributes);

	/* creation of StartSegger */
	StartSeggerHandle = osThreadNew(StartSeggerTask, NULL,
			&StartSegger_attributes);

	/* creation of Synchronization */
	SynchronizationHandle = osThreadNew(StartSynchronization, NULL,
			&Synchronization_attributes);

	/* creation of PollingServer */
	PollingServerHandle = osThreadNew(StartPollingServer, NULL,
			&PollingServer_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* Create the event(s) */
	/* creation of flagsOS */
	flagsOSHandle = osEventFlagsNew(&flagsOS_attributes);

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
void StartReadController(void *argument) {
	/* USER CODE BEGIN StartReadController */

	waitForSynchonization();

	const uint32_t T = ms_to_ticks(T_REMOTE_CONTROLLER);
	uint32_t next = osKernelGetTickCount();

	int8_t result = PAD_ERR;

	/* Infinite loop */
	for (;;) {

#if REAL_TASK
		result = PadReceiver_Request();

		if (result == PAD_ERR) {
//			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
//			break;         // Riprova
		}

		while (PadReceiver_GetStatus() == PAD_RX_IN_PROGRESS) {

		};

		// Trasferisce i dati dal buffer del driver alla struttura locale
		// La struttura locale del modello Simulink è copiata all'interno di uno stato locale
		// e mai più usata altrove, quindi non ci sono problemi di concorrenza
		PadReceiver_Read(&Board2_U.remoteController);

#if PRINT_TASK
		printRemoteController(&Board2_U.remoteController);
		HAL_Delay(2000); // Per non intasare la seriale
#endif

#else
		HAL_Delay(WCET_CONTROLLER);
#endif

		periodic_wait(&next, T, &MissReadController);
	}

	osThreadTerminate(osThreadGetId());
	/* USER CODE END StartReadController */
}

/* USER CODE BEGIN Header_StartReadGyroscope */
/**
 * @brief Function implementing the ReadGyroscope thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadGyroscope */
void StartReadGyroscope(void *argument) {
	/* USER CODE BEGIN StartReadGyroscope */

	waitForSynchonization();

	const uint32_t T = ms_to_ticks(T_GYROSCOPE);
	uint32_t next = osKernelGetTickCount();

	int8_t result = MPU_ERR;

	/* Infinite loop */
	for (;;) {

#if REAL_TASK
		result = MPU6050_Read_Yaw_IT(&hi2c3, &MPU6050_Yaw);

		if (result == MPU_ERR) {
//			HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
//			break;         // Riprova
		}

		while (MPU6050_GetStatus() == MPU_RX_IN_PROGRESS) {

		}
		MPU6050_Process_Yaw_IT_Data();

		// rivedere la struttua, magari fare una get come per il padreceiver
		Board2_U.gyroscope = MPU6050_Yaw.KalmanAngleZ;

#if PRINT_TASK
		printGyroscope((float)Board2_U.gyroscope);
		HAL_Delay(2000); // Per non intasare la seriale
#endif

#else
		HAL_Delay(WCET_GYROSCOPE);

#endif

		periodic_wait(&next, T, &MissReadGyroscope);
	}

	osThreadTerminate(osThreadGetId());

	/* USER CODE END StartReadGyroscope */
}

/* USER CODE BEGIN Header_StartSupervisor */
/**
 * @brief Function implementing the Supervisor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSupervisor */
void StartSupervisor(void *argument) {
	/* USER CODE BEGIN StartSupervisor */

	waitForSynchonization();

	const uint32_t T = ms_to_ticks(T_SUPERVISOR);
	uint32_t next = osKernelGetTickCount();

	/* Infinite loop */
	for (;;) {

		/* FAKE SONAR */
#include "controller_masks.h"

		// 0 = enabled, 1 = disabled
		static uint8_t disable_left = 0;
		static uint8_t disable_front = 0;
		static uint8_t disable_right = 0;

		uint32_t current = Board2_U.remoteController.buttons;
		uint32_t risingEdges = (~Board2_DW.previousButtons) & current;

		/* Toggle su fronte di salita */
		if (risingEdges & DISABLE_LEFT_SONAR)
			disable_left = !disable_left;

		if (risingEdges & DISABLE_FRONT_SONAR)
			disable_front = !disable_front;

		if (risingEdges & DISABLE_RIGHT_SONAR)
			disable_right = !disable_right;

		/* Applica disabilitazione */
		if (disable_left)
			Board2_U.sonar.left = 500;

		if (disable_front)
			Board2_U.sonar.front = 500;

		if (disable_right)
			Board2_U.sonar.right = 500;

		/* END FAKE SONAR */

		do {
			Board2_step();
		} while (Board2_Y.supervision_ended != 1);

//		static uint32_t counter_print = 0;
//		counter_print++;
//		if (counter_print >= 100) { // Approx 2 seconds (50ms * 40)
//			printMsg("Miss C:");
//			printInt(MissReadController);
//			printMsg(" G:");
//			printInt(MissReadGyroscope);
//			printMsg(" S:");
//			printInt(MissSupervisor);
//			printMsg(" Sn:");
//			printInt(MissReadSonars);
//			printNewLine();
//			counter_print = 0;
//		}

#if PRINT_TASK

#endif

		HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);
		periodic_wait(&next, T, &MissSupervisor);

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
void StartReadSonars(void *argument) {
	/* USER CODE BEGIN StartReadSonars */

	waitForSynchonization();

	const uint32_t T = ms_to_ticks(T_SONAR);
	const uint32_t TIMEOUT_TICKS = ms_to_ticks(40); // 40ms safety timeout
	uint32_t next = osKernelGetTickCount();

	uint32_t start_wait;

	/* Infinite loop */
	for (;;) {

#if REAL_TASK

		hcsr04_trigger(&sonarLeft);
		hcsr04_trigger(&sonarFront);
		hcsr04_trigger(&sonarRight);

		start_wait = osKernelGetTickCount();

		while (1) {

			if (all_sonar_done() == 1) {
				break; // Tutti i sonar hanno finito
			}

			// Timeout safety
			if ((osKernelGetTickCount() - start_wait) > TIMEOUT_TICKS) {
#if PRINT_TASK
				printMsg("Sonar read timeout!\r\n");
#endif
				break;
			}
		}

		// Valutazione esito lettura per ogni sensore
		hcsr04_handle_reading(&sonarLeft);
		hcsr04_handle_reading(&sonarFront);
		hcsr04_handle_reading(&sonarRight);

		Board2_U.sonar = (BUS_Sonar ) { sonarLeft.distance, sonarFront.distance,
						sonarRight.distance };
//		Board2_U.sonar = (BUS_Sonar ) { sonarLeft.distance, (uint16_t) 500,
//						sonarRight.distance };

#if PRINT_TASK
        printSonar(&Board2_U.sonar);
#endif

		periodic_wait(&next, T, &MissReadSonars);

#else
		HAL_Delay(WCET_SONAR);
#endif
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
void StartSeggerTask(void *argument) {
	/* USER CODE BEGIN StartSeggerTask */

#if SEGGER_BUILD
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();
	#endif

	/* Infinite loop */
	for (;;) {
		break;
	}
	osThreadTerminate(osThreadGetId());

	/* USER CODE END StartSeggerTask */
}

/* USER CODE BEGIN Header_StartSynchronization */
/**
 * @brief Function implementing the Synchronization thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSynchronization */
void StartSynchronization(void *argument) {
	/* USER CODE BEGIN StartSynchronization */

	osEventFlagsSet(flagsOSHandle, FLAG_SYNCHRONIZATION);

	/* Infinite loop */
	for (;;) {
		break;
	}

	osThreadTerminate(osThreadGetId());
	/* USER CODE END StartSynchronization */
}

/* USER CODE BEGIN Header_StartPollingServer */
/**
 * @brief Function implementing the PollingServer thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartPollingServer */
void StartPollingServer(void *argument) {
	/* USER CODE BEGIN StartPollingServer */

	waitForSynchonization();

	const uint32_t T = ms_to_ticks(T_POLLING_SERVER);
	uint32_t next = osKernelGetTickCount();

	/* Infinite loop */
	for (;;) {



		periodic_wait(&next, T, &MissPollingServer);

	}

	osThreadTerminate(osThreadGetId());

	/* USER CODE END StartPollingServer */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static uint32_t ms_to_ticks(uint32_t ms) {
	uint32_t hz = osKernelGetTickFreq();
	return (ms * hz + 999u) / 1000u;
}

static void periodic_wait(uint32_t *next_release, uint32_t period_ticks,
		volatile uint32_t *miss_counter) {
	uint32_t now = osKernelGetTickCount();

	/* Calcola il prossimo rilascio */
	*next_release += period_ticks;

	/* Controllo deadline miss (safe con wrap-around) */
	if ((int32_t) (now - *next_release) > 0) {
		*next_release = now + period_ticks;
		if (miss_counter != NULL) {
			(*miss_counter)++;
		}
		return;
	}

	/* Sleep assoluta fino al prossimo periodo */
	osDelayUntil(*next_release);
}

static void waitForSynchonization(void) {
	uint32_t flags = osEventFlagsWait(flagsOSHandle,
	FLAG_SYNCHRONIZATION,
	osFlagsWaitAny | osFlagsNoClear,
	osWaitForever);

	/* Se osEventFlagsWait fallisce ritorna un codice errore (valore negativo) */
	if ((int32_t) flags < 0) {
		osThreadTerminate(osThreadGetId());
		return;
	}

	/* Se per qualche motivo il flag non è presente (non dovrebbe accadere) */
	if ((flags & FLAG_SYNCHRONIZATION) == 0U) {
		osThreadTerminate(osThreadGetId());
		return;
	}

	/* OK */
	return;
}

/* USER CODE END Application */

