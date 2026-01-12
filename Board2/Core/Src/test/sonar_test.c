#include "sonar_test.h"
#include "print.h"
#include "HCSR04.h"
#include "tim.h"

// Extern dal main.c
extern volatile uint8_t sonarLeft_done;
extern volatile uint8_t sonarFront_done;
extern volatile uint8_t sonarRight_done;
extern uint8_t all_sonar_done(void);

uint8_t dummyReceiveSonar;
uint8_t flagReceiveSonar;

void SonarTest(void) {

    HAL_UART_Receive_IT(&huart2, &dummyReceiveSonar, 1); // Se vuoi sbloccarti via UART
    
    while (1) {
        // Timeout check variables
        uint32_t startTick;
        const uint32_t TEST_TIMEOUT = 50; // Un po' più lasso per il test

        // 1. LEFT
        sonarLeft_done = 0;
        hcsr04_trigger(&sonarLeft);
        startTick = HAL_GetTick();
        while (!sonarLeft_done && (HAL_GetTick() - startTick < TEST_TIMEOUT)) {
            if (flagReceiveSonar) break;
        }
        if (flagReceiveSonar) break; // Exit loop

        // 2. FRONT
        sonarFront_done = 0;
        hcsr04_trigger(&sonarFront);
        startTick = HAL_GetTick();
        while (!sonarFront_done && (HAL_GetTick() - startTick < TEST_TIMEOUT)) {
            if (flagReceiveSonar) break;
        }
        if (flagReceiveSonar) break; // Exit loop

        // 3. RIGHT
        sonarRight_done = 0;
        hcsr04_trigger(&sonarRight);
        startTick = HAL_GetTick();
        while (!sonarRight_done && (HAL_GetTick() - startTick < TEST_TIMEOUT)) {
            if (flagReceiveSonar) break;
        }
        
        // Verifica finale uscita
        if (flagReceiveSonar) {
            flagReceiveSonar = 0;
            // break; // Se vuoi uscire dal while(1) esterno
        }

        // stampa
        BUS_Sonar busSonar = (BUS_Sonar ) { sonarLeft.distance,
                        sonarFront.distance, sonarRight.distance };
        printSonar(&busSonar);

        // aspetta 1 secondo e riparte
        HAL_Delay(1000);
    }
}
