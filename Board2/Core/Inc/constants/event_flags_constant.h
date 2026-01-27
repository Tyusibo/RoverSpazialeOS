#ifndef INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_
#define INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_

/* Event Flags (CMSIS-RTOS v2 / FreeRTOS)
 * Bitmask definitions
 */

#define FLAG_START                  (1U << 0)
#define FLAG_SYNC                   (1U << 1)
#define FLAG_ACK                    (1U << 2)

/* Controller / Pad */
#define FLAG_PAD_OK                 (1U << 3)
#define FLAG_PAD_ERROR              (1U << 4)

/* Gyroscope */
#define FLAG_GYRO_OK                (1U << 5)
#define FLAG_GYRO_ERROR             (1U << 6)

/* Sonar Left */
#define FLAG_SONAR_LEFT_OK          (1U << 7)
#define FLAG_SONAR_LEFT_TIMEOUT     (1U << 8)

/* Sonar Front */
#define FLAG_SONAR_FRONT_OK         (1U << 9)
#define FLAG_SONAR_FRONT_TIMEOUT    (1U << 10)

/* Sonar Right */
#define FLAG_SONAR_RIGHT_OK         (1U << 11)
#define FLAG_SONAR_RIGHT_TIMEOUT    (1U << 12)

/* Mask for all events the Polling Server waits for (everything except START and SYNC) */
#define FLAGS_POLLING_SERVER_WAIT   (FLAG_PAD_OK | FLAG_PAD_ERROR | \
                                     FLAG_GYRO_OK | FLAG_GYRO_ERROR | \
                                     FLAG_SONAR_LEFT_OK | FLAG_SONAR_LEFT_TIMEOUT | \
                                     FLAG_SONAR_FRONT_OK | FLAG_SONAR_FRONT_TIMEOUT | \
                                     FLAG_SONAR_RIGHT_OK | FLAG_SONAR_RIGHT_TIMEOUT)

#endif /* INC_CONSTANTS_EVENT_FLAGS_CONSTANT_H_ */
