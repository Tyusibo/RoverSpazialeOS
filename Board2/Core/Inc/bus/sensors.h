#ifndef INC_BUS_SENSORS_H_
#define INC_BUS_SENSORS_H_

#include <stdint.h>

/* Speed */
typedef struct {
    float motor1;
    float motor2;
    float motor3;
    float motor4;
} BUS_Speed;

/* Temperature */
typedef float Temperature;

/* Battery Level */
typedef uint8_t BatteryLevel;

/* Sonar */
typedef struct {
    uint16_t left;
    uint16_t front;
    uint16_t right;
} BUS_Sonar;

/* Gyroscope */
typedef uint16_t Gyroscope;

/* RemoteController */
typedef struct {
    int16_t  y_lever;
    int16_t  x_lever;
    
    uint16_t buttons;
} BUS_RemoteController;

#endif /* INC_BUS_SENSORS_H_ */
