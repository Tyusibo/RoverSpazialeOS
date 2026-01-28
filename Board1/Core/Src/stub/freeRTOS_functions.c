#include "freeRTOS_functions.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"
#include "uart_functions.h"
#include "print.h"

// Extern thread handles
extern osThreadId_t PIDHandle;
extern osThreadId_t SupervisorHandle;
extern osThreadId_t ReadTemperatureHandle;
extern osThreadId_t ReadBatteryHandle;
extern osThreadId_t StartSeggerHandle;
extern osThreadId_t SynchronizationHandle;

// Extern peripheral handles
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;


void enterDegraded(){
    // Abort transmit e receive eventualmente

    // Board 1: Mortem

    // Board 2:

    // Chiusura task comunicazione

    // Avvio task single board

    /*
    // 1. Kill all tasks
    osThreadTerminate(PIDHandle);
    osThreadTerminate(SupervisorHandle);
    osThreadTerminate(ReadTemperatureHandle);
    osThreadTerminate(ReadBatteryHandle);
    osThreadTerminate(StartSeggerHandle);
    osThreadTerminate(SynchronizationHandle);

    // 2. Abort UART communications
    UART_HandleTypeDef *comm_uart = getComunicationHandler();
    UART_HandleTypeDef *print_uart = getPrinterHandler();
    
    if (comm_uart != NULL) {
        HAL_UART_Abort(comm_uart);
        HAL_UART_Abort_IT(comm_uart);
    }
    if (print_uart != NULL) {
        HAL_UART_Abort(print_uart);
        HAL_UART_Abort_IT(print_uart);
    }

    // 3. Stop Timers
    // Stop PWMs for Motors (TIM1, TIM2, TIM3, TIM4 usually used for motors/encoders)
    // Check specific usage in main.c/motors_init.c to be precise, but stopping all is safe for degraded
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim1);
    
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim2);

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim3);
    
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim4);
    
    HAL_TIM_Base_Stop_IT(&htim8); // Likely auxiliary

    // 4. Stop ADCs
    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_Stop_IT(&hadc2);

    // 5. Reset GPIOs (Leds, Actuators)
    HAL_GPIO_WritePin(WhiteLedRight_GPIO_Port, WhiteLedRight_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RedLedRight_GPIO_Port, RedLedRight_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(WhiteLedLeft_GPIO_Port, WhiteLedLeft_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RedLedLeft_GPIO_Port, RedLedLeft_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET);
    */

	HAL_GPIO_WritePin(Actuator_GPIO_Port, Actuator_Pin, GPIO_PIN_RESET); //
	osThreadTerminate(osThreadGetId());

}
