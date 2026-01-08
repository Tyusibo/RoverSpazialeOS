#ifndef INC_DRIVER_MOTOR_OPEN_LOOP_H_
#define INC_DRIVER_MOTOR_OPEN_LOOP_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>

typedef struct {
  // --- PWM HW ---
  TIM_HandleTypeDef *htim_pwm;
  uint64_t arr_pwm_plus_one;   // (ARR + 1) cache
  uint32_t pwm_channel;

  // --- Riferimento (open-loop) ---
  // ref_cmd tipicamente in [-1, +1]:
  //  -1 = full reverse, 0 = stop, +1 = full forward
  float reference_cmd;

  // --- Mapping verso analog Sabertooth (PWM + RC -> 0..Vref) ---
  // Vref_adc: tensione "alta" equivalente dopo RC (tipicamente 5.0V)
  float vref_analog;

  // Neutral (stop) in Volt: in analog mode è 2.5V
  float v_neutral;

  // Limiti di uscita in Volt (tipicamente 0..5)
  float v_min;
  float v_max;

  // Duty clamp (in percento) per eventuali limiti HW/RC
  float duty_min;
  float duty_max;

  // --- Debug opzionale ---
  float last_volt;
  float last_duty;
  int   last_pulse;

} MotorOpenLoop;

void MotorOpenLoop_Init(
  MotorOpenLoop *mol,
  TIM_HandleTypeDef *htim_pwm,
  uint32_t pwm_channel,
  float vref_analog,     // tipicamente 5.0
  float v_min, float v_max,   // tipicamente 0..5
  float v_neutral,       // tipicamente 2.5
  float duty_min, float duty_max // tipicamente 0..100
);

// ref_cmd in [-1..+1] (consigliato)
void MotorOpenLoop_SetReferenceCmd(MotorOpenLoop *mol, float ref_cmd);

/**
 * Converte il riferimento in una tensione target (Volt) per l'ingresso analogico:
 * -1 -> v_min
 *  0 -> v_neutral
 * +1 -> v_max
 */
float MotorOpenLoop_ComputeVolt(MotorOpenLoop *mol);

/**
 * Applica la tensione (Volt) via PWM:
 * volt -> duty -> CCR
 * Ritorna pulse scritto nel CCR.
 */
int MotorOpenLoop_ActuateVolt(MotorOpenLoop *mol, float volt);

/**
 * Convenience: compute + actuate (usa reference_cmd interno).
 */
int MotorOpenLoop_Update(MotorOpenLoop *mol);


#endif /* INC_DRIVER_MOTOR_OPEN_LOOP_H_ */
