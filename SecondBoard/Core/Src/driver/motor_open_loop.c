#include "motor_open_loop.h"
#include <math.h>  // roundf

static inline float clampf(float x, float lo, float hi)
{
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static inline float volt_to_duty_percent(const MotorOpenLoop *m, float volt)
{
  // PWM+RC produce una tensione media ~ duty * Vref
  // duty% = volt / Vref * 100
  float duty = (volt / m->vref_analog) * 100.0f;
  return duty;
}

static inline int duty_percent_to_pulse(const MotorOpenLoop *m, float duty_percent)
{
  float pulse_f = (duty_percent / 100.0f) * (float)m->arr_pwm_plus_one;
  return (int)roundf(pulse_f);
}

void MotorOpenLoop_Init(
  MotorOpenLoop *mol,
  TIM_HandleTypeDef *htim_pwm,
  uint32_t pwm_channel,
  float vref_analog,
  float v_min, float v_max,
  float v_neutral,
  float duty_min, float duty_max
)
{
  mol->htim_pwm = htim_pwm;
  mol->pwm_channel = pwm_channel;

  mol->arr_pwm_plus_one = (uint64_t)__HAL_TIM_GET_AUTORELOAD(htim_pwm) + 1ULL;

  mol->vref_analog = vref_analog;
  mol->v_min = v_min;
  mol->v_max = v_max;
  mol->v_neutral = v_neutral;

  mol->duty_min = duty_min;
  mol->duty_max = duty_max;

  mol->reference_cmd = 0.0f;

  mol->last_volt  = v_neutral;
  mol->last_duty  = volt_to_duty_percent(mol, v_neutral);
  mol->last_pulse = 0;
}

void MotorOpenLoop_SetReferenceCmd(MotorOpenLoop *mol, float ref_cmd)
{
  // clamp del comando in [-1..+1]
  mol->reference_cmd = clampf(ref_cmd, -1.0f, 1.0f);
}

float MotorOpenLoop_ComputeVolt(MotorOpenLoop *mol)
{
  float r = clampf(mol->reference_cmd, -1.0f, 1.0f);

  // Mappa piecewise attorno al neutral:
  // r in [-1..0] => [v_min .. v_neutral]
  // r in [ 0..1] => [v_neutral .. v_max]
  float v;
  if (r >= 0.0f) {
    v = mol->v_neutral + r * (mol->v_max - mol->v_neutral);
  } else {
    v = mol->v_neutral + r * (mol->v_neutral - mol->v_min); // r negativo
  }

  v = clampf(v, mol->v_min, mol->v_max);
  mol->last_volt = v;
  return v;
}

int MotorOpenLoop_ActuateVolt(MotorOpenLoop *mol, float volt)
{
  // clamp volt
  float v = clampf(volt, mol->v_min, mol->v_max);

  // volt -> duty%
  float duty = volt_to_duty_percent(mol, v);

  // clamp duty (se vuoi limitare l'intervallo realmente usato)
  duty = clampf(duty, mol->duty_min, mol->duty_max);

  // duty -> CCR pulse
  int pulse = duty_percent_to_pulse(mol, duty);

  // clamp CCR in [0, ARR]
  if (pulse < 0) pulse = 0;
  if ((uint64_t)pulse > (mol->arr_pwm_plus_one - 1ULL)) {
    pulse = (int)(mol->arr_pwm_plus_one - 1ULL);
  }

  __HAL_TIM_SET_COMPARE(mol->htim_pwm, mol->pwm_channel, (uint32_t)pulse);

  mol->last_duty  = duty;
  mol->last_pulse = pulse;
  return pulse;
}

int MotorOpenLoop_Update(MotorOpenLoop *mol)
{
  float v = MotorOpenLoop_ComputeVolt(mol);
  return MotorOpenLoop_ActuateVolt(mol, v);
}
