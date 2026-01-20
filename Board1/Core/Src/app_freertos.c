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

// Simulink Model
#include "Board1.h"

// UART Handlers for communication inter board and debugging
#include "uart_functions.h"
#include "print.h"

// Driver lights
#include "lights_init.h"   // #include "a4wd3_led.h"

// Driver motors
#include "encoders_init.h"   // #include "encoders.h"
#include "motors_init.h"     // #include "motors_control.h"
// both #include "motor_constants.h"

#include "battery_init.h"        // #include "batt_level.h"
#include "temperature_init.h"    //#include "temperature_adc.h"

#include "DWT.h"

/* TEST */
#include "lights_test.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define REAL_TASK 1
// 1: Esegue il codice reale, 0: Simula il carico con HAL_Delay
#define PRINT 0
 // 1: Abilita stampe di debug, 0: Disabilita stampe di debug

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

volatile unsigned long ulHighFrequencyTimerTicks = 0;
volatile uint32_t MissPID = 0;
volatile uint32_t MissSupervisor = 0;
volatile uint32_t MissReadTemperature = 0;
volatile uint32_t MissReadBattery = 0;

/* USER CODE END Variables */
/* Definitions for PID */
osThreadId_t PIDHandle;
uint32_t PIDBuffer[ 1024 ];
osStaticThreadDef_t PIDControlBlock;
const osThreadAttr_t PID_attributes = {
  .name = "PID",
  .stack_mem = &PIDBuffer[0],
  .stack_size = sizeof(PIDBuffer),
  .cb_mem = &PIDControlBlock,
  .cb_size = sizeof(PIDControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
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
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ReadTemperature */
osThreadId_t ReadTemperatureHandle;
uint32_t ReadTemperatureBuffer[ 1024 ];
osStaticThreadDef_t ReadTemperatureControlBlock;
const osThreadAttr_t ReadTemperature_attributes = {
  .name = "ReadTemperature",
  .stack_mem = &ReadTemperatureBuffer[0],
  .stack_size = sizeof(ReadTemperatureBuffer),
  .cb_mem = &ReadTemperatureControlBlock,
  .cb_size = sizeof(ReadTemperatureControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for ReadBattery */
osThreadId_t ReadBatteryHandle;
uint32_t ReadBatteryBuffer[ 1024 ];
osStaticThreadDef_t ReadBatteryControlBlock;
const osThreadAttr_t ReadBattery_attributes = {
  .name = "ReadBattery",
  .stack_mem = &ReadBatteryBuffer[0],
  .stack_size = sizeof(ReadBatteryBuffer),
  .cb_mem = &ReadBatteryControlBlock,
  .cb_size = sizeof(ReadBatteryControlBlock),
  .priority = (osPriority_t) osPriorityAboveNormal2,
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

/* SCHEDULING FUNCTIONS */
static uint32_t ms_to_ticks(uint32_t ms);
static void periodic_wait(uint32_t *next_release, uint32_t period_ticks, volatile uint32_t *miss_counter);

/* DECISION FUNCTIONS */
static inline void actuate_white_leds(void);
static inline void actuate_red_leds(void);
static inline void change_set_point(void);
static inline void change_regulator(void);

/* USER CODE END FunctionPrototypes */

void StartPID(void *argument);
void StartSupervisor(void *argument);
void StartReadTemperature(void *argument);
void StartReadBattery(void *argument);
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
  /* creation of PID */
  PIDHandle = osThreadNew(StartPID, NULL, &PID_attributes);

  /* creation of Supervisor */
  SupervisorHandle = osThreadNew(StartSupervisor, NULL, &Supervisor_attributes);

  /* creation of ReadTemperature */
  ReadTemperatureHandle = osThreadNew(StartReadTemperature, NULL, &ReadTemperature_attributes);

  /* creation of ReadBattery */
  ReadBatteryHandle = osThreadNew(StartReadBattery, NULL, &ReadBattery_attributes);

  /* creation of StartSegger */
  StartSeggerHandle = osThreadNew(StartSeggerTask, NULL, &StartSegger_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartPID */
/**
 * @brief  Function implementing the PID thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartPID */
void StartPID(void *argument)
{
  /* USER CODE BEGIN StartPID */
    const uint32_t T = ms_to_ticks(T_PID);
    uint32_t next = osKernelGetTickCount();

    float current_speed[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    Board1_Y.setPoint.leftAxis = 0;
    Board1_Y.setPoint.rightAxis = 0;
    change_set_point();

    /* Infinite loop */
    for (;;) {
#if REAL_TASK
        for (int i = 0; i < 4; i++) {
            Encoder_Update(&encoders[i]);
            current_speed[i] = Encoder_GetSpeedRPM(&encoders[i]);
            MotorControl_Update(&motors[i], current_speed[i]);
        }

        Board1_U.speed = (BUS_Speed ) { current_speed[0], current_speed[1],
                        current_speed[2], current_speed[3] };

#if PRINT
        printMotorSpeeds(&Board1_U.speed);
        //HAL_Delay(2000); // Per non intasare la seriale
#endif

#else
        HAL_Delay(C_PID);
#endif

        periodic_wait(&next, T, &MissPID);
    }

    osThreadTerminate(osThreadGetId());

  /* USER CODE END StartPID */
}

/* USER CODE BEGIN Header_StartSupervisor */

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
	uint32_t wait_start;


	Board1_U.speed = (BUS_Speed ) { 0.0f, 0.0f, 0.0f, 0.0f };
	/* Infinite loop */
	for (;;) {

		//printMsg("Supervisor Cycle Start\r\n");
		wait_start = osKernelGetTickCount();
		do {
//			if ((osKernelGetTickCount() - wait_start) > ms_to_ticks(40)) {
//				printMsg("Supervisor timeout!\r\n");
//				break;
//			}
			Board1_step();
		} while (Board1_Y.supervision_ended != 1);

		Board1_U.continua = (Board1_U.continua == 0) ? 1 : 0;

		/* FINALIZZAZIONE DECISIONE */
		actuate_white_leds();
		actuate_red_leds();
		change_set_point();
		change_regulator();

#if PRINT

#endif
		// Stampa ogni 50 cicli
		static uint32_t cycle_count = 0;
		cycle_count++;
		if (cycle_count >= 50) {
			cycle_count = 0;
			//printMsg("Supervisor Cycle End\r\n");ù
			printGlobalState(&Board1_B.board1GlobalState);
//			printMsg("---- Supervisor Stats ----\r\n");
//            printLabel("Miss PID");
//            printInt(MissPID);
//            printNewLine();
//            printLabel("Miss Sup");
//            printInt(MissSupervisor);
//            printNewLine();
//            printLabel("Miss Temp");
//            printInt(MissReadTemperature);
//            printNewLine();
//            printLabel("Miss Batt");
//            printInt(MissReadBattery);
//            printNewLine();
		}
		HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET);
		periodic_wait(&next, T, &MissSupervisor);
	}

	osThreadTerminate(osThreadGetId());

  /* USER CODE END StartSupervisor */
}

/* USER CODE BEGIN Header_StartReadTemperature */
/**
 * @brief Function implementing the ReadTemperature thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadTemperature */
void StartReadTemperature(void *argument)
{
  /* USER CODE BEGIN StartReadTemperature */

    const uint32_t T = ms_to_ticks(T_TEMPERATURE);
    uint32_t next = osKernelGetTickCount();

    /* Infinite loop */
    for (;;) {
#if REAL_TASK
        Board1_U.temperature = (Temperature) temp_ky028_read_temperature(&temp_sensor);

#if PRINT
        printTemperature((float)Board1_U.temperature);
        //HAL_Delay(2000);
#endif

#else
        HAL_Delay(C_TEMPERATURE);
#endif

        periodic_wait(&next, T, &MissReadTemperature);
    }

    osThreadTerminate(osThreadGetId());

  /* USER CODE END StartReadTemperature */
}

/* USER CODE BEGIN Header_StartReadBattery */
/**
 * @brief Function implementing the ReadBattery thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadBattery */
void StartReadBattery(void *argument)
{
  /* USER CODE BEGIN StartReadBattery */

    const uint32_t T = ms_to_ticks(T_BATTERY);
    uint32_t next = osKernelGetTickCount();

    /* Infinite loop */
    for (;;) {
#if REAL_TASK
        Board1_U.batteryLevel = (BatteryLevel) battery_get_percentage_linear(
                battery_read_voltage(&battery), MIN_VOLTAGE, MAX_VOLTAGE);

#if PRINT
        printBattery((float)Board1_U.batteryLevel);
        //HAL_Delay(2000);
#endif

#else
        HAL_Delay(C_BATTERY);
#endif

        periodic_wait(&next, T, &MissReadBattery);
    }

    osThreadTerminate(osThreadGetId());

  /* USER CODE END StartReadBattery */
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
  for(;;)
  {
    break;
  }
  osThreadTerminate(osThreadGetId());

  /* USER CODE END StartSeggerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* SCHEDULING FUNCTIONS */

static uint32_t ms_to_ticks(uint32_t ms) {
	uint32_t hz = osKernelGetTickFreq();
	return (ms * hz + 999u) / 1000u;
}

static void periodic_wait(uint32_t *next_release, uint32_t period_ticks, volatile uint32_t *miss_counter) {
	uint32_t now = osKernelGetTickCount();

	/* Calcola il prossimo rilascio */
	*next_release += period_ticks;

	/* Controllo deadline miss (safe con wrap-around) */
//	if ((int32_t) (now - *next_release) > 0) {
//		/* Deadline miss: siamo già oltre il rilascio */
//		//HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_SET); // Accendo LED di errore
//		return 1;
//	}
	if ((int32_t)(now - *next_release) > 0) {
	    *next_release = now + period_ticks;
        if(miss_counter != NULL) {
            (*miss_counter)++;
        }
	    return;
	}

	/* Sleep assoluta fino al prossimo periodo */
	osDelayUntil(*next_release);
	//HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET); // Accendo LED di errore
}

/* DECISION FUNCTIONS */

static inline void actuate_white_leds(void) {
	A4WD3_White_Set(&led_left, Board1_DW.board1Decision.leds.white.left);
	A4WD3_White_Set(&led_right, Board1_DW.board1Decision.leds.white.right);
}

static inline void actuate_red_leds(void) {
	A4WD3_Red_Set(&led_left, Board1_DW.board1Decision.leds.red.left);
	A4WD3_Red_Set(&led_right, Board1_DW.board1Decision.leds.red.right);

	// Se deve fare toggle allora dopo il set si avvia un timer
}


static inline void change_set_point(void)
{
    const float left  = Board1_DW.board1Decision.setPoint.leftAxis;
    const float right = Board1_DW.board1Decision.setPoint.rightAxis;

    MotorControl_SetReferenceRPM(&motors[MOTOR_FRONT_LEFT],  left);
    MotorControl_SetReferenceRPM(&motors[MOTOR_REAR_LEFT],   left);

    MotorControl_SetReferenceRPM(&motors[MOTOR_FRONT_RIGHT], right);
    MotorControl_SetReferenceRPM(&motors[MOTOR_REAR_RIGHT],  right);
}

static inline void change_regulator(void)
{
    const uint8_t use_slow =
        (Board1_DW.board1Decision.roverAction == RA_BRAKING_SMOOTH);

    for (int i = 0; i < N_MOTORS; i++) {
        MotorControl_SelectSlow(&motors[i], 0);
    }
}
/* USER CODE END Application */

