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
 * @file encoders_init.c
 * @brief Implementation of encoder initialization and startup functions.
 */

#include "encoders_init.h"
#include "stm32g4xx_hal.h"   // o il tuo include HAL principale

/** @brief External handle for TIM2. */
extern TIM_HandleTypeDef htim2;
/** @brief External handle for TIM3. */
extern TIM_HandleTypeDef htim3;
/** @brief External handle for TIM4. */
extern TIM_HandleTypeDef htim4;
/** @brief External handle for TIM8. */
extern TIM_HandleTypeDef htim8;

/** @brief Global array of encoder instances. */
Encoder encoders[N_ENCODER];

/**
 * @brief Initializes all encoder instances.
 *
 * Assigns the corresponding timer handles to each encoder instance
 * and initializes them with the defined sampling time and counts per revolution.
 */
void Encoders_InitAll(void)
{
    TIM_HandleTypeDef *enc_tims[N_ENCODER] = { &htim2, &htim3, &htim4, &htim8 };

    for (int i = 0; i < N_ENCODER; i++)
    {
        Encoder_Init(&encoders[i], enc_tims[i], TS, COUNTS);
    }
}

/**
 * @brief Starts all encoder timers.
 *
 * Starts the encoder interface mode for all configured timers.
 */
void Encoders_StartAll(void)
{
    for (int i = 0; i < N_ENCODER; i++)
    {
        HAL_TIM_Encoder_Start(encoders[i].htim_enc, TIM_CHANNEL_ALL);
    }
}
