/*
 * gpio.h includes all the declarations given for GPIO.
 * This include gpio_init() and the button_check function for GPIO external interrupt.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No Leveraged code
 */
#ifndef GPIO_H_
#define GPIO_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#define PORT_A 			(uint32_t)(1 <<9)

/*
 * Initializes pins as GPIO intput at PORTA pin 13
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void gpio_init();

/*
 * Checks in the Button has been pressed.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   True if the button is pressed or false if it isn't.
 */
bool button_check();

#endif /* GPIO_H_ */
