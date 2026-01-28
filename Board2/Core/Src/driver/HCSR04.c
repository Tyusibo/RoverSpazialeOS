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
 * @file HCSR04.c
 * @brief HC-SR04 Ultrasonic Sensor Driver Implementation.
 *
 * This file contains the implementation of the driver for the HC-SR04
 * ultrasonic distance sensor. It handles initialization, triggering,
 * and echo capture using STM32 HAL Timer peripherals and GPIOs.
 * The distance is calculated based on the flight time of the sound wave.
 */

#include "HCSR04.h"
#include "cmsis_os.h"  // osPriority definitions
#include "scheduler_utils.h"

#define SOUND_SPEED_CM_PER_US (0.0343f / 2.0f)  // 0.01715 cm/us
#define TIM2_TICKS_PER_US     (16.0f)           // 16 MHz -> 16 ticks/us
#define TRIG_PULSE_US         (10u)
#define DISTANCE_LIMIT 		  400  // 400 cm = 4 m
#define DEFAULT_TIMEOUT_VALUE 390  // 390 cm = 3.9 m
#define MAX_SECONDS 240000  // 24,000 us = 24 ms = 0.024 s
// Equivalent to time to travel 8 meters round trip (4m max range standard sonar)

/**
 * @brief Converts a TIM channel to its corresponding interrupt flag.
 *
 * @param ch The timer channel (e.g., TIM_CHANNEL_1).
 * @return uint32_t The corresponding interrupt flag (e.g., TIM_IT_CC1), or 0 if invalid.
 */
static inline uint32_t channel_to_it(uint32_t ch)
{
    switch (ch) {
    case TIM_CHANNEL_1: return TIM_IT_CC1;
    case TIM_CHANNEL_2: return TIM_IT_CC2;
    case TIM_CHANNEL_3: return TIM_IT_CC3;
    case TIM_CHANNEL_4: return TIM_IT_CC4;
    default: return 0;
    }
}


/**
 * @brief Computes elapsed ticks between start and end for a timer that wraps.
 *
 * @param start The starting counter value.
 * @param end The ending counter value.
 * @param period The period of the timer (ARR + 1).
 * @return uint64_t The number of elapsed ticks.
 */
static inline uint64_t elapsed_ticks_period(uint32_t start, uint32_t end, uint64_t period)
{
    if (end >= start) return (uint64_t)(end - start);
    return (period - (uint64_t)start) + (uint64_t)end;
}

/**
 * @brief Performs a busy-wait delay in ticks using the timer counter.
 *
 * This function accounts for timer wrapping.
 *
 * @param htim Pointer to the TIM handle.
 * @param ticks Number of ticks to delay.
 * @param period The period of the timer (ARR + 1).
 */
static inline void delay_ticks(TIM_HandleTypeDef *htim, uint32_t ticks, uint64_t period)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(htim);
    while (elapsed_ticks_period(start, __HAL_TIM_GET_COUNTER(htim), period) < (uint64_t)ticks) { }
}


/**
 * @brief Initializes the HC-SR04 sensor structure.
 *
 * Configures the sensor handle with the provided GPIO and Timer settings,
 * initializes internal state, prepares the trigger pin, and sets up the
 * echo timer for rising edge capture.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @param trigger GPIO Port for the Trigger pin.
 * @param pin_trigger GPIO Pin number for the Trigger pin.
 * @param echo_tim Timer handle used for Echo capture.
 * @param echo_channel Timer channel used for Echo capture.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on invalid parameters.
 */
int8_t hcsr04_init(hcsr04_t *sensor,
                   GPIO_TypeDef *trigger, uint16_t pin_trigger,
                   TIM_HandleTypeDef *echo_tim, uint16_t echo_channel)
{
    if (!sensor || !trigger || !echo_tim) return HCSR04_ERR;

    sensor->trigger_port = trigger;
    sensor->trigger_pin  = pin_trigger;

    sensor->echo_tim     = echo_tim;
    sensor->echo_channel = echo_channel;
    
    // (ARR + 1) cache
    sensor->arr_timer_plus_one = (uint64_t)__HAL_TIM_GET_AUTORELOAD(echo_tim) + 1ULL;

    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sensor->start_counter = 0;
    sensor->end_counter   = 0;
    sensor->distance      = 0;
    sensor->rx_done       = 0;

    // Trigger pin low
    HAL_GPIO_WritePin(sensor->trigger_port, sensor->trigger_pin, GPIO_PIN_RESET);

    // Start with rising edge capture
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, sensor->current_polarity);

    // Make sure channel interrupt is disabled until we trigger
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it != 0u) __HAL_TIM_DISABLE_IT(sensor->echo_tim, it);

    return HCSR04_OK;
}

