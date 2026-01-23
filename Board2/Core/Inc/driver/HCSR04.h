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
 * @file HCSR04.h
 * @brief Header file for the HC-SR04 Ultrasonic Sensor Driver.
 *
 * This file contains the data structures and function prototypes for
 * controlling the HC-SR04 ultrasonic distance sensor using STM32
 * peripherals.
 */

#ifndef INC_DRIVER_HCSR04_H_
#define INC_DRIVER_HCSR04_H_

#include "stm32g4xx_hal.h"

/**
 * @brief Error code indicating failure.
 */
#define HCSR04_ERR -1

/**
 * @brief Success code indicating successful operation.
 */
#define HCSR04_OK 1

/**
 * @brief HC-SR04 Sensor Handle Structure.
 *
 * Holds all the configuration and state information for a single
 * HC-SR04 sensor instance.
 */
typedef struct {

	GPIO_TypeDef *trigger_port; /**< GPIO Port for the Trigger pin */
	uint16_t trigger_pin;       /**< GPIO Pin for the Trigger pin */

	TIM_HandleTypeDef *echo_tim; /**< Timer handle for Echo capture */
	uint16_t echo_channel;       /**< Timer channel for Echo capture */

	uint64_t arr_timer_plus_one; /**< Cached period (ARR + 1) for tick calculations */

	uint32_t start_counter; /**< Timer counter value at rising edge */
	uint32_t end_counter;   /**< Timer counter value at falling edge */

	uint32_t current_polarity; /**< Current expected timer input capture polarity */

	uint16_t distance; /**< Calculated distance in centimeters */

	uint8_t rx_done; /**< Flag indicating if a measurement is complete (1=done, 0=busy/idle) */
} hcsr04_t;


/**
 * @brief Initializes the HC-SR04 sensor struct.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @param trigger GPIO Port of the Trigger pin.
 * @param pin_trigger GPIO Pin number of the Trigger pin.
 * @param TIM_ECHO Timer handle used for Echo capture.
 * @param echo_channel Timer channel used for Echo capture.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_init(hcsr04_t *sensor, GPIO_TypeDef *trigger,
		uint16_t pin_trigger, TIM_HandleTypeDef *TIM_ECHO,
		uint16_t echo_channel);

/**
 * @brief Triggers a new distance measurement.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_trigger(hcsr04_t *sensor);

/**
 * @brief Callback to capture the rising edge of the Echo signal.
 *
 * Should be called from the timer input capture interrupt.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_capture_rising_edge(hcsr04_t *sensor);

/**
 * @brief Callback to capture the falling edge of the Echo signal.
 *
 * Should be called from the timer input capture interrupt.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_capture_falling_edge(hcsr04_t *sensor);

/**
 * @brief Calculates the distance based on captured timer values.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_process_distance(hcsr04_t *sensor);

/**
 * @brief Sets the distance to a default value (e.g. on timeout).
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_set_default_distance(hcsr04_t *sensor);

/**
 * @brief Resets the sensor state and configuration.
 *
 * Useful to recover from error states or timeouts.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_reset_sonar(hcsr04_t *sensor);

/**
 * @brief Checks if a measurement is complete.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return uint8_t 1 if data is ready, 0 otherwise.
 */
uint8_t hcsr04_is_done(hcsr04_t *sensor);


#endif /* INC_DRIVER_HCSR04_H_ */
