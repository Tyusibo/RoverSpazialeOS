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
 * @file sonar_init.c
 * @brief Initialization source file for sonar sensors.
 *
 * This file contains the implementation for initializing the sonar sensors,
 * managing the sonar timer and interrupts, and handling distance readings.
 */

#include "sonar_init.h"
#include "gpio.h"

extern TIM_HandleTypeDef htim2;

/**
 * @brief Left sonar sensor instance.
 */
hcsr04_t sonarLeft;

/**
 * @brief Front sonar sensor instance.
 */
hcsr04_t sonarFront;

/**
 * @brief Right sonar sensor instance.
 */
hcsr04_t sonarRight;

/**
 * @brief Initializes all sonar sensors.
 *
 * This function initializes the left, front, and right sonar sensors with their
 * respective GPIO ports, pins, and timer channels.
 *
 * @return uint8_t 0 on success, 1 on failure.
 */
uint8_t Sonar_InitAll(void) {
	if (hcsr04_init(&sonarLeft, SonarLeftTrig_GPIO_Port, SonarLeftTrig_Pin, &htim2,
			SONAR_LEFT_CHANNEL) != HCSR04_OK) return 1;
	if (hcsr04_init(&sonarFront, SonarFrontTrig_GPIO_Port, SonarFrontTrig_Pin, &htim2,
			SONAR_FRONT_CHANNEL) != HCSR04_OK) return 1 ;
	if (hcsr04_init(&sonarRight, SonarRightTrig_GPIO_Port, SonarRightTrig_Pin, &htim2,
			SONAR_RIGHT_CHANNEL) != HCSR04_OK) return 1;
	return 0;
}

/**
 * @brief Starts the timer used for sonar measurements.
 *
 * This function starts the base counter for the shared timer and initiates
 * the Input Capture channels for all sonars. Interrupts are initially disabled
 * and will be enabled on demand by `hcsr04_trigger()`.
 *
 * @retval None
 */
void StartSonarTimer(void) {
    // Start the base counter without update interrupts (not needed for standard HCSR04)
    HAL_TIM_Base_Start(sonarLeft.echo_tim);

    // Starts Input Capture channels BUT keeps interrupts DISABLED.
    // hcsr04_trigger() will enable the specific interrupt "on demand".
    HAL_TIM_IC_Start(sonarLeft.echo_tim, sonarLeft.echo_channel);
    HAL_TIM_IC_Start(sonarFront.echo_tim, sonarFront.echo_channel);
    HAL_TIM_IC_Start(sonarRight.echo_tim, sonarRight.echo_channel);
}

/**
 * @brief Handles the reading process for a specific sonar sensor.
 *
 * Checks if the sonar reading is complete. If so, it processes the distance.
 * If not, it sets a default distance and resets the sonar state.
 *
 * @param[in,out] sonar Pointer to the sonar sensor structure to handle.
 * @retval None
 */
void hcsr04_handle_reading(hcsr04_t *sonar) {
    if (hcsr04_is_done(sonar)) {
        hcsr04_process_distance(sonar);
    } else {
        hcsr04_set_default_distance(sonar);
        hcsr04_reset_sonar(sonar);
    }
}

/**
 * @brief Checks if all sonar sensors have completed their measurements.
 *
 * @return uint8_t 1 if all sonars have finished readings, 0 otherwise.
 */
uint8_t all_sonar_done(void) {
	return (hcsr04_is_done(&sonarLeft) && hcsr04_is_done(&sonarFront)
			&& hcsr04_is_done(&sonarRight));
}
