#ifndef INC_CONSTANTS_SCHEDULING_CONSTANTS_H_
#define INC_CONSTANTS_SCHEDULING_CONSTANTS_H_

/* =========================
 *  Board1: motori + encoder + sonar + comm
 * ========================= */

/* Hard-ish realtime: controllo motori */

#define T_PID           		20u    /* 200 Hz: PID/FOC semplice, update PWM */

#define T_SUPERVISOR           	50u    /* 200 Hz: PID/FOC semplice, update PWM */

#define T_BATTERY           	200u    /* 200 Hz: PID/FOC semplice, update PWM */

#define T_TEMPERATURE           200u    /* 200 Hz: PID/FOC semplice, update PWM */

/* Acquisizione encoder (se non fai tutto nel controllo) */
#define T_ENCODER_PROCESS_MS         5u    /* 200 Hz: calcolo velocità/filtri */

/* Sonar: sequenziato (3 sonar) -> “macro-periodo” */
#define T_SONAR_SEQUENCE_MS          60u   /* ~16.7 Hz: un giro completo sui 3 sonar */
#define SONAR_INTER_PING_GAP_MS      20u   /* spacing minimo tra ping (anti-echo) */

/* Sensori “lenti”/health */
#define T_BATTERY_ADC_MS             100u  /* 10 Hz: tensione/percentuale */
#define T_THERMAL_MS                 200u  /* 5 Hz: temp/diagnostica */

/* Comunicazione tra board */
#define T_UART_EXCHANGE_MS           20u   /* 50 Hz: frame sensori/stato (IT/DMA) */
#define T_PROTOCOL_WATCHDOG_MS       100u  /* 10 Hz: timeout link/heartbeat */

/* Supervisione/safety */
#define T_SUPERVISOR_MS              20u   /* 50 Hz: safety, arbitration, limitazioni */

/* Logging/telemetria (best-effort) */
#define T_LOG_MS                     200u  /* 5 Hz */

/* Utilità */
#define MS_TO_TICKS(ms)              (ms)  /* se kernel tick = 1ms; altrimenti adatta */


#endif /* INC_CONSTANTS_SCHEDULING_CONSTANTS_H_ */