/**
 * @brief Sends a trigger pulse to the sensor to start a measurement.
 *
 * This function generates a 10us high pulse on the trigger pin. It raises
 * the task priority to osPriorityHigh5 to ensure precise timing without
 * disabling interrupts. It also resets the capture state and enables the
 * interrupt for the echo pin.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_trigger(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    sensor->rx_done = 0; // Reset flag before each measurement

    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, sensor->current_polarity);

    // 10us pulse on TRIG (TIM2 @ 16MHz => 160 ticks)
    // Elevate priority to prevent preemption (but allow ISRs)
    osPriority_t old_prio = scheduler_set_high_priority();

    HAL_GPIO_WritePin(sensor->trigger_port, sensor->trigger_pin, GPIO_PIN_SET);
    delay_ticks(sensor->echo_tim, TRIG_PULSE_US * (uint32_t)TIM2_TICKS_PER_US, sensor->arr_timer_plus_one);
    HAL_GPIO_WritePin(sensor->trigger_port, sensor->trigger_pin, GPIO_PIN_RESET);

    scheduler_restore_priority(old_prio);       // Restore previous priority

    // Enable the correct capture interrupt for this channel
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it == 0u) return HCSR04_ERR;

    __HAL_TIM_CLEAR_IT(sensor->echo_tim, it);
    __HAL_TIM_ENABLE_IT(sensor->echo_tim, it);

    return HCSR04_OK;
}

/**
 * @brief Callback to handle the rising edge capture event.
 *
 * Stores the start timer value and reconfigures the timer to capture
 * the falling edge.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_capture_rising_edge(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    // Rising edge -> store start
    sensor->start_counter = HAL_TIM_ReadCapturedValue(sensor->echo_tim, sensor->echo_channel);
    
    // Switch state to falling
    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_FALLING;

    // Next capture on falling edge
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_FALLING);

    return HCSR04_OK;
}

/**
 * @brief Callback to handle the falling edge capture event.
 *
 * Stores the end timer value, resets polarity to rising for the next cycle,
 * disables the interrupt, and marks the reception as done.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_capture_falling_edge(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    // Falling edge -> store end
    sensor->end_counter = HAL_TIM_ReadCapturedValue(sensor->echo_tim, sensor->echo_channel);
    
    // Reset state to rising (prepare for next cycle)
    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_RISING;

    // Restore for next measurement
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_RISING);

    // Disable this channel interrupt until next trigger
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it != 0u) __HAL_TIM_DISABLE_IT(sensor->echo_tim, it);
    
    sensor->rx_done = 1;

    return HCSR04_OK;
}

/**
 * @brief Processes the captured timer values to calculate distance.
 *
 * Computes the time difference between rising and falling edges, converts it
 * to microseconds, and then to centimeters. Limits the result to the
 * logical range.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_process_distance(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    // Use cached Period (ARR + 1)
    uint64_t diff_ticks = elapsed_ticks_period(sensor->start_counter, sensor->end_counter, sensor->arr_timer_plus_one);

    // Convert ticks -> us -> cm
    float time_us = (float)diff_ticks / TIM2_TICKS_PER_US;
    float dist_f = 0.0f;

    if (time_us > 24000.0f) {
        dist_f = (float)DISTANCE_LIMIT;
    } else {
        dist_f = time_us * SOUND_SPEED_CM_PER_US;
    }

    // Clamp (in cm)
    if (dist_f > (float)DISTANCE_LIMIT)
        dist_f = (float)DISTANCE_LIMIT;
    if (dist_f < 0.0f)
        dist_f = 0.0f;

    sensor->distance = (uint16_t)dist_f;

    return HCSR04_OK;
}

/**
 * @brief Sets the sensor distance to a default timeout value.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_set_default_distance(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;
    sensor->distance = DEFAULT_TIMEOUT_VALUE;
    return HCSR04_OK;
}

/**
 * @brief Resets the sonar internal state and interrupts.
 *
 * Disables interrupts, clears pending flags, resets counters, and sets the
 * capture polarity back to rising edge.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return int8_t HCSR04_OK on success, HCSR04_ERR on error.
 */
int8_t hcsr04_reset_sonar(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    // 1. Disable interrupt
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it != 0u) __HAL_TIM_DISABLE_IT(sensor->echo_tim, it);

    // 2. Clear any pending interrupt flags
    __HAL_TIM_CLEAR_IT(sensor->echo_tim, it);

    // 3. Reset internal state variables
    sensor->rx_done = 0;
    sensor->start_counter = 0;
    sensor->end_counter = 0;
    
    // 4. Ensure polarity is reset for next trigger
    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_RISING);

    return HCSR04_OK;
}

/**
 * @brief Checks if the measurement reception is complete.
 *
 * @param sensor Pointer to the HC-SR04 sensor handle.
 * @return uint8_t 1 if reception is done, 0 otherwise.
 */
uint8_t hcsr04_is_done(hcsr04_t *sensor) {
    if (!sensor) return 0;
    return sensor->rx_done;
}
