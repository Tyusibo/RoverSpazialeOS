#include "motor_control.h"
#include <math.h>


static inline float saturate_volt(const MotorControl *m, float u)
{
  if (u > m->max_volt) return m->max_volt;
  if (u < m->min_volt) return m->min_volt;
  return u;
}

void MotorControl_Init(
  MotorControl *mc,
  UART_HandleTypeDef *huart,
  uint8_t address,
  uint8_t motor_id,
  float Ts,
  float min_volt, float max_volt,
  float dc_gain, 
  Coefficients pi_fast, Coefficients pi_slow
)
{
  mc->huart = huart;
  mc->address = address;
  mc->motor_id = motor_id;
  mc->Ts = Ts;
  mc->min_volt = min_volt; // es. -12.0
  mc->max_volt = max_volt; // es. +12.0
  mc->dc_gain = dc_gain; 
  mc->pi_fast = pi_fast;
  mc->pi_slow = pi_slow;
  mc->use_slow = 0;

  mc->reference_rpm = 0.0f;
  mc->last_error = 0.0f;
  mc->z = 0.0f;
  mc->last_u = 0.0f;
  mc->last_cmd = 0;
}

void MotorControl_SetReferenceRPM(MotorControl *mc, float ref_rpm)
{
  mc->reference_rpm = ref_rpm;
}

void MotorControl_SelectSlow(MotorControl *mc, uint8_t enable)
{
  mc->use_slow = enable ? 1U : 0U;
}

float MotorControl_ComputeU(MotorControl *mc, float speed_rpm)
{
  float e = mc->reference_rpm - speed_rpm;
  Coefficients pi = mc->use_slow ? mc->pi_slow : mc->pi_fast;
  
  float q = pi.k_err * e + pi.k_last_err * mc->last_error;
  float u = q + mc->z;
  float sat = saturate_volt(mc, u);

  mc->z = sat;
  mc->last_error = e;
  mc->last_u = sat;

  return sat;
}

int MotorControl_Actuate(MotorControl *mc, float u_volt)
{
  // 1. Mappa tensione (-12V..+12V) a range Sabertooth (-127..+127)
  // Assumiamo che max_volt corrisponda alla massima velocità
  float scale_factor = 127.0f / mc->max_volt; 
  float val = u_volt * scale_factor;
  
  // Saturazione
  if (val > 127.0f) val = 127.0f;
  if (val < -127.0f) val = -127.0f;
  
  int speed = (int)val;
  uint8_t command;
  uint8_t data = (uint8_t)(speed < 0 ? -speed : speed);
  
  // Sabertooth Packet Serial - Normal Mode
  // Motor 1: Forward=0, Backward=1
  // Motor 2: Forward=4, Backward=5

  if (mc->motor_id == 1) {
      if (speed >= 0) command = 0; // Forward
      else command = 1;            // Backward
  } else { // Motore 2
      if (speed >= 0) command = 4; // Forward
      else command = 5;            // Backward
  }

  // Calcolo Checksum: (Address + Command + Data) & 0x7F
  uint8_t checksum = (mc->address + command + data) & 0x7F;
  
  uint8_t packet[4] = { mc->address, command, data, checksum };
  
  // Invio UART
  HAL_UART_Transmit(mc->huart, packet, 4, 10);
  
  mc->last_cmd = speed;
  return speed;
}

int MotorControl_Update(MotorControl *mc, float speed_rpm)
{
  float u = MotorControl_ComputeU(mc, speed_rpm);
  return MotorControl_Actuate(mc, u);
}

void MotorControl_OpenLoopActuate(MotorControl *mc){
    if (mc->dc_gain > 0.001f) {
        float u_volt = mc->reference_rpm / mc->dc_gain;
        MotorControl_Actuate(mc, u_volt);
    } else {
        MotorControl_Actuate(mc, 0.0f);
    }
}

