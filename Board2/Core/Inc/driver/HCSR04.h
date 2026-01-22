#ifndef INC_DRIVER_HCSR04_H_
#define INC_DRIVER_HCSR04_H_

#include "stm32g4xx_hal.h"

#define HCSR04_ERR -1
#define HCSR04_OK 1

typedef struct {

	/* Trigger Pin */
	GPIO_TypeDef *trigger_port;
	uint16_t trigger_pin;

	/* Echo Timer and Channel */
	TIM_HandleTypeDef *echo_tim;
	uint16_t echo_channel;

	uint64_t arr_timer_plus_one;

	uint32_t start_counter;
	uint32_t end_counter;

	uint32_t current_polarity;

	uint16_t distance;

	uint8_t rx_done;
} hcsr04_t;


int8_t hcsr04_init(hcsr04_t *sensor, GPIO_TypeDef *trigger,
		uint16_t pin_trigger, TIM_HandleTypeDef *TIM_ECHO,
		uint16_t echo_channel);
int8_t hcsr04_trigger(hcsr04_t *sensor);

int8_t hcsr04_capture_rising_edge(hcsr04_t *sensor);
int8_t hcsr04_capture_falling_edge(hcsr04_t *sensor);

int8_t hcsr04_process_distance(hcsr04_t *sensor);
int8_t hcsr04_set_default_distance(hcsr04_t *sensor);
int8_t hcsr04_reset_sonar(hcsr04_t *sensor);

uint8_t hcsr04_is_done(hcsr04_t *sensor);


#endif /* INC_DRIVER_HCSR04_H_ */
