/*
 * batt_level.c
 *
 *  Created on: Jan 11, 2026
 *      Author: Armando Book
 */

#include "batt_level.h"
#include <math.h>

void battery_init(batt_level_t* bat, ADC_HandleTypeDef* hadc, float r1, float r2, float adc_vref){
	bat->hadc = hadc;
	bat->r1 = r1;
	bat->r2 = r2;
	bat->adc_vref = adc_vref;
	bat->adc_resolution = 4095;
}


/*
 *
static uint32_t read_adc_value(ADC_HandleTypeDef* hadc){
	uint32_t value = 0;

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 1);
	value = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	return value;
}
 *
 * */

static uint32_t read_adc_value(ADC_HandleTypeDef* hadc){
    uint32_t value = 0;
    HAL_StatusTypeDef status;

    HAL_ADC_Start(hadc);
    status = HAL_ADC_PollForConversion(hadc, 1);  // Timeout 1ms

    if(status == HAL_OK) {
        value = HAL_ADC_GetValue(hadc);
    } else {
        // Timeout scaduto o errore
        value = 0;  // O un valore di default
        // Opzionale: gestisci l'errore
    }

    HAL_ADC_Stop(hadc);
    return value;
}


float battery_read_voltage(batt_level_t* bat){
	uint32_t adc_raw = 0;
	float adc_voltage = 0;
	float battery_voltage = 0;

	if(bat->hadc == NULL){
		return adc_voltage;
	}

	adc_raw = read_adc_value(bat->hadc);
	adc_voltage = (adc_raw*bat->adc_vref)/bat->adc_resolution;
	battery_voltage = adc_voltage*((bat->r1 + bat->r2)/bat->r2);

	if(battery_voltage < MIN_VOLTAGE){
		battery_voltage = MIN_VOLTAGE;
	}
	if(battery_voltage > MAX_VOLTAGE){
		battery_voltage = MAX_VOLTAGE;
	}

	return battery_voltage;
}

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

uint8_t battery_get_percentage_linear(float voltage, float min_v, float max_v){
	float percentage = 0;

	if(voltage <= min_v){
		return MIN_BATT_PERC;
	}
	if(voltage >= max_v){
		return MAX_BATT_PERC;
	}

	percentage = ((voltage - min_v)/(max_v-min_v))*100.0f;

	return (uint8_t)(percentage + 0.5f);
}
