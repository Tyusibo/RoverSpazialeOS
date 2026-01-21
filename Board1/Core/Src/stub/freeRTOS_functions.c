#include "freeRTOS_functions.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "main.h"

void enterDegraded(){
    // Abort transmit e receive eventualmente

    // Board 1: Mortem

    // Board 2:

    // Chiusura task comunicazione

    // Avvio task single board

	HAL_GPIO_WritePin(Actuator_GPIO_Port, Actuator_Pin, GPIO_PIN_RESET); //
	osThreadTerminate(osThreadGetId());

}
