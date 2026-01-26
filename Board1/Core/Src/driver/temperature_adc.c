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
 * @file temperature_adc.c
 * @brief Implementation of the Temperature ADC driver using NTC thermistor.
 *
 * This file contains the implementation of functions to initialize and read
 * temperature from an NTC thermistor (e.g. KY-028) connected via ADC.
 */

#include "driver/temperature_adc.h"
#include <math.h>

/**
 * @brief Initialize the temperature sensor structure.
 *
 * Sets up the ADC handle, reference voltage, and loads default parameters for the KY-028 sensor.
 *
 * @param temp Pointer to the sensor structure.
 * @param hadc Pointer to the ADC handle.
 * @param adc_vref ADC reference voltage.
 */
void temp_ky028_init(temp_ky028_t* temp, ADC_HandleTypeDef* hadc, float adc_vref) {
    temp->hadc = hadc;
    temp->adc_vref = adc_vref;
    temp->adc_resolution = 4095;  // For 12-bit ADC

    // Default parameters for KY-028 (NTC 10k @ 25°C, B=3950)
    temp->ntc_r25 = NTC_RESISTANCE_25C;
    temp->ntc_b = NTC_B_COEFFICIENT;
    temp->series_r = SERIES_RESISTOR;
}

/**
 * @brief internal helper to read raw ADC value.
 *
 * Starts the ADC, polls for conversion, reads the value, and stops the ADC.
 *
 * @param hadc Pointer to ADC handle.
 * @return uint32_t Raw ADC value (0 on failure).
 */
static uint32_t read_adc_value(ADC_HandleTypeDef* hadc) {
    uint32_t value = 0;
    HAL_StatusTypeDef status;

    if (hadc == NULL) {
        return 0;
    }

    HAL_ADC_Start(hadc);
    status = HAL_ADC_PollForConversion(hadc, 1);  // Timeout 1ms

    if (status == HAL_OK) {
        value = HAL_ADC_GetValue(hadc);
    } else {
        // Reading error
        value = 0;
    }

    HAL_ADC_Stop(hadc);
    return value;
}

/**
 * @brief Read the resistance of the NTC thermistor.
 *
 * Reads the ADC value, calculates voltage, and derives resistance based on the
 * voltage divider configuration.
 *
 * @param temp Pointer to the sensor structure.
 * @return float Measured resistance in Ohms (0 or INFINITY on error).
 */
float temp_ky028_read_resistance(temp_ky028_t* temp) {
    uint32_t adc_raw = 0;
    float adc_voltage = 0;
    float resistance = 0;

    if (temp->hadc == NULL) {
        return 0;
    }

    adc_raw = read_adc_value(temp->hadc);

    if (adc_raw == 0 || adc_raw >= temp->adc_resolution) {
        return 0;  // Reading error or saturation
    }

    adc_voltage = (adc_raw * temp->adc_vref) / temp->adc_resolution;
    if (adc_voltage < temp->adc_vref) {
        // If thermistor is connected to VCC, use this:
        resistance = temp->series_r * ((temp->adc_vref - adc_voltage) / adc_voltage);
    } else {
        resistance = INFINITY;
    }

    return resistance;
}

/**
 * @brief Read the temperature from the sensor.
 *
 * High-level function to get temperature in Celsius.
 *
 * @param temp Pointer to the sensor structure.
 * @param temperature Pointer to float where the temperature will be stored.
 * @return int8_t 0 on success, -1 on error.
 */
int8_t temp_ky028_read_temperature(temp_ky028_t* temp, float* temperature) {
    float resistance = temp_ky028_read_resistance(temp);

    if (resistance <= 0) {
        return TEMP_DRIVER_FAIL;
    }

    if (isinf(resistance)) {
        return TEMP_DRIVER_FAIL;
    }

    *temperature = temp_ky028_resistance_to_temp(resistance, temp->ntc_r25, temp->ntc_b);
    return TEMP_DRIVER_OK;
}

/**
 * @brief Read the temperature from the sensor with averaging.
 *
 * Takes multiple samples, ignores outliers/invalid readings, and averages the result.
 *
 * @param temp Pointer to the sensor structure.
 * @param samples Number of samples to average.
 * @param temperature Pointer to float where the temperature will be stored.
 * @return int8_t 0 on success, -1 if no valid samples were obtained.
 */
int8_t temp_ky028_read_temperature_avg(temp_ky028_t* temp, uint8_t samples, float* temperature) {
    float sum = 0;
    uint8_t valid_samples = 0;

    if (samples == 0) {
        samples = 1;
    }

    for (uint8_t i = 0; i < samples; i++) {
        float temp_val = 0;
        if (temp_ky028_read_temperature(temp, &temp_val) == TEMP_DRIVER_OK) {
            if (temp_val > -200.0f && temp_val < 200.0f) {
                sum += temp_val;
                valid_samples++;
            }
        }

        HAL_Delay(1);
    }

    if (valid_samples == 0) {
        return TEMP_DRIVER_FAIL;
    }

    *temperature = sum / valid_samples;
    return TEMP_DRIVER_OK;
}

/**
 * @brief Convert resistance to temperature using the Beta parameter equation.
 *
 * Steinhart-Hart equation variation (Beta parameter equation).
 *
 * @param resistance Measured resistance.
 * @param r25 Resistance at 25°C.
 * @param b_coeff Beta coefficient.
 * @return float Temperature in Celsius.
 */
float temp_ky028_resistance_to_temp(float resistance, float r25, float b_coeff) {
    float temperature_k = 0;
    float temperature_c = 0;

    temperature_k = 1.0f / ((1.0f / NTC_TEMPERATURE_25C) +
                          (1.0f / b_coeff) * logf(resistance / r25));

    // Convert from Kelvin to Celsius
    temperature_c = temperature_k - 273.15f;

    // Limit value to supported range
    if (temperature_c < MIN_TEMPERATURE) {
        temperature_c = MIN_TEMPERATURE;
    }
    if (temperature_c > MAX_TEMPERATURE) {
        temperature_c = MAX_TEMPERATURE;
    }

    return temperature_c;
}
