#include "motors_init.h"
#include "stm32g4xx_hal.h"
#include "usart.h" 

// Handle UART definiti in usart.c
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

RoverMotors motors;

static Coefficients fastGains[N_MOTORS] = {
    { .k_err = 0.097f, .k_last_err = -0.083f },
    { .k_err = 0.097f, .k_last_err = -0.083f },
    { .k_err = 0.097f, .k_last_err = -0.083f },
    { .k_err = 0.097f, .k_last_err = -0.083f },
};

static Coefficients slowGains[N_MOTORS] = {
    { .k_err = 0.0002033f, .k_last_err = 0.0002033f },
    { .k_err = 0.0002033f, .k_last_err = 0.0002033f },
    { .k_err = 0.0002033f, .k_last_err = 0.0002033f },
    { .k_err = 0.0002033f, .k_last_err = 0.0002033f },
};

void Motors_InitAll(void)
{
    // Configurazione UART per i 4 motori:
    // Motori 1 e 2 -> USART1 (Sabertooth A)
    // Motori 3 e 4 -> USART3 (Sabertooth B)
    UART_HandleTypeDef* uarts[N_MOTORS] = { &huart3, &huart3, &huart1, &huart1 };
    
    // ID Motore sulla Sabertooth (1 o 2)
    uint8_t sabertooth_ids[N_MOTORS] = { 2, 1, 2, 1 };

    // Vettore dei guadagni statici definito in motor_constants.h
    float dc_gains[N_MOTORS] = {
        DC_GAIN_MOT1, 
        DC_GAIN_MOT2, 
        DC_GAIN_MOT3, 
        DC_GAIN_MOT4
    };

    MotorControl* m_ptrs[N_MOTORS] = {
        &motors.front_left,
        &motors.front_right,
        &motors.rear_right,
        &motors.rear_left
    };

    for (int i = 0; i < N_MOTORS; i++)
    {
        MotorControl_Init(m_ptrs[i],
                          uarts[i],          // UART Handle
                          128,               // Indirizzo Sabertooth (default 128)
                          sabertooth_ids[i], // Motore 1 o 2
                          TS, MIN_VOLT, MAX_VOLT,
                          dc_gains[i],
                          fastGains[i], slowGains[i]);

        // Stop iniziale per sicurezza
        MotorControl_Actuate(m_ptrs[i], 0.0f);
    }
}
