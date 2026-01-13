#include "encoders_init.h"
#include "stm32g4xx_hal.h"   // o il tuo include HAL principale

// Encoder timers (definiti in tim.c / CubeMX)
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;

Encoder encoders[N_ENCODER];

void Encoders_InitAll(void)
{
    TIM_HandleTypeDef *enc_tims[N_ENCODER] = { &htim2, &htim3, &htim4, &htim8 };

    for (int i = 0; i < N_ENCODER; i++)
    {
        Encoder_Init(&encoders[i], enc_tims[i], TS, COUNTS);
    }
}

void Encoders_StartAll(void)
{
    for (int i = 0; i < N_ENCODER; i++)
    {
        HAL_TIM_Encoder_Start(encoders[i].htim_enc, TIM_CHANNEL_ALL);
    }
}
