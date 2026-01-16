#include "temperature_init.h"

#include "temperature_adc.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;

temp_ky028_t temp_sensor;

void temperature_sensor_init(){
	temp_ky028_init(&temp_sensor, &hadc1, ADC_VREF);

}
