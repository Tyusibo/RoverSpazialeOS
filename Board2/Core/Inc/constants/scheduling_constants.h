#ifndef INC_CONSTANTS_SCHEDULING_CONSTANTS_H_
#define INC_CONSTANTS_SCHEDULING_CONSTANTS_H_

/* =========================
 *  Board2: remote + IMU + decisione + comm
 * ========================= */

/* IMU (accelerometro/gyro su I2C) */
#define T_GYROSCOPE                  40u   /* 100 Hz: stato assetto/filtri */

/* Remote controller (I2C) */
#define T_REMOTE_CONTROLLER          20u   /* 50 Hz: input utente */

/* Supervisore / decisione rover (stato globale + safety) */
#define T_SUPERVISOR             50u   /* 50 Hz */

/* Supervisore / decisione rover (stato globale + safety) */
#define T_SONAR          100u   /* 50 Hz */

// Costanti di simulazione carico (in ms)
#define C_CONTROLLER    2
#define C_GYROSCOPE     4
#define C_SONAR         25

#endif /* INC_CONSTANTS_SCHEDULING_CONSTANTS_H_ */
