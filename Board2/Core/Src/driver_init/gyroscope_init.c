/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file gyroscope_init.c
 * @brief Initialization of the gyroscope sensor.
 *
 * This file contains the implementation for initializing the MPU6050 gyroscope
 * and setting up the yaw calculation structure.
 */

#include "gyroscope_init.h"

/**
 * @brief Global MPU6050 Yaw instance.
 *
 * This structure holds the state and data for the MPU6050 gyroscope yaw calculation.
 */
MPU6050_Yaw_t MPU6050_Yaw;
