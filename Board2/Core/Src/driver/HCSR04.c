#include "HCSR04.h"
#include "cmsis_os.h"  // osKernelLock, osKernelRestoreLock

#define SOUND_SPEED_CM_PER_US (0.0343f / 2.0f)  // 0.01715 cm/us
#define TIM2_TICKS_PER_US     (16.0f)           // 16 MHz -> 16 ticks/us
#define TRIG_PULSE_US         (10u)
#define DISTANCE_LIMIT 		  400  // 400 cm = 4 m
#define DEFAULT_TIMEOUT_VALUE 500  // 500 cm = 5 m
#define MAX_SECONDS 240000  // 24,000 us = 24 ms = 0.024 s
// Equivale al tempo per percorrere 8 metri andata e ritorno (4m max range sonar standard)

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


// Compute elapsed ticks between start and end for a timer that wraps with Period (ARR + 1).
static inline uint64_t elapsed_ticks_period(uint32_t start, uint32_t end, uint64_t period)
{
    if (end >= start) return (uint64_t)(end - start);
    return (period - (uint64_t)start) + (uint64_t)end;
}

// Busy-wait in ticks using the timer counter itself (works with wrap because unsigned subtraction).
static inline void delay_ticks(TIM_HandleTypeDef *htim, uint32_t ticks, uint64_t period)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(htim);
    while (elapsed_ticks_period(start, __HAL_TIM_GET_COUNTER(htim), period) < (uint64_t)ticks) { }
}


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

int8_t hcsr04_trigger(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;

    sensor->rx_done = 0; // Reset flag prima di ogni misura

    sensor->current_polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    __HAL_TIM_SET_CAPTUREPOLARITY(sensor->echo_tim, sensor->echo_channel, sensor->current_polarity);

    // 10us pulse on TRIG (TIM2 @ 16MHz => 160 ticks)
    int32_t lock = osKernelLock();   // blocca lo scheduler (no context switch)

    HAL_GPIO_WritePin(sensor->trigger_port, sensor->trigger_pin, GPIO_PIN_SET);
    delay_ticks(sensor->echo_tim, TRIG_PULSE_US * (uint32_t)TIM2_TICKS_PER_US, sensor->arr_timer_plus_one);
    HAL_GPIO_WritePin(sensor->trigger_port, sensor->trigger_pin, GPIO_PIN_RESET);

    osKernelRestoreLock(lock);       // ripristina lo stato precedente

    // Enable the correct capture interrupt for this channel
    uint32_t it = channel_to_it(sensor->echo_channel);
    if (it == 0u) return HCSR04_ERR;

    __HAL_TIM_CLEAR_IT(sensor->echo_tim, it);
    __HAL_TIM_ENABLE_IT(sensor->echo_tim, it);

    return HCSR04_OK;
}

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

int8_t hcsr04_set_default_distance(hcsr04_t *sensor)
{
    if (!sensor) return HCSR04_ERR;
    sensor->distance = DEFAULT_TIMEOUT_VALUE;
    return HCSR04_OK;
}

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

uint8_t hcsr04_is_done(hcsr04_t *sensor) {
    if (!sensor) return 0;
    return sensor->rx_done;
}
