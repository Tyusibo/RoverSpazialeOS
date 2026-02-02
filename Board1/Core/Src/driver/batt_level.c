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
 * @file batt_level.c
 * @brief Battery level driver implementation.
 *
 * This file contains the implementation of the battery level monitoring functionalities,
 * including ADC initialization, voltage reading, and percentage calculation.
 *
 * @date Jan 11, 2026
 * @author Armando Book
 */

#include "batt_level.h"
#include <math.h>

// Calibration offset
//#define VOLTAGE_OFFSET (0.75f)

/**
 * @brief Initializes the battery level monitor structure.
 *
 * @param bat Pointer to the battery level structure to initialize.
 * @param hadc Pointer to the ADC handle used for reading the voltage.
 * @param r1 Value of the first resistor in the voltage divider.
 * @param r2 Value of the second resistor in the voltage divider.
 * @param adc_vref Reference voltage of the ADC.
 */
void battery_init(batt_level_t* bat, ADC_HandleTypeDef* hadc, float r1, float r2, float adc_vref){
    bat->hadc = hadc;
    bat->r1 = r1;
    bat->r2 = r2;
    bat->adc_vref = adc_vref;
    bat->adc_resolution = 4095;
}

/**
 * @brief Reads the raw value from the ADC.
 *
 * @param hadc Pointer to the ADC handle.
 * @return int32_t The raw ADC value (0-4095) or -1 if an error occurred / timeout.
 */
static int32_t read_adc_value(ADC_HandleTypeDef* hadc){
    int32_t value = 0;
    HAL_StatusTypeDef status;

    HAL_ADC_Start(hadc);
    status = HAL_ADC_PollForConversion(hadc, 1);  // Timeout 1ms

    if(status == HAL_OK) {
        value = (int32_t)HAL_ADC_GetValue(hadc);
    } else {
        // Timeout expired or error
        value = -1;  
    }

    HAL_ADC_Stop(hadc);
    return value;
}

/**
 * @brief Reads the current battery voltage.
 *
 * This function reads the raw ADC value, converts it to voltage based on the
 * reference voltage and resolution, and then scales it according to the
 * voltage divider resistors. It also clamps the result between MIN_VOLTAGE
 * and MAX_VOLTAGE.
 *
 * @param bat Pointer to the battery level structure.
 * @return float The calculated battery voltage in Volts, or -1.0f if an error occurs.
 */
float battery_read_voltage(batt_level_t* bat){
    int32_t adc_raw = 0;
    float adc_voltage = 0;
    float battery_voltage = 0;

    if(bat->hadc == NULL){
        return -1.0f;
    }

    adc_raw = read_adc_value(bat->hadc);

    if(adc_raw < 0){
        return -1.0f;
    }

    adc_voltage = ((float)adc_raw*bat->adc_vref)/bat->adc_resolution;
    
    // Calculate voltage and apply calibration offset
    battery_voltage = (adc_voltage*((bat->r1 + bat->r2)/bat->r2)); // + VOLTAGE_OFFSET;

    if(battery_voltage < MIN_VOLTAGE){
        battery_voltage = MIN_VOLTAGE;
    }
    if(battery_voltage > MAX_VOLTAGE){
        battery_voltage = MAX_VOLTAGE;
    }

    return battery_voltage;
}

/**
 * @brief Reads the average battery voltage over multiple samples.
 *
 * @param bat Pointer to the battery level structure.
 * @param samples Number of samples to read for the average properly.
 * @return float The average battery voltage in Volts.
 */
float battery_read_voltage_avg(batt_level_t* bat, uint8_t samples){
    float sum = 0;

    if(samples == 0){
        samples = 1;
    }

    for(uint8_t i = 0; i < samples; i++){
        sum += battery_read_voltage(bat);
    }

    return sum/samples;
}

/**
 * @brief Calculates the battery percentage linearly based on voltage limits.
 *
 * @param voltage Measured battery voltage.
 * @param min_v Minimum battery voltage corresponding to 0%.
 * @param max_v Maximum battery voltage corresponding to 100%.
 * @param percentage Pointer to store the calculated percentage (0-100).
 * @return int8_t 0 if successful, -1 if voltage is invalid or pointer is NULL.
 */
int8_t battery_get_percentage_linear(float voltage, float min_v, float max_v, uint8_t* percentage){
    float perc = 0;

    if(percentage == NULL){
        return BATT_LEVEL_FAIL;
    }

    if(voltage < 0){
        return BATT_LEVEL_FAIL;
    }

    if(voltage <= min_v){
        *percentage = MIN_BATT_PERC;
        return BATT_LEVEL_OK;
    }
    if(voltage >= max_v){
        *percentage = MAX_BATT_PERC;
        return BATT_LEVEL_OK;
    }

    perc = ((voltage - min_v)/(max_v-min_v))*100.0f;
    *percentage = (uint8_t)(perc + 0.5f);

    return BATT_LEVEL_OK;
}
