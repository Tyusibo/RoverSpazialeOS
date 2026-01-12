/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_GY521_H_
#define INC_GY521_H_

#endif /* INC_GY521_H_ */

#include <stdint.h>
#include "i2c.h"

// MPU6050 structure
typedef struct
{

    int16_t Accel_X_RAW;
    int16_t Accel_Y_RAW;
    int16_t Accel_Z_RAW;
    double Ax;
    double Ay;
    double Az;

    int16_t Gyro_X_RAW;
    int16_t Gyro_Y_RAW;
    int16_t Gyro_Z_RAW;
    double Gx;
    double Gy;
    double Gz;

    float Temperature;

    double KalmanAngleX;
    double KalmanAngleY;
    double KalmanAngleZ; // Aggiunto campo per l'asse Z
} MPU6050_t;

// Nuova struct per la lettura esclusiva dello Yaw (asse Z)
typedef struct
{
    int16_t Gyro_Z_RAW;
    double Gz;
    
    // Accumulatore interno 'double' per non perdere precisione durante l'integrazione
    double _YawAcc; 
    
    // Output finale in uint16_t (0-360 gradi)
    uint16_t KalmanAngleZ;
} MPU6050_Yaw_t;

// Kalman structure
typedef struct
{
    double Q_angle;
    double Q_bias;
    double R_measure;
    double angle;
    double bias;
    double P[2][2];
} Kalman_t;

uint8_t MPU6050_Init(I2C_HandleTypeDef *I2Cx);

void MPU6050_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

void MPU6050_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

void MPU6050_Read_All(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);

// Prototipo della nuova funzione
void MPU6050_Read_Yaw(I2C_HandleTypeDef *I2Cx, MPU6050_Yaw_t *DataStruct);

// *** NUOVI PROTOTIPI PER INTERRUPT ***
// Avvia la lettura dello Yaw in modalità Interrupt (Non-Blocking)
void MPU6050_Read_Yaw_IT(I2C_HandleTypeDef *I2Cx, MPU6050_Yaw_t *DataStruct);

// Funzione da chiamare dentro HAL_I2C_MemRxCpltCallback nel main.c o i2c.c
void MPU6050_Yaw_RxCpltCallback(I2C_HandleTypeDef *I2Cx);

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);
