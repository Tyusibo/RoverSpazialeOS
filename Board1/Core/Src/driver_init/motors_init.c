#include "motors_init.h"
#include "stm32g4xx_hal.h"

extern TIM_HandleTypeDef htim1;

MotorControl motors[N_MOTORS];


void Motors_InitAll(void)
{
	Coefficients fastGains[N_MOTORS] = {
	    [MOTOR_FRONT_LEFT]  = { FAST_MOT1_K_ERR, FAST_MOT1_K_LAST_ERR },
	    [MOTOR_FRONT_RIGHT] = { FAST_MOT2_K_ERR, FAST_MOT2_K_LAST_ERR },
	    [MOTOR_REAR_RIGHT]  = { FAST_MOT3_K_ERR, FAST_MOT3_K_LAST_ERR },
	    [MOTOR_REAR_LEFT]   = { FAST_MOT4_K_ERR, FAST_MOT4_K_LAST_ERR }
	};

	Coefficients slowGains[N_MOTORS] = {
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
        MotorControl_Init(&motors[i],
                          &htim1, pwm_ch[i],
                          TS, MIN_VOLT, MAX_VOLT,
                          IN_MIN, IN_MAX, OUT_MIN, OUT_MAX,
                          dc_gains[i],
                          fastGains[i], slowGains[i]);
    }
}

void Motors_StartAllPwm(void)
{
    for (int i = 0; i < N_MOTORS; i++)
    {
        HAL_TIM_PWM_Start(motors[i].htim_pwm, motors[i].pwm_channel);
    }
}

void Motors_SetDefaultCcr(uint32_t ccr)
{
    for (int i = 0; i < N_MOTORS; i++)
    {
        __HAL_TIM_SET_COMPARE(motors[i].htim_pwm, motors[i].pwm_channel, ccr);
    }
}
