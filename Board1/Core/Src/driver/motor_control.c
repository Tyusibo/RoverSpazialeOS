#include "motor_control.h"
#include <math.h>  // roundf


/**
 * Funzione generica di mappatura lineare (Arduino style 'map').
 * Mappa il valore x dal range [in_min, in_max] al range [out_min, out_max].
 * Formula: out = out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min)
 */
static inline float map_linear(float x, float in_min, float in_max, float out_min, float out_max)
{
  return out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min);
}

static inline float volt_to_duty_percent(const MotorControl *m, float volt)
{
  // 1. Normalizza volt in un valore "grezzo" percentuale (0-100 basato su 12V)
  // Nota: Questo passaggio dipende dalla tua logica specifica dove 12V = 100%
  float duty_raw = (volt * 100.0f) / 12.0f;

  // 2. Mappa il range di input (es. -100 a +100) al range di output configurato (es. 56.8 a 94.6)
  return map_linear(duty_raw, m->in_min, m->in_max, m->out_min, m->out_max);
}

static inline int duty_percent_to_pulse(const MotorControl *m, float duty_percent)
{
  float pulse_f = (duty_percent / 100.0f) * (float)m->arr_pwm_plus_one;
  return (int)roundf(pulse_f);
}

void MotorControl_Init(
  MotorControl *mc,
  TIM_HandleTypeDef *htim_pwm,
  uint32_t pwm_channel,
  float Ts,
  float min_volt, float max_volt,
  float in_min, float in_max, float out_min, float out_max,
  float dc_gain, 
  float pulse_theo_min, float pulse_theo_max, 
  float pulse_real_min, float pulse_real_max,
  PIDController *default_regulator
)
{
  mc->htim_pwm = htim_pwm;
  mc->pwm_channel = pwm_channel;
  mc->Ts = Ts;

  mc->min_volt = min_volt;
  mc->max_volt = max_volt;
  mc->in_min = in_min;
  mc->in_max = in_max;
  mc->out_min = out_min;
  mc->out_max = out_max;
  
  mc->dc_gain = dc_gain; 
  
  mc->pulse_theo_min = pulse_theo_min;
  mc->pulse_theo_max = pulse_theo_max;
  mc->pulse_real_min = pulse_real_min;
  mc->pulse_real_max = pulse_real_max;

  mc->current_regulator = default_regulator;

  mc->reference_rpm = 0.0f;

  mc->arr_pwm_plus_one = (uint64_t)__HAL_TIM_GET_AUTORELOAD(htim_pwm) + 1ULL;

  mc->last_u = 0.0f;
  mc->last_pulse = 0;
}

void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm)
{
  mc->reference_rpm = ref_rpm;
}

void MotorControl_SetRegulator(MotorControl *mc, PIDController *reg)
{
    if (reg != NULL)
    {
    	PID_Change_Context(reg, mc->current_regulator);
        mc->current_regulator = reg;
    }
}

float MotorControl_ComputeU(MotorControl *mc, float speed_rpm)
{
  float e = mc->reference_rpm - speed_rpm;

  float u_sat = PID_Compute(mc->current_regulator, e, mc->min_volt, mc->max_volt);

  mc->last_u = u_sat;

  return u_sat;
}

/* Funzione di ricalibrazione empirica statica basata sui dati dell'istanza */
static int recalibrate_pulse(const MotorControl *mc, int pulse_theoretical)
{
    // Utilizza la funzione generica map_linear con i parametri della struct
    float pulse_corrected = map_linear((float)pulse_theoretical, 
                                       mc->pulse_theo_min, mc->pulse_theo_max, 
                                       mc->pulse_real_min, mc->pulse_real_max);

    return (int)roundf(pulse_corrected);
}

int MotorControl_Actuate(MotorControl *mc, float u_volt)
{
  float duty = volt_to_duty_percent(mc, u_volt);
  int pulse = duty_percent_to_pulse(mc, duty);

  // Ricalibrazione Hardware-Specific passandogli l'oggetto mc
  //pulse = recalibrate_pulse(mc, pulse);

  // (opzionale ma utile) clamp CCR in [0, ARR]
  if (pulse < 0) pulse = 0;
  if ((uint64_t)pulse > (mc->arr_pwm_plus_one - 1ULL)) pulse = (int)(mc->arr_pwm_plus_one - 1ULL);

  __HAL_TIM_SET_COMPARE(mc->htim_pwm, mc->pwm_channel, (uint32_t)pulse);

  mc->last_pulse = pulse;
  return pulse;
}

int MotorControl_Update(MotorControl *mc, float speed_rpm)
{
  float u = MotorControl_ComputeU(mc, speed_rpm);
  return MotorControl_Actuate(mc, u);
}

void MotorControl_OpenLoopActuate(MotorControl *mc){
    // Usa il guadagno specifico del motore salvato nella struct
    // u_volt = ref_rpm / k_gain
    if (mc->dc_gain > 0.001f) { // Evita divisione per zero
        float u_volt = mc->reference_rpm / mc->dc_gain;
        MotorControl_Actuate(mc, u_volt);
    } else {
        MotorControl_Actuate(mc, 0.0f);
    }
}

