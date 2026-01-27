#ifndef INC_CONSTANTS_SCHEDULING_CONSTANTS_H_
#define INC_CONSTANTS_SCHEDULING_CONSTANTS_H_

/* =========================
 *  Board1
 * ========================= */

/* Hard-ish realtime: controllo motori */
#define T_PID           		20u    /* 20ms (50Hz) */
#define WCET_PID           		2

/* Supervisor Task */
#define T_SUPERVISOR           	50u    /* 50ms (20Hz) */

/* Battery Task */
#define T_BATTERY           	200u    /* 200ms (5Hz) */
#define WCET_BATTERY       		1

/* Temperature Task */
#define T_TEMPERATURE           200u    /* 200ms (5Hz) */
#define WCET_TEMPERATURE   		1


#endif /* INC_CONSTANTS_SCHEDULING_CONSTANTS_H_ */
