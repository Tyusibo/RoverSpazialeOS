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
 * @file battery_init.h
 * @brief Header file for battery sensor initialization.
 *
 * This file contains the external declarations for the battery sensor instance
 * and the initialization function prototype.
 */

#ifndef INC_DRIVER_INIT_BATTERY_INIT_H_
#define INC_DRIVER_INIT_BATTERY_INIT_H_

#include "batt_level.h"

/**
 * @brief Global battery level instance.
 *
 * This variable holds the state and configuration of the battery level sensor.
 */
extern batt_level_t battery;

/**
 * @brief Initializes the battery sensor subsystem.
 *
 * This function performs the necessary setup for the battery sensor,
 * including ADC calibration and parameter initialization.
 */
void battery_sensor_init(void);

#endif /* INC_DRIVER_INIT_BATTERY_INIT_H_ */
