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
 * @file battery_init.c
 * @brief Initialization of the battery level sensor.
 *
 * This file contains the implementation for initializing the battery sensor,
 * setting up the ADC and configuring the battery parameters.
 */

#include "battery_init.h"

extern ADC_HandleTypeDef hadc2;

/**
 * @brief Global battery level instance.
 */
batt_level_t battery;


/**
 * @brief Initializes the battery sensor subsystem.
 *
 * This function performs the calibration of the ADC and initializes the
 * battery structure with the voltage divider resistors and reference voltage.
 *
 * @retval None
 */
void battery_sensor_init(void){
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	battery_init(&battery, &hadc2, R1, R2, ADC_VREF);
	return;
}
