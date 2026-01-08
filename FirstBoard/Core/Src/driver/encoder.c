#include "encoder.h"
#include <stdlib.h>  // llabs

void Encoder_Init(Encoder *e, TIM_HandleTypeDef *htim_enc, float Ts, uint32_t counts)
{
  e->htim_enc = htim_enc;
  e->counts = counts;

  e->past_cnt = (uint32_t)__HAL_TIM_GET_COUNTER(htim_enc);
  e->last_tick = HAL_GetTick(); // Inizializza il timer
  e->arr_enc_plus_one = (uint64_t)__HAL_TIM_GET_AUTORELOAD(htim_enc) + 1ULL;

  e->speed_rpm = 0.0f;
}

int64_t Encoder_Update(Encoder *e)
{
  uint32_t current_cnt = (uint32_t)__HAL_TIM_GET_COUNTER(e->htim_enc);
  int64_t delta = (int64_t)current_cnt - (int64_t)e->past_cnt;

  // range = ARR+1 (cache)
  int64_t range = (int64_t)e->arr_enc_plus_one;
  int64_t half  = range / 2;

  // correzione overflow/underflow
  if (llabs(delta) > half) {
    if (delta < 0) delta += range;
    else           delta -= range;
  }

  // --- Calcolo DT basato su HAL_GetTick
  uint32_t current_tick = HAL_GetTick();
  float dt_seconds = (float)(current_tick - e->last_tick) / 1000.0f;

  if (dt_seconds < 0.0001f) {
      dt_seconds = e->Ts; // Fallback se chiamato troppo velocemente
  }
  e->last_tick = current_tick;

  // Average speed
  e->speed_rpm = ((float)delta * 60.0f) / ((float)e->counts * dt_seconds);

  e->past_cnt = current_cnt;
  return delta;
}
