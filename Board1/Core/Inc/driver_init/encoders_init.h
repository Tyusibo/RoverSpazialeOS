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
 * @file encoders_init.h
 * @brief Header file for encoder initialization and management.
 */

#ifndef INC_DRIVER_INIT_ENCODERS_INIT_H_
#define INC_DRIVER_INIT_ENCODERS_INIT_H_

#include <stdint.h>
#include "encoder.h"
#include "motor_constants.h"   // per TS, COUNTS (se li hai lì dentro)

/**
 * @brief Number of encoders.
 */
#define N_ENCODER (4)

/**
 * @brief Array of Encoder instances.
 */
extern Encoder encoders[N_ENCODER];

/**
 * @brief Initializes all encoders.
 */
void Encoders_InitAll(void);

/**
 * @brief Starts all encoders.
 */
void Encoders_StartAll(void);


#endif /* INC_DRIVER_INIT_ENCODERS_INIT_H_ */
