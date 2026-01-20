#ifndef INC_DRIVER_REGULATOR_H_
#define INC_DRIVER_REGULATOR_H_

/**
 * @brief PID Controller structure.
 */
typedef struct {
  // Coefficients
  float k_err;
  float k_last_err;
  
  // State
  float last_error;
  float z; // memory (accumulated output / integral state)
} PIDController;

/**
 * @brief Initialize the PID Controller.
 * @param pid Pointer to the PIDController structure.
 * @param k_err Error gain.
 * @param k_last_err Last Error gain.
 */
void PID_Init(PIDController *pid, float k_err, float k_last_err);

/**
 * @brief Reset the internal state of the PID Controller.
 * @param pid Pointer to the PIDController structure.
 */
void PID_Reset(PIDController *pid);

/**
 * @brief Compute the control output.
 * @param pid Pointer to the PIDController structure.
 * @param error Current error (reference - measure).
 * @param min_sat Minimum saturation limit.
 * @param max_sat Maximum saturation limit.
 * @return Saturated control output.
 */
float PID_Compute(PIDController *pid, float error, float min_sat, float max_sat);

#endif /* INC_DRIVER_REGULATOR_H_ */
