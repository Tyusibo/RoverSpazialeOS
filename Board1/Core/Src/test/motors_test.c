#include "motors_test.h"

#include "stm32g4xx_hal.h"
#include "print.h"
#include <math.h>

#include "encoders_init.h"     // extern encoders[]
#include "motors_init.h"       // extern motors[], N_MOTORS
#include "encoder.h"
#include "motor_control.h"

#define PRINT_INTERVAL_MS 500
#define CONTROL_TOLERANCE 5.0f  // RPM difference to consider "at target"
#define MAX_SETTLE_TIME_MS 10000 // Timeout for closed loop settling

static void print_speeds(void) {
    printMsg("SPEEDS: [ ");
    for(int i=0; i<N_MOTORS; i++) {
        float spd = Encoder_GetSpeedRPM(&encoders[i]);
        printFloat(spd, 2);
        printMsg(" ");
    }
    printMsg("] RPM\r\n");
}

void test_open_loop(float ref) {
    printMsg("=== START TEST OPEN LOOP (Ref=");
    printFloat(ref, 2);
    printMsg(") ===\r\n");

    // 1. Applica Open Loop
    for (int i = 0; i < N_MOTORS; i++) {
        // Imposta il riferimento (che OpenLoopActuate userà come target volt/duty o rpm-open-loop)
        MotorControl_SetReferenceRPM(&motors[i], ref); 
        MotorControl_OpenLoopActuate(&motors[i]);
    }

    // 2. Loop di stampa
    while (1) {
        // Aggiorna encoder per leggere la velocità reale
        for (int i = 0; i < N_MOTORS; i++) {
            Encoder_Update(&encoders[i]);
        }
        
        print_speeds();
        HAL_Delay(PRINT_INTERVAL_MS);
    }
}

void test_closed_loop(float ref) {
    printMsg("=== START TEST CLOSED LOOP (Ref=");
    printFloat(ref, 2);
    printMsg(") ===\r\n");

    // 1. Setup Riferimento
    for (int i = 0; i < N_MOTORS; i++) {
        MotorControl_SetReferenceRPM(&motors[i], ref);
    }

    uint32_t t_start = HAL_GetTick();
    uint8_t at_steady_state = 0;

    // 2. Loop di Controllo fino a regime
    printMsg("--- Controlling... ---\r\n");
    while (!at_steady_state) {
        uint32_t t_now = HAL_GetTick();
        
        // Timeout
        if ((t_now - t_start) > MAX_SETTLE_TIME_MS) {
            printMsg("--- Timeout waiting for steady state ---\r\n");
            break;
        }

        // Simula il passo di campionamento
        uint32_t dt_ms = (uint32_t)(motors[0].Ts * 1000.0f);
        if (dt_ms == 0) dt_ms = 1;

        // Check convergenza
        int check_ok = 1;

        for (int i = 0; i < N_MOTORS; i++) {
            Encoder_Update(&encoders[i]);
            float spd = Encoder_GetSpeedRPM(&encoders[i]);
            
            // Calcola e applica U
            float u = MotorControl_ComputeU(&motors[i], spd);
            MotorControl_Actuate(&motors[i], u);

            // Verifica errore
            if (fabs(motors[i].reference_rpm - spd) > CONTROL_TOLERANCE) {
                check_ok = 0;
            }
        }

        if (check_ok) {
            // Se siamo stabili (nota: qui controlliamo un solo campione, 
            // per robustezza servirebbe un contatore, ma per ora va bene così)
            printMsg("--- Steady State Reached ---\r\n");
            at_steady_state = 1;
        }

        HAL_Delay(dt_ms);
    }

    // 3. Loop di Stampa a regime (U costante)
    // Il controllore non viene più chiamato, la U resta quella dell'ultima iterazione
    printMsg("--- Holding Last U & Printing Speeds ---\r\n");
    while (1) {
        // Basta aggiornare gli encoder per misurare, non attuiamo più
        for (int i = 0; i < N_MOTORS; i++) {
            Encoder_Update(&encoders[i]);
        }
        
        print_speeds();
        HAL_Delay(PRINT_INTERVAL_MS);
    }
}
