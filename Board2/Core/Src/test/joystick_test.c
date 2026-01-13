#include "print.h"
#include "Board2.h"
#include "i2c.h"
#include "driver/pad_receiver.h"
#include "DWT.h"

// Test joystick: lettura via driver -> stampa -> delay -> repeat
void JoystickTest(void) {
    printMsg("\r\n--- START Joystick/PadReceiver Test ---\r\n");
    
    while (1) {

        uint32_t t0 = DWT_Begin();

        // ----------------------------------------
        // FASE 1: Richiesta Lettura (Asincrona)
        // ----------------------------------------
        // Avvia la transazione I2C verso il controller
        uint8_t status = PadReceiver_Request();

        if (status == 0) {
			printMsg("ERROR: PadReceiver_Request failed to start\r\n");
			HAL_Delay(1000); // Attesa prima del retry
			continue;         // Riprova
		}

        // ----------------------------------------
        // FASE 2: Attesa Completamento
        // ----------------------------------------
        // Polling attivo finché i dati non sono pronti
        while (!PadReceiver_IsDone()) {
            // attesa attiva
        };

        // ----------------------------------------
        // FASE 3: Copia dei Dati
        // ----------------------------------------
        // Trasferisce i dati dal buffer del driver alla struttura locale
        BUS_RemoteController rc_data;
        PadReceiver_Read(&rc_data);

        uint32_t cycles = DWT_End(t0);

        // ----------------------------------------
        // FASE 4: Stampa telemetria e debug
        // ----------------------------------------
        DWT_PrintCyclesAndUs("CTRL", cycles);

        printRemoteController(&rc_data);

        // Attesa 1 secondo (test bloccante)
        HAL_Delay(1000);
    }
}
