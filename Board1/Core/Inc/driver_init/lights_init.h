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
 * @file lights_init.h
 * @brief Header file for LED lighting initialization and control.
 */

#ifndef INC_DRIVER_INIT_LIGHTS_INIT_H_
#define INC_DRIVER_INIT_LIGHTS_INIT_H_

#include "a4wd3_led.h"

/** @brief Left LED instance. */
extern A4WD3_Led_t led_left;

/** @brief Right LED instance. */
extern A4WD3_Led_t led_right;

/**
 * @brief Initialize the LED mapping (to be called once).
 */
void led_init(void);

#endif /* INC_DRIVER_INIT_LIGHTS_INIT_H_ */
