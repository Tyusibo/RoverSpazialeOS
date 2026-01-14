/*
 * temperature_adc.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Armando Book
 */

#include "temperature_adc.h"
#include <math.h>


void temp_ky028_init(temp_ky028_t* temp, ADC_HandleTypeDef* hadc, float adc_vref) {
    temp->hadc = hadc;
    temp->adc_vref = adc_vref;
    temp->adc_resolution = 4095;  // Per ADC 12-bit

    // Parametri default per KY-028 (NTC 10k @ 25°C, B=3950)
    temp->ntc_r25 = NTC_RESISTANCE_25C;
    temp->ntc_b = NTC_B_COEFFICIENT;
    temp->series_r = SERIES_RESISTOR;
}

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
        // Errore di lettura
        value = 0;
    }

    HAL_ADC_Stop(hadc);
    return value;
}

float temp_ky028_read_resistance(temp_ky028_t* temp) {
    uint32_t adc_raw = 0;
    float adc_voltage = 0;
    float resistance = 0;

    if (temp->hadc == NULL) {
        return 0;
    }

    adc_raw = read_adc_value(temp->hadc);

    if (adc_raw == 0 || adc_raw >= temp->adc_resolution) {
        return 0;  // Errore di lettura o saturazione
    }

    adc_voltage = (adc_raw * temp->adc_vref) / temp->adc_resolution;
    if (adc_voltage < temp->adc_vref) {
    	// Se il termistore è verso VCC, usa questa:
    	resistance = temp->series_r * ((temp->adc_vref - adc_voltage) / adc_voltage);
    } else {
        resistance = INFINITY;
    }

    return resistance;
}


float temp_ky028_read_temperature(temp_ky028_t* temp) {
    float resistance = temp_ky028_read_resistance(temp);

    if (resistance <= 0) {
        return -273.15f;  // Ritorna 0 Kelvin in caso di errore
    }

    if (isinf(resistance)) {
        return MIN_TEMPERATURE;  // Temperatura molto bassa
    }

    return temp_ky028_resistance_to_temp(resistance, temp->ntc_r25, temp->ntc_b);
}

float temp_ky028_read_temperature_avg(temp_ky028_t* temp, uint8_t samples) {
    float sum = 0;
    uint8_t valid_samples = 0;

    if (samples == 0) {
        samples = 1;
    }

    for (uint8_t i = 0; i < samples; i++) {
        float temp_val = temp_ky028_read_temperature(temp);

        if (temp_val > -200.0f && temp_val < 200.0f) {
            sum += temp_val;
            valid_samples++;
        }

        HAL_Delay(1);
    }

    if (valid_samples == 0) {
        return -273.15f;
    }

    return sum / valid_samples;
}


float temp_ky028_resistance_to_temp(float resistance, float r25, float b_coeff) {
    float temperature_k = 0;
    float temperature_c = 0;

    temperature_k = 1.0f / ((1.0f / NTC_TEMPERATURE_25C) +
                          (1.0f / b_coeff) * logf(resistance / r25));

    // Converti da Kelvin a Celsius
    temperature_c = temperature_k - 273.15f;

    // Limita il valore all'intervallo supportato
    if (temperature_c < MIN_TEMPERATURE) {
        temperature_c = MIN_TEMPERATURE;
    }
    if (temperature_c > MAX_TEMPERATURE) {
        temperature_c = MAX_TEMPERATURE;
    }

    return temperature_c;
}
