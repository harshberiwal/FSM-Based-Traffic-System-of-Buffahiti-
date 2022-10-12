/*
 * system.h includes all the declaration of initialzing the SysTick
 * This include initialize_Systick(), get_timer(), reset_timer()
 * and now() functions.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
*/


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

typedef uint32_t ticktime_t;

/*
 * Initializes SysTick and its registers like VAL, LOAD for precise delays
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void initialize_SysTick();

/*
 * Returns the time code is running since boot.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   32-bit time its running since startup
 */
ticktime_t now();


/*
 * Resets the timer VAL register and timeout_timer for the next callback.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void reset_timer();


/*
 * Function to get the Timer Value from the current Systick Tick.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   32-bit time since the reset of the timer after state change
*/
ticktime_t get_timer();


#endif
