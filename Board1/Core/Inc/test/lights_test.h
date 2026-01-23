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
 * @file lights_test.h
 * @brief Basic LED tests (red/white, left/right).
 */

#ifndef INC_TEST_LIGHTS_TEST_H_
#define INC_TEST_LIGHTS_TEST_H_

/**
 * @brief Tests all Set operations (ON then OFF), repeated 2 times.
 */
void led_test_set(void);

/**
 * @brief Tests all Toggle operations, repeated 2 times.
 */
void led_test_toggle(void);

/**
 * @brief Tests All_On and All_Off functionalities.
 */
void led_test_all(void);

#endif /* INC_TEST_LIGHTS_TEST_H_ */
