#include "sonar_test.h"
#include "print.h"
#include "HCSR04.h"
#include "tim.h"
#include "DWT.h"

// Importo le variabili per il controllo di flusso dal main.c
extern volatile uint8_t rx_debug_byte;
extern volatile uint8_t flow_control_flag;

#define TEST_TIMEOUT 50

void testSonarSeq(void) {
	// 1. LEFT
//	hcsr04_trigger(&sonarLeft);
//	while (!hcsr04_is_done(&sonarLeft)){};
    uint32_t t0 = DWT_Begin();
    HAL_GPIO_TogglePin(LedDebug_GPIO_Port, LedDebug_Pin);
    uint32_t cycles = DWT_End(t0);
	DWT_PrintCyclesAndUs("LED", cycles);

	// 2. FRONT
//	hcsr04_trigger(&sonarFront);
//	while (!hcsr04_is_done(&sonarFront)){};
//
	// 3. RIGHT
	hcsr04_trigger(&sonarRight);
	while (!hcsr04_is_done(&sonarRight)){};
}

void testSonarParal(void) {
	// Trigger simultaneo (o quasi, sequenza istantanea)
	hcsr04_trigger(&sonarLeft);
	hcsr04_trigger(&sonarFront);
	hcsr04_trigger(&sonarRight);

	while ((!hcsr04_is_done(&sonarLeft) || !hcsr04_is_done(&sonarFront)
					|| !hcsr04_is_done(&sonarRight))) {};
}

void SonarTest(uint8_t mode) {

    // Avvia ricezione interrupt sul canale di debug
    HAL_UART_Receive_IT(getPrinterHandler(), (uint8_t*) &rx_debug_byte, 1);

    // Assicuriamoci che il flag sia pulito all'ingresso
    flow_control_flag = 0;

    while (1) {

        // CONTROLLO PAUSA/RIPRESA VIA UART
        if (flow_control_flag == 1) {
            printMsg("--- SONAR TEST PAUSED (Press Any Key to Resume) ---\r\n");
            
            // Consumo il flag di pausa
            flow_control_flag = 0; 
            
            // Blocco finché non arriva il prossimo byte (Resume)
            while(flow_control_flag == 0) {
                // Attesa attiva
            }

            printMsg("--- RESUMED ---\r\n");
            // Consumo il flag di ripresa
            flow_control_flag = 0;
        }


        printMsg("\r\n--- SONAR TEST CYCLE START ---\r\n");
        uint32_t t0 = DWT_Begin();
        // Seleziona la modalità desiderata:
		if (mode == 0) {
			testSonarSeq();
		} else if (mode == 1) {
			testSonarParal();
		} else {
			// Default
			testSonarSeq();
		}

        uint32_t cycles = DWT_End(t0);
		DWT_PrintCyclesAndUs("SONAR", cycles);

		BUS_Sonar busSonar = (BUS_Sonar ) { sonarLeft.distance,
						sonarFront.distance, sonarRight.distance };
		printSonar(&busSonar);

		// aspetta 1 secondo e riparte
		HAL_Delay(500);
	}
}
