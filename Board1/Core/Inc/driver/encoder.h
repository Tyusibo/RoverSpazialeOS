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
 * @file encoder.h
 * @brief Header file for the Encoder driver.
 */

#ifndef INC_DRIVER_ENCODER_H_
#define INC_DRIVER_ENCODER_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>

/**
 * @brief Encoder structure definition.
 */
typedef struct {
  TIM_HandleTypeDef *htim_enc;    /**< TIM handle in Encoder mode */
  uint64_t arr_enc_plus_one;      /**< (ARR + 1) cached value for overflow/underflow calculation */

  float Ts;                       /**< Control period in seconds [s] */
  uint32_t counts;                /**< Total counts per revolution (PPR * GEAR_RATIO * 4) */

  uint32_t past_cnt;              /**< Last read Counter (CNT) value */

  uint32_t last_tick;             /**< Last tick value used for dt calculation */

  float speed_rpm;                /**< Last calculated speed in RPM */
} Encoder;

/**
 * @brief Initialize the Encoder structure.
 * 
 * @param e Pointer to the Encoder structure.
 * @param htim_enc Pointer to the TIM handle configured in Encoder mode.
 * @param Ts Control period in seconds.
 * @param counts Total counts per revolution.
 */
void Encoder_Init(Encoder *e, TIM_HandleTypeDef *htim_enc, float Ts, uint32_t counts);

/**
 * @brief Update the encoder state, read the counter, and calculate speed.
 * 
 * Reads the current counter value, calculates the delta considering overflow/underflow,
 * and updates the speed_rpm and past_cnt fields.
 * 
 * @param e Pointer to the Encoder structure.
 * @return int64_t Delta ticks in the period Ts.
 */
int64_t Encoder_Update(Encoder *e);

/**
 * @brief Get the last calculated speed in RPM.
 * 
 * @param e Pointer to the Encoder structure.
 * @return float Speed in RPM.
 */
static inline float Encoder_GetSpeedRPM(const Encoder *e) { return e->speed_rpm; }


#endif /* INC_DRIVER_ENCODER_H_ */
