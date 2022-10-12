/*
 * state_machine.h includes all the definitions of functions declared below.
 *
 * We used A Table Driven Approach for the State Machine which made the State_check
 * code concise and easy to follow.
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "touch.h"
#include "log.h"
#include "pwm.h"
#include "system.h"
#include "log.h"
#include "gpio.h"

/*
 * Table driven State machine for checking events based on Timeout or Button Pressed
 * and changing the states accordingly. This functions also polls the Touch Slider and
 * Prints debug messages during the DEBUG cycle.
 *
 * Parameters:
 *   THE TIME CURRENT STATE -GET_TIMER();
 *
 * Returns:
 *   None
 */
void state_check(uint16_t timeout_time_elapsed);


/*
 * When the Timeout occurs from GO, this function is called from the state machine using
 * a function pointer. The STOP transition takes place from the GO state and then it
 * stays in the STOP mode for 20 sec or 5 sec depending on the Production or DEBUg code
 * Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO) and its transition colors
 *
 * Returns:
 *   None
 */
void Signal_Red_on(int last_st, uint32_t last_col);

/*
 * When the Timeout occurs from CROSSWALK OR STOP, this function is called from the
 * state machine using a function pointer. The GO transition takes place from the
 * CROSSWALK OR STOP state and then it stays in the GO mode for 20 sec or 5 sec depending
 * on the Production or DEBUg code Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (CROSSWALK OR STOP) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Green_on(int last_st, uint32_t last_col);

/*
 * When the Timeout occurs from GO, this function is called from the
 * state machine using a function pointer. The WARNING transition takes place from the
 * GO state and then it stays in the GO mode for 5 sec or 3 sec depending
 * on the Production or DEBUg code Respectively.
 *
 * It calls the Transition function given below.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Yellow_on(int last_st, uint32_t last_col);

/*
 * When the Button is pressed, this function callback is generated from the
 * state machine using a function pointer. The CROSSWALK transition takes place from
 * any state and then it stays in the CROSSWALK mode for 10 sec.
 *
 * AFTER THE SMOOTH TRANSITION FOR THE BUFFAHITI PEOPLE, THE LED FOLLOWS a 250 msec off
 * and 750 msec on for 10 seconds.
 *
 * Parameters:
 *   TAKES THE LAST STATE (GO,STOP,WARNING) and its corresponding transition colors.
 *
 * Returns:
 *   None
 */
void Signal_Blue_on(int last_st, uint32_t last_col);

/*
 * This functions calculates the curr transition value on the basis of the formula
 * which includes the below parameters and returns the next transition color for that
 * particular LED.
 *
 * Parameters:
 *   FOUR
 *   1)THE COLOR OF STATE TO TRANSITION TO
 *   2)THE COLOR OF STATE TO TRANSITION FROM
 *   3)THE LED VALUE WE NEED (WHETHER RED GREEN OR BLUE)
 *   4)CORRESPONDING SHIFT AS PER THE LED TYPE
 *
 * Returns:
 *   The Required Transition color for each LED.
 */
int get_curr_val(uint32_t req_color, uint32_t last_color, uint32_t LED_Type, uint8_t shift);


/*
 * Takes the Current transition value of Red Green and Blue LED and returns
 * the updated Color if the button is pressed during the transition.
 *
 * Parameters:
 * 	 None
 *
 * Returns:
 *   The last transition color for RED GREEN And BLUE.
 */
uint32_t update_last_val();


/*
 * HANDLES THE TRANSITION BY PUTTING THE VALUES RECEIVED FROM get_curr_val
 * FUNCTION to the corresponding TPM channel for smooth Transitions.
 *
 * Parameters:
 * 	 1)CURRENT COLOR TO TRANSITION TO
 * 	 2)LAST COLOR TO TRANSTION FROM
 *
 * Returns:
 *   None
 */
void transitions(uint32_t Curr_Color, uint32_t last_color);

#endif
