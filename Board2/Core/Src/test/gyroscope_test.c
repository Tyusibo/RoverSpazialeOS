#include "gyroscope_test.h"
#include "print.h"
#include "gyroscope_init.h"
#include "i2c.h"

// Dichiarazioni EXTERN per variabili definite in main.c
extern volatile uint8_t gyro_done;
extern MPU6050_Yaw_t MPU6050_Yaw; // Assicurati che nel main.c non sia static

// Buffer per ricevere byte di sblocco (se necessario riattivare interrupt)
uint8_t dummyReceiveGyro;

// Flag gestito in stm32g4xx_it.c (Printer UART Callback)
extern volatile uint8_t flow_control_flag;

void GyroscopeTest(void) {

    // Abilita interrupt ricezione UART per uscire dal loop alla pressione di un tasto
    // L'interrupt handler in stm32g4xx_it.c imposterà flow_control_flag = 1
    HAL_UART_Receive_IT(&huart2, &dummyReceiveGyro, 1);
    
    printMsg("\r\n--- START Gyroscope Test ---\r\n");
    printMsg("Press any key to exit.\r\n");

    while (1) {
        // Controllo uscita
        if (flow_control_flag) {
            flow_control_flag = 0;
            printMsg("\r\n--- END Gyroscope Test ---\r\n");
            break;
        }

        // --- Lettura Yaw via Interrupt ---
        
        // 1. Reset flag
        gyro_done = 0;
        
        // 2. Avvia lettura asincrona
        MPU6050_Read_Yaw_IT(&hi2c3, &MPU6050_Yaw);

        // 3. Attesa attiva con Timeout (50ms)
        // Nota: Assicurarsi che HAL_I2C_MemRxCpltCallback in main.c o altrove
        // imposti gyro_done = 1 quando riceve interrupt da I2C3.
        uint32_t startTick = HAL_GetTick();
        while (!gyro_done && (HAL_GetTick() - startTick < 50));

        // Stampa valori (solo Yaw Z)
        char buf[64];
        sprintf(buf, "YAW (Raw/Angle): %.2f / %u\r\n",
                MPU6050_Yaw.Gz, 
                MPU6050_Yaw.KalmanAngleZ);
        printMsg(buf);

        // Delay leggibile (10 Hz)
        HAL_Delay(100);
    }
}
