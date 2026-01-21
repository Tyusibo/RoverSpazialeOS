#include "freeRTOS_functions.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2

void enterDegraded(){
    // Abort transmit e receive eventualmente

    // Board 1: Mortem

    // Board 2:

    // Chiusura task comunicazione

    // Avvio task single board
	osThreadTerminate(osThreadGetId());

}
