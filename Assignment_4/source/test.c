/*
 * test.c includes all the definitions of functions declared in test.h.
 *
 * It includes the Int Main().
 *
 * Author: Harsh Beriwal
 * IDE Used: MCUXpresso IDE v11.6.0 [Build 8187] [2022-07-13]
 *
 * Github Link: https://github.com/harshberiwal/PES_Assignment_4
 *
 * No leveraged code or files
 *
 */

#include "test.h"

#define PERIOD 256

uint32_t Timeout_time = 0;

int main(void) {

    //**********************Init board hardware***************************
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    //*********************Init FSL debug console*************************
    BOARD_InitDebugConsole();
#endif
    //********************Init TPM FOR Transitions************************
    Init_TPM(PERIOD);
	//gpio_init();
    //******************Init System Timer for Timing**********************
	initialize_SysTick();

	//*****************Init Touch for CROSSWALK STATE*********************
	Touch_Init();
	LOG("Main Loop is starting\n\r");

    while(1) {

    	//*******************Checks current time**************************
    	Timeout_time = get_timer();

    	//******************Calling State Machine*************************
    	state_check(Timeout_time);
    }
}




