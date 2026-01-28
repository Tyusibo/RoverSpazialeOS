#ifndef INC_CONSTANTS_SCHEDULING_CONSTANTS_H_
#define INC_CONSTANTS_SCHEDULING_CONSTANTS_H_

/* =========================
 *  Board2: remote + IMU + decisione + comm
 * ========================= */

/* IMU (accelerometro/gyro su I2C) */
#define T_GYROSCOPE                  40u   /* 40ms (25Hz) */
#define WCET_GYROSCOPE     			  1u

/* Remote controller (I2C) */
#define T_REMOTE_CONTROLLER          20u   /* 20ms (50Hz) */
#define WCET_CONTROLLER    			  1u

/* Supervisore / decisione rover (stato globale + safety) */
#define T_SUPERVISOR             	 50u   /* 50ms (20Hz) */
#define WCET_SUPERVISOR              20u

/* Sonar Task */
#define T_SONAR          			 100u   /* 100ms (10Hz) */
#define WCET_SONAR         			   1u

/* Comunicazione inter-board (UART) */
#define T_POLLING_SERVER 	  		 10u   /* 10ms (100Hz) */
#define WCET_POLLING_SERVER  		  3u

#endif /* INC_CONSTANTS_SCHEDULING_CONSTANTS_H_ */
