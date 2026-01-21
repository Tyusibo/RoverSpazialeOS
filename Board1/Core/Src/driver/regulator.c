#include "regulator.h"

void PID_Init(PIDController *pid, float k_err, float k_last_err)
{
    pid->k_err = k_err;
    pid->k_last_err = k_last_err;
    PID_Reset(pid);
}

void PID_Reset(PIDController *pid)
{
    pid->last_error = 0.0f;
    pid->z = 0.0f;
}

void PID_Change_Context(PIDController *pid_1, PIDController *pid_2)
{
	pid_1->last_error = pid_2->last_error;
	pid_1->z = pid_2->z;
}

float PID_Compute(PIDController *pid, float error, float min_sat, float max_sat)
{
  // Control law: q = k_err * e + k_last_err * e_prev
  float q = pid->k_err * error + pid->k_last_err * pid->last_error;
  
  // Update: u = q + z_prev
  float u = q + pid->z;

  // Saturation
  float sat = u;
  if (sat > max_sat)
  {
      sat = max_sat;
  }
  else if (sat < min_sat)
  {
      sat = min_sat;
  }

  // Update state (anti-windup by using saturated value)
  pid->z = sat;
  pid->last_error = error;
  
  return sat;
}
