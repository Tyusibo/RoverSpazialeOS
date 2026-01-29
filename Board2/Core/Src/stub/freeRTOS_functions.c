#include "freeRTOS_functions.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "i2c.h"
#include "uart_functions.h"
#include "print.h"

// Extern thread handles
extern osThreadId_t ReadControllerHandle;
extern osThreadId_t ReadGyroscopeHandle;
extern osThreadId_t SupervisorHandle;
extern osThreadId_t ReadSonarsHandle;
extern osThreadId_t StartSeggerHandle;
extern osThreadId_t SynchronizationHandle;
extern osThreadId_t PollingServerHandle;

// Extern peripheral handles
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;

void enterDegraded(){
    // Abort transmit e receive eventualmente
    // Board 1: Mortem

    // Board 2:
    
    // Chiusura task comunicazione
    
    // Avvio task single board

    /*
    // 1. Kill all tasks
    osThreadTerminate(ReadControllerHandle);
    osThreadTerminate(ReadGyroscopeHandle);
    osThreadTerminate(SupervisorHandle);
    osThreadTerminate(ReadSonarsHandle);
    osThreadTerminate(StartSeggerHandle);
    osThreadTerminate(SynchronizationHandle);
    osThreadTerminate(PollingServerHandle);

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
    // Stop PWMs for Motors (TIM1) and Sonars (TIM2)
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim1);
    
    HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Stop_IT(&htim2);
    


    // 4. Stop I2C Communications
    HAL_I2C_Master_Abort_IT(&hi2c1); // Controller
    HAL_I2C_Master_Abort_IT(&hi2c3); // Gyroscope

    // 5. Reset GPIOs
    HAL_GPIO_WritePin(SonarLeftTrig_GPIO_Port, SonarLeftTrig_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SonarRightTrig_GPIO_Port, SonarRightTrig_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SonarFrontTrig_GPIO_Port, SonarFrontTrig_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LedDebug_GPIO_Port, LedDebug_Pin, GPIO_PIN_RESET);
    */

	HAL_GPIO_WritePin(RTR_OUT_GPIO_Port, RTR_OUT_Pin, GPIO_PIN_RESET); // Reset Ready To Receive
	osThreadTerminate(osThreadGetId());

}

#include "Board2.h"

void enterEmergency(){
	Board2_Y.supervision_ended = 1;
}

void enterFaulty(){
	Board2_Y.supervision_ended = 1;
}
