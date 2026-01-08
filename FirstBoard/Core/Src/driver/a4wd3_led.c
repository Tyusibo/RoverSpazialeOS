/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
 * @file a4wd3_led.c
 * @brief Implementation of LED control functions for A4WD3 rover.
 *
 * This file contains the implementation of functions to control the Red and White LEDs
 * on the A4WD3 rover. It uses the HAL library for GPIO manipulation.
 */

#include "a4wd3_led.h"

void A4WD3_Init(A4WD3_Led_t* dev,
                GPIO_TypeDef* red_port, uint16_t red_pin,
                GPIO_TypeDef* white_port, uint16_t white_pin)
{
    dev->red_GPIO_Port   = red_port;
    dev->red_Pin         = red_pin;
    dev->white_GPIO_Port = white_port;
    dev->white_Pin       = white_pin;

    dev->red_state   = GPIO_PIN_RESET;
    dev->white_state = GPIO_PIN_RESET;

    // Forzatura del reset
    // L'alternativa è leggere il valore del PIN e inserirlo nelle variabili
    // red_state e white_state
    HAL_GPIO_WritePin(dev->red_GPIO_Port, dev->red_Pin, dev->red_state);
    HAL_GPIO_WritePin(dev->white_GPIO_Port, dev->white_Pin, dev->white_state);
}

/**
 * @brief Sets the state of the Red LEDs.
 * @param dev Pointer to the LED configuration structure.
 * @param state The desired state of the LED (LED_ON or LED_OFF).
 */
void A4WD3_Red_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state) {
	// dopo aver aggiornato il valore del pin si aggiorna lo stato
    HAL_GPIO_WritePin(dev->red_GPIO_Port, dev->red_Pin, state);
    dev->red_state = state;
}

/**
 * @brief Toggles the state of the Red LEDs
 * @param dev Pointer to the LED configuration structure.
 */
void A4WD3_Red_Toggle(A4WD3_Led_t* dev) {
    A4WD3_LedState_t next = (dev->red_state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    A4WD3_Red_Set(dev, next);
}

/**
 * @param dev Pointer to the LED configuration structure.
 * @param state The desired state of the LED (LED_ON or LED_OFF).
 * @brief Sets the state of the White LED
 */
void A4WD3_White_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state) {
	// dopo aver aggiornato il valore del pin si aggiorna lo stato
    HAL_GPIO_WritePin(dev->white_GPIO_Port, dev->white_Pin, state);
    dev->white_state = state;
}

/**
 * @brief Toggles the state of the White LED
 * @param dev Pointer to the LED configuration structure.
 */
void A4WD3_White_Toggle(A4WD3_Led_t* dev) {
    A4WD3_LedState_t next = (dev->white_state == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
    A4WD3_White_Set(dev, next);
}

/**
 * @param dev Pointer to the LED configuration structure.
 * @brief Turns off all LEDs
 */
void A4WD3_All_Off(A4WD3_Led_t* dev) {
    A4WD3_Red_Set(dev, GPIO_PIN_RESET);
    A4WD3_White_Set(dev, GPIO_PIN_RESET);
}

/**
 * @brief Turns on all LEDs
 * @param dev Pointer to the LED configuration structure.
 */
void A4WD3_All_On(A4WD3_Led_t* dev) {
    A4WD3_Red_Set(dev, GPIO_PIN_SET);
    A4WD3_White_Set(dev, GPIO_PIN_SET);
}
