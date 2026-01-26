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
 * @file temperature_adc.h
 * @brief Header file for Temperature ADC driver using NTC thermistor (KY-028).
 *
 * This file contains the definitions and function prototypes for reading
 * temperature from an NTC thermistor connected to an ADC (e.g., KY-028 module).
 */

#ifndef INC_TEMPERATURE_ADC_H_
#define INC_TEMPERATURE_ADC_H_

#include "stm32g4xx_hal.h"

// NTC Thermistor parameters (10k ohm @ 25°C, Beta coefficient = 3950)
#define NTC_RESISTANCE_25C     (10000.0f)  /**< Resistance at 25°C */
#define NTC_B_COEFFICIENT      (3950.0f)   /**< Beta coefficient */
#define NTC_TEMPERATURE_25C    (298.15f)   /**< 25°C in Kelvin (25 + 273.15) */
#define SERIES_RESISTOR        (4700.0f)   /**< Series resistor in voltage divider (KY-028 often uses 4.7k or 10k) */

// Temperature Range
#define MIN_TEMPERATURE        (-55.0f)    /**< Minimum supported temperature */
#define MAX_TEMPERATURE        (125.0f)    /**< Maximum supported temperature */

#define TEMP_DRIVER_OK          0
#define TEMP_DRIVER_FAIL       -1

/**
 * @brief Structure to hold temperature sensor configuration and state.
 */
typedef struct {
    ADC_HandleTypeDef* hadc;    /**< Pointer to ADC handle */
    float adc_vref;             /**< ADC reference voltage (3.3V or 3.0V) */
    uint32_t adc_resolution;    /**< 4095 for 12-bit, 1023 for 10-bit */

    // Configurable parameters for different thermistors
    float ntc_r25;              /**< Resistance at 25°C */
    float ntc_b;                /**< Beta coefficient */
    float series_r;             /**< Series resistance */
} temp_ky028_t;

// Initialization functions

/**
 * @brief Initialize the temperature sensor structure.
 *
 * @param temp Pointer to the sensor structure.
 * @param hadc Pointer to the ADC handle.
 * @param adc_vref ADC reference voltage.
 */
void temp_ky028_init(temp_ky028_t* temp, ADC_HandleTypeDef* hadc, float adc_vref);

// Reading functions

/**
 * @brief Read the resistance of the NTC thermistor.
 *
 * @param temp Pointer to the sensor structure.
 * @return float Measured resistance in Ohms.
 */
float temp_ky028_read_resistance(temp_ky028_t* temp);

/**
 * @brief Read the temperature from the sensor.
 *
 * @param temp Pointer to the sensor structure.
 * @param temperature Pointer to float where the temperature will be stored.
 * @return int8_t 0 on success, error code otherwise.
 */
int8_t temp_ky028_read_temperature(temp_ky028_t* temp, float* temperature);

/**
 * @brief Read the temperature from the sensor with averaging.
 *
 * @param temp Pointer to the sensor structure.
 * @param samples Number of samples to average.
 * @param temperature Pointer to float where the temperature will be stored.
 * @return int8_t 0 on success, error code otherwise.
 */
int8_t temp_ky028_read_temperature_avg(temp_ky028_t* temp, uint8_t samples, float* temperature);

// Utility functions

/**
 * @brief Convert resistance to temperature using the Beta parameter equation.
 *
 * @param resistance Measured resistance.
 * @param r25 Resistance at 25°C.
 * @param b_coeff Beta coefficient.
 * @return float Temperature in Celsius.
 */
float temp_ky028_resistance_to_temp(float resistance, float r25, float b_coeff);



#endif /* INC_TEMPERATURE_ADC_H_ */
