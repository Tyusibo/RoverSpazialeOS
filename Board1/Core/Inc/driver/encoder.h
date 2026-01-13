#ifndef INC_DRIVER_ENCODER_H_
#define INC_DRIVER_ENCODER_H_

#include "stm32g4xx_hal.h"
#include <stdint.h>

typedef struct {
  TIM_HandleTypeDef *htim_enc;    // TIM in Encoder mode
  uint64_t arr_enc_plus_one;      // (ARR + 1) cache in init

  float Ts;                       // periodo controllo [s]
  uint32_t counts;                // COUNTS = PPR * GEAR_RATIO * 4

  uint32_t past_cnt;              // ultimo CNT letto

  uint32_t last_tick;             // ultimo tick letto per calcolo dt

  float speed_rpm;                // ultima velocità calcolata (Average)
} Encoder;

void Encoder_Init(Encoder *e, TIM_HandleTypeDef *htim_enc, float Ts, uint32_t counts);

/**
 * Legge il contatore, calcola delta con correzione overflow/underflow,
 * aggiorna speed_rpm e past_cnt.
 * Ritorna delta (tick nel periodo Ts), utile per debug.
 */
int64_t Encoder_Update(Encoder *e);

/** Utility: ritorna l'ultima velocità calcolata */
static inline float Encoder_GetSpeedRPM(const Encoder *e) { return e->speed_rpm; }


#endif /* INC_DRIVER_ENCODER_H_ */
