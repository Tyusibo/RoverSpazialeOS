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
 * @file motor_control.c
 * @brief Implementation of motor control functionalities including PID regulation and PWM actuation.
 *
 * This file contains the implementation of functions for initializing motor control,
 * setting references, computing control signals (U), and actuating the motor via PWM.
 * It also includes helper functions for linear mapping and voltage-to-duty cycle conversion.
 */

#include "motor_control.h"
#include <math.h>  // roundf


/**
 * @brief Generic linear mapping function (Arduino style 'map').
 * 
 * Maps the value x from range [in_min, in_max] into range [out_min, out_max].
 * Formula: out = out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min)
 * 
 * @param x Input value to map.
 * @param in_min Lower bound of the input range.
 * @param in_max Upper bound of the input range.
 * @param out_min Lower bound of the output range.
 * @param out_max Upper bound of the output range.
 * @return float Mapped value.
 */
static inline float map_linear(float x, float in_min, float in_max, float out_min, float out_max)
{
  return out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min);
}

/**
 * @brief Converts voltage to duty cycle percentage.
 * 
 * @param m Pointer to the MotorControl structure.
 * @param volt Voltage value to convert.
 * @return float Duty cycle percentage.
 */
static inline float volt_to_duty_percent(const MotorControl *m, float volt)
{
  // 1. Normalize volt to a "raw" percentage value (0-100 based on 12V).
  // Note: This step depends on specific logic where 12V = 100%.
  float duty_raw = (volt * 100.0f) / 12.0f;

  // 2. Map the input range (e.g. -100 to +100) to the configured output range (e.g. 56.8 to 94.6).
  return map_linear(duty_raw, m->in_min, m->in_max, m->out_min, m->out_max);
}


/**
 * @brief Initializes the MotorControl structure with the given parameters.
 * 
 * @param mc Pointer to the MotorControl structure to initialize.
 * @param htim_pwm Handle to the TIM object used for PWM.
 * @param pwm_channel PWM channel to be used.
 * @param Ts Sampling time.
 * @param min_volt Minimum voltage for the controller saturation.
 * @param max_volt Maximum voltage for the controller saturation.
 * @param in_min Input range minimum for mapping.
 * @param in_max Input range maximum for mapping.
 * @param out_min Output range minimum for mapping.
 * @param out_max Output range maximum for mapping.
 * @param dc_gain DC gain for open loop control.
 * @param default_regulator Pointer to the default PID regulator.
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
)
{
  mc->htim_pwm = htim_pwm;
  mc->pwm_channel = pwm_channel;
  mc->Ts = Ts;

  mc->min_volt = min_volt;
  mc->max_volt = max_volt;
  mc->in_min = in_min;
  mc->in_max = in_max;
  mc->out_min = out_min;
  mc->out_max = out_max;
  
  mc->dc_gain = dc_gain; 

  mc->current_regulator = default_regulator;

  mc->reference_rpm = 0.0f;

  mc->arr_pwm_plus_one = (uint64_t)__HAL_TIM_GET_AUTORELOAD(htim_pwm) + 1ULL;

  mc->last_u = 0.0f;
  mc->last_pulse = 0;
}

/**
 * @brief Sets the reference speed in RPM.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param ref_rpm Reference speed in RPM.
 */
void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm)
{
  mc->reference_rpm = ref_rpm;
}

/**
 * @brief Sets the PID regulator to be used.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param reg Pointer to the PIDController structure.
 */
void MotorControl_SetRegulator(MotorControl *mc, PIDController *reg)
{
    if (reg != NULL)
    {
        mc->current_regulator = reg;
    }
}

/**
 * @brief Computes the control signal (U) based on the current speed and reference.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param speed_rpm Current speed in RPM.
 * @return float Control signal U (saturated).
 */
float MotorControl_ComputeU(MotorControl *mc, float speed_rpm)
{
  float e = mc->reference_rpm - speed_rpm;

  float u_sat = PID_Compute(mc->current_regulator, e, mc->min_volt, mc->max_volt);

  mc->last_u = u_sat;

  return u_sat;
}

/**
 * @brief Actuates the motor with the given control voltage.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param u_volt Control voltage to apply.
 * @return int The pulse value applied to the PWM.
 */
int MotorControl_Actuate(MotorControl *mc, float u_volt){
  float duty = volt_to_duty_percent(mc, u_volt);
  int pulse = duty_percent_to_pulse(mc, duty);

  // Hardware-Specific recalibration passing the mc object
  //pulse = recalibrate_pulse(mc, pulse);

  // (optional but useful) clamp CCR in [0, ARR]
  if (pulse < 0) pulse = 0;
  if ((uint64_t)pulse > (mc->arr_pwm_plus_one - 1ULL)) pulse = (int)(mc->arr_pwm_plus_one - 1ULL);

  __HAL_TIM_SET_COMPARE(mc->htim_pwm, mc->pwm_channel, (uint32_t)pulse);

  mc->last_pulse = pulse;
  return pulse;
}

/**
 * @brief Updates the motor control loop.
 * 
 * Computes the control signal and actuates the motor.
 * 
 * @param mc Pointer to the MotorControl structure.
 * @param speed_rpm Current speed in RPM.
 * @return int The pulse value applied to the PWM.
 */
int MotorControl_Update(MotorControl *mc, float speed_rpm)
{
  float u = MotorControl_ComputeU(mc, speed_rpm);
  return MotorControl_Actuate(mc, u);
}

/**
 * @brief Actuates the motor in open loop using the DC gain.
 * 
 * @param mc Pointer to the MotorControl structure.
 */
void MotorControl_OpenLoopActuate(MotorControl *mc){
    // Use the specific motor gain saved in the struct
    // u_volt = ref_rpm / k_gain
    if (mc->dc_gain > 0.001f) { // Avoid division by zero
        float u_volt = mc->reference_rpm / mc->dc_gain;
        MotorControl_Actuate(mc, u_volt);
    } else {
        MotorControl_Actuate(mc, 0.0f);
    }
}


