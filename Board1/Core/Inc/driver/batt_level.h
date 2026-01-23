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
 * @file batt_level.h
 * @brief Battery level driver header.
 *
 * This file contains the definitions and function prototypes for the
 * battery level monitoring driver.
 *
 * @date Jan 11, 2026
 * @author Armando Book
 */

#ifndef INC_BATT_LEVEL_H_
#define INC_BATT_LEVEL_H_

#include "stm32g4xx_hal.h"
#include "adc_constants.h"

/**
 * @brief Maximum expected battery voltage.
 */
#define MAX_VOLTAGE (12.6f)

/**
 * @brief Minimum safe battery voltage.
 */
#define MIN_VOLTAGE (10.5f)

/**
 * @brief Maximum battery percentage value (100%).
 */
#define MAX_BATT_PERC (100)

/**
 * @brief Minimum battery percentage value (0%).
 */
#define MIN_BATT_PERC (0)

/**
 * @brief Battery level monitor structure.
 *
 * This structure holds the configuration and state for a battery level monitor instance.
 */
typedef struct{
	ADC_HandleTypeDef* hadc;  /**< Pointer to the ADC handle */
	float r1;                 /**< Value of the first resistor in the divider (Ohms) */
	float r2;                 /**< Value of the second resistor in the divider (Ohms) */
	float adc_vref;           /**< ADC reference voltage (e.g. 3.3V or 3.0V) */
	uint32_t adc_resolution;  /**< ADC resolution (e.g. 4095 for 12-bit, 1023 for 10-bit) */
}batt_level_t;

/**
 * @brief Initializes the battery level monitor structure.
 *
 * @param bat Pointer to the battery level structure to initialize.
 * @param hadc Pointer to the ADC handle used for reading the voltage.
 * @param r1 Value of the first resistor in the voltage divider.
 * @param r2 Value of the second resistor in the voltage divider.
 * @param adc_vref Reference voltage of the ADC.
 */
void battery_init(batt_level_t* bat, ADC_HandleTypeDef* hadc, float r1, float r2, float adc_vref);

/**
 * @brief Reads the current battery voltage.
 *
 * @param bat Pointer to the battery level structure.
 * @return float The calculated battery voltage in Volts.
 */
float battery_read_voltage(batt_level_t* bat);

/**
 * @brief Reads the average battery voltage over multiple samples.
 *
 * @param bat Pointer to the battery level structure.
 * @param samples Number of samples to read for interpolation.
 * @return float The average battery voltage in Volts.
 */
float battery_read_voltage_avg(batt_level_t* bat, uint8_t samples);

/**
 * @brief Calculates the battery percentage linearly based on voltage limits.
 *
 * @param voltage Measured battery voltage.
 * @param min_v Minimum battery voltage corresponding to 0%.
 * @param max_v Maximum battery voltage corresponding to 100%.
 * @return int8_t Battery percentage from 0 to 100, or -1 if voltage is invalid.
 */
int8_t battery_get_percentage_linear(float voltage, float min_v, float max_v);

#endif /* INC_BATT_LEVEL_H_ */
