#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_

#define DISTANCE_LIMIT 400  // 400 cm = 4 m

#include "stm32g4xx_hal.h"

typedef struct {
	GPIO_TypeDef *gpio_port_sensor;
	uint16_t gpio_pin_sensor;
	TIM_HandleTypeDef *echo_tim;
	uint16_t echo_channel;
	uint8_t capture_flag;
	uint32_t start_counter;
	uint32_t end_counter;
	float distance;
	uint8_t rx_done; // Flag aggiunto
} hcsr04_t;

typedef enum {
	HCSR04_ERR = -1, HCSR04_OK = 1
} hcsr04_enum;

int8_t hcsr04_init(hcsr04_t *sensor, GPIO_TypeDef *trigger,
		uint16_t pin_trigger, TIM_HandleTypeDef *TIM_ECHO,
		uint16_t echo_channel);
int8_t hcsr04_trigger(hcsr04_t *sensor);
int8_t hcsr04_read_distance(hcsr04_t *sensor); // To be called in the callback of the TIMER
uint8_t hcsr04_is_done(hcsr04_t *sensor); // Nuova funzione

#endif /* INC_HCSR04_H_ */
