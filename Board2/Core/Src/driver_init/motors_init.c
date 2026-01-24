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
 * @file motors_init.c
 * @brief Initialization source file for motors and PID controllers.
 *
 * This file contains the initialization sequences for all motors, their associated
 * PID controllers for different speeds (fast, medium, slow), and PWM start functions.
 */

#include "motors_init.h"
#include "stm32g4xx_hal.h"

extern TIM_HandleTypeDef htim1;

/**
 * @brief Array of motor control structures.
 */
MotorControl motors[N_MOTORS];

/**
 * @brief Array of PID controllers for fast speed.
 */
PIDController pid_fast[N_MOTORS];

/**
 * @brief Array of PID controllers for medium speed.
 */
PIDController pid_medium[N_MOTORS];

/**
 * @brief Array of PID controllers for slow speed.
 */
PIDController pid_slow[N_MOTORS];

/**
 * @brief Initializes all motors and their PID controllers.
 *
 * This function sets up the PID gains for fast, medium, and slow modes,
 * configures the PWM channels and DC gains for each motor, and initializes
 * the motor control structures.
 *
 * @retval None
 */
void Motors_InitAll(void)
{
	float fastGains[N_MOTORS][2] = {
	    [MOTOR_FRONT_LEFT]  = { FAST_MOT1_K_ERR, FAST_MOT1_K_LAST_ERR },
	    [MOTOR_FRONT_RIGHT] = { FAST_MOT2_K_ERR, FAST_MOT2_K_LAST_ERR },
	    [MOTOR_REAR_RIGHT]  = { FAST_MOT3_K_ERR, FAST_MOT3_K_LAST_ERR },
	    [MOTOR_REAR_LEFT]   = { FAST_MOT4_K_ERR, FAST_MOT4_K_LAST_ERR }
	};

    float mediumGains[N_MOTORS][2] = {
	    [MOTOR_FRONT_LEFT]  = { MEDIUM_MOT1_K_ERR, MEDIUM_MOT1_K_LAST_ERR },
	    [MOTOR_FRONT_RIGHT] = { MEDIUM_MOT2_K_ERR, MEDIUM_MOT2_K_LAST_ERR },
	    [MOTOR_REAR_RIGHT]  = { MEDIUM_MOT3_K_ERR, MEDIUM_MOT3_K_LAST_ERR },
	    [MOTOR_REAR_LEFT]   = { MEDIUM_MOT4_K_ERR, MEDIUM_MOT4_K_LAST_ERR }
	};

	float slowGains[N_MOTORS][2] = {
	    [MOTOR_FRONT_LEFT]  = { SLOW_MOT1_K_ERR, SLOW_MOT1_K_LAST_ERR },
	    [MOTOR_FRONT_RIGHT] = { SLOW_MOT2_K_ERR, SLOW_MOT2_K_LAST_ERR },
	    [MOTOR_REAR_RIGHT]  = { SLOW_MOT3_K_ERR, SLOW_MOT3_K_LAST_ERR },
	    [MOTOR_REAR_LEFT]   = { SLOW_MOT4_K_ERR, SLOW_MOT4_K_LAST_ERR }
	};

	static const uint32_t pwm_ch[N_MOTORS] = {
	    [MOTOR_FRONT_LEFT]  = TIM_CHANNEL_1,
	    [MOTOR_FRONT_RIGHT] = TIM_CHANNEL_2,
	    [MOTOR_REAR_RIGHT]  = TIM_CHANNEL_3,
	    [MOTOR_REAR_LEFT]   = TIM_CHANNEL_4
	};

	static const float dc_gains[N_MOTORS] = {
	    [MOTOR_FRONT_LEFT]  = DC_GAIN_MOT1,
	    [MOTOR_FRONT_RIGHT] = DC_GAIN_MOT2,
	    [MOTOR_REAR_RIGHT]  = DC_GAIN_MOT3,
	    [MOTOR_REAR_LEFT]   = DC_GAIN_MOT4
	};

    for (int i = 0; i < N_MOTORS; i++)
    {
        PID_Init(&pid_fast[i], fastGains[i][0], fastGains[i][1]);
        PID_Init(&pid_medium[i], mediumGains[i][0], mediumGains[i][1]);
        PID_Init(&pid_slow[i], slowGains[i][0], slowGains[i][1]);

        MotorControl_Init(&motors[i],
                          &htim1, pwm_ch[i],
                          TS, MIN_VOLT, MAX_VOLT,
                          IN_MIN, IN_MAX, OUT_MIN, OUT_MAX,
                          dc_gains[i],
						  PULSE_THEO_MIN, PULSE_THEO_MAX,
						  PULSE_REAL_MIN, PULSE_REAL_MAX,
                          &pid_fast[i]);
    }
}

/**
 * @brief Starts PWM signals for all motors.
 *
 * This function enables the PWM generation on the timer channels associated with each motor.
 *
 * @retval None
 */
void Motors_StartAllPwm(void)
{
    for (int i = 0; i < N_MOTORS; i++)
    {
        HAL_TIM_PWM_Start(motors[i].htim_pwm, motors[i].pwm_channel);
    }
}

/**
 * @brief Sets a default Capture Compare Register (CCR) value for all motors.
 *
 * This function updates the PWM duty cycle for all motors to the specified CCR value.
 *
 * @param ccr The new CCR value to set for all motors.
 * @retval None
 */
void Motors_SetDefaultCcr(uint32_t ccr)
{
    for (int i = 0; i < N_MOTORS; i++)
    {
        __HAL_TIM_SET_COMPARE(motors[i].htim_pwm, motors[i].pwm_channel, ccr);
    }
}
