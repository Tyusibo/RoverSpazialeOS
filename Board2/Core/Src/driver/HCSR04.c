#include "HCSR04.h"

#define SOUND_SPEED_CM_PER_US (0.0343f / 2.0f)  // 0.01715 cm/us
#define TIM2_TICKS_PER_US     (16.0f)           // 16 MHz -> 16 ticks/us
#define TRIG_PULSE_US         (10u)

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

// Busy-wait in ticks using the timer counter itself (works with wrap because unsigned subtraction).
static inline void delay_ticks(TIM_HandleTypeDef *htim, uint32_t ticks)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(htim);
    while ((uint32_t)(__HAL_TIM_GET_COUNTER(htim) - start) < ticks) { }
}

// Compute elapsed ticks between start and end for a timer that wraps at ARR (auto-reload).
static inline uint32_t elapsed_ticks_arr(uint32_t start, uint32_t end, uint32_t arr)
{
    // Timer counts 0..ARR then wraps to 0
    if (end >= start) return end - start;
    return (arr - start + 1u) + end;
}

int8_t hcsr04_init(hcsr04_t *sensor,
                   GPIO_TypeDef *trigger, uint16_t pin_trigger,
                   TIM_HandleTypeDef *echo_tim, uint16_t echo_channel)
{
    if (!sensor || !trigger || !echo_tim) return HCSR04_ERR;

    sensor->gpio_port_sensor = trigger;
    sensor->gpio_pin_sensor  = pin_trigger;
    sensor->echo_tim         = echo_tim;
    sensor->echo_channel     = echo_channel;

    sensor->capture_flag  = 0;
    sensor->start_counter = 0;
    sensor->end_counter   = 0;
    sensor->distance      = 0.0f;

    // Trigger pin low
    HAL_GPIO_WritePin(sensor->gpio_port_sensor, sensor->gpio_pin_sensor, GPIO_PIN_RESET);

    // Start with rising edge capture
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_RISING);

    // Make sure channel interrupt is disabled until we trigger
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it != 0u) __HAL_TIM_DISABLE_IT(sensor->echo_tim, it);

    return HCSR04_OK;
}

int8_t hcsr04_trigger(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    // Prepare state for a fresh measurement
    sensor->capture_flag = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_RISING);

    // 10us pulse on TRIG (TIM2 @ 16MHz => 160 ticks)
    HAL_GPIO_WritePin(sensor->gpio_port_sensor, sensor->gpio_pin_sensor, GPIO_PIN_SET);
    delay_ticks(sensor->echo_tim, TRIG_PULSE_US * (uint32_t)TIM2_TICKS_PER_US);
    HAL_GPIO_WritePin(sensor->gpio_port_sensor, sensor->gpio_pin_sensor, GPIO_PIN_RESET);

    // Enable the correct capture interrupt for this channel
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it == 0u) return HCSR04_ERR;

    __HAL_TIM_CLEAR_IT(sensor->echo_tim, it);
    __HAL_TIM_ENABLE_IT(sensor->echo_tim, it);
    return HCSR04_OK;
}

// Call this from HAL_TIM_IC_CaptureCallback(), for the right sensor matching the active channel.
int8_t hcsr04_read_distance(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    switch (sensor->capture_flag) {
    case 0: {
        // Rising edge -> store start
        sensor->start_counter = HAL_TIM_ReadCapturedValue(sensor->echo_tim, sensor->echo_channel);
        sensor->capture_flag  = 1;

        // Next capture on falling edge
        __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_FALLING);
        return HCSR04_OK;
    }

    case 1: {
        // Falling edge -> store end
        sensor->end_counter = HAL_TIM_ReadCapturedValue(sensor->echo_tim, sensor->echo_channel);
        sensor->capture_flag = 0;

        // Compute elapsed ticks with the real wrap value (ARR)
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(sensor->echo_tim);
        uint32_t diff_ticks = elapsed_ticks_arr(sensor->start_counter, sensor->end_counter, arr);

        // Convert ticks -> us -> cm
        float time_us = (float)diff_ticks / TIM2_TICKS_PER_US;
        sensor->distance = time_us * SOUND_SPEED_CM_PER_US;

        // Clamp (in cm)
        if (sensor->distance > (float)DISTANCE_LIMIT) sensor->distance = (float)DISTANCE_LIMIT;
        if (sensor->distance < 0.0f) sensor->distance = 0.0f;

        // Restore for next measurement
        __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, TIM_INPUTCHANNELPOLARITY_RISING);

        // Disable this channel interrupt until next trigger
        uint32_t it = channel_to_it(sensor->echo_channel);
        if (it != 0u) __HAL_TIM_DISABLE_IT(sensor->echo_tim, it);

        return HCSR04_OK;
    }

    default:
        sensor->capture_flag = 0;
        return HCSR04_ERR;
    }
}
