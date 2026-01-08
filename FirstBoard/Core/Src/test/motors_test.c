#include "motors_test.h"

#include "stm32g4xx_hal.h"
#include "print.h"

#include "encoders_init.h"     // extern encoders[],
#include "motors_init.h"      // motors[], N_MOTORS
#include "encoder.h"
#include "motor_control.h"

/* =========================================================
 * CONFIG TEST (MODIFICA QUI)
 * ========================================================= */

// tempi: i regolatori sono pensati per Ts=0.005s -> dt_ms ~ 5ms
#define TEST_SETTLE_MS   1000U   // dopo quanto considero "a regime" e stampo
#define TEST_HOLD_MS     4000U   // quanto tempo resto fermo per leggere su PuTTY
#define TEST_STOP_MS      200U   // stop "attivo" prima dell'hold

typedef struct {
  float ref_m[N_MOTORS];
  uint8_t slow;     // 0=fast, 1=slow
} MotorsTestCase;

/* Sequenza di test: cambia qui i setpoint */
static const MotorsTestCase seq[] = {
  { .ref_m = { 0,   0,   0,   0 },   .slow = 0 },
  { .ref_m = { 60,  60,  60,  60 },  .slow = 0 },
  { .ref_m = { 120, 120, 120, 120 }, .slow = 0 },
  { .ref_m = { 0,   0,   0,   0 },   .slow = 0 },
};

static const uint32_t seq_len = (uint32_t)(sizeof(seq) / sizeof(seq[0]));

/* =========================================================
 * IMPLEMENTATION
 * ========================================================= */

static void print_motor_line(int i, float ref, float speed, float err, float u, int pulse)
{
  printMsg("M");
  printInt(i);
  printMsg(" ref=");
  printFloat(ref, 2);
  printMsg(" rpm  spd=");
  printFloat(speed, 2);
  printMsg(" rpm  err=");
  printFloat(err, 2);
  printMsg("  u=");
  printFloat(u, 3);
  printMsg("  pulse=");
  printInt(pulse);
  printNewLine();
}

static void stop_all(uint32_t stop_ms)
{
  for (int i = 0; i < N_MOTORS; i++) {
    MotorControl_SetReferenceRPM(&motors[i], 0.0f);
    MotorControl_SelectSlow(&motors[i], 0);
  }

  uint32_t t0 = HAL_GetTick();
  while ((HAL_GetTick() - t0) < stop_ms) {
    for (int i = 0; i < N_MOTORS; i++) {
      Encoder_Update(&encoders[i]);
      float spd = Encoder_GetSpeedRPM(&encoders[i]);

      float u = MotorControl_ComputeU(&motors[i], spd);
      (void)MotorControl_Actuate(&motors[i], u);
    }

    uint32_t dt_ms = (uint32_t)(motors[0].Ts * 1000.0f);
    if (dt_ms == 0) dt_ms = 1;
    HAL_Delay(dt_ms);
  }
}

static void run_case(const MotorsTestCase *tc, uint32_t settle_ms, uint32_t hold_ms)
{
  if (tc == NULL) return;

  // Applica config
  for (int i = 0; i < N_MOTORS; i++) {
    MotorControl_SelectSlow(&motors[i], tc->slow);
    MotorControl_SetReferenceRPM(&motors[i], tc->ref_m[i]);
  }

  // Run per settle_ms
  uint32_t t0 = HAL_GetTick();
  while ((HAL_GetTick() - t0) < settle_ms) {
    for (int i = 0; i < N_MOTORS; i++) {
      Encoder_Update(&encoders[i]);
      float spd = Encoder_GetSpeedRPM(&encoders[i]);

      float u = MotorControl_ComputeU(&motors[i], spd);
      (void)MotorControl_Actuate(&motors[i], u);
    }

    uint32_t dt_ms = (uint32_t)(motors[0].Ts * 1000.0f);
    if (dt_ms == 0) dt_ms = 1;
    HAL_Delay(dt_ms);
  }

  // Snapshot
  printMsg("--- Snapshot (post-settle) ---\r\n");
  for (int i = 0; i < N_MOTORS; i++) {
    float ref = motors[i].reference_rpm;
    float spd = Encoder_GetSpeedRPM(&encoders[i]);
    float err = ref - spd;
    float u   = motors[i].last_u;
    int pulse = motors[i].last_pulse;
    print_motor_line(i, ref, spd, err, u, pulse);
  }

  // Stop + hold
  printMsg("--- STOP motors ---\r\n");
  stop_all(TEST_STOP_MS);

  printMsg("--- HOLD ---\r\n");
  uint32_t th = HAL_GetTick();
  while ((HAL_GetTick() - th) < hold_ms) {
    HAL_Delay(50);
  }
}

void MotorsTest_RunDefaultSequenceBlocking(void)
{
  printMsg("\r\n=== MotorsTest Default Sequence START ===\r\n");

  for (uint32_t k = 0; k < seq_len; k++) {
    printMsg("\r\n--- Step ");
    printInt((int32_t)k);
    printMsg(" ---\r\n");
    run_case(&seq[k], TEST_SETTLE_MS, TEST_HOLD_MS);
  }

  printMsg("\r\n=== MotorsTest Default Sequence END ===\r\n");
}
