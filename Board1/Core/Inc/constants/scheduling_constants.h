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

// Costanti di simulazione carico (in ms)
#define C_PID           2
#define C_TEMPERATURE   1
#define C_BATTERY       1


#endif /* INC_CONSTANTS_SCHEDULING_CONSTANTS_H_ */
