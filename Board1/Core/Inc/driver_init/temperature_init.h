#ifndef INC_DRIVER_INIT_TEMPERATURE_INIT_H_
#define INC_DRIVER_INIT_TEMPERATURE_INIT_H_

#include "temperature_adc.h"

#include "adc_constants.h"

extern temp_ky028_t temp_sensor;

void temperature_sensor_init();

#endif /* INC_DRIVER_INIT_TEMPERATURE_INIT_H_ */
