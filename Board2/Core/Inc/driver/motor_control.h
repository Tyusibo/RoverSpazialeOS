#ifndef INC_DRIVER_MOTOR_CONTROL_H_
#define INC_DRIVER_MOTOR_CONTROL_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include "regulator.h"

typedef struct {
  // --- PWM HW ---
  TIM_HandleTypeDef *htim_pwm;
  uint64_t arr_pwm_plus_one;  // (ARR + 1) cache
  uint32_t pwm_channel;

  // --- Parametri (teniamo Ts per futuro) ---
  float Ts;

  // --- Limiti e mapping Volt -> Duty ---
  float min_volt;
  float max_volt;
  float in_min;
  float in_max;
  float out_min;
  float out_max;

  // --- Parametri Open Loop ---
  float dc_gain; // Guadagno statico k (RPM/V) specifico per il motore

  // --- Parametri Calibrazione Pulse (Hardware Specific) ---
  float pulse_theo_min;
  float pulse_theo_max;
  float pulse_real_min;
  float pulse_real_max;

  // --- Regolatore ---
  PIDController *current_regulator;

  // --- Stato controllore ---
  float reference_rpm;

  // --- Debug opzionale ---
  float last_u;       
  int last_pulse;
} MotorControl;

void MotorControl_Init(
  MotorControl *mc,
  TIM_HandleTypeDef *htim_pwm,
  uint32_t pwm_channel,
  float Ts,
  float min_volt, float max_volt,
  float in_min, float in_max, float out_min, float out_max,
  float dc_gain, 
  float pulse_theo_min, float pulse_theo_max, 
  float pulse_real_min, float pulse_real_max,
  PIDController *default_regulator
);

void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm);
void MotorControl_SetRegulator(MotorControl *mc, PIDController *reg);

/**
 * Calcola l'uscita di controllo (Volt) usando la misura speed_rpm.
 * - delega il calcolo al PID corrente
 * - ritorna u_saturata (Volt), pronta per l'attuatore
 */
float MotorControl_ComputeU(MotorControl *mc, float speed_rpm);

/**
 * Applica u (Volt) al PWM (mapping + CCR write).
 * Ritorna pulse scritto nel CCR.
 */
int MotorControl_Actuate(MotorControl *mc, float u_volt);

/**
 * (Opzionale) convenience: compute + actuate in un colpo.
 */
int MotorControl_Update(MotorControl *mc, float speed_rpm);


void MotorControl_OpenLoopActuate(MotorControl *mc);

#endif /* INC_DRIVER_MOTOR_CONTROL_H_ */
