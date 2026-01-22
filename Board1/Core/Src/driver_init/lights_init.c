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
 * @file lights_init.c
 * @brief Initialization source file for LED lights.
 */

#include "lights_init.h"
#include "gpio.h"  // Per la conoscenza delle macro definite nel .ioc

/** @brief Left side LED instance. */
A4WD3_Led_t led_left;
/** @brief Right side LED instance. */
A4WD3_Led_t led_right;

/**
 * @brief Initialize the LED drivers.
 *
 * Configures the left and right LED instances with the corresponding
 * GPIO ports and pins defined in gpio.h.
 */
void led_init(void)
{
    /* Init LED sinistro */
    A4WD3_Init(&led_left,
               RedLedLeft_GPIO_Port,  RedLedLeft_Pin,
               WhiteLedLeft_GPIO_Port, WhiteLedLeft_Pin);

    /* Init LED destro */
    A4WD3_Init(&led_right,
               RedLedRight_GPIO_Port,  RedLedRight_Pin,
               WhiteLedRight_GPIO_Port, WhiteLedRight_Pin);
}


