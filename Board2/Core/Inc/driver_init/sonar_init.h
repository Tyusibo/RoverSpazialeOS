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
 * @file sonar_init.h
 * @brief Header file for sonar sensors initialization and management.
 *
 * This file contains external declarations for sonar sensor instances and
 * function prototypes for initializing, managing timers, handling readings,
 * and checking completion status of sonar sensors.
 */

#ifndef INC_DRIVER_INIT_SONAR_INIT_H_
#define INC_DRIVER_INIT_SONAR_INIT_H_

#include "HCSR04.h"
#include "sonar_constants.h"

/**
 * @brief External declaration for the left sonar sensor.
 */
extern hcsr04_t sonarLeft;

/**
 * @brief External declaration for the front sonar sensor.
 */
extern hcsr04_t sonarFront;

/**
 * @brief External declaration for the right sonar sensor.
 */
extern hcsr04_t sonarRight;

/**
 * @brief Initializes all sonar sensors.
 *
 * Sets up the configuration for left, front, and right sonar sensors on Board 2.
 *
 * @return uint8_t 0 on success, non-zero error code otherwise.
 */
uint8_t Sonar_InitAll(void);

/**
 * @brief Starts the timer used for sonar measurements.
 *
 * Initiates the timer required for triggering and measuring echo pulses.
 */
void StartSonarTimer(void);

/**
 * @brief Handles the reading process for a specific sonar sensor.
 *
 * This function processes the echo signal for the given sonar instance to calculate distance.
 *
 * @param[in,out] sonar Pointer to the sonar sensor structure to handle.
 */
void hcsr04_handle_reading(hcsr04_t *sonar);

/**
 * @brief Checks if all sonar sensors have completed their measurements.
 *
 * @return uint8_t 1 if all sonars have finished readings, 0 otherwise.
 */
uint8_t all_sonar_done(void);

#endif /* INC_DRIVER_INIT_SONAR_INIT_H_ */
