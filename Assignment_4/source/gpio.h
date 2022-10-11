/*
 * gpio.h includes all the declarations of functions related to GPIO.
 * This include gpio_Init()
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
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

#define red_LED 		((uint32_t)(1 << 18))
#define green_LED 		((uint32_t)(1 << 19))
#define blue_LED 		((uint32_t)(1 << 1))
#define RED 			18
#define GREEN 			19
#define BLUE 			1
#define PORT_A 			(uint32_t)(1 <<10)
#define PORT_B 			(uint32_t)(1 <<12)

void Init_TPM(uint16_t period);
/*
 * Initializes pins as GPIO output
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void gpio_init();

#endif /* GPIO_H_ */
