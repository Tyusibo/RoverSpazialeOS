/*
 * temperature_adc.h
 *
 *  Created on: Jan 11, 2026
 *      Author: Armando Book
 */

#ifndef INC_TEMPERATURE_ADC_H_
#define INC_TEMPERATURE_ADC_H_

#include "stm32g4xx_hal.h"

// Parametri del termistore NTC (10k ohm @ 25°C, coefficiente B = 3950)
#define NTC_RESISTANCE_25C     (10000.0f)  // Resistenza a 25°C
#define NTC_B_COEFFICIENT      (3950.0f)   // Coefficiente B
#define NTC_TEMPERATURE_25C    (298.15f)   // 25°C in Kelvin (25 + 273.15)
#define SERIES_RESISTOR        (4700.0f)   // Resistenza di serie nel partitore (KY-028 spesso usa 4.7k o 10k)

// Range di temperatura
#define MIN_TEMPERATURE        (-55.0f)    // Temperatura minima supportata
#define MAX_TEMPERATURE        (125.0f)    // Temperatura massima supportata

typedef struct {
    ADC_HandleTypeDef* hadc;
    float adc_vref;           // Tensione riferimento ADC (3.3V o 3.0V)
    uint32_t adc_resolution;  // 4095 per 12-bit, 1023 per 10-bit

    // Parametri configurabili per diversi termistori
    float ntc_r25;            // Resistenza a 25°C
    float ntc_b;              // Coefficiente B
    float series_r;           // Resistenza di serie
} temp_ky028_t;

// Funzioni di inizializzazione
void temp_ky028_init(temp_ky028_t* temp, ADC_HandleTypeDef* hadc, float adc_vref);

// Funzioni di lettura
float temp_ky028_read_resistance(temp_ky028_t* temp);
float temp_ky028_read_temperature(temp_ky028_t* temp);
float temp_ky028_read_temperature_avg(temp_ky028_t* temp, uint8_t samples);

// Funzioni di utilità
float temp_ky028_resistance_to_temp(float resistance, float r25, float b_coeff);



#endif /* INC_TEMPERATURE_ADC_H_ */
