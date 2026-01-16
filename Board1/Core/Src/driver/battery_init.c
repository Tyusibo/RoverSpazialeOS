#include "battery_init.h"

extern ADC_HandleTypeDef hadc2;

batt_level_t battery;


void battery_sensor_init(void){
	HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	battery_init(&battery, &hadc2, R1, R2, ADC_VREF);
	return;
}
