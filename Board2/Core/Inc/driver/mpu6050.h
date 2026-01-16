/*
 * mpu6050.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Bulanov Konstantin
 */

#ifndef INC_GY521_H_
#define INC_GY521_H_

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

// --- Function Prototypes ---

uint8_t MPU6050_Init(I2C_HandleTypeDef *I2Cx);

void MPU6050_Read_Accel(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
void MPU6050_Read_Gyro(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
void MPU6050_Read_Temp(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
void MPU6050_Read_All(I2C_HandleTypeDef *I2Cx, MPU6050_t *DataStruct);
void MPU6050_Read_Yaw(I2C_HandleTypeDef *I2Cx, MPU6050_Yaw_t *DataStruct);

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt);

// *** GESTIONE ASINCRONA / INTERRUPT (Logica PadReceiver) ***
/**
 * @brief Avvia la lettura dello Yaw in modalità Interrupt (Non-Blocking).
 * @return 1 se avviata con successo, 0 se occupato o errore.
 */
uint8_t MPU6050_Read_Yaw_IT(I2C_HandleTypeDef *I2Cx, MPU6050_Yaw_t *DataStruct);

/**
 * @brief Da chiamare dentro HAL_I2C_MemRxCpltCallback (o RxCpltCallback) nel main.c 
 *        per elaborare i dati appena ricevuti.
 */
void MPU6050_Process_Yaw_IT_Data(void);

/**
 * @brief Da chiamare nella HAL_I2C_ErrorCallback per gestire errori e sbloccare il bus.
 */
void MPU6050_Error_Callback(void);

/**
 * @brief Restituisce lo stato dell'operazione.
 * @return 1 se completato/idle (RxDone), 0 se in corso.
 */
uint8_t MPU6050_IsDone(void);

void MPU6050_Set_Done(void);

#endif /* INC_GY521_H_ */
