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
 * @file a4wd3_led.h
 * @brief Driver for controlling the A4WD3 LEDs (Red and White).
 */

#ifndef INC_DRIVER_A4WD3_LED_H_
#define INC_DRIVER_A4WD3_LED_H_

#include "stm32g4xx_hal.h"

/**
 * @brief LED State type definition (GPIO_PinState).
 */
typedef GPIO_PinState A4WD3_LedState_t;

/**
 * @brief A4WD3 LED Device Structure.
 */
typedef struct {

    GPIO_TypeDef* 		  red_GPIO_Port; /**< GPIO Port for the Red LED */
    uint16_t              red_Pin;       /**< GPIO Pin for the Red LED */
    A4WD3_LedState_t      red_state;     /**< Current state of the Red LED */

    GPIO_TypeDef* 	      white_GPIO_Port; /**< GPIO Port for the White LED */
    uint16_t              white_Pin;       /**< GPIO Pin for the White LED */
    A4WD3_LedState_t      white_state;     /**< Current state of the White LED */

} A4WD3_Led_t;

/**
 * @brief Initialize the LED device structure with GPIO ports and pins.
 *
 * @param dev Pointer to the LED device structure.
 * @param red_port GPIO Port for Red LED.
 * @param red_pin GPIO Pin for Red LED.
 * @param white_port GPIO Port for White LED.
 * @param white_pin GPIO Pin for White LED.
 */
void A4WD3_Init(A4WD3_Led_t* dev,
                GPIO_TypeDef* red_port, uint16_t red_pin,
                GPIO_TypeDef* white_port, uint16_t white_pin);

/**
 * @brief Set the state of the Red LED.
 * 
 * @param dev Pointer to the LED device structure.
 * @param state State to set (GPIO_PIN_SET or GPIO_PIN_RESET).
 */
void A4WD3_Red_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state);

/**
 * @brief Turn ON the Red LED.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_Red_On(A4WD3_Led_t* dev);

/**
 * @brief Turn OFF the Red LED.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_Red_Off(A4WD3_Led_t* dev);

/**
 * @brief Toggle the Red LED state.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_Red_Toggle(A4WD3_Led_t* dev);

/**
 * @brief Set the state of the White LED.
 * 
 * @param dev Pointer to the LED device structure.
 * @param state State to set (GPIO_PIN_SET or GPIO_PIN_RESET).
 */
void A4WD3_White_Set(A4WD3_Led_t* dev, A4WD3_LedState_t state);

/**
 * @brief Turn ON the White LED.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_White_On(A4WD3_Led_t* dev);

/**
 * @brief Turn OFF the White LED.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_White_Off(A4WD3_Led_t* dev);

/**
 * @brief Toggle the White LED state.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_White_Toggle(A4WD3_Led_t* dev);

/**
 * @brief Turn OFF both Red and White LEDs.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_All_Off(A4WD3_Led_t* dev);

/**
 * @brief Turn ON both Red and White LEDs.
 * @param dev Pointer to the LED device structure.
 */
void A4WD3_All_On(A4WD3_Led_t* dev);

#endif /* INC_DRIVER_A4WD3_LED_H_ */
