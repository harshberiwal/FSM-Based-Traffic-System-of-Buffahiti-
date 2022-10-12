/*
 * touch.h includes all the declarations of functions related to Touch.
 * This include Touch_Init() and Touch_Poll()
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#define OSC_PRESCALER	4
#define ELECTRODES		11
#define ELEC_CHARGE		7
#define CHANNEL 		(10u)


/*
 * Scans the capacitive Touch Slider and returns value corresponding to
 * the touch strength and area.
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   If Touched the touch Sluider, it returns true, otherwise it returns false
 */
bool Touch_Poll(void);

/*
 * Initializes the capacitive Touch Slider and the GENCS and DATA register
 * to get the Touch Value
 *
 * Parameters:
 *   None
 *
 * Returns:
 *   None
 */
void Touch_Init();



#endif /* TOUCH_H_ */


