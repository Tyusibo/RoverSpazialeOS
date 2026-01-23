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
 * @file lights_test.c
 * @brief Implementation of LED test functions.
 */

#include "lights_test.h"

#include "lights_init.h"     // Gets the lights to test and #include "a4wd3_led.h"

#include "stm32g4xx_hal.h" // HAL_Delay

#define LUCI_TEST_DELAY_MS 1000

/**
 * @brief Tests setting LEDs on and off.
 *
 * Repeats the sequence 2 times.
 * Sequence: Left Red ON -> Right Red ON -> Left White ON -> Right White ON ->
 *           Left White OFF -> Right White OFF -> Left Red OFF -> Right Red OFF.
 */
void led_test_set(void)
{
    /* Ensure luci_init() has been called in main */

    for (int rep = 0; rep < 2; rep++)
    {
        /* --- ON (Set) --- */
        A4WD3_Red_On(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_On(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_On(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_On(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        /* --- OFF (Set) --- */
        A4WD3_White_Off(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Off(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Off(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Off(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);
    }
}

/**
 * @brief Tests toggling LEDs.
 *
 * Repeats the sequence 2 times.
 * Toggles Red Left, Red Right, White Left, White Right.
 */
void led_test_toggle(void)
{
    /* Ensure luci_init() has been called in main */

    for (int rep = 0; rep < 2; rep++)
    {
        /* Red toggle */
        A4WD3_Red_Toggle(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_Red_Toggle(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        /* White toggle */
        A4WD3_White_Toggle(&led_left);
        HAL_Delay(LUCI_TEST_DELAY_MS);

        A4WD3_White_Toggle(&led_right);
        HAL_Delay(LUCI_TEST_DELAY_MS);
    }
}

/**
 * @brief Tests turning all LEDs on and off.
 *
 * Sequence: Left All ON -> Right All ON -> Left All OFF -> Right All OFF.
 */
void led_test_all(void)
{
    /* Ensure luci_init() has been called in main */

    A4WD3_All_On(&led_left);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_On(&led_right);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_Off(&led_left);
    HAL_Delay(LUCI_TEST_DELAY_MS);

    A4WD3_All_Off(&led_right);
    HAL_Delay(LUCI_TEST_DELAY_MS);
}
