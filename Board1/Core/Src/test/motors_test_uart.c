//#include "motors_test.h"
//
//#include "stm32g4xx_hal.h"
//#include "print.h"
//#include <math.h>
//#include <motor_control_uart.h>
//
//#include "encoders_init.h"     // extern encoders[]
//#include "motors_init.h"       // extern motors[], N_MOTORS
//#include "encoder.h"
//
//#define PRINT_INTERVAL_MS 500
//#define CONTROL_TOLERANCE 5.0f   // RPM difference to consider "at target"
//#define MAX_SETTLE_TIME_MS 10000 // Timeout for closed loop settling
//
//static void print_speeds(void) {
//    printMsg("SPEEDS: [ ");
//    for(int i=0; i<N_MOTORS; i++) {
//        float spd = Encoder_GetSpeedRPM(&encoders[i]);
//        printFloat(spd, 2);
//        printMsg(" ");
//    }
//    printMsg("] RPM\r\n");
//}
//
///*
//   FIX: Sabertooth ha un timeout (watchdog).
//   Se non inviamo comandi continuamente, i motori si fermano dopo ~100ms.
//   Quindi MotorControl_OpenLoopActuate va chiamato nel loop while.
//*/
//void test_open_loop(float ref) {
//    printMsg("=== START TEST OPEN LOOP (Ref=");
//    printFloat(ref, 2);
//    printMsg(") ===\r\n");
//    printMsg("VERIFICA: Se Ref > 0, la SPEED deve essere POSITIVA. Se e' negativa, invertire encoder o motore.\r\n");
//
//    uint32_t last_print = 0;
//
//
//    // Loop infinito per mantenere il watchdog attivo
//    while (1) {
//        for (int i = 0; i < N_MOTORS; i++) {
//            MotorControl_SetReferenceRPM(&motors[i], ref);
//            MotorControl_OpenLoopActuate(&motors[i]);
//
//            Encoder_Update(&encoders[i]);
//        }
//
//        // Stampa periodica
//        if (HAL_GetTick() - last_print > PRINT_INTERVAL_MS) {
//            print_speeds();
//            last_print = HAL_GetTick();
//        }
//
//        HAL_Delay(20); // Refresh rate ~50Hz
//    }
//}
//
//void test_closed_loop(float ref) {
//    printMsg("=== START TEST CLOSED LOOP (Ref=");
//    printFloat(ref, 2);
//    printMsg(") ===\r\n");
//    printMsg("VERIFICA: I motori devono mantenere la velocita' costante pari a Ref.\r\n");
//
//    uint32_t last_print = 0;
//    const uint32_t LOOP_PERIOD_MS = 20; // 50Hz nominali
//
//
//    // Loop infinito per mantenere il watchdog attivo
//    while (1) {
//        uint32_t loop_start = HAL_GetTick();
//
//        for (int i = 0; i < N_MOTORS; i++) {
//            // 1. Aggiorno encoder e leggo velocità attuale (Feedback)
//            Encoder_Update(&encoders[i]);
//            float current_rpm = Encoder_GetSpeedRPM(&encoders[i]);
//
//            // 2. Imposto riferimento e eseguo controllo Closed Loop
//            MotorControl_SetReferenceRPM(&motors[i], ref);
//            MotorControl_Update(&motors[i], current_rpm);
//
//            // Inseriamo un piccolo ritardo tra i comandi ai due motori sullo stesso bus UART.
//            // Inviare pacchetti back-to-back potrebbe sovraccaricare il Sabertooth o causare errori di framing.
//            HAL_Delay(1);
//        }
//
//        // Stampa periodica
//        if (HAL_GetTick() - last_print > PRINT_INTERVAL_MS) {
//            print_speeds();
//            last_print = HAL_GetTick();
//        }
//
//        // Attesa dinamica: dorme solo per il tempo rimanente ai 20ms
//        uint32_t elapsed = HAL_GetTick() - loop_start;
//        if (elapsed < LOOP_PERIOD_MS) {
//            HAL_Delay(LOOP_PERIOD_MS - elapsed);
//        }
//    }
//}
