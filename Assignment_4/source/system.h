/*

 * system.h includes all the declaration of initialzing the SysTick
 * This include initialize_Systick()
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"


#define HUNDRED_MS 		4800000
#define TICKS 			(HUNDRED_MS + TIME_OFFSET)
#define TIME_OFFSET 	1100
#define SEC 			10
 * Initializes SysTick and its registers like VAL, LOAD for precise delays
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None

void initialize_SysTick();

#endif  SYSTEM_H_
*/
