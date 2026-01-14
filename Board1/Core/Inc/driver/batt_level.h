/*
 * batt_level.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Armando Book
 */

#ifndef INC_BATT_LEVEL_H_
#define INC_BATT_LEVEL_H_

#include "stm32g4xx_hal.h"

#define MAX_VOLTAGE (12.6f)
#define MIN_VOLTAGE (10.5f)

#define MAX_BATT_PERC (100)
#define MIN_BATT_PERC (0)

#define R1 (36000.0f) // Resistenza superiore (collegata a Vbat)
#define R2 (12000.0f) // Resistenza inferiore (collegata a GND)

#define ADC_VREF (3.3f) // Tensione di riferimento ADC

typedef struct{
	ADC_HandleTypeDef* hadc;
	float r1;
	float r2;
	float adc_vref;           // Tensione riferimento ADC (3.3V o 3.0V)
	uint32_t adc_resolution;  // 4095 per 12-bit, 1023 per 10-bit
}batt_level_t;


void battery_init(batt_level_t* bat, ADC_HandleTypeDef* hadc, float r1, float r2, float adc_vref);

float battery_read_voltage(batt_level_t* bat);

float battery_read_voltage_avg(batt_level_t* bat, uint8_t samples);

uint8_t battery_get_percentage_linear(float voltage, float min_v, float max_v);



#endif /* INC_BATT_LEVEL_H_ */
