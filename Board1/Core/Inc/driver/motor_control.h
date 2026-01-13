#ifndef INC_DRIVER_MOTOR_CONTROL_H_
#define INC_DRIVER_MOTOR_CONTROL_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>

typedef struct {
  float k_err;
  float k_last_err;
} Coefficients;

typedef struct {
  // --- Sabertooth UART HW ---
  UART_HandleTypeDef *huart;
  uint8_t address; 
  uint8_t motor_id; // 1 o 2

  // --- Parametri ---
  float Ts;

  // --- Limiti ---
  float min_volt;
  float max_volt;

  // --- Parametri Open Loop ---
  float dc_gain; // Guadagno statico k (RPM/V)

  // --- Regolatori ---
  Coefficients pi_fast;
  Coefficients pi_slow;
  uint8_t use_slow;

  // --- Stato regolatore ---
  float reference_rpm;
  float last_error;
  float z;           

  // --- Debug ---
  float last_u;       
  int last_cmd;       // Ultimo comando inviato (-127 a 127)
} MotorControl;

void MotorControl_Init(
  MotorControl *mc,
  UART_HandleTypeDef *huart,
  uint8_t address,
  uint8_t motor_id,
  float Ts,
  float min_volt, float max_volt,
  float dc_gain, 
  Coefficients pi_fast, Coefficients pi_slow
);

void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm);
void MotorControl_SelectSlow(MotorControl *mc, uint8_t enable);
float MotorControl_ComputeU(MotorControl *mc, float speed_rpm);
int MotorControl_Actuate(MotorControl *mc, float u_volt);
int MotorControl_Update(MotorControl *mc, float speed_rpm);
void MotorControl_OpenLoopActuate(MotorControl *mc);

#endif /* INC_DRIVER_MOTOR_CONTROL_H_ */
