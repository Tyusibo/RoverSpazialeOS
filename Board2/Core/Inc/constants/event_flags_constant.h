#ifndef INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_
#define INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_

/* Event Flags (CMSIS-RTOS v2 / FreeRTOS)
 * Bitmask definitions
 */

#define FLAG_SYNCHRONIZATION        (1U << 0)

/* Controller / Pad */
#define FLAG_PAD_OK                 (1U << 1)
#define FLAG_PAD_ERROR              (1U << 2)

/* Gyroscope */
#define FLAG_GYRO_OK                (1U << 3)
#define FLAG_GYRO_ERROR             (1U << 4)

/* Sonar Left */
#define FLAG_SONAR_LEFT_OK          (1U << 5)
#define FLAG_SONAR_LEFT_TIMEOUT     (1U << 6)

/* Sonar Front */
#define FLAG_SONAR_FRONT_OK         (1U << 7)
#define FLAG_SONAR_FRONT_TIMEOUT    (1U << 8)

/* Sonar Right */
#define FLAG_SONAR_RIGHT_OK         (1U << 9)
#define FLAG_SONAR_RIGHT_TIMEOUT    (1U << 10)


#endif /* INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_ */
