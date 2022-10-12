/*
 * pwm.h includes all the declarations given for TPM.
 * This include Init_TPM() and initializes all the peripherals used
 * for transitions and PWM in the State_Machine.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * The Init_TPM code was inspired by the Alexander, Dean. Embedded Systems
 * Fundamentals with ARM Cortex-M Based Microcontrollers : A Practical Approach,
 * ARM Education Media, 2017. ProQuest Ebook Central.
 *
 * The fragments were taken from the Init_Blue_LED_TPM code(CH. 7) and was altered
 * accordingly for the application. Different Prescaler and Clock were used along with
 * initializing different Gated Registers and setting their bits for Port B.
 */
#ifndef PWM_H_
#define PWM_H_

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
#define PORT_A 			(uint32_t)(1 <<9)
#define PORT_B 			(uint32_t)(1 <<12)

/*
 * Initializes pins as TPM output with desired clock frequency
 * Clock working at 750kHz.
 * Parameters:
 *   THE PERIOD IN THE MOD REGISTER
 *
 * Returns:
 *   None
 */
void Init_TPM(uint16_t period);

#endif
