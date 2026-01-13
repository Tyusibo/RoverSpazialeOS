#include "gyroscope_test.h"
#include "print.h"
#include "i2c.h"
#include "DWT.h"

// Variabile globale definita altrove (es. main.c)
extern MPU6050_Yaw_t MPU6050_Yaw; 

void GyroscopeTest(void) {
    
    printMsg("\r\n--- START Gyroscope Test (Async MPU) ---\r\n");

    while (1) {
        
        uint32_t t0 = DWT_Begin();

        // ----------------------------------------
        // FASE 1: Avvio Lettura (Asincrona)
        // ----------------------------------------
        // Lancia la lettura I2C (Interrupt) verso il sensore.
        // Il flag di stato interno al driver viene impostato su Busy.
        uint8_t status = MPU6050_Read_Yaw_IT(&hi2c3, &MPU6050_Yaw);
        if (status == 0) {
			printMsg("ERROR: MPU6050_Read_Yaw_IT failed to start\r\n");
			HAL_Delay(1000); // Attesa prima del retry
			continue;         // Riprova
		}

        // ----------------------------------------
        // FASE 2: Attesa Completamento
        // ----------------------------------------
        // Polling del flag gestito internamente al driver (esattamente come PadReceiver).
        // Attende che l'interrupt di ricezione o errore setti il flag su Done.
        while (!MPU6050_IsDone()) {
             // Attesa attiva
        }

        uint32_t cycles = DWT_End(t0);

        // ----------------------------------------
        // FASE 3: Stampa Telemetria
        // ----------------------------------------
        DWT_PrintCyclesAndUs("GYRO", cycles);

        char buf[64];
        sprintf(buf, "YAW (Raw/Angle): %.2f / %u\r\n",
                MPU6050_Yaw.Gz, 
                MPU6050_Yaw.KalmanAngleZ);
        printMsg(buf);

        // Delay (10Hz)
        HAL_Delay(1000);
    }
}
