#include "motors_init.h"
#include "stm32g4xx_hal.h"

// PWM timer (definito in tim.c / CubeMX)
extern TIM_HandleTypeDef htim1;

MotorControl motors[N_MOTORS];

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
    uint32_t pwm_ch[N_MOTORS] = {
        TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4
    };

    for (int i = 0; i < N_MOTORS; i++)
    {
        MotorControl_Init(&motors[i],
                          &htim1, pwm_ch[i],
                          TS, MIN_VOLT, MAX_VOLT,
                          IN_MIN, IN_MAX, OUT_MIN, OUT_MAX,
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
