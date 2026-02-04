/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file motor_control.h
 * @brief Header file for Motor Control driver.
 */

#ifndef INC_DRIVER_MOTOR_CONTROL_H_
#define INC_DRIVER_MOTOR_CONTROL_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include "regulator.h"

/**
 * @brief Motor Control structure containing hardware handles, configuration parameters, and state.
 */
typedef struct {
  /* --- PWM HW --- */
  TIM_HandleTypeDef *htim_pwm; /**< Pointer to PWM Timer handle */
  uint64_t arr_pwm_plus_one;   /**< Cache for (ARR + 1) to optimize calculations */
  uint32_t pwm_channel;        /**< PWM Channel */

  /* --- Parametri (teniamo Ts per futuro) --- */
  float Ts; /**< Sampling time (reserved for future use) */

  /* --- Limiti e mapping Volt -> Duty --- */
  float min_volt; /**< Minimum voltage output */
  float max_volt; /**< Maximum voltage output */
  float in_min;   /**< Mapping input minimum (Volt equivalent) */
  float in_max;   /**< Mapping input maximum (Volt equivalent) */
  float out_min;  /**< Mapping output minimum (Duty % equivalent) */
  float out_max;  /**< Mapping output maximum (Duty % equivalent) */

  /* --- Parametri Open Loop --- */
  float dc_gain; /**< Static DC gain k (RPM/V) specific to the motor */

  /* --- Regolatore --- */
  PIDController *current_regulator; /**< Pointer to the currently active PID regulator */

  /* --- Stato controllore --- */
  float reference_rpm; /**< Target speed reference in RPM */

  /* --- Debug opzionale --- */
  float last_u;   /**< Last computed control signal (Voltage) */
  int last_pulse; /**< Last actuated PWM pulse width */
} MotorControl;

/**
 * @brief Initialize the Motor Control structure.
 * @param mc Pointer to the MotorControl structure.
 * @param htim_pwm PWM timer handle.
 * @param pwm_channel PWM channel.
 * @param Ts Sampling time.
 * @param min_volt Minimum voltage.
 * @param max_volt Maximum voltage.
 * @param in_min Input range minimum (for map).
 * @param in_max Input range maximum (for map).
 * @param out_min Output range minimum (for map).
 * @param out_max Output range maximum (for map).
 * @param dc_gain Motor DC gain.
 * @param default_regulator Default PID regulator.
 */
void MotorControl_Init(
  MotorControl *mc,
  TIM_HandleTypeDef *htim_pwm,
  uint32_t pwm_channel,
  float Ts,
  float min_volt, float max_volt,
  float in_min, float in_max, float out_min, float out_max,
  float dc_gain, 
  PIDController *default_regulator
);

/**
 * @brief Sets the reference RPM for the motor.
 * @param mc Pointer to the MotorControl structure.
 * @param ref_rpm Reference Speed in RPM.
 */
void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm);

/**
 * @brief Switches the active PID regulator.
 * @param mc Pointer to the MotorControl structure.
 * @param reg Pointer to the new PIDController.
 */
void MotorControl_SetRegulator(MotorControl *mc, PIDController *reg);

/**
 * @brief Computes the control output (Voltage) using the measured speed.
 * 
 * Delegates the calculation to the current PID regulator.
 * Returns the saturated control signal (Voltage) ready for the actuator.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param speed_rpm Measured speed in RPM.
 * @return Saturated control signal (Voltage).
 */
float MotorControl_ComputeU(MotorControl *mc, float speed_rpm);

/**
 * @brief Applies the control signal (Voltage) to the PWM.
 * 
 * Performs mapping and CCR write.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param u_volt Control signal in Volts.
 * @return Pulse width value written to CCR.
 */
int MotorControl_Actuate(MotorControl *mc, float u_volt);

/**
 * @brief Convenience function: Computes control signal and actuates the motor.
 * @param mc Pointer to the MotorControl structure.
 * @param speed_rpm Measured speed in RPM.
 * @return Pulse width value written to CCR.
 */
int MotorControl_Update(MotorControl *mc, float speed_rpm);

/**
 * @brief Provides open-loop actuation based on reference RPM and DC gain.
 * @param mc Pointer to the MotorControl structure.
 */
void MotorControl_OpenLoopActuate(MotorControl *mc);

#endif /* INC_DRIVER_MOTOR_CONTROL_H_ */
